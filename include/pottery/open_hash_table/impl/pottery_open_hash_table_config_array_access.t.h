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

#ifndef POTTERY_OPEN_HASH_TABLE_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"

#define POTTERY_ARRAY_ACCESS_PREFIX POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _array_access)

#ifdef POTTERY_OPEN_HASH_TABLE_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_ARRAY_ACCESS_EXTERNAL_CONTAINER_TYPES POTTERY_OPEN_HASH_TABLE_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_ARRAY_ACCESS_EXTERNAL_CONTAINER_TYPES POTTERY_OPEN_HASH_TABLE_PREFIX
#endif

#define POTTERY_ARRAY_ACCESS_CLEANUP 0



// Forward types
//!!! AUTOGENERATED:container_types/forward.m.h SRC:POTTERY_OPEN_HASH_TABLE DEST:POTTERY_ARRAY_ACCESS
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards container_types configuration from POTTERY_OPEN_HASH_TABLE to POTTERY_ARRAY_ACCESS

    // Forward the types
    #if defined(POTTERY_OPEN_HASH_TABLE_VALUE_TYPE)
        #define POTTERY_ARRAY_ACCESS_VALUE_TYPE POTTERY_OPEN_HASH_TABLE_VALUE_TYPE
    #endif
    #if defined(POTTERY_OPEN_HASH_TABLE_REF_TYPE)
        #define POTTERY_ARRAY_ACCESS_REF_TYPE POTTERY_OPEN_HASH_TABLE_REF_TYPE
    #endif
    #if defined(POTTERY_OPEN_HASH_TABLE_KEY_TYPE)
        #define POTTERY_ARRAY_ACCESS_KEY_TYPE POTTERY_OPEN_HASH_TABLE_KEY_TYPE
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
        #define POTTERY_ARRAY_ACCESS_CONTEXT_TYPE POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
    #endif

    // Forward the conversion and comparison expressions
    #if defined(POTTERY_OPEN_HASH_TABLE_REF_KEY)
        #define POTTERY_ARRAY_ACCESS_REF_KEY POTTERY_OPEN_HASH_TABLE_REF_KEY
    #endif
    #if defined(POTTERY_OPEN_HASH_TABLE_REF_EQUAL)
        #define POTTERY_ARRAY_ACCESS_REF_EQUAL POTTERY_OPEN_HASH_TABLE_REF_EQUAL
    #endif
//!!! END_AUTOGENERATED



// Forward array_access configuration
//!!! AUTOGENERATED:array_access/forward.m.h SRC:POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS DEST:POTTERY_ARRAY_ACCESS
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards array_access configuration from POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS to POTTERY_ARRAY_ACCESS

    // entry type
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_ENTRY_TYPE
        #define POTTERY_ARRAY_ACCESS_ENTRY_TYPE POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_ENTRY_TYPE
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_ENTRY_REF
        #define POTTERY_ARRAY_ACCESS_ENTRY_REF POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_ENTRY_REF
    #endif

    // absolute addressing configs
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_BEGIN
        #define POTTERY_ARRAY_ACCESS_BEGIN POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_BEGIN
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_END
        #define POTTERY_ARRAY_ACCESS_END POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_END
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_COUNT
        #define POTTERY_ARRAY_ACCESS_COUNT POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_COUNT
    #endif

    // required configs for non-standard array access
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_SELECT
        #define POTTERY_ARRAY_ACCESS_SELECT POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_SELECT
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_INDEX
        #define POTTERY_ARRAY_ACCESS_INDEX POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_INDEX
    #endif

    // required config for non-trivial entries
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_EQUAL
        #define POTTERY_ARRAY_ACCESS_EQUAL POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_EQUAL
    #endif

    // optional configs
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_SHIFT
        #define POTTERY_ARRAY_ACCESS_SHIFT POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_SHIFT
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_OFFSET
        #define POTTERY_ARRAY_ACCESS_OFFSET POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_OFFSET
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_NEXT
        #define POTTERY_ARRAY_ACCESS_NEXT POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_NEXT
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREVIOUS
        #define POTTERY_ARRAY_ACCESS_PREVIOUS POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREVIOUS
    #endif
    #ifdef POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_EXISTS
        #define POTTERY_ARRAY_ACCESS_EXISTS POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_EXISTS
    #endif
//!!! END_AUTOGENERATED
