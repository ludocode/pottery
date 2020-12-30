# Hello world!

This is a simple "Hello world!" example using [Pottery](../../../).

The example instantiates a vector of allocated C strings:

- A `DESTROY` expression is provided to call `free()`. The vector therefore owns the strings, and will free them when they are removed or when it is destroyed.

- `MOVE_BY_VALUE` is declared so we can pass allocated strings into it with `insert()`.

- The contained strings are accessed by index with `at()`.
