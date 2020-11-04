# Vector

A dynamically growable contiguous array of values. This is a close C analogue to C++ [`std::vector`](https://en.cppreference.com/w/cpp/container/vector).

The low-level API for inserting and removing values is `emplace()` and `displace()`. These create or remove space for uninitializes values within the vector. When calling these functions, you must manually initialize and destroy the values.

The pass-by-value API is `insert()` and `extract()`. These require that your value type can be moved by value (e.g. `LIFECYCLE_MOVE_BY_VALUE` or `LIFECYCLE_BY_VALUE`, but not `LIFECYCLE_MOVE`.) The vector takes ownership of a value passed to `insert()` and relinquishes ownership of a value returned by `extract()`.

The `remove()` functions destroy and remove values. An element destroy expression is required (e.g. `LIFECYCLE_DESTROY`, `LIFECYCLE_DESTROY_BY_VALUE` or `LIFECYCLE_BY_VALUE`)

If a destroy expression is not provided, the vector does not claim ownership over its contained values (it owns only its allocated array.) You must manually clear such a vector before destroying it. It is an error to destroy a non-empty vector without a configured destroy expression.



## Configuration Options

Vector supports all of the configuration options of the [lifecycle](../lifecycle/) and [alloc](../alloc/) templates. This allows you to configure the lifecycle of objects contained within the vector.

Of these, only a lifecycle move expression is required: the vector must be able to move its contents when growing its allocation. You must define one of `LIFECYCLE_MOVE`, `LIFECYCLE_MOVE_BY_VALUE` or `LIFECYCLE_BY_VALUE`.

### Types

#### `VALUE_TYPE`

The type for values in the vector.

#### `CONTEXT_TYPE`

A context for configured lifecycle and alloc expressions. The context must be trivially copyable and destroyable. It is passed to the vector during initialization and stored within the vector.

### Behaviour

#### `AUTO_SHRINK`

A flag (1 or 0). If 1, the vector will automatically shrink when removing items if the remaining count is below some threshold. If 0, the vector does not automatically shrink.

The default is 1.

You can call `shrink()` to shrink manually regardless of the value of this flag.

#### `DOUBLE_ENDED`

A flag (1 or 0). If 1, the vector stores unused capacity at both the start and end of the allocated storage. This allows amortized O(1) insert/remove at either end of the vector. This also makes the code larger and makes each vector larger since it has to store an offset into the allocation. This increases complexity, code size and memory usage, and may or may not improve performance.

The default is 0.

#### `INTERNAL_CAPACITY`

An integer indicating the amount of internal space available for values. The vector will be able to store up to this many values without having to allocate.

The default is 0. If 0, the vector has no internal capacity, so it needs to allocate as soon as the first element is inserted. A vector is bitwise-movable if and only if its internal capacity is 0. A non-zero internal capacity adds some additional complexity and code size.

For some value types it's possible for this to be non-zero without making the vector itself larger. The internal capacity is shared with a `size_t` value for the allocated capacity, so you can fit as many values as fit in a `size_t`. In particular, for a vector of pointers, you can make the internal capacity 1 without increasing the size of each vector.

#### `CXX`

A flag (1 or 0). If 1, the vector will instantiate additional C++ functions when compiled as C++.

This is 0 by default so that the vector can be instantiated in an `extern "C"` block. Set this to 1 to get the additional functions.

These functions include:

- `construct_*()` -- construct an element in-place in the vector
- `insert_*()` overloads for various reference types allowing more efficient insertion
