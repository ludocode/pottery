## Features

Pottery's templates are:

### Clean

Pottery does not use void pointer casts, function pointers, code block macros, or any other messyness of typical C container libraries. It does not rely on compiler extensions like `typeof`, statement expressions, or anything else that is not in the common subset of C11 and C++11 implemented by major compilers.

Pottery's macros are quite tame in comparison. The implementation code tends to look like idiomatic C, and the resulting code generated from the preprocessor is close to what you might have written by hand for your specific use case. Pottery builds cleanly under at least `-Wall -Wextra -Wpedantic` under GCC and Clang (as C99 or C++11) and `/W4` under MSVC.

### Strongly Typed

An instantiated Pottery template provides real functions that operate on concrete types. You do not cast anything, nor do you pass types around in function-like macros to use Pottery. With real typed functions, the compiler can alert you on type or argument mismatches and you get optimal performance. Pottery brings strong static typing to an otherwise weakly typed language.

### Composable

Pottery's templates can themselves instantiate other templates in their implementations in order to share code. For example, both the `red_black_tree` and the `weight_balanced_tree` use the `binary_tree` template to implement their tree traversal and rotations. Most dynamic container templates use shared helper templates to manage element lifecycles and allocate memory.

### Nestable

Want to make a vector of maps, or a priority queue of linked lists? No problem. You can configure Pottery containers to manage the lifecycle of their contained values, including non-trivial move, copy, and destroy operations. This means you can put objects by value in flat Pottery containers even if they are not bitwise-movable.

### Configurable

Pottery templates can be extensively configured. For example all of the array algorithms like [`heap`](../include/pottery/heap/) or [`intro_sort`](../include/pottery/intro_sort/) operate on a generalized array. This means you can configure arbitrary expressions to access the i'th element and to swap and compare elements. These expressions can do whatever you like: index a non-contigous array, access a database, pull elements off a tape drive, make network requests, or simply manipulate a normal array in memory.

Dynamic containers are highly configurable as well. For example [`vector`](../include/pottery/heap/) can be configured as a double-ended vector, and can be configured with some internal capacity to avoid allocations.

### Documented

Pottery is heavily documentated. Take a look at the [documentation index in the README](../README.md#documentation) to start.

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

You can put C++ types in Pottery containers and declare that they are value types, and Pottery will properly run constructors and destructors, move by move construction and move assignment, copy by copy construction and copy assignment, swap with a swap function found by argument-dependent lookup, etc.

Pottery provides the strong exception guarantee even for C++ objects with throwing copy constructors. In Pottery's usual C functions, Pottery will catch any exceptions thrown, unwind whatever mutation operation was in progress, and convert them to error codes. Unrecognized exceptions are converted to `POTTERY_ERROR_CXX_EXCEPTION`.

Pottery also provides additional functions in C++. For example, you can `construct()` an object in-place in a dynamic container. This is equivalent to the `emplace()` operation in C++ STL containers. (Pottery uses the term "emplace" for getting an uninitialized but constructed pointer to a newly inserted element.)

There are some limitations. In particular, Pottery does support exceptions from move constructors or from `std::swap()`. You are allowed to use a move constructor that throws as long as you are OK with the program aborting if it actually does throw. If the only possible exception is an out-of-memory error, it's fine to ignore this on platforms that don't gracefully handle out-of-memory situations (e.g. modern desktop operating systems.)

Pottery's templates are instantiated into C++ templates under [`bindings/cxx`](bindings/cxx). Yes you really can just instantiate the C algorithms into C++ class templates and they "just work". This makes it possible to use Pottery's algorithms in C++, and it also makes it easy to benchmark Pottery against C++ container libraries.
