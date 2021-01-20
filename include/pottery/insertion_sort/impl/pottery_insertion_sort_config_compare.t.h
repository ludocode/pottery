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

#ifndef POTTERY_INSERTION_SORT_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"

#define POTTERY_COMPARE_PREFIX POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _compare)

#define POTTERY_COMPARE_EXTERNAL_CONTAINER_TYPES POTTERY_INSERTION_SORT_PREFIX
#define POTTERY_COMPARE_CLEANUP 0



// Forward types
//!!! AUTOGENERATED:container_types/forward.m.h SRC:POTTERY_INSERTION_SORT DEST:POTTERY_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards container_types configuration from POTTERY_INSERTION_SORT to POTTERY_COMPARE

    // Forward the types
    #if defined(POTTERY_INSERTION_SORT_VALUE_TYPE)
        #define POTTERY_COMPARE_VALUE_TYPE POTTERY_INSERTION_SORT_VALUE_TYPE
    #endif
    #if defined(POTTERY_INSERTION_SORT_REF_TYPE)
        #define POTTERY_COMPARE_REF_TYPE POTTERY_INSERTION_SORT_REF_TYPE
    #endif
    #if defined(POTTERY_INSERTION_SORT_KEY_TYPE)
        #define POTTERY_COMPARE_KEY_TYPE POTTERY_INSERTION_SORT_KEY_TYPE
    #endif
    #ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
        #define POTTERY_COMPARE_CONTEXT_TYPE POTTERY_INSERTION_SORT_CONTEXT_TYPE
    #endif

    // Forward the conversion and comparison expressions
    #if defined(POTTERY_INSERTION_SORT_REF_KEY)
        #define POTTERY_COMPARE_REF_KEY POTTERY_INSERTION_SORT_REF_KEY
    #endif
    #if defined(POTTERY_INSERTION_SORT_REF_EQUAL)
        #define POTTERY_COMPARE_REF_EQUAL POTTERY_INSERTION_SORT_REF_EQUAL
    #endif
//!!! END_AUTOGENERATED



// Forward compare configuration
//!!! AUTOGENERATED:compare/forward.m.h SRC:POTTERY_INSERTION_SORT_COMPARE DEST:POTTERY_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards compare configuration from POTTERY_INSERTION_SORT_COMPARE to POTTERY_COMPARE

    #ifdef POTTERY_INSERTION_SORT_COMPARE_BY_VALUE
        #define POTTERY_COMPARE_BY_VALUE POTTERY_INSERTION_SORT_COMPARE_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_COMPARE_EQUAL
        #define POTTERY_COMPARE_EQUAL POTTERY_INSERTION_SORT_COMPARE_EQUAL
    #endif
    #ifdef POTTERY_INSERTION_SORT_COMPARE_NOT_EQUAL
        #define POTTERY_COMPARE_NOT_EQUAL POTTERY_INSERTION_SORT_COMPARE_NOT_EQUAL
    #endif
    #ifdef POTTERY_INSERTION_SORT_COMPARE_LESS
        #define POTTERY_COMPARE_LESS POTTERY_INSERTION_SORT_COMPARE_LESS
    #endif
    #ifdef POTTERY_INSERTION_SORT_COMPARE_LESS_OR_EQUAL
        #define POTTERY_COMPARE_LESS_OR_EQUAL POTTERY_INSERTION_SORT_COMPARE_LESS_OR_EQUAL
    #endif
    #ifdef POTTERY_INSERTION_SORT_COMPARE_GREATER
        #define POTTERY_COMPARE_GREATER POTTERY_INSERTION_SORT_COMPARE_GREATER
    #endif
    #ifdef POTTERY_INSERTION_SORT_COMPARE_GREATER_OR_EQUAL
        #define POTTERY_COMPARE_GREATER_OR_EQUAL POTTERY_INSERTION_SORT_COMPARE_GREATER_OR_EQUAL
    #endif
    #ifdef POTTERY_INSERTION_SORT_COMPARE_THREE_WAY
        #define POTTERY_COMPARE_THREE_WAY POTTERY_INSERTION_SORT_COMPARE_THREE_WAY
    #endif
//!!! END_AUTOGENERATED
