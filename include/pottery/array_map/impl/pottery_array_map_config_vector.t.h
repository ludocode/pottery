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

#ifndef POTTERY_ARRAY_MAP_IMPL
#error "This header is internal to Pottery. Do not include it."
#include "pottery_fatal"
#endif

#define POTTERY_VECTOR_PREFIX POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _vector)

#ifdef POTTERY_ARRAY_MAP_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_VECTOR_EXTERNAL_CONTAINER_TYPES POTTERY_ARRAY_MAP_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_VECTOR_EXTERNAL_CONTAINER_TYPES POTTERY_ARRAY_MAP_PREFIX
#endif

#ifdef POTTERY_ARRAY_MAP_EXTERNAL_LIFECYCLE
    #define POTTERY_VECTOR_EXTERNAL_LIFECYCLE POTTERY_ARRAY_MAP_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_VECTOR_EXTERNAL_LIFECYCLE POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _lifecycle)
#endif


//!!! AUTOGENERATED:lifecycle/forward.m.h SRC:POTTERY_ARRAY_MAP_LIFECYCLE DEST:POTTERY_VECTOR_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Forwards lifecycle configuration from POTTERY_ARRAY_MAP_LIFECYCLE to POTTERY_VECTOR_LIFECYCLE

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_DESTROY
        #define POTTERY_VECTOR_LIFECYCLE_DESTROY POTTERY_ARRAY_MAP_LIFECYCLE_DESTROY
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_DESTROY_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_DESTROY_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_DESTROY_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_INIT
        #define POTTERY_VECTOR_LIFECYCLE_INIT POTTERY_ARRAY_MAP_LIFECYCLE_INIT
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_INIT_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_INIT_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_INIT_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_INIT_COPY
        #define POTTERY_VECTOR_LIFECYCLE_INIT_COPY POTTERY_ARRAY_MAP_LIFECYCLE_INIT_COPY
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_INIT_COPY_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_INIT_COPY_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_INIT_COPY_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_INIT_STEAL
        #define POTTERY_VECTOR_LIFECYCLE_INIT_STEAL POTTERY_ARRAY_MAP_LIFECYCLE_INIT_STEAL
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_INIT_STEAL_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_INIT_STEAL_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_INIT_STEAL_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_MOVE
        #define POTTERY_VECTOR_LIFECYCLE_MOVE POTTERY_ARRAY_MAP_LIFECYCLE_MOVE
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_MOVE_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_MOVE_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_MOVE_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_COPY
        #define POTTERY_VECTOR_LIFECYCLE_COPY POTTERY_ARRAY_MAP_LIFECYCLE_COPY
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_COPY_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_COPY_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_COPY_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_STEAL
        #define POTTERY_VECTOR_LIFECYCLE_STEAL POTTERY_ARRAY_MAP_LIFECYCLE_STEAL
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_STEAL_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_STEAL_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_STEAL_BY_VALUE
    #endif

    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_SWAP
        #define POTTERY_VECTOR_LIFECYCLE_SWAP POTTERY_ARRAY_MAP_LIFECYCLE_SWAP
    #endif
    #ifdef POTTERY_ARRAY_MAP_LIFECYCLE_SWAP_BY_VALUE
        #define POTTERY_VECTOR_LIFECYCLE_SWAP_BY_VALUE POTTERY_ARRAY_MAP_LIFECYCLE_SWAP_BY_VALUE
    #endif
//!!! END_AUTOGENERATED