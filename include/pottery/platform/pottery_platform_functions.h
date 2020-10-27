/*
 * MIT License
 *
 * Copyright (c) 2020 Nicholas Fraser
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef POTTERY_PLATFORM_FUNCTIONS_H
#define POTTERY_PLATFORM_FUNCTIONS_H 1

#ifdef __cplusplus
extern "C" {
#endif



/*
 * Assertions
 */

pottery_noreturn
static inline void pottery_abort() {
    #if POTTERY_DEBUG && defined(POTTERY_UNIT_TEST)
    // segfault on purpose so that ASAN gives us a stack trace.
    *((int*)-1)=1;
    #endif
    abort();
}

#if POTTERY_DEBUG || defined(POTTERY_UNIT_TEST)
#define pottery_assert(x) (!(x) ? pottery_abort() : ((void)0))
#else
#define pottery_assert(x) /*nothing*/
#endif



/*
 * Math
 */

static inline size_t pottery_min_s(size_t left, size_t right) {
    return (left < right) ? left : right;
}

static inline size_t pottery_max_s(size_t left, size_t right) {
    return (left > right) ? left : right;
}

static inline size_t pottery_clamp_s(size_t value, size_t min, size_t max) {
    return (value < min) ? min : ((value > max) ? max : value);
}

static inline size_t pottery_ispow2_s(size_t n) {
    return n && !(n & (n - 1));
}

/**
 * Places a times b in out.
 *
 * Returns true if the multiplication overflowed, false if it is safe.
 */
static inline bool pottery_mul_overflow_s(size_t a, size_t b, size_t* out) {
    #if defined(__clang__)
        #if __has_builtin(__builtin_mul_overflow)
            #define POTTERY_MUL_OVERFLOW_S_DEFINED
            return __builtin_mul_overflow(a, b, out);
        #endif
    #elif defined(__GNUC__) && !defined(__CPARSER__)
        #if __GNUC__ >= 5
            #define POTTERY_MUL_OVERFLOW_S_DEFINED
            return __builtin_mul_overflow(a, b, out);
        #endif
    #endif

    #ifndef POTTERY_MUL_OVERFLOW_S_DEFINED
    size_t large = ((size_t)1 << (size_t)(sizeof(size_t) * 4));
    *out = a * b;
    return (a >= large || b >= large) && a != 0 && *out / a != b;
    #undef POTTERY_MUL_OVERFLOW_S_DEFINED
    #endif
}



/*
 * Memory operations
 */

// Typed null
// (Used for -Wzero-as-null-pointer-constant under Clang C++)

#ifdef __cplusplus
    #define pottery_null nullptr
#else
    // In C we don't cast to void* to avoid compatibility warnings. For example
    // with -Wc++-compat, it has to be plain NULL to be usable in one branch of
    // a ternary operator where the other branch is a typed pointer. This means
    // unfortunately if a literal pottery_null is passed to a _Generic, you may
    // get int or void* depending on the platform's definition of NULL. (Don't
    // use _Generic.)
    #define pottery_null NULL
#endif

// array count

#define pottery_array_count(x) (sizeof(x) / sizeof((x)[0]))

// mem functions

#define pottery_memcmp memcmp
#define pottery_memmove memmove
#define pottery_memcpy memcpy
#define pottery_memset memset



/*
 * Memory allocation
 */

// You can define POTTERY_DEFAULT_MALLOC to 0 to disable all of Pottery's
// allocator defaults. If you then want to use templates that require
// allocation you must either define some of these yourself or provide custom
// allocators in each such template instantiation.

#ifndef POTTERY_DEFAULT_MALLOC
    // We disable all of this in the Linux kernel. You'll need to use kmalloc()
    // or whatever. Most likely you won't want to use dynamic containers in the
    // kernel anyway.
    #ifdef __KERNEL__
        #define POTTERY_DEFAULT_MALLOC 0
    #endif
#endif

// Allocator defaults are otherwise on by default.
#ifndef POTTERY_DEFAULT_MALLOC
    #define POTTERY_DEFAULT_MALLOC 1
