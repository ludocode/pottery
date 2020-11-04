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

#ifndef POTTERY_HEAP_IMPL
#error "This is an internal header. Do not include it."
#endif

// user configuration

#undef POTTERY_HEAP_PREFIX
#undef POTTERY_HEAP_EXTERN
#undef POTTERY_HEAP_USE_MOVE
#undef POTTERY_HEAP_EXTERNAL_LIFECYCLE
#undef POTTERY_HEAP_EXTERNAL_COMPARE
#undef POTTERY_HEAP_CONTEXT_VAL

//!!! AUTOGENERATED:sort/unconfigure.t PREFIX:POTTERY_HEAP
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) common sort configuration on POTTERY_HEAP
    #undef POTTERY_HEAP_REF_TYPE
    #undef POTTERY_HEAP_VALUE_TYPE
    #undef POTTERY_HEAP_CONTEXT_TYPE
    #undef POTTERY_HEAP_ACCESS
    #undef POTTERY_HEAP_BY_VALUE
//!!! END_AUTOGENERATED



// unrename identifiers

#undef POTTERY_HEAP_NAME

// legacy stuff
#undef pottery_access
#undef pottery_value_t
#undef pottery_context_t
#undef pottery_state_t

//!!! AUTOGENERATED:heap/unrename.t PREFIX:pottery_heap
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames heap identifiers with prefix "pottery_heap"

    #undef pottery_heap_ref_t
    #undef pottery_heap_value_t
    #undef pottery_heap_context_t
    #undef pottery_heap_state_t

    #undef pottery_heap
    #undef pottery_heap_access
    #undef pottery_heap_set_index

    #undef pottery_heap_parent
    #undef pottery_heap_child_left
    #undef pottery_heap_child_right
    #undef pottery_heap_sift_down
    #undef pottery_heap_sift_up
    #undef pottery_heap_build_impl
    #undef pottery_heap_push_impl
    #undef pottery_heap_pop_impl
    #undef pottery_heap_remove_impl
    #undef pottery_heap_valid_count_impl
    #undef pottery_heap_build
    #undef pottery_heap_push
    #undef pottery_heap_pop
    #undef pottery_heap_remove
    #undef pottery_heap_valid
    #undef pottery_heap_valid_count
//!!! END_AUTOGENERATED



// lifecycle functions

#undef POTTERY_HEAP_LIFECYCLE_NAME

//!!! AUTOGENERATED:lifecycle/unrename.t PREFIX:pottery_heap_lifecycle
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames lifecycle identifiers with prefix "pottery_heap_lifecycle"

    // types
    #undef pottery_heap_lifecycle_ref_t
    #undef pottery_heap_lifecycle_const_ref_t
    #undef pottery_heap_lifecycle_value_t
    #undef pottery_heap_lifecycle_context_t

    // fundamental operations
    #undef pottery_heap_lifecycle_init_steal
    #undef pottery_heap_lifecycle_init_copy
    #undef pottery_heap_lifecycle_init
    #undef pottery_heap_lifecycle_destroy
    #undef pottery_heap_lifecycle_swap
    #undef pottery_heap_lifecycle_move
    #undef pottery_heap_lifecycle_steal
    #undef pottery_heap_lifecycle_copy

    // bulk operations
    #undef pottery_heap_lifecycle_move_bulk
    #undef pottery_heap_lifecycle_move_bulk_restrict
    #undef pottery_heap_lifecycle_move_bulk_restrict_impl
    #undef pottery_heap_lifecycle_move_bulk_up
    #undef pottery_heap_lifecycle_move_bulk_up_impl
    #undef pottery_heap_lifecycle_move_bulk_down
    #undef pottery_heap_lifecycle_move_bulk_down_impl
    #undef pottery_heap_lifecycle_destroy_bulk
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:lifecycle/unconfigure.t PREFIX:POTTERY_HEAP_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Undefines (forwardable/forwarded) lifecycle configuration on POTTERY_HEAP_LIFECYCLE

    #undef POTTERY_HEAP_LIFECYCLE_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_DESTROY
    #undef POTTERY_HEAP_LIFECYCLE_DESTROY_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_INIT
    #undef POTTERY_HEAP_LIFECYCLE_INIT_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_INIT_COPY
    #undef POTTERY_HEAP_LIFECYCLE_INIT_COPY_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_INIT_STEAL
    #undef POTTERY_HEAP_LIFECYCLE_INIT_STEAL_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_MOVE
    #undef POTTERY_HEAP_LIFECYCLE_MOVE_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_COPY
    #undef POTTERY_HEAP_LIFECYCLE_COPY_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_STEAL
    #undef POTTERY_HEAP_LIFECYCLE_STEAL_BY_VALUE
    #undef POTTERY_HEAP_LIFECYCLE_SWAP
    #undef POTTERY_HEAP_LIFECYCLE_SWAP_BY_VALUE
//!!! END_AUTOGENERATED



// compare

//!!! AUTOGENERATED:compare/unrename.t PREFIX:pottery_heap_compare
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames compare identifiers with prefix "pottery_heap_compare"

    #undef pottery_heap_compare_ref_t
    #undef pottery_heap_compare_value_t
    #undef pottery_heap_compare_context_t

    #undef pottery_heap_compare_three_way
    #undef pottery_heap_compare_equal
    #undef pottery_heap_compare_not_equal
    #undef pottery_heap_compare_less
    #undef pottery_heap_compare_less_or_equal
    #undef pottery_heap_compare_greater
    #undef pottery_heap_compare_greater_or_equal

    #undef pottery_heap_compare_min
    #undef pottery_heap_compare_max
    #undef pottery_heap_compare_clamp
    #undef pottery_heap_compare_median
//!!! END_AUTOGENERATED

#undef POTTERY_HEAP_COMPARE_NAME
//!!! AUTOGENERATED:compare/unconfigure.t PREFIX:POTTERY_HEAP_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) compare configuration on POTTERY_HEAP_COMPARE

    #undef POTTERY_HEAP_COMPARE_REF_TYPE
    #undef POTTERY_HEAP_COMPARE_VALUE_TYPE
    #undef POTTERY_HEAP_COMPARE_CONTEXT_TYPE
    #undef POTTERY_HEAP_COMPARE_STATE_TYPE
    #undef POTTERY_HEAP_COMPARE_STATE_CONTEXT
    #undef POTTERY_HEAP_COMPARE_BY_VALUE

    #undef POTTERY_HEAP_COMPARE_EQUAL
    #undef POTTERY_HEAP_COMPARE_NOT_EQUAL
    #undef POTTERY_HEAP_COMPARE_LESS
    #undef POTTERY_HEAP_COMPARE_LESS_OR_EQUAL
    #undef POTTERY_HEAP_COMPARE_GREATER
    #undef POTTERY_HEAP_COMPARE_GREATER_OR_EQUAL
    #undef POTTERY_HEAP_COMPARE_THREE_WAY
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
