# Vector

## Configuration Options

### `AUTO_SHRINK`

- `AUTO_SHRINK`, a flag

If 1, the vector will automatically shrink when removing items if the remaining count is below some threshold. If 0, the vector does not automatically shrink.

The default is 1.

You can call `shrink()` to shrink manually regardless of the value of this flag.

### `DOUBLE_ENDED`

- `DOUBLE_ENDED`, a flag

If 1, the vector stores unused capacity at both the start and end of the allocated storage. This allows amortized O(1) insert/remove at either end of the vector. This also makes the code larger and makes each vector larger since it has to store an offset into the allocation.

The default is 0.

### `INTERNAL_CAPACITY`

- `INTERNAL_CAPACITY`, an integer

The amount of internal space available for values. The vector will be able to store up to this many values without having to allocate.

The default is 0. If 0, the vector has no internal capacity, so it needs to allocate as soon as the first element is inserted. A vector is bitwise-movable if and only if its internal capacity is 0. This adds some additional complexity and code size if the value is non-zero.

For some value types it's possible for this to be non-zero without making the vector itself larger. The internal capacity is shared with a `size_t` value for the allocated capacity, so you can fit as many values as fit in a `size_t`. In particular, for a vector of pointers, you can make the internal capacity 1 without increasing the size of each vector.