#endif

#if POTTERY_DEFAULT_MALLOC

#ifndef POTTERY_GENERATE_CODE
#if defined(__linux__) || defined(_WIN32)
#include <malloc.h>
#elif defined(__FreeBSD__)
#include <malloc_np.h>
#elif defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <stdlib.h>
#endif
#endif

// You can define POTTERY_JEMALLOC to 1 if you're using it.
#ifndef POTTERY_JEMALLOC
    #ifdef __FreeBSD__
        #define POTTERY_JEMALLOC 1
    #else
        #define POTTERY_JEMALLOC 0
    #endif
#endif

#ifndef POTTERY_FREE
    #define POTTERY_FREE free
#endif

#ifndef POTTERY_MALLOC
    #define POTTERY_MALLOC malloc
#endif

#ifndef POTTERY_ZALLOC
    #if POTTERY_JEMALLOC
        #define POTTERY_ZALLOC(size) mallocx(size, ALLOCM_ZERO)
    #else
        #define POTTERY_ZALLOC(size) calloc(size, 1)
    #endif
#endif

#ifndef POTTERY_REALLOC
    #define POTTERY_REALLOC realloc
#endif

// Expand is similar to realloc except it is only used to expand in-place to
// usable size without moving the allocation. it takes a pointer to both ptr
// and size since the underlying platform may need them. It can change the size
// but not the pointer.
#ifndef POTTERY_MALLOC_EXPAND
    // void POTTERY_MALLOC_EXPAND(void** ptr, size_t* size)
    #if POTTERY_JEMALLOC
        // this isn't actually used since we have MALLOC_GOOD_SIZE but this is
        // how it should be implemented.
        #define POTTERY_MALLOC_EXPAND(ptr, size) *size = xallocx(*ptr, *size, 0, 0)
    #else
        // we assign ptr to let Pottery assert that it did not change.
        #define POTTERY_MALLOC_EXPAND(ptr, size) *ptr = realloc(*ptr, *size)
    #endif
#endif

#ifndef POTTERY_MALLOC_GOOD_SIZE
    #if POTTERY_JEMALLOC
        #define POTTERY_MALLOC_GOOD_SIZE(size) nallocx(size, 0)
    #elif defined(__APPLE__)
        #define POTTERY_MALLOC_GOOD_SIZE malloc_good_size
    #endif
#endif

#ifndef POTTERY_MALLOC_USABLE_SIZE
    // In order to support POTTERY_MALLOC_USABLE_SIZE, we need a usable size
    // function that is fast, and we need to be able to POTTERY_REALLOC() to the
    // usable size without it relocating the data. (This could potentially be
    // improved by adding a MALLOC_AT_LEAST macro. At least FreeBSD could use
    // it and custom user allocators could as well. It's not worth implementing
    // right now.)

    // We assume this is true on Linux. Both glibc and musl support
    // malloc_usable_size(). Other libc may not; in this case we can detect
    // them here. Jemalloc also supports this although the alloc template will
    // use nallocx() instead.
    #if defined(__linux__) || POTTERY_JEMALLOC
        #define POTTERY_MALLOC_USABLE_SIZE malloc_usable_size

    // On macOS and iOS malloc_size() exists but it may be slow. At least this
    // blog post claims it is:
    //     https://lemire.me/blog/2017/09/15/how-fast-are-malloc_size-and-malloc_usable_size-in-c/
    // We have malloc_good_size() so this won't actually be used but we define
    // it anyway for completeness.
    #elif defined(__APPLE__)
        #define POTTERY_MALLOC_USABLE_SIZE malloc_size
    #endif

    // Windows has _msize() but it returns the exact size requested, not the
    // usable size. There's no point in using it.

    // Other BSDs as far as we know don't support something like
    // malloc_usable_size(), and we know nothing about other platforms.
#endif

#ifndef POTTERY_ALIGNED_FREE
    // Windows uses a separate heap for aligned allocations. We can't use the
    // normal free() on them.
    #ifdef _WIN32
        #define POTTERY_ALIGNED_FREE _aligned_free
    #else
        #define POTTERY_ALIGNED_FREE free
    #endif
