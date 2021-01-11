This demonstrates how to sort a non-contiguous array with Pottery.

The [array_access](../../../include/pottery/array_access/) template supports the configuration of custom array access expressions that can access a generalized array which does not have to be contiguous in memory.

In our case we will sort a [pager](../../../include/pottery/pager/), which is a paged array. The entry type for our sort algorithm will be the entry type of the pager. This is a small struct containing a pointer to the value as well as the page in which it's contained to support fast iteration through the pager. We'll forward along `ARRAY_ACCESS_NEXT` and similar functions so that our sort algorithm can quickly move around in the pager.

We also demonstrate the inherent base and inherent count support in the array_access template. Since a pager has a built-in array, we pass its base and count to the sort algorithm as `ARRAY_ACCESS_BEGIN` and `ARRAY_ACCESS_COUNT`. This means we don't have to pass these to the sort algorithm, so `sort_pager()` takes only the pager itself as its argument.

Note that we sort the trivial type `int`. You can instead put a non-bitwise-movable type in the pager and sort over it instead. You would just need to supply a `LIFECYCLE_MOVE` expression to the pager and sort algorithm. As an exercise you could extend this example to sort a pager of [Pottery's example string](../string/).
