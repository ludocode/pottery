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

#define POTTERY_LIFECYCLE_PREFIX POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _lifecycle)



// Forward types
//!!! AUTOGENERATED:container/types.m.h SRC:POTTERY_INSERTION_SORT DEST:POTTERY_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Forwards container type configuration from POTTERY_INSERTION_SORT to POTTERY_LIFECYCLE

    // Forward the value or ref
    #if defined(POTTERY_INSERTION_SORT_REF_TYPE) && !defined(POTTERY_INSERTION_SORT_VALUE_TYPE)
        #define POTTERY_LIFECYCLE_REF_TYPE POTTERY_INSERTION_SORT_REF_TYPE
    #elif defined(POTTERY_INSERTION_SORT_VALUE_TYPE) && !defined(POTTERY_INSERTION_SORT_REF_TYPE)
        #define POTTERY_LIFECYCLE_VALUE_TYPE POTTERY_INSERTION_SORT_VALUE_TYPE
    #else
        #error "Exactly one of POTTERY_INSERTION_SORT_REF_TYPE or POTTERY_INSERTION_SORT_VALUE_TYPE must be defined."
    #endif

    // Forward the entry
    #if defined(POTTERY_INSERTION_SORT_ENTRY_TYPE)
        #define POTTERY_LIFECYCLE_ENTRY_TYPE POTTERY_INSERTION_SORT_ENTRY_TYPE
    #endif

    // Forward the entry dereference mechanism
    #if defined(POTTERY_INSERTION_SORT_ENTRY_VALUE)
        #define POTTERY_LIFECYCLE_ENTRY_VALUE POTTERY_INSERTION_SORT_ENTRY_VALUE
    #endif

    // Forward the context
    #ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
        #define POTTERY_LIFECYCLE_CONTEXT_TYPE POTTERY_INSERTION_SORT_CONTEXT_TYPE
    #endif
//!!! END_AUTOGENERATED



// Forward lifecycle configuration
//!!! AUTOGENERATED:lifecycle/forward.m.h SRC:POTTERY_INSERTION_SORT_LIFECYCLE DEST:POTTERY_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Forwards lifecycle configuration from POTTERY_INSERTION_SORT_LIFECYCLE to POTTERY_LIFECYCLE

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_BY_VALUE
        #define POTTERY_LIFECYCLE_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_DESTROY
        #define POTTERY_LIFECYCLE_DESTROY POTTERY_INSERTION_SORT_LIFECYCLE_DESTROY
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_DESTROY_BY_VALUE
        #define POTTERY_LIFECYCLE_DESTROY_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_DESTROY_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_INIT
        #define POTTERY_LIFECYCLE_INIT POTTERY_INSERTION_SORT_LIFECYCLE_INIT
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_INIT_BY_VALUE
        #define POTTERY_LIFECYCLE_INIT_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_INIT_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_INIT_COPY
        #define POTTERY_LIFECYCLE_INIT_COPY POTTERY_INSERTION_SORT_LIFECYCLE_INIT_COPY
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_INIT_COPY_BY_VALUE
        #define POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_INIT_COPY_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_INIT_STEAL
        #define POTTERY_LIFECYCLE_INIT_STEAL POTTERY_INSERTION_SORT_LIFECYCLE_INIT_STEAL
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_INIT_STEAL_BY_VALUE
        #define POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_INIT_STEAL_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_MOVE
        #define POTTERY_LIFECYCLE_MOVE POTTERY_INSERTION_SORT_LIFECYCLE_MOVE
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_MOVE_BY_VALUE
        #define POTTERY_LIFECYCLE_MOVE_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_MOVE_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_COPY
        #define POTTERY_LIFECYCLE_COPY POTTERY_INSERTION_SORT_LIFECYCLE_COPY
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_COPY_BY_VALUE
        #define POTTERY_LIFECYCLE_COPY_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_COPY_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_STEAL
        #define POTTERY_LIFECYCLE_STEAL POTTERY_INSERTION_SORT_LIFECYCLE_STEAL
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_STEAL_BY_VALUE
        #define POTTERY_LIFECYCLE_STEAL_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_STEAL_BY_VALUE
    #endif

    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_SWAP
        #define POTTERY_LIFECYCLE_SWAP POTTERY_INSERTION_SORT_LIFECYCLE_SWAP
    #endif
    #ifdef POTTERY_INSERTION_SORT_LIFECYCLE_SWAP_BY_VALUE
        #define POTTERY_LIFECYCLE_SWAP_BY_VALUE POTTERY_INSERTION_SORT_LIFECYCLE_SWAP_BY_VALUE
    #endif
//!!! END_AUTOGENERATED
