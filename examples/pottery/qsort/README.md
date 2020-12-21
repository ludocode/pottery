### Fast and fully-featured `qsort()` implemented with Pottery

This is a very large implementation of `qsort()` and its platform-specific extensions using Pottery. It provides type specializations for common element sizes to maximize speed at the expense of code size. This causes it to significantly outperform glibc's `qsort()` in some synthetic benchmarks. Run `test/benchmarks.sh` to run Pottery's mini benchmark suite.

Check out the [`qsort_simple`](../qsort_simple/) example first to see how you can implement `qsort()` with Pottery. This example expands on it.

`qsort()` is often called on an array of ints, pointers, or small structs. With Pottery, it's easy to provide a different instantiation of [`intro_sort`](../../../include/pottery/intro_sort/) for each of a set of common element sizes. If the element size is known, the sort algorithm can move values around directly rather than through `memcpy()`, and can move whole values into temporaries (which can be in registers) rather than being restricted to swapping elements.

We can also repeat this process for the `qsort_r()` extension variations rather than share an implementation, that way we aren't adding branches to each comparison.

These are the variations of `qsort()` implemented here. They all have mutually incompatible interfaces, though some are implemented by wrapping others where possible without performance cost. There's a good description of the differences between them on [this StackOverflow post](https://stackoverflow.com/a/39561369).

- `pottery_qsort()` - Standard C `qsort()`
- `pottery_gnu_qsort_r()` - GNU glibc (and uClibc) `qsort_r()`
- `pottery_bsd_qsort_r()` - BSD (and macOS) `qsort_r()`
- `pottery_win_qsort_s()` - Windows `qsort_s()`
- `pottery_c11_qsort_s()` - C11 Annex K `qsort_s()`

A size specialization is instantiated for each of 2, 4, 8, 12, 16, 24 and 32 byte element sizes (with 2, 4 or 8 byte alignment), as well as an arbitrarily large element size. This instantiates Pottery's [`intro_sort`](../../../include/pottery/intro_sort/) template 24 times, resulting in over 200 Pottery template instantiations from thousands of `#include` directives. It compiles to roughly 90kb of machine code with gcc -O3 on amd64.

(Compiling this with gcc -O3 takes over three seconds on my machine! This is a nearly insufferable amount of time to compile a C source file. Still, it demonstrates that compile times are still manageable even with extensive use of Pottery. They are nothing compared to the glacial compile times of some other languages.)

It probably doesn't make sense to include this in every application that wants to use `qsort()`. It would be faster and smaller to replace every call to `qsort()` in your app with a strongly-typed instantiation of introsort (or just use [`qsort_simple`](../qsort_simple/) since this is unlikely to be a bottleneck in your app.) But maybe this code makes sense for a dynamically linked libc focused on maximum speed.

If you did want to statically link this into your app, it would make sense to pull the entry points into the header file and force-inline them, and eliminate the alignment checks on platforms that allow unaligned access. Element sizes are ostensibly compile-time constants: usually you're passing the result of `sizeof` as the element size. This would in theory optimize to a direct call to a size-specific implementation of introsort and the linker could gc all of the ones you don't use. We don't do this here because this is just an example (but let me know if you want this.)

A potential size optimization for this code would be to share a single implementation of the heapsort fallback code. This would make the slow heapsort code even slower, but it would be no slower than traditional implementations and it would still provide safe worst-case time complexity which is its only purpose. This is not implemented yet.

Another size optimization would be to share the implementation between `qsort()` and `gnu_qsort_r()` on platforms which support casting a function pointer to a function type with additional ignored arguments. This is technically undefined behaviour so it is not implemented.

This has not been extensively tested. Use at your own risk.
