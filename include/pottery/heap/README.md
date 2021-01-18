# Heap

A set of functions to manipulate a binary heap over a generalized array.

This implementation is very much incomplete. Currently it only supports `expand()` and `contract()` functions. These are like C++ `std::push_heap` and `std::pop_heap` in that they move elements to/from the end of the heap portion of the array. This should have proper `emplace()`, `displace()`, `insert()`, `extract()`, `remove()` etc. functions using a `KEY_TYPE` but they're not implemented yet.

The implementation also only supports swap, not move. Move should be implemented to improve performance. Sifting elements should not involve so many moves, and functions like `insert()` should be able to insert an element directly into place without having to first move it into the end of the array.

This documentation needs to be expanded once more of the heap is implemented.

### Index Tracking

This heap has an interesting novelty that I haven't seen in other heap implementations. Elements can track their position within the heap via a configured `SET_INDEX` expression. This allows you to pop arbitrary elements rather than only being able to pop the top of the heap.

This makes it extremely useful for things like request priority queues where requests can be cancelled. See the example [Pointers with index tracking](../priority_queue/#pointers-with-index-tracking) in Pottery's [`priority_queue`](../priority_queue/).
