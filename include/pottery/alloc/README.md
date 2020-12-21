# Alloc

This is a helper template for other Pottery containers. It is not intended to be used directly.

This wraps external allocation functions for use in Pottery templates. All Pottery dynamic containers and some Pottery algorithms use this to allocate memory.

The alloc template serves as a portable allocator wrapper that can zero, align, expand, relocate, multiply with overflow checks, etc. even where such features are not provided by the underlying allocator.

You can configure it with custom allocation functions and a custom context to provide your own allocator, perform arena-specific allocations, etc. Usually you would configure this on the container and it will use it to instantiate this template.

```c
// A vector of ints with a custom arena allocator using arena as context
#define POTTERY_VECTOR_PREFIX my_arena_vector
#define POTTERY_VECTOR_VALUE_TYPE int
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#define POTTERY_VECTOR_CONTEXT_TYPE arena_t*
#define POTTERY_VECTOR_ALLOC_MALLOC(arena, size) arena_malloc(arena, size)
#define POTTERY_VECTOR_ALLOC_FREE(arena, ptr) arena_free(arena, ptr)
#include "pottery/vector/pottery_vector_static.t.h"

// You must pass the arena context as an argument to init()
my_arena_vector_t vector;
my_arena_vector_init(&vector, some_arena);
```

See the Configuration section below for more information.

## Alignment

