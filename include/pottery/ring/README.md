# Ring

A ring is a dynamically growable array implemented as a [circular buffer](https://en.wikipedia.org/wiki/Circular_buffer) around a single allocation. It provides O(1) random access and amortized O(1) insert/remove at both ends.

An insert or remove operation may cause the ring to grow. In this case the ring allocates a new buffer and moves all values from the old buffer to the new one. It is similar to a double-ended [vector](../vector/), except that a ring's logical array of values is not contiguous in memory since it wraps around the buffer.

This ring implementation is incomplete. It currently only supports insert/remove at either end of the ring, not at arbitrary indexes; it does not support a context for configured expressions; its bulk operations are very inefficient; it does not shrink; and it uses modulus rather than masking to wrap. It's also poorly tested and poorly documented. It needs a lot more work; right now it's mostly just implemented for [deque](../deque/).
