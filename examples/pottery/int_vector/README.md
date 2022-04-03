This demonstrates how to build a vector of ints with Pottery.

The template instantiation gives us a type called `int_vector_t`. You can initialize it with `int_vector_init()`; add to it with `int_vector_insert_at()` and friends; access the contents by `int_vector_count()` and `int_vector_at()`; and so forth.

These are all real functions. You can set a breakpoint on `int_vector_insert_at()` and step through it, bringing you into the [vector implementation code](../../../include/pottery/vector/impl/pottery_vector_definitions.t.h). You can get nice stack traces if something goes wrong. You'll even get somewhat legible compiler errors (`-fmax-errors=1` in GCC or `-Wfatal-errors` in Clang can help.)

We've defined `LIFECYCLE_BY_VALUE` to 1 above because `int` is trivially copyable and destroyable. You could instead provide a destroy expression for the vector to call on your elements to clean them up (for example if it contained pointers, you could tell it to destroy them with `free()`.) You could provide a move expression in case your type is not bitwise-movable. You could give it a copy initialization expression for your type to allow whole vector copies. Pottery evaluates your expressions directly; it does not call callbacks through function pointers. Pottery's dynamic containers can fully and efficiently manage the lifecycles of the elements they contain.

You could also provide the vector with a custom allocator and context. You could configure it to provide some internal space for a small number of elements to avoid small allocations. You could configure it as a double-ended vector. You could put C++ values in it and it will properly run move constructors, destructors, etc. You can even instantiate separate header and source files to use it in multiple translation units without each having a copy of the implementation.


