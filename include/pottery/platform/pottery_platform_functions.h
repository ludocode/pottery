/*
 * MIT License
 *
 * Copyright (c) 2020-2021 11037650 Canada Inc.
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

#ifndef POTTERY_PLATFORM_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

// We need to push our pragmas to disable warnings within this file. (MSVC
// warns about things like constant expressions in if statements, etc.)
#include "pottery/platform/pottery_platform_pragma_push.t.h"

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
    *(pottery_reinterpret_cast(int*, -1))=1;
    #endif
    abort();
}

#if POTTERY_DEBUG || defined(POTTERY_UNIT_TEST)
#define pottery_assert(x) (pottery_unlikely(!(x)) ? pottery_abort() : ((void)0))
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
 * Hashing
 */

/**
 * Performs a Knuth multiplicative hash and range reduction on the given 32-bit
 * value, returning the given number of most-significant bits.
 *
 * @see pottery_knuth_hash_u64
 */
static inline uint32_t pottery_knuth_hash_u32(uint32_t value, size_t bits) {
    pottery_assert(bits > 0);
    pottery_assert(bits <= 32);

    // The original Knuth hash. This is the largest prime less than the
    // quotient of UINT32_MAX by the golden ratio.
    //     https://www.wolframalpha.com/input/?i=largest+prime+less+than+floor%28%282%5E32-1%29%2F%28%281%2Bsqrt%285%29%29%2F2%29%29
    uint32_t prime = UINT32_C(2654435761);
    return (value * prime) >> (32 - bits);
}

/**
 * Performs a Knuth multiplicative hash and range reduction on the given 64-bit
 * value, returning the given number of most-significant bits.
 *
 * This is a trivial hash function to reduce a 64-bit hash value to a power
 * of two, ensuring only that all bits of the input are used. It provides a
 * basic pseudo-random distribution even for sequential values, making it
 * not entirely terrible for non-cryptographic hashing of keys (such as in a
 * hash table.)
 */
static inline uint64_t pottery_knuth_hash_u64(uint64_t value, size_t bits) {
    pottery_assert(bits > 0);
    pottery_assert(bits <= 64);

    // This number is defined for 64-bit the same way as the prime in Knuth's
    // 32-bit multiplicative hash: it is the largest prime that is less than
    // the quotient of UINT64_MAX by the golden ratio.
    //     https://www.wolframalpha.com/input/?i=largest+prime+less+than+floor%28%282%5E64-1%29%2F%28%281%2Bsqrt%285%29%29%2F2%29%29
    uint64_t prime = UINT64_C(11400714819323198393);
    return (value * prime) >> (64 - bits);
}

/**
 * Performs a Knuth multiplicative hash and range reduction on the given
 * `size_t` value, returning the given number of most-significant bits.
 *
 * The return value is architecture-dependent; it is different on different
 * machines.
 *
 * @see pottery_knuth_hash_u64
 */
