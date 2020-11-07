# Heap

A set of functions to manipulate a binary heap over a generalized array.

This implementation is very much incomplete. Currently it only supports `expand()` and `contract()` functions. These are like C++ `std::push_heap` and `std::pop_heap` in that they move elements to/from the end of the array. We should have proper `emplace()`, `displace()`, `insert()`, `extract()`, `remove()` etc. functions like other containers.

The implementation also only supports swap, not move. Move should be implemented to improve performance. Sifting elements should not involve so many moves, and functions like `insert()` should be able to insert an element directly into place without having to first move it into the end of the array.

This heap has an interesting novelty that I haven't seen in other heap implementations. Elements can track their position within the heap via a configured expression. This allows you to pop arbitrary elements rather than only being able to pop the top of the heap. This makes it extremely useful for things like request priority queues where requests can be cancelled. This is used extensively in [HomeFort](https://homefort.app/) (mostly via a dynamic priority queue container that is not ready to be released yet.) This is also incomplete; currently the configuration option for this is `SET_INDEX` but it may change.

This documentation needs to be expanded once more of the heap is implemented.
