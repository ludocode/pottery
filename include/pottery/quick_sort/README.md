# Quick Sort

This is Pottery's template implementation of [Quicksort](https://en.wikipedia.org/wiki/Quicksort).

The pivot is chosen as the [ninther (median-of-medians)](https://en.wikipedia.org/wiki/Quicksort#Choice_of_pivot) for large partitions, or the median for smaller partitions.

An optional three-way partitioning scheme is used to optimize for repeated elements. This is the [Dutch national flag problem](https://en.wikipedia.org/wiki/Dutch_national_flag_problem), but using the algorithm described in Bentley and McIlroy (1993). It will be enabled automatically if a `COMPARE_THREE_WAY` expression is configured. You can also enable it by defining `USE_FAT_PARTITION` to 1, but beware that without a three-way comparison it may perform multiple boolean comparisons for each pair of values. If disabled, [Hoare partitioning](https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme) is used instead.

Quicksort supports the configuration of fallback sort algorithms for both small partitions and deep (unbalanced) partitions. The [intro_sort](../intro_sort/) template uses this to fallback to [heap_sort](../heap_sort/) to provide guaranteed safe worst-case performance.

Additional documentation still needs to be written.
