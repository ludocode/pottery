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

#ifndef POTTERY_HEAP_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"

#define POTTERY_CONTAINER_TYPES_PREFIX POTTERY_HEAP_PREFIX

#define POTTERY_CONTAINER_TYPES_NO_CLEANUP



//!!! AUTOGENERATED:container_types/forward.m.h SRC:POTTERY_HEAP DEST:POTTERY_CONTAINER_TYPES
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards container_types configuration from POTTERY_HEAP to POTTERY_CONTAINER_TYPES

    // Forward the types
    #if defined(POTTERY_HEAP_VALUE_TYPE)
        #define POTTERY_CONTAINER_TYPES_VALUE_TYPE POTTERY_HEAP_VALUE_TYPE
    #endif
    #if defined(POTTERY_HEAP_REF_TYPE)
        #define POTTERY_CONTAINER_TYPES_REF_TYPE POTTERY_HEAP_REF_TYPE
    #endif
    #if defined(POTTERY_HEAP_ENTRY_TYPE)
        #define POTTERY_CONTAINER_TYPES_ENTRY_TYPE POTTERY_HEAP_ENTRY_TYPE
    #endif
    #if defined(POTTERY_HEAP_KEY_TYPE)
        #define POTTERY_CONTAINER_TYPES_KEY_TYPE POTTERY_HEAP_KEY_TYPE
    #endif
    #ifdef POTTERY_HEAP_CONTEXT_TYPE
        #define POTTERY_CONTAINER_TYPES_CONTEXT_TYPE POTTERY_HEAP_CONTEXT_TYPE
    #endif

    // Forward the conversion and comparison expressions
    #if defined(POTTERY_HEAP_ENTRY_REF)
        #define POTTERY_CONTAINER_TYPES_ENTRY_REF POTTERY_HEAP_ENTRY_REF
    #endif
    #if defined(POTTERY_HEAP_REF_KEY)
        #define POTTERY_CONTAINER_TYPES_REF_KEY POTTERY_HEAP_REF_KEY
    #endif
    #if defined(POTTERY_HEAP_REF_EQUAL)
        #define POTTERY_CONTAINER_TYPES_REF_EQUAL POTTERY_HEAP_REF_EQUAL
    #endif
//!!! END_AUTOGENERATED
