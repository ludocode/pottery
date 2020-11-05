## Features

Pottery's templates are:

### Clean

Pottery does not use void pointer casts, function pointers, code block macros, or any other messyness of typical C container libraries. It does not rely on compiler extensions like `typeof`, statement expressions, or anything else that is not in the common subset of C11 and C++11 implemented by major compilers.

Pottery's macros are quite tame in comparison. The implementation code tends to look like idiomatic C, and the resulting code generated from the preprocessor is close to what you might have written by hand for your specific use case. Pottery builds cleanly under at least `-Wall -Wextra -Wpedantic` under GCC and Clang (as C99 or C++11) and `/W4` under MSVC.

### Strongly Typed

An instantiated Pottery template provides real functions that operate on concrete types. You do not cast anything, nor do you pass types around in function-like macros to use Pottery.

With real typed functions, the compiler can alert you on type or argument mismatches and you get optimal performance. Pottery brings strong static typing to an otherwise weakly typed language.

Pottery will even respect extended alignment requirements for your types. For example if you put a type with extended alignment in a vector, the vector will detect this and call `aligned_alloc()` instead of `malloc()` to allocate its storage.

### Composable

Pottery's templates can themselves instantiate other templates in their implementations in order to share code. Most dynamic container templates use shared helper templates to manage element lifecycles and allocate memory.

### Nestable

Want to make a vector of maps, or a priority queue of linked lists? No problem. You can configure Pottery containers to manage the lifecycle of their contained values, including non-trivial move, copy, and destroy operations. This means you can put objects by value in flat Pottery containers even if they are not bitwise-movable.

### Configurable

Pottery templates can be extensively configured. For example all of the array algorithms like [`heap`](../include/pottery/heap/) or [`intro_sort`](../include/pottery/intro_sort/) operate on a generalized array. This means you can configure arbitrary expressions to access the i'th element and to swap and compare elements. These expressions can do whatever you like: access a database, pull elements off a tape drive, make network requests, or simply manipulate a normal array in memory.

The behaviour of dynamic containers is highly configurable as well. For example [`vector`](../include/pottery/heap/) can be configured as a double-ended vector, can be configured with some internal capacity to avoid allocations, and can be configured to disable its auto-shrink behaviour.

### Documented

Pottery is heavily documented. Take a look at the [documentation index](../README.md#documentation) in the project README to start.

Documentation for individual templates is inline in the [`include/`](../include/pottery/) folder. Click any template folder to see its documentation.

### Consistent

Pottery uses highly predictable naming conventions for all of its containers and algorithms. All dynamic containers are described by a shared [API reference](docs/container_api.md) because they all share the same API. Names such as `insert()` and `extract()` mean the same in any container.

### Fast

Pottery containers can fully manage the lifecycles of objects they contain. This means Pottery containers can be flat, containing their elements by value rather than having them individually allocated. This can provide huge performance advantages.

Moreover, Pottery code can be well-optimized by compilers. The strong typing and real functions means the compiler gets to decide when it should inline functions. Typical C containers either have code-block macros that force inlining everywhere or function pointers that prevent it. Despite being generic, the goal is for Pottery's containers to beat any custom container implementation that hasn't had extensive use-case-specific optimizations. Pottery's templates depend only on ordinary function inlining for performance which is an optimization well supported on modern compilers.

### Practical

Pottery is intended to be practical. It's not just an experiment to push the boundaries of meta-programming in C. It's a real container library for writing real programs, and it's used throughout [HomeFort](https://homefort.app). Pottery hopefully serves as an example of a workable way to do composable templates in C.

## Additional Features

### Error Handling

Pottery contains extensive error-handling code.

Pottery attempts to provide a C equivalent to the "strong exception guarantee", except instead of exceptions, all mutating methods return error codes. If an error occurs during the mutation of a container, Pottery will undo whatever operations it did in order to restore the container to its original state.

If you don't want to handle errors you can just ignore them. This is normal if you're targeting a platform where `malloc()` doesn't fail gracefully because you wouldn't get a proper out-of-memory error anyway.

Errors are of type `pottery_error_t` which is a typedef of `int`. We follow the typical C style of 0 meaning no error and error codes being non-zero. Here are some of the error values defined so far:

```c
#define POTTERY_OK 0
#define POTTERY_ERROR_ALLOC 1 // malloc() failed or std:bad_alloc was caught.
#define POTTERY_ERROR_OVERFLOW 2 // A math operation overflowed.
#define POTTERY_ERROR_FULL 3 // A size-limited container is full.
#define POTTERY_ERROR_CXX_EXCEPTION 4 // A C++ exception other than std::bad_alloc was caught.
```

These are not in an enum so that you can use your own codes for custom errors, for example on a configured element copy expression. Your custom errors will get bubbled out of whatever Pottery mutation function you called. You should use negative values for custom errors.


### C++ Compatibility

Pottery goes to extreme lengths to maintain compatiblity with C++.

All Pottery code should build cleanly as C++ with no warnings on any compiler. Pottery even supports C++ warning flags that are hostile to C code such as `-Wold-style-cast`.

You can put C++ types in Pottery containers and simply declare that they are value types. Pottery will properly run constructors and destructors, move and copy by constructors and assignment operators, swap with a swap function found by argument-dependent lookup, etc.

Pottery provides the strong exception guarantee even for C++ objects with throwing copy constructors. Pottery will catch any exceptions thrown, convert them to error codes, and unwind whatever mutation operation was in progress. Unrecognized exceptions are converted to `POTTERY_ERROR_CXX_EXCEPTION`.

Pottery provides additional functions in C++. For example you configure dynamic containers to support `construct()`ing an object in-place in a dynamic container. This uses perfect forwarding though a C++ variadic template. This is equivalent to the `emplace()` operation in C++ STL containers. (Pottery uses the term "emplace" for getting an uninitialized pointer to a newly inserted element.) Pottery also provides r-value reference overloads for `insert()` and friends.

Pottery can optimize the moving of "[trivially relocatable](https://quuxplusone.github.io/blog/code/object-relocation-in-terms-of-move-plus-destroy-draft-7.html)" C++ values using `memcpy()`. It detects this automatically based on `std::is_trivially_copyable`, but you can specialize type traits (`pottery::is_bitwise_movable` and `pottery::is_bitwise_copyable`) to declare to Pottery that your C++ type can be moved or copied bitwise. This can make it faster than standard C++ STL containers.

There are some limitations. In particular, Pottery does not support exceptions from move constructors or from `std::swap()`. You are allowed to use a move constructor that may throw, but the program will abort if it actually does throw. If the only possible exception is an out-of-memory error, it's fine to ignore this on platforms that don't gracefully handle out-of-memory situations (e.g. modern desktop operating systems.)

Pottery's templates are instantiated into C++ templates under [`bindings/cxx`](bindings/cxx). These can serve as drop-in replacements for the STL. Yes, you really can just instantiate the C algorithms into C++ class templates and they "just work". This makes it possible to use Pottery's algorithms in C++, and it will hopefully make it easy to benchmark Pottery against C++ container libraries.
