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

#ifndef POTTERY_VECTOR_IMPL
#error "This header is internal to Pottery. Do not include it."
#endif

#define POTTERY_ALLOC_PREFIX POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _alloc)



// Forward lifecycle configuration
//!!! AUTOGENERATED:alloc/forward.m.h SRC:POTTERY_VECTOR_ALLOC DEST:POTTERY_ALLOC
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards alloc configuration from POTTERY_VECTOR_ALLOC to POTTERY_ALLOC
    #ifdef POTTERY_VECTOR_ALLOC_CONTEXT_TYPE
        #define POTTERY_ALLOC_CONTEXT_TYPE POTTERY_VECTOR_ALLOC_CONTEXT_TYPE
    #endif

    #ifdef POTTERY_VECTOR_ALLOC_FREE
        #define POTTERY_ALLOC_FREE POTTERY_VECTOR_ALLOC_FREE
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_MALLOC
        #define POTTERY_ALLOC_MALLOC POTTERY_VECTOR_ALLOC_MALLOC
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_ZALLOC
        #define POTTERY_ALLOC_ZALLOC POTTERY_VECTOR_ALLOC_ZALLOC
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_REALLOC
        #define POTTERY_ALLOC_REALLOC POTTERY_VECTOR_ALLOC_REALLOC
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_MALLOC_GOOD_SIZE
        #define POTTERY_ALLOC_GOOD_SIZE POTTERY_VECTOR_ALLOC_MALLOC_USABLE_SIZE
    #endif

    #ifdef POTTERY_VECTOR_ALLOC_EXTENDED_ALIGNMENT
        #define POTTERY_ALLOC_EXTENDED_ALIGNMENT POTTERY_VECTOR_ALLOC_EXTENDED_ALIGNMENT
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_ALIGNED_FREE
        #define POTTERY_ALLOC_ALIGNED_FREE POTTERY_VECTOR_ALLOC_ALIGNED_FREE
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_ALIGNED_MALLOC
        #define POTTERY_ALLOC_ALIGNED_MALLOC POTTERY_VECTOR_ALLOC_ALIGNED_MALLOC
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_ALIGNED_ZALLOC
        #define POTTERY_ALLOC_ALIGNED_ZALLOC POTTERY_VECTOR_ALLOC_ALIGNED_ZALLOC
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_ALIGNED_REALLOC
        #define POTTERY_ALLOC_ALIGNED_REALLOC POTTERY_VECTOR_ALLOC_ALIGNED_REALLOC
    #endif
    #ifdef POTTERY_VECTOR_ALLOC_ALIGNED_MALLOC_GOOD_SIZE
        #define POTTERY_ALLOC_ALIGNED_GOOD_SIZE POTTERY_VECTOR_ALLOC_ALIGNED_MALLOC_USABLE_SIZE
    #endif
//!!! END_AUTOGENERATED
