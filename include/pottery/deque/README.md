# Deque

A paged [double-ended queue](https://en.wikipedia.org/wiki/Double-ended_queue), implemented as a [ring](../ring/) of pointers to allocated fixed-size pages. The deque provides O(1) insert/remove at both ends, O(1) random access, and pointer stability for queue operations.

Deque provides better performance than other queues in many use cases. Unused capacity exists at both the start and end of the deque so you can insert and extract from either end in amortized O(1). Growing or shrinking the deque does not move any elements because it just adds or removes pages.

A deque provides pointer stability as long as you only ever insert/emplace or extract/remove/displace at the ends of the array. The value type in a deque therefore does not need to be movable. (Array functions, e.g. insert and remove in the middle of the deque could be done by moving at most half the elements in the deque. This is not yet implemented.)

Deque provides provides a lot of functionality for operating on values in bulk. For example you can emplace a large number of values spanning many pages (with a single error check if you need to check for malloc failure), then you can iterate in bulk over the added space to `memcpy()` whole pages of elements into the deque. This is used to make Pottery's example [fifobuffer](../../../examples/pottery/fifobuffer/) fast.
