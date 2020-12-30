# FAQ



### Why does Pottery exist?

Pottery is designed to provide fast, high-level data structures in plain C programs. The goal is to make it as easy to declare a strongly-typed data structure in C as it is in Java, C++, or any other language with built-in support for templates or generics. In those languages it's really easy to write `std::vector<MyType>` or `ArrayList<MyType>` for example. Pottery makes it _almost_ as easy in C.

A major objection that C++ programmers have to using C over C++ is that the STL exists. C programmers often hear "Why didn't you just use C++? You could just use `std::map` and be done!" Pottery can reduce the strength of that argument. There are many reasons to use C over C++ or other languages. Having easy-to-use high-level data structures can continue to tip the scales in favour of C.

In addition, Pottery can be used even in C++ in situations where the STL is inappopriate, for example in OS kernels or in microcontrollers. It provides low-level intrusive containers and algorithms for resource-constrained platforms without a memory allocator. There are also many pre-existing software projects that are written in C rather than C++ where Pottery could be useful.



### Doesn't this bloat compiled binary size?

Yes, if used carelessly. Generating templated code rather than using a weakly typed (`void*`) container can bloat the compiled size of your program, as templates tend to do in C++.

It can in fact be worse with Pottery. In C++, if two unrelated translation units use `std::vector<int>`, those template definitions use COMDAT linkage and will be merged by the linker. With Pottery there are no such guarantees: Pottery containers and algorithms must be instantiated with unique names or with static linkage. Identical code folding may help you a bit, but it's just an optimization that you really can't count on. It requires a good compiler with a proper configuration and it still may not actually work.

Pottery lets you mitigate this by providing separate declare/define headers to instantiate templates: you can declare a template instantiation in a header file and define it in one translation unit to share the definition. This can require coordination among different parts of your code which may not scale.

Pottery is meant to replace those algorithms and data structures that you would have otherwise implemented manually in an ad-hoc manner. When used this way, it does not bloat code size, because you'd have written the code it generates anyway. But Pottery also makes it really easy to, say, instantiate a full-blown introsort whereas you might have just hand-coded a perfectly sufficient insertion sort.

Care is always needed to keep binary sizes under control. This is one reason why Pottery doesn't just pick an implementation to call "sort" or "map" or whatever as the C++ STL does (see the question below for more.) You should know the implementation of what you are instantiating so you can weigh the costs against the possible benefits. Remember, small code is usually faster than fast code.



### Why isn't there just a `map` or a `sort` template? Why do I have to choose an implementation?

C++ names its templates based on abstract behaviour rather than implementation, for example `std::map` and `std::sort`. These are meant to be generic so the underlying algorithms can vary.

Pottery does not. If you want a map, you have to choose whether you want a `red_black_map`, a `btree_map`, an `array_map`, an `open_hash_map`, etc.

Unfortunately the promised genericity of C++ templates doesn't really work in practice. The pointer stability and iterator invalidation rules require that `std::map` be implemented as a self-balancing search tree with non-mutating lookup and individually allocated mutable nodes. The red-black tree is the only sensible choice. There are better C++ map containers available but none can be called `map` because that's now reserved for the red-black tree.

Similarly with `std::sort`, it guarantees O(n log n) worst case, and it de-facto must have at least an in-place fallback (technically it's allowed to throw `std::bad_alloc` but the vast majority of code assumes it won't.) The only sensible choice is therefore Introsort. This is a poor choice for the standard sort algorithm because it's huge. Smaller sort algorithms like Quicksort and more modern sort algorithms like Timsort are available but they can never be called `sort`.

Pottery does not use abstract names because you need to be aware of the underlying algorithms in order to weigh their overhead. This includes compilation speed, runtime performance, and compiled code size.

- Ignoring compilation speed can lead to a painfully long edit, compile, debug cycle which makes debugging time consuming and agonizing. The speed of this cycle has been argued to be the most critical component of programmer productivity and programmer happiness.

