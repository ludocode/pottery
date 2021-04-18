## Simple `qsort()` with Pottery

This is an implementation of the standard C `qsort()` and several extensions using Pottery's [intro_sort](../../../include/pottery/intro_sort/`) template. It defines the following functions:

- `pottery_simple_qsort()` - Standard C `qsort()`
- `pottery_simple_gnu_qsort_r()` - GNU glibc (and uClibc) `qsort_r()`
- `pottery_simple_bsd_qsort_r()` - BSD (and macOS) `qsort_r()`

This implementation is intended to be simple for demonstration purposes. Despite this, it provides great performance with safe worst-case time complexity, small optimized code size, no memory allocation and strictly bounded stack usage (it does not use recursion.)

This would be suitable for a minimal libc. For an even faster implementation, see [qsort](../qsort/).



### How It Works

Most templated C sort algorithms (such as the excellent [swensort/sort](https://github.com/swenson/sort) templates) cannot be used to implement `qsort()`. This is because they only operate on strongly typed C arrays: they use operator `[]` to access values. But `qsort()` takes an element size at runtime so operator `[]` can't be used.

Pottery's templates are different: they operate on generalized arrays over an abstract reference type. You can define your own array access expressions for accessing array elements. This means you can implement `qsort()` with Pottery by offsetting pointers by the element size at runtime.

This is demonstrated by this example. Instead of defining a `VALUE_TYPE`, we define a `REF_TYPE` of `void*`, and we define `CONTEXT_TYPE` to a struct containing our arguments. These types are passed back to our configured expressions. The custom array access functions offset by the element size, the swap function swaps bitwise, and a custom compare function calls the user's comparator through a function pointer. This is all we need to configure Pottery's `intro_sort` to build `qsort()`.

Read the [How It Works](../../../docs/how_it_works.md) documentation for more information on the inner workings of Pottery's templates.