#endif

#ifndef POTTERY_ALIGNED_MALLOC
    // jemalloc provides the standard aligned_alloc().
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_MALLOC aligned_alloc
    #endif
#endif
#ifndef POTTERY_ALIGNED_MALLOC
    // Windows provides _aligned_malloc() but the argument order is swapped.
    #ifdef _WIN32
        #define POTTERY_ALIGNED_MALLOC(alignment, size) _aligned_malloc((size), (alignment))
    #endif
#endif
#ifndef POTTERY_ALIGNED_MALLOC
    // C11 requires aligned_alloc().
    #ifdef __STDC_VERSION__
        #if __STDC_VERSION__ >= 201112L
            #define POTTERY_ALIGNED_MALLOC aligned_alloc
        #endif
    #endif
#endif
#ifndef POTTERY_ALIGNED_MALLOC
    // posix_memalign() requires POSIX. Note that this isn't available under
    // -std=c99 but it is under -std=gnu89 and up.
    #ifdef _POSIX_VERSION
        #if _POSIX_VERSION >= 200112L
            static pottery_always_inline
            void* pottery_aligned_malloc(size_t alignment, size_t size) {
                void* p = pottery_null;
                posix_memalign(&p, alignment, size);
                return p;
            }
            #define POTTERY_ALIGNED_MALLOC pottery_aligned_malloc
        #endif
    #endif
#endif

#ifndef POTTERY_ALIGNED_ZALLOC
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_ZALLOC(alignment, size) mallocx(size, ALLOCM_ZERO | MALLOCX_ALIGN(alignment));
    #endif
#endif

#ifndef POTTERY_ALIGNED_REALLOC
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_REALLOC(ptr, alignment, size) rallocx((ptr), (size), MALLOCX_ALIGN((alignment)))
    #elif defined(_WIN32)
        #define POTTERY_ALIGNED_REALLOC(ptr, alignment, size) _aligned_realloc((ptr), (size), (alignment))
    #endif
#endif

#ifndef POTTERY_ALIGNED_MALLOC_EXPAND
    // void POTTERY_ALIGNED_MALLOC_EXPAND(void** ptr, size_t alignment, size_t* size)
    #if POTTERY_JEMALLOC
        // This isn't actually used since we have ALIGNED_MALLOC_GOOD_SIZE but
        // this is how it should be implemented.
        #define POTTERY_ALIGNED_MALLOC_EXPAND(ptr, alignment, size) *size = xallocx(*ptr, *size, 0, MALLOCX_ALIGN(alignment))
    #elif !defined(_WIN32)
        // We assume malloc_usable_size() can also be used on aligned
        // allocations and that realloc() can resize them in-place to the
        // usable size. If we discover platforms where this does not work we
        // must disable them. Pottery will abort() if the pointer changed.
        #define POTTERY_ALIGNED_MALLOC_EXPAND(ptr, alignment, size) *ptr = realloc(*ptr, *size)
    #endif
#endif

#ifndef POTTERY_ALIGNED_MALLOC_GOOD_SIZE
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_MALLOC_GOOD_SIZE(alignment, size) nallocx(size, MALLOCX_ALIGN(alignment))
    #endif
#endif

#ifndef POTTERY_ALIGNED_MALLOC_USABLE_SIZE
    // See note above about POTTERY_ALIGNED_MALLOC_EXPAND. We assume malloc_usable_size()
    // can be used on aligned allocations. If we discover platforms where this
    // does not work we must disable them.
    #if defined(__linux__) || POTTERY_JEMALLOC
        #define POTTERY_MALLOC_ALIGNED_USABLE_SIZE(alignment, ptr) malloc_usable_size(ptr)
    #endif

    // Windows has _msize() but it returns the exact size requested, not the
    // usable size. There's no point in using it.
#endif

#endif // !defined(POTTERY_NO_DEFAULT_MALLOC)



#ifdef __cplusplus
}
#endif



#endif
