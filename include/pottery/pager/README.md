# Pager

A paged [double-ended queue](https://en.wikipedia.org/wiki/Double-ended_queue), implemented as a [ring](../ring/) of pointers to allocated fixed-size pages. The pager provides O(1) insert/remove at both ends, O(1) random access, and pointer stability for queue operations.

Pager provides better performance than other queues in many use cases. Unused capacity exists at both the start and end of the pager so you can insert and extract from either end in amortized O(1). Growing or shrinking the pager does not move any elements because it just adds or removes pages.

A pager provides pointer stability as long as you only ever insert/emplace or extract/remove/displace at the ends of the array. The value type in a pager therefore does not need to be movable. (Array functions, e.g. insert and remove in the middle of the pager could be done by moving at most half the elements in the pager. This is not yet implemented.)

The pager is the closest Pottery analogue to a typical implementation of C++ `std::deque`.

### Bulk Operations

Pager provides provides a lot of functionality for operating on values in bulk. For example you can emplace a large number of values spanning many pages (with a single error check if you need to check for malloc failure), then you can iterate in bulk over the added space to `memcpy()` whole pages of elements into the pager. This is used to make Pottery's example [fifobuffer](../../../examples/pottery/fifobuffer/) fast.

Note that when pushing in bulk, you almost certainly want to push at the end of the array (`insert_last_bulk()`) and pop from the front of the array (`extract_first_bulk()`). This will allow you to push and pop arbitrary unmatched array sizes without re-ordering values.