pottery_always_inline
static size_t pottery_knuth_hash_s(size_t value, size_t bits) {
    pottery_assert(bits <= (8 * sizeof(size_t)));
    if (sizeof(size_t) <= 4)
        return pottery_cast(size_t, pottery_knuth_hash_u32(pottery_cast(uint32_t, value), bits));
    pottery_assert(sizeof(size_t) <= sizeof(uint64_t));
    return pottery_cast(size_t, pottery_knuth_hash_u64(pottery_cast(uint64_t, value), bits));
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
#ifndef POTTERY_GENERATE_CODE
    #if POTTERY_JEMALLOC && !defined(__FreeBSD__)
        #include <jemalloc/jemalloc.h>
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
        #define POTTERY_ZALLOC(size) mallocx(size, MALLOCX_ZERO)
    #else
        #define POTTERY_ZALLOC(size) calloc(size, 1)
    #endif
#endif

#ifndef POTTERY_REALLOC
    #define POTTERY_REALLOC realloc
#endif

// On platforms where we don't have an equivalent of malloc_good_size() and we
// can't expand in-place to a usable size, we make up our own good size. This
// will hopefully waste less memory than just allocating directly. (This needs
// a lot more testing to see how effective it is in practice.)
static inline size_t pottery_malloc_estimate_good_size(size_t size) {

    // for very small allocations, we round up to 24. this appears to be the
    // smallest block ptmalloc* can give us. better allocators like jemalloc
    // support as little as 2 bytes, but they also support a proper
    // malloc_good_size() equivalent so (as long as we're aware of them) this
    // won't be called.
    const size_t min_size = 24;
    if (size < min_size) {
        return min_size;
    }

    // threshold between small and large allocations
    const size_t page_size = 4096;
    const size_t threshold = page_size * 4;

    // for relatively small allocations, round up to a multiple of the
    // alignment. this is usually the alignment requirement for malloc on all
    // platforms.
    const size_t alignment = 16;
    if (size <= threshold) {
        return (size + alignment - 1) & ~pottery_cast(size_t, (alignment - 1));
    }

    // this is the potential overhead for allocators that store metadata along
    // with the allocation (e.g. ptmalloc*). we subtract it from the final size
    // after rounding up to the page size to make sure the allocator doesn't
    // waste an entire page just to store its metadata. for better allocators
    // that store metadata separately, a malloc_good_size() equivalent should
    // be defined.
    const size_t overhead = 24;

    // round up to the page size checking for overflow
    size_t padded = size + page_size + overhead - 1;
    if (pottery_unlikely(padded < size)) {
        return size;
    }
    return (padded / page_size) * page_size - overhead;
}

#ifndef POTTERY_MALLOC_GOOD_SIZE
    #if POTTERY_JEMALLOC
        #define POTTERY_MALLOC_GOOD_SIZE(size) nallocx(size, 0)
    #elif defined(__APPLE__)
        #define POTTERY_MALLOC_GOOD_SIZE malloc_good_size
    #else
        #define POTTERY_MALLOC_GOOD_SIZE pottery_malloc_estimate_good_size
    #endif
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
    // (Unfortunately Apple doesn't define it even in C11 unless
    // __DARWIN_C_LEVEL>=__DARWIN_C_FULL which is not the default. For safety
    // we just won't use it on their platforms.)
    #if defined(__STDC_VERSION__) && !defined(__APPLE__)
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
                // posix_memalign() doesn't modify the given pointer on failure
                // (always on Linux, elsewhere as of POSIX.1-2008) so we
                // shouldn't need to check the return value, except that glibc
                // declares the return value warn_unused_result under
                // _FORTIFY_SOURCE.
                if (pottery_unlikely(0 != posix_memalign(&p, alignment, size)))
                    return pottery_null;
                return p;
            }
            #define POTTERY_ALIGNED_MALLOC pottery_aligned_malloc
        #endif
    #endif
#endif

#ifndef POTTERY_ALIGNED_ZALLOC
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_ZALLOC(alignment, size) mallocx(size, MALLOCX_ZERO | MALLOCX_ALIGN(alignment));
    #endif
#endif

#ifndef POTTERY_ALIGNED_REALLOC
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_REALLOC(ptr, alignment, size) rallocx((ptr), (size), MALLOCX_ALIGN((alignment)))
    #elif defined(_WIN32)
        #define POTTERY_ALIGNED_REALLOC(ptr, alignment, size) _aligned_realloc((ptr), (size), (alignment))
    #endif
#endif

#ifndef POTTERY_ALIGNED_MALLOC_GOOD_SIZE
    #if POTTERY_JEMALLOC
        #define POTTERY_ALIGNED_MALLOC_GOOD_SIZE(alignment, size) nallocx(size, MALLOCX_ALIGN(alignment))
    #else
        #define POTTERY_ALIGNED_MALLOC_GOOD_SIZE(alignment, size) \
                ((void)(alignment), pottery_malloc_estimate_good_size(size))
    #endif
#endif

#endif // !defined(POTTERY_NO_DEFAULT_MALLOC)



#ifdef __cplusplus
}
#endif

#include "pottery/platform/pottery_platform_pragma_pop.t.h"

#endif
