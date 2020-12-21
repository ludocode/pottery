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

#ifndef POTTERY_ALLOC_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



// configuration

#ifndef POTTERY_ALLOC_EXTERN
#define POTTERY_ALLOC_EXTERN /* nothing */
#endif

#if !defined(POTTERY_ALLOC_FREE) && ( \
        defined(POTTERY_ALLOC_MALLOC) || \
        defined(POTTERY_ALLOC_ZALLOC) || \
        defined(POTTERY_ALLOC_REALLOC))
    #error "FREE is required for a corresponding MALLOC, ZALLOC or REALLOC."
#endif

#if defined(POTTERY_ALLOC_FREE) && \
        !defined(POTTERY_ALLOC_MALLOC) && \
        !defined(POTTERY_ALLOC_ZALLOC) && \
        !defined(POTTERY_ALLOC_REALLOC)
    #error "FREE is defined but no corresponding MALLOC, ZALLOC or REALLOC is defined."
#endif

#if defined(POTTERY_ALLOC_MALLOC_GOOD_SIZE) && \
        !defined(POTTERY_ALLOC_MALLOC) && \
        !defined(POTTERY_ALLOC_ZALLOC) && \
        !defined(POTTERY_ALLOC_REALLOC)
    #error "MALLOC_GOOD_SIZE is defined but no corresponding MALLOC, ZALLOC or REALLOC is defined."
#endif

#if !defined(POTTERY_ALLOC_ALIGNED_FREE) && ( \
        defined(POTTERY_ALLOC_ALIGNED_MALLOC) || \
        defined(POTTERY_ALLOC_ALIGNED_ZALLOC) || \
        defined(POTTERY_ALLOC_ALIGNED_REALLOC))
    #error "ALIGNED_FREE is required for a corresponding ALIGNED_MALLOC, ALIGNED_ZALLOC or ALIGNED_REALLOC."
#endif

#if defined(POTTERY_ALLOC_ALIGNED_FREE) && \
        !defined(POTTERY_ALLOC_ALIGNED_MALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_ZALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_REALLOC)
    #error "ALIGNED_FREE is defined but no corresponding ALIGNED_MALLOC, ALIGNED_ZALLOC or ALIGNED_REALLOC."
#endif

#if defined(POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE) && \
        !defined(POTTERY_ALLOC_ALIGNED_MALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_ZALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_REALLOC)
    #error "ALIGNED_MALLOC_GOOD_SIZE is defined but no corresponding ALIGNED_MALLOC, ALIGNED_ZALLOC or ALIGNED_REALLOC is defined."
#endif

// If we have no custom allocators defined, use the defaults.
#if !defined(POTTERY_ALLOC_MALLOC) && \
        !defined(POTTERY_ALLOC_ZALLOC) && \
        !defined(POTTERY_ALLOC_REALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_MALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_ZALLOC) && \
        !defined(POTTERY_ALLOC_ALIGNED_REALLOC)

    #ifdef POTTERY_ALLOC_CONTEXT_TYPE
        #error "An ALLOC_CONTEXT is not allowed with the default allocators."
    #endif

    #ifdef POTTERY_FREE
        #define POTTERY_ALLOC_FREE POTTERY_FREE
    #endif
    #ifdef POTTERY_MALLOC
        #define POTTERY_ALLOC_MALLOC POTTERY_MALLOC
    #endif
    #ifdef POTTERY_ZALLOC
        #define POTTERY_ALLOC_ZALLOC POTTERY_ZALLOC
    #endif
    #ifdef POTTERY_REALLOC
        #define POTTERY_ALLOC_REALLOC POTTERY_REALLOC
    #endif
    #ifdef POTTERY_MALLOC_GOOD_SIZE
        #define POTTERY_ALLOC_MALLOC_GOOD_SIZE POTTERY_MALLOC_GOOD_SIZE
    #endif

    #ifdef POTTERY_ALIGNED_FREE
        #define POTTERY_ALLOC_ALIGNED_FREE POTTERY_ALIGNED_FREE
    #endif
    #ifdef POTTERY_ALIGNED_MALLOC
        #define POTTERY_ALLOC_ALIGNED_MALLOC POTTERY_ALIGNED_MALLOC
    #endif
    #ifdef POTTERY_ALIGNED_ZALLOC
        #define POTTERY_ALLOC_ALIGNED_ZALLOC POTTERY_ALIGNED_ZALLOC
    #endif
    #ifdef POTTERY_ALIGNED_REALLOC
        #define POTTERY_ALLOC_ALIGNED_REALLOC POTTERY_ALIGNED_REALLOC
    #endif
    #ifdef POTTERY_ALIGNED_MALLOC_GOOD_SIZE
        #define POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE POTTERY_ALIGNED_MALLOC_GOOD_SIZE
    #endif
#endif

// Extended alignment is off by default.
#ifndef POTTERY_ALLOC_EXTENDED_ALIGNMENT
    #define POTTERY_ALLOC_EXTENDED_ALIGNMENT 0
#endif

// If extended alignment is disabled, we need fundamental alignment
// expressions.
#if !POTTERY_ALLOC_EXTENDED_ALIGNMENT && !defined(POTTERY_ALLOC_FREE)
    #error "Fundamental alignment allocation expressions are required when extended alignment is disabled. (Extended alignment expressions are ignored unless EXTENDED_ALIGNMENT is 1.)"
#endif



// Context

#ifdef POTTERY_ALLOC_CONTEXT_TYPE
#define POTTERY_ALLOC_CONTEXT_ARG pottery_alloc_context_t context,
#define POTTERY_ALLOC_CONTEXT_VAL context,
#define POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED (void)context
#else
#define POTTERY_ALLOC_CONTEXT_ARG    /* nothing */
#define POTTERY_ALLOC_CONTEXT_VAL    /* nothing */
#define POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED /* nothing */
#endif



/*
 * Renaming
 */

//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_alloc DEST:POTTERY_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_alloc to POTTERY_ALLOC_PREFIX

    #define pottery_alloc_free POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _free)
    #define pottery_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_alloc_malloc POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _malloc)
    #define pottery_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_alloc_malloc_zero POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _malloc_zero)
    #define pottery_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_alloc_malloc_array POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _malloc_array)
    #define pottery_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED
