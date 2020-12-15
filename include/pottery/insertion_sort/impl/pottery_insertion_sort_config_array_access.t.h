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

#ifndef POTTERY_INSERTION_SORT_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"

#ifndef POTTERY_INSERTION_SORT_EXTERNAL_ARRAY_ACCESS



#define POTTERY_ARRAY_ACCESS_PREFIX POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _array_access)

// Forward the ref and value
#if defined(POTTERY_INSERTION_SORT_VALUE_TYPE)
    #define POTTERY_ARRAY_ACCESS_VALUE_TYPE POTTERY_INSERTION_SORT_VALUE_TYPE
#endif
#if defined(POTTERY_INSERTION_SORT_REF_TYPE)
    #define POTTERY_ARRAY_ACCESS_REF_TYPE POTTERY_INSERTION_SORT_REF_TYPE
#endif
#if defined(POTTERY_INSERTION_SORT_REF_VALUE)
    #define POTTERY_ARRAY_ACCESS_REF_VALUE POTTERY_INSERTION_SORT_REF_VALUE
#endif
#if !defined(POTTERY_INSERTION_SORT_REF_TYPE) && !defined(POTTERY_INSERTION_SORT_VALUE_TYPE)
    #error "At least one of POTTERY_INSERTION_SORT_REF_TYPE and POTTERY_INSERTION_SORT_VALUE_TYPE must be defined."
#endif

// Forward the context
#ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
    #define POTTERY_ARRAY_ACCESS_CONTEXT_TYPE POTTERY_INSERTION_SORT_CONTEXT_TYPE
#endif



// Forward array_access configuration
//!!! AUTOGENERATED:array_access/forward.m.h SRC:POTTERY_INSERTION_SORT_ARRAY_ACCESS DEST:POTTERY_ARRAY_ACCESS
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards array_access configuration from POTTERY_INSERTION_SORT_ARRAY_ACCESS to POTTERY_ARRAY_ACCESS

    // absolute addressing configs
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_BEGIN
        #define POTTERY_ARRAY_ACCESS_BEGIN POTTERY_INSERTION_SORT_ARRAY_ACCESS_BEGIN
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_END
        #define POTTERY_ARRAY_ACCESS_END POTTERY_INSERTION_SORT_ARRAY_ACCESS_END
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_COUNT
        #define POTTERY_ARRAY_ACCESS_COUNT POTTERY_INSERTION_SORT_ARRAY_ACCESS_COUNT
    #endif

    // required configs for non-standard array access
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_SELECT
        #define POTTERY_ARRAY_ACCESS_SELECT POTTERY_INSERTION_SORT_ARRAY_ACCESS_SELECT
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_INDEX
        #define POTTERY_ARRAY_ACCESS_INDEX POTTERY_INSERTION_SORT_ARRAY_ACCESS_INDEX
    #endif

    // required config for non-trivial refs
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_EQUAL
        #define POTTERY_ARRAY_ACCESS_EQUAL POTTERY_INSERTION_SORT_ARRAY_ACCESS_EQUAL
    #endif

    // optional configs
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_SHIFT
        #define POTTERY_ARRAY_ACCESS_SHIFT POTTERY_INSERTION_SORT_ARRAY_ACCESS_SHIFT
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_OFFSET
        #define POTTERY_ARRAY_ACCESS_OFFSET POTTERY_INSERTION_SORT_ARRAY_ACCESS_OFFSET
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_NEXT
        #define POTTERY_ARRAY_ACCESS_NEXT POTTERY_INSERTION_SORT_ARRAY_ACCESS_NEXT
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREVIOUS
        #define POTTERY_ARRAY_ACCESS_PREVIOUS POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREVIOUS
    #endif
    #ifdef POTTERY_INSERTION_SORT_ARRAY_ACCESS_EXISTS
        #define POTTERY_ARRAY_ACCESS_EXISTS POTTERY_INSERTION_SORT_ARRAY_ACCESS_EXISTS
    #endif
//!!! END_AUTOGENERATED



#endif // !defined(POTTERY_INSERTION_SORT_EXTERNAL_ARRAY_ACCESS)