- Ignoring performance characteristics can result in [Schlemiel the painter’s algorithms](https://www.joelonsoftware.com/2001/12/11/back-to-basics/). This can lead to unnacceptable user experience and even denial-of-service vulnerabilities.

- Ignoring the size of your templates can lead to executables tens of megabytes in size. The resulting instruction cache thrashing will significantly reduce overall performance in ways that are hard to quantify; it's death by a thousand cuts. (See the question on bloat above for more.)

The name `red_black_map` confronts you with the implementation and forces you to question whether it's the right decision for your purpose.



### Why can't `move()` fail?

C++ allows move construction, move assignment and `std::swap()` to throw exceptions. The standard C++ containers are designed to handle this possibility. In Pottery, `init_steal()` (corresponding to C++ move assignment) is allowed to fail. But `steal()` (corresponding to C++ move assignment), `move()` (with no direct C++ equivalent) and `swap()` cannot fail. You may be wondering why Pottery does not allow failures like C++. The main reason is that it is not often possible to undo a failed move.

For example when insertion sort uses move, it declares a temporary on the stack and moves values to and from that temporary. If it moves a value from your array to that temporary, but then fails to move the value back to your array, there is no reasonable way for it to clean up. It cannot simply destroy the temporary and return an error because there is now an uninitialized value somewhere in your array.

As another example, when a dynamic container like a vector or an open hash map grows, it needs to move all values from the old allocation to the new one. If a move fails after moving several elements, it now has some initialized and some uninitialized elements in both allocations. It cannot simply move them back because those moves may fail as well! There's no sane way to recover from this.

How does C++ solve this problem? In an algorithm where a failing move cannot be undone, like `std::vector` for example, if move construction can fail it uses an alternate algorithm that copies instead! This way if a copy fails there's nothing to undo: it simply destroys the partial copy. This essentially makes fallible move useless. Nobody really catches and handles a failed move; if it can throw they just don't use it. This is why it's critical to [mark your move constructors `noexcept` to get good performance](https://gieseanw.wordpress.com/2020/08/28/friendly-reminder-to-mark-your-move-constructors-noexcept/) from standard containers.

There are some exceptions to this, but they are not terribly useful. If you have no copy constructor but a throwing move, `std::vector` will use it and waive the strong exception guarantee: a move that throws may ruin the contents of the vector. Another example is [std::uninitialized_move](https://en.cppreference.com/w/cpp/memory/uninitialized_move): if a move exception is thrown, some objects in the source array are left in a valid but unspecified state, so if it fails to move any element it ruins the source array without leaving anything in the destination. This destructive behaviour makes these algorithms useless for a move constructor that can actually fail in practice. Still, these aren't anything like Pottery's `move()`; they're more like Pottery's `init_steal()` which is in fact allowed to fail.

Pottery could therefore solve the fallible move problem similar to the above solutions. Instead of allowing `move()` to fail, we could allow containers and algorithms to work without move. For example we could allow you to provide `INIT_COPY`, `COPY` and `DESTROY` to containers and algorithms rather than `MOVE`. This would allow insertion sort for example to copy to and from the temporary rather than move, so in case of error it could destroy the temporary and leave the array intact. Or if you're okay with ruining the data on error, we could use `INIT_STEAL`, `STEAL` and `DESTROY`. Like `std::uninitialized_move()` or `std::vector` with the above waiver, a failed `init_steal()` could leave the data in a valid but junk state, and you could recover by destroying the data.

There doesn't seem to be any point in implementing this yet, although the lifecycle template is designed to possibly support this in the future. In the meantime, these algorithms are designed for stability and performance, and C++'s workarounds are bad for both. If you truly have a type with a fallible move, don't use it with Pottery.

(Some third-party C++ containers go even further and [don't support non-trivial relocation at all](https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md#object-relocation). They'll tell you that very few types are non-relocatable... you know, those rarely-used types like `std::string`. Pottery strikes a good balance by supporting non-trivial but infallible move.)
