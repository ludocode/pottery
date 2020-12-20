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

#ifndef POTTERY_HEAP_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif



/*
 * Configuration
 */

#undef POTTERY_HEAP_SORT_PREFIX
#undef POTTERY_HEAP_SORT_EXTERN
#undef POTTERY_HEAP_SORT_USE_MOVE
#undef POTTERY_HEAP_SORT_EXTERNAL_LIFECYCLE
#undef POTTERY_HEAP_SORT_EXTERNAL_COMPARE
#undef POTTERY_HEAP_SORT_CONTEXT_VAL

//!!! AUTOGENERATED:sort/unconfigure.m.h PREFIX:POTTERY_HEAP_SORT
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) common sort configuration on POTTERY_HEAP_SORT
    #undef POTTERY_HEAP_SORT_REF_TYPE
    #undef POTTERY_HEAP_SORT_VALUE_TYPE
    #undef POTTERY_HEAP_SORT_CONTEXT_TYPE
    #undef POTTERY_HEAP_SORT_ACCESS
//!!! END_AUTOGENERATED



/*
 * Renaming
 */

#undef POTTERY_HEAP_SORT_NAME

#undef pottery_heap_sort_ref_t
#undef pottery_heap_sort_value_t
#undef pottery_heap_sort_context_t
#undef pottery_heap_sort_lifecycle_context_t
#undef pottery_heap_sort_compare_context_t

#undef pottery_heap_sort



// heap

#undef POTTERY_HEAP_SORT_HEAP_PREFIX

//!!! AUTOGENERATED:heap/unrename.m.h PREFIX:pottery_heap_sort_heap
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames heap identifiers with prefix "pottery_heap_sort_heap"

    #undef pottery_heap_sort_heap_value_t
    #undef pottery_heap_sort_heap_ref_t
    #undef pottery_heap_sort_heap_entry_t
    #undef pottery_heap_sort_heap_context_t

    #undef pottery_heap_sort_heap_state_t

    #undef pottery_heap_sort_heap_access
    #undef pottery_heap_sort_heap_set_index

    #undef pottery_heap_sort_heap_parent
    #undef pottery_heap_sort_heap_child_left
    #undef pottery_heap_sort_heap_child_right
    #undef pottery_heap_sort_heap_sift_down
    #undef pottery_heap_sort_heap_sift_up
    #undef pottery_heap_sort_heap_build_impl
    #undef pottery_heap_sort_heap_expand_bulk_impl
    #undef pottery_heap_sort_heap_contract_bulk_impl
    #undef pottery_heap_sort_heap_contract_at_impl
    #undef pottery_heap_sort_heap_valid_count_impl
    #undef pottery_heap_sort_heap_build
    #undef pottery_heap_sort_heap_expand_bulk
    #undef pottery_heap_sort_heap_contract_bulk
    #undef pottery_heap_sort_heap_contract_at
    #undef pottery_heap_sort_heap_valid
    #undef pottery_heap_sort_heap_valid_count
//!!! END_AUTOGENERATED



// lifecycle functions

//!!! AUTOGENERATED:lifecycle/unrename.m.h PREFIX:pottery_heap_sort_lifecycle
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames lifecycle identifiers with prefix "pottery_heap_sort_lifecycle"

    #undef pottery_heap_sort_lifecycle_const_ref_t

    #undef pottery_heap_sort_lifecycle_init_steal
    #undef pottery_heap_sort_lifecycle_init_copy
    #undef pottery_heap_sort_lifecycle_init
    #undef pottery_heap_sort_lifecycle_destroy
    #undef pottery_heap_sort_lifecycle_swap
    #undef pottery_heap_sort_lifecycle_move
    #undef pottery_heap_sort_lifecycle_steal
    #undef pottery_heap_sort_lifecycle_copy

    #undef pottery_heap_sort_lifecycle_move_bulk
    #undef pottery_heap_sort_lifecycle_move_bulk_restrict
    #undef pottery_heap_sort_lifecycle_move_bulk_restrict_impl
    #undef pottery_heap_sort_lifecycle_move_bulk_up
    #undef pottery_heap_sort_lifecycle_move_bulk_up_impl
    #undef pottery_heap_sort_lifecycle_move_bulk_down
    #undef pottery_heap_sort_lifecycle_move_bulk_down_impl
    #undef pottery_heap_sort_lifecycle_destroy_bulk
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:lifecycle/unconfigure.m.h PREFIX:POTTERY_HEAP_SORT_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Undefines (forwardable/forwarded) lifecycle configuration on POTTERY_HEAP_SORT_LIFECYCLE

    #undef POTTERY_HEAP_SORT_LIFECYCLE_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_DESTROY
    #undef POTTERY_HEAP_SORT_LIFECYCLE_DESTROY_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_INIT
    #undef POTTERY_HEAP_SORT_LIFECYCLE_INIT_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_INIT_COPY
    #undef POTTERY_HEAP_SORT_LIFECYCLE_INIT_COPY_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_INIT_STEAL
    #undef POTTERY_HEAP_SORT_LIFECYCLE_INIT_STEAL_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_MOVE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_MOVE_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_COPY
    #undef POTTERY_HEAP_SORT_LIFECYCLE_COPY_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_STEAL
    #undef POTTERY_HEAP_SORT_LIFECYCLE_STEAL_BY_VALUE
    #undef POTTERY_HEAP_SORT_LIFECYCLE_SWAP
    #undef POTTERY_HEAP_SORT_LIFECYCLE_SWAP_BY_VALUE
//!!! END_AUTOGENERATED



// compare functions

//!!! AUTOGENERATED:compare/unrename.m.h PREFIX:pottery_heap_sort_compare
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames compare identifiers with prefix "pottery_heap_sort_compare"

    #undef pottery_heap_sort_compare_three_way
    #undef pottery_heap_sort_compare_equal
    #undef pottery_heap_sort_compare_not_equal
    #undef pottery_heap_sort_compare_less
    #undef pottery_heap_sort_compare_less_or_equal
    #undef pottery_heap_sort_compare_greater
    #undef pottery_heap_sort_compare_greater_or_equal

    #undef pottery_heap_sort_compare_min
    #undef pottery_heap_sort_compare_max
    #undef pottery_heap_sort_compare_clamp
    #undef pottery_heap_sort_compare_median
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:compare/unconfigure.m.h PREFIX:POTTERY_HEAP_SORT_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) compare configuration on POTTERY_HEAP_SORT_COMPARE

    #undef POTTERY_HEAP_SORT_COMPARE_BY_VALUE

    #undef POTTERY_HEAP_SORT_COMPARE_EQUAL
    #undef POTTERY_HEAP_SORT_COMPARE_NOT_EQUAL
    #undef POTTERY_HEAP_SORT_COMPARE_LESS
    #undef POTTERY_HEAP_SORT_COMPARE_LESS_OR_EQUAL
    #undef POTTERY_HEAP_SORT_COMPARE_GREATER
    #undef POTTERY_HEAP_SORT_COMPARE_GREATER_OR_EQUAL
    #undef POTTERY_HEAP_SORT_COMPARE_THREE_WAY
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
