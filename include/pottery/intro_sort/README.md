This is an implementation of [Introsort](https://en.wikipedia.org/wiki/Introsort). It is intended to be competitive with a typical implementation of C++ `std::sort`. It is generally faster and safer to use than `qsort()`, with the downside of larger compiled code size.

This instantiates Pottery's [quick\_sort](../quick_sort/) and [heap\_sort](../heap_sort/) templates: heapsort is configured as a depth limit fallback for quicksort. There is almost no actual implementation code here so this is a good example of how a template can configure and share a large number of template dependencies.

Documentation still needs to be written. In the meantime, see the example [sort\_strings](../../../examples/pottery/sort_strings/).
