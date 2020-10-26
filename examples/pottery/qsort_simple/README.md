### Simple `qsort()` implementation using Pottery

This is an implementation of the standard C `qsort()` using Pottery's introsort. This implementation is intended to be simple for demonstration purposes. For a faster and more fully-featured implementation, see [qsort](../qsort/).

Like many Pottery algorithms, Pottery's introsort supports a custom accessor and abstract reference type to index elements. In the case of `qsort()`, we can use `void*` as a reference type and use the user's element size to offset our pointers and move elements around.

The custom swap function performs a swap by `memcpy()` through a temporary buffer. There isn't really a limit on how big elements will be, so if they're bigger than our buffer, we'll loop around, swapping the elements in buffer-sized chunks. This is by far the biggest part of the implementation.

And finally a custom compare function calls the user's comparator through a function pointer. Those three functions are all we need to configure introsort to build `qsort()`.

To pass our arguments around, we store them in a context. We pass this to introsort and it passes it back to our configured expressions. Our context is small so we pass it by value.

The above would be suitable for a minimal libc. Of course there are several ways we can improve this. The most important are implementing `qsort_r()`-style extensions and improving performance. Check out the other [qsort](../qsort/) example to see more.
