### Simple `qsort()` implementation using Pottery

This is an implementation of the standard C `qsort()` and GNU-style `qsort_r()` using Pottery's introsort. This implementation is intended to be simple for demonstration purposes. Despite this, it provides great performance with safe worst-case time complexity and no memory allocation. For an even faster and more fully-featured implementation, see [qsort](../qsort/).

Like many Pottery algorithms, Pottery's introsort supports custom array access expressions over an abstract reference type. In the case of `qsort()`, we can use `void*` as a reference type and use the user's element size to offset our pointers and move elements around.

The custom swap function performs a swap byte-by-byte, and a custom compare function calls the user's comparator through a function pointer (with the optional user context.) This is all we need to configure introsort to build `qsort()`.

To pass our arguments around, we store them in a context. We pass this to introsort and it passes it back to our configured expressions. Our context is small so we pass it by value.

The above would be suitable for a minimal libc. Check out the other [qsort](../qsort/) example to see more.
