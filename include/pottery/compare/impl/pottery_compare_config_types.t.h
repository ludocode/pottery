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

#ifndef POTTERY_COMPARE_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#define POTTERY_CONTAINER_TYPES_PREFIX POTTERY_COMPARE_PREFIX

#define POTTERY_CONTAINER_TYPES_CLEANUP 0



//!!! AUTOGENERATED:container_types/forward.m.h SRC:POTTERY_COMPARE DEST:POTTERY_CONTAINER_TYPES
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards container_types configuration from POTTERY_COMPARE to POTTERY_CONTAINER_TYPES

    // Forward the types
    #if defined(POTTERY_COMPARE_VALUE_TYPE)
        #define POTTERY_CONTAINER_TYPES_VALUE_TYPE POTTERY_COMPARE_VALUE_TYPE
    #endif
    #if defined(POTTERY_COMPARE_REF_TYPE)
        #define POTTERY_CONTAINER_TYPES_REF_TYPE POTTERY_COMPARE_REF_TYPE
    #endif
    #if defined(POTTERY_COMPARE_ENTRY_TYPE)
        #define POTTERY_CONTAINER_TYPES_ENTRY_TYPE POTTERY_COMPARE_ENTRY_TYPE
    #endif
    #if defined(POTTERY_COMPARE_KEY_TYPE)
        #define POTTERY_CONTAINER_TYPES_KEY_TYPE POTTERY_COMPARE_KEY_TYPE
    #endif
    #ifdef POTTERY_COMPARE_CONTEXT_TYPE
        #define POTTERY_CONTAINER_TYPES_CONTEXT_TYPE POTTERY_COMPARE_CONTEXT_TYPE
    #endif

    // Forward the conversion and comparison expressions
    #if defined(POTTERY_COMPARE_ENTRY_REF)
        #define POTTERY_CONTAINER_TYPES_ENTRY_REF POTTERY_COMPARE_ENTRY_REF
    #endif
    #if defined(POTTERY_COMPARE_REF_KEY)
        #define POTTERY_CONTAINER_TYPES_REF_KEY POTTERY_COMPARE_REF_KEY
    #endif
    #if defined(POTTERY_COMPARE_REF_EQUAL)
        #define POTTERY_CONTAINER_TYPES_REF_EQUAL POTTERY_COMPARE_REF_EQUAL
    #endif
//!!! END_AUTOGENERATED