A "fundamental alignment" is an alignment requirement of at most `pottery_alignmax`, e.g. `_Alignof(max_align_t)` or equivalent. An "extended alignment" is an alignment requirement greater than this. (Read more [here](https://en.cppreference.com/w/c/language/object)).

Pottery determines whether to call fundamental alignment functions (e.g. `malloc()`) or extended alignment functions (e.g. `aligned_alloc()`) based on the alignment requirement of the allocation. It can manually perform extended alignment if a platform extended alignment allocation function is unavailable.

Alignment must always be a power of two.

## Using Alloc Directly

Although this is intended as a helper for other Pottery templates, it could be useful on its own to serve as a wrapper to an allocator that can provide additional features. You can instantiate it with no configuration other than `PREFIX` to use the default allocator of the platform, or additionally define `EXTENDED_ALIGNMENT` to 1 to enable extended alignment allocations.

For example, in a common header file for your project:

```c
#define POTTERY_ALLOC_PREFIX myproject
#define POTTERY_ALLOC_EXTENDED_ALIGNMENT 1
#include "pottery/alloc/pottery_alloc_declare.t.h"
```

In one translation unit:

```c
#define POTTERY_ALLOC_PREFIX myproject
#define POTTERY_ALLOC_EXTENDED_ALIGNMENT 1
#include "pottery/alloc/pottery_alloc_define.t.h"
```

Allocate a `foo_t`, zeroing it and respecting whatever its alignment requirements may be. This will optimize directly to calloc() if it is not over-aligned:

```c
foo_t* foo = (foo_t*) myproject_malloc_zero(alignof(foo_t), sizeof(foo_t));
/* use foo */
myproject_free(foo, alignof(foo_t));
```

Allocate an array of at least ten ints along an L1 cache line, expanding the given count to avoid wasting padding around the allocation:

```c
#define L1_CACHE_LINE_SIZE 64
size_t count = 10;
int* ints = (int*) myproject_malloc_at_least(L1_CACHE_LINE_SIZE, &count, sizeof(int));
/* count may have increased; use up to count ints */
myproject_free(ints, L1_CACHE_LINE_SIZE);
```

There are some important caveats with this though:

### Strictness about Zero and NULL

The Pottery allocation functions are very strict about zero and null. You cannot ever allocate zero bytes, and you cannot ever realloc or free NULL.

The former restriction works around platform incompatibilities with allocating zero bytes. Some platforms return NULL, others allocate at least one byte, others still may return a non-NULL reserved pointer, and some allocators even consider it undefined behaviour (like jemalloc's `allocm()`.) By forbidding zero byte allocations we avoid all these problems.

The latter restriction is purely for correctness and auditability. A `malloc()` is always paired with a `free()`, and a `realloc()` always goes between and is not paired with anything.

### Non-Static Templates in Header Files

When `EXTENDED_ALIGNMENT` is enabled, it's important to not instantiate allocation functions `static` in a shared header file if you will compile different translation units with different language versions.

In general it's bad form to instantiate templates static in header files since it duplicates all of the implementation code in each translation unit. You should always avoid this anyway to prevent code bloat. But with extended alignment functions it can cause serious incompatibilities.

For example when compiling this template with GCC on Linux, `-std=c11` will use `aligned_alloc()`; `-std=gnu89` will use `posix_memalign()`; and `-std=c99` will perform manual alignment. The first two can be freed directly with `free()` but the third cannot. By instantiating the template in only one translation unit these will always agree on implementation.

These issues also exist for all Pottery dynamic containers, so you should not be instantiating those static in a header file either. It's not a huge concern for them though because `EXTENDED_ALIGNMENT` is off by default. It's expected that these will almost always be used with types that do not have extended alignment requirements so it is unlikely to cause problems in practice.


## Configuration Options

### Allocate

If you define at least one allocation function in this section, the template will use only those you defined to implement the rest. This means you do not have to provide all of them for a custom allocator. You can simply define one of them and let the template handle it.

In particular, if you define, say, only `MALLOC`, the template will manually perform extended alignment using your custom allocation function. If you define only `ALIGNED_MALLOC`, the template will use it for all allocations, even those of fundamental alignment.

- `void* MALLOC(size_t size)`

Allocates memory of the given size with fundamental alignment.

- `void* ZALLOC(size_t size)`

Allocates zeroed memory of the given size with fundamental alignment.

- `void* REALLOC(void* ptr, size_t size)`

Re-allocates memory of fundamental alignment to a new size, relocating it if necessary.

If re-allocation fails, the original pointer must not be affected.

- `void* ALIGNED_MALLOC(size_t alignment, size_t size)`

Allocates memory of extended alignment.

Pottery will never call this with a size of zero or with an alignment less than or equal to `alignof(max_align_t)`.

This is ignored unless `EXTENDED_ALIGNMENT` is 1.

- `void* ALIGNED_ZALLOC(size_t alignment, size_t size)`

Allocates zeroed memory of the given size with extended alignment.

Pottery will never call this with dimensions zero or with an alignment less than or equal to `alignof(max_align_t)`.

This is ignored unless `EXTENDED_ALIGNMENT` is 1.

- `void* ALIGNED_REALLOC(void* ptr, size_t alignment, size_t size)`

Re-allocates memory of extended alignment to a new size, relocating it if necessary.

The given alignment is the same as the alignment originally used to allocate the given memory. This cannot be used to change the alignment requirement of an allocation.

Pottery will never call this with a size of zero, or with an alignment less than or equal to `alignof(max_align_t)`.

This is ignored unless `EXTENDED_ALIGNMENT` is 1.

### Free

- `void FREE(void* ptr)`

Frees a pointer allocated with one the the fundamental alignment allocation functions.

You must define `FREE` if and only if you define one of `MALLOC`, `ZALLOC` or `REALLOC`.

- `void ALIGNED_FREE(void* ptr)`

Frees a pointer allocated with one the the extended alignment allocation functions.

You must define `ALIGNED_FREE` if and only if you define one of `ALIGNED_MALLOC`, `ALIGNED_ZALLOC` or `ALIGNED_REALLOC`.

This is ignored unless `EXTENDED_ALIGNMENT` is 1.

### Other

- `EXTENDED_ALIGNMENT`

A flag to enable extended alignment allocations.

If 1, it enables the code that wraps extended alignment allocation functions such as `aligned_alloc()` or `posix_memalign()`.

If 0, an attempt to allocate with extended alignment will abort at runtime.

With most templates that use alloc, or if alloc is used directly, this is 0 by default since such code is rarely used. Even if you set this to 1 the code can usually be optimized away with proper compiler flags if unused. Extended alignment code is only used if you define a dynamic container (or other allocating template) containing values with a greater alignment requirement than that of the platform allocator (i.e. greater than [`max_align_t`](https://en.cppreference.com/w/cpp/types/max_align_t).)

Some templates may enable this by default in the future if they need extended alignment for their internal implementation.

**WARNING**: It's important to avoid instantiating any template static in a header file if `EXTENDED_ALIGNMENT` is enabled because this can cause incompatibilities depending on compiler flags. See the section above on Non-Static Templates in Header Files.

- `size_t MALLOC_GOOD_SIZE(size_t size)`

Rounds the given size of an allocation of fundamental alignment up to a size the allocator can allocate without padding.

This allows Pottery to minimize allocator space wasted on padding.

You can only define this if you also define one of `MALLOC`, `ZALLOC` or `REALLOC`.

- `size_t ALIGNED_MALLOC_GOOD_SIZE(size_t size)`

Rounds the given size of an allocation of extended alignment up to a size the allocator can allocate without padding.

This allows Pottery to minimize allocator space wasted on padding.

You can only define this if you also define one of `ALIGNED_MALLOC`, `ALIGNED_ZALLOC` or `ALIGNED_REALLOC`.

This is ignored unless `EXTENDED_ALIGNMENT` is 1.



## API reference

### Alignment

All alloc template functions take the alignment as an argument. This allows each one to determine whether it should call an extended alignment function (e.g. `aligned_alloc()`) or a fundamental alignment function (e.g. `malloc()`.) Usually when allocating a type `foo_t` you would pass `alignof(foo_t)` as the alignment.

This is true even for `realloc()` and `free()`:

- Some platforms have separate `realloc()` and `free()` functions for aligned allocations (e.g. `_aligned_realloc()` and `_aligned_free()` on Windows);

- Most platforms do not have an extended-alignment `realloc()` at all (e.g. Linux, macOS);

- Some platforms do not support aligned allocations. In this case the template will allocate extra space and manually align the returned pointer within that space. This must be undone to free it.

For all the above reasons **you must pass the same alignment to `realloc()` and `free()` as you passed to the original allocation** so that the correct functions are called. You cannot change the alignment of an allocation.

This determination is usually done at compile-time. All functions in the public alloc template API are always inline and branch on alignment since alignment is usually a compile-time constant.

### Functions

- `void free(size_t alignment, void* ptr);`

Frees the memory at ptr that was allocated with the given alignment.

- `void* malloc(size_t alignment, size_t size);`

Allocates memory with the given alignment requirement.

- `void* malloc_zero(size_t alignment, size_t size);`

Allocates zeroed memory with the given alignment requirement.

- `void* malloc_at_least(size_t alignment, size_t* size);`

Allocates at least the requested amount of memory with the given alignment requirement.

The given size may be increased to reflect the actual amount of memory allocated. A future realloc() to grow this allocation will preserve the memory stored in this new size, not the original requested size.

- `void* malloc_array_zero(size_t alignment, size_t count, size_t element_size);`

Allocates a zeroed array of the given number of elements with the given element size and alignment requirement.

- `void* malloc_at_least(size_t alignment, size_t* count, size_t element_size);`

Allocates an array of at least the given number of elements with the given element size and alignment requirement.

The given count may be increased to reflect the number of elements that fit in the actual amount of memory allocated. A future realloc() to grow this allocation will preserve this new number of elements, not the original requested number.

- `void* realloc_at_least(size_t alignment, void* ptr, size_t old_count, size_t* new_count, size_t element_size);`

Re-allocates an array of elements to at least the request number of elements with the given element size, original count and alignment requirement, relocating it if necessary.

The given count may be increased to reflect the number of elements that fit in the actual amount of memory allocated. A future realloc() to grow this allocation will preserve this new number of elements, not the original requested number.
