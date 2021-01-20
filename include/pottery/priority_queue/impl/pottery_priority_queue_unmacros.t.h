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

#ifndef POTTERY_PRIORITY_QUEUE_IMPL
#error "This is an internal header. Do not include it."
#endif

#undef pottery_pq_t // TODO remove, should be part of container meta
#undef pottery_pq_value_t // TODO remove

// internal funcs
#undef pottery_pq_set_index
#undef pottery_pq_heap_build
#undef pottery_pq_heap_expand_bulk
#undef pottery_pq_heap_contract_at

#undef POTTERY_PRIORITY_QUEUE_NAME
#undef POTTERY_PRIORITY_QUEUE_NAME2
#undef POTTERY_PRIORITY_QUEUE_NAME3

#undef POTTERY_PRIORITY_QUEUE_PREFIX
#undef POTTERY_PRIORITY_QUEUE_SET_INDEX
#undef POTTERY_PRIORITY_QUEUE_EXTERN



// container types

#undef POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX
//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_pq
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_pq

    #undef pottery_pq_value_t
    #undef pottery_pq_ref_t
    #undef pottery_pq_const_ref_t
    #undef pottery_pq_key_t
    #undef pottery_pq_context_t

    #undef pottery_pq_ref_key
    #undef pottery_pq_ref_equal
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_PRIORITY_QUEUE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_PRIORITY_QUEUE

    // types
    #undef POTTERY_PRIORITY_QUEUE_VALUE_TYPE
    #undef POTTERY_PRIORITY_QUEUE_REF_TYPE
    #undef POTTERY_PRIORITY_QUEUE_ENTRY_TYPE
    #undef POTTERY_PRIORITY_QUEUE_KEY_TYPE
    #undef POTTERY_PRIORITY_QUEUE_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_PRIORITY_QUEUE_ENTRY_REF
    #undef POTTERY_PRIORITY_QUEUE_REF_KEY
    #undef POTTERY_PRIORITY_QUEUE_REF_EQUAL
//!!! END_AUTOGENERATED



// lifecycle

//!!! AUTOGENERATED:lifecycle/unconfigure.m.h PREFIX:POTTERY_PRIORITY_QUEUE_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Undefines (forwardable/forwarded) lifecycle configuration on POTTERY_PRIORITY_QUEUE_LIFECYCLE

    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_DESTROY
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_DESTROY_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_INIT
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_INIT_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_INIT_COPY
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_INIT_COPY_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_INIT_STEAL
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_INIT_STEAL_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_MOVE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_MOVE_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_COPY
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_COPY_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_STEAL
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_STEAL_BY_VALUE
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_SWAP
    #undef POTTERY_PRIORITY_QUEUE_LIFECYCLE_SWAP_BY_VALUE
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:lifecycle/unrename.m.h PREFIX:pottery_pq_element
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames lifecycle identifiers with prefix "pottery_pq_element"

    #undef pottery_pq_element_init_steal
    #undef pottery_pq_element_init_copy
    #undef pottery_pq_element_init
    #undef pottery_pq_element_destroy
    #undef pottery_pq_element_swap
    #undef pottery_pq_element_move
    #undef pottery_pq_element_steal
    #undef pottery_pq_element_copy

    #undef pottery_pq_element_move_bulk
    #undef pottery_pq_element_move_bulk_restrict
    #undef pottery_pq_element_move_bulk_restrict_impl
    #undef pottery_pq_element_move_bulk_up
    #undef pottery_pq_element_move_bulk_up_impl
    #undef pottery_pq_element_move_bulk_down
    #undef pottery_pq_element_move_bulk_down_impl
    #undef pottery_pq_element_destroy_bulk
//!!! END_AUTOGENERATED



// compare

#undef POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX
//!!! AUTOGENERATED:compare/unrename.m.h PREFIX:pottery_pq_compare
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames compare identifiers with prefix "pottery_pq_compare"

    #undef pottery_pq_compare_three_way
    #undef pottery_pq_compare_equal
    #undef pottery_pq_compare_not_equal
    #undef pottery_pq_compare_less
    #undef pottery_pq_compare_less_or_equal
    #undef pottery_pq_compare_greater
    #undef pottery_pq_compare_greater_or_equal

    #undef pottery_pq_compare_min
    #undef pottery_pq_compare_max
    #undef pottery_pq_compare_clamp
    #undef pottery_pq_compare_median
//!!! END_AUTOGENERATED

#undef POTTERY_PRIORITY_QUEUE_COMPARE_NAME
//!!! AUTOGENERATED:compare/unconfigure.m.h PREFIX:POTTERY_PRIORITY_QUEUE_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) compare configuration on POTTERY_PRIORITY_QUEUE_COMPARE

    #undef POTTERY_PRIORITY_QUEUE_COMPARE_EQUAL
    #undef POTTERY_PRIORITY_QUEUE_COMPARE_NOT_EQUAL
    #undef POTTERY_PRIORITY_QUEUE_COMPARE_LESS
    #undef POTTERY_PRIORITY_QUEUE_COMPARE_LESS_OR_EQUAL
    #undef POTTERY_PRIORITY_QUEUE_COMPARE_GREATER
    #undef POTTERY_PRIORITY_QUEUE_COMPARE_GREATER_OR_EQUAL
    #undef POTTERY_PRIORITY_QUEUE_COMPARE_THREE_WAY
//!!! END_AUTOGENERATED



// priority_queue container funcs
//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_pq
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_pq"

    // types
    #undef pottery_pq_t
    #undef pottery_pq_value_t
    #undef pottery_pq_ref_t
    #undef pottery_pq_entry_t
    #undef pottery_pq_context_t

    // lifecycle operations
    #undef pottery_pq_copy
    #undef pottery_pq_destroy
    #undef pottery_pq_init
    #undef pottery_pq_init_copy
    #undef pottery_pq_init_steal
    #undef pottery_pq_move
    #undef pottery_pq_steal
    #undef pottery_pq_swap

    // entry operations
    #undef pottery_pq_entry_exists
    #undef pottery_pq_entry_equal
    #undef pottery_pq_entry_value

    // lookup
    #undef pottery_pq_count
    #undef pottery_pq_is_empty
    #undef pottery_pq_at
    #undef pottery_pq_find
    #undef pottery_pq_contains_key
    #undef pottery_pq_first
    #undef pottery_pq_last
    #undef pottery_pq_any

    // array access
    #undef pottery_pq_select
    #undef pottery_pq_index
    #undef pottery_pq_shift
    #undef pottery_pq_offset
    #undef pottery_pq_begin
    #undef pottery_pq_end
    #undef pottery_pq_next
    #undef pottery_pq_previous

    // misc
    #undef pottery_pq_capacity
    #undef pottery_pq_reserve
    #undef pottery_pq_shrink

    // insert
    #undef pottery_pq_insert
    #undef pottery_pq_insert_at
    #undef pottery_pq_insert_first
    #undef pottery_pq_insert_last
    #undef pottery_pq_insert_before
    #undef pottery_pq_insert_after
    #undef pottery_pq_insert_bulk
    #undef pottery_pq_insert_at_bulk
    #undef pottery_pq_insert_first_bulk
    #undef pottery_pq_insert_last_bulk
    #undef pottery_pq_insert_before_bulk
    #undef pottery_pq_insert_after_bulk

    // emplace
    #undef pottery_pq_emplace
    #undef pottery_pq_emplace_at
    #undef pottery_pq_emplace_key
    #undef pottery_pq_emplace_first
    #undef pottery_pq_emplace_last
    #undef pottery_pq_emplace_before
    #undef pottery_pq_emplace_after
    #undef pottery_pq_emplace_bulk
    #undef pottery_pq_emplace_at_bulk
    #undef pottery_pq_emplace_first_bulk
    #undef pottery_pq_emplace_last_bulk
    #undef pottery_pq_emplace_before_bulk
    #undef pottery_pq_emplace_after_bulk

    // construct
    #undef pottery_pq_construct
    #undef pottery_pq_construct_at
    #undef pottery_pq_construct_key
    #undef pottery_pq_construct_first
    #undef pottery_pq_construct_last
    #undef pottery_pq_construct_before
    #undef pottery_pq_construct_after

    // extract
    #undef pottery_pq_extract
    #undef pottery_pq_extract_at
    #undef pottery_pq_extract_key
    #undef pottery_pq_extract_first
    #undef pottery_pq_extract_first_bulk
    #undef pottery_pq_extract_last
    #undef pottery_pq_extract_last_bulk
    #undef pottery_pq_extract_and_next
    #undef pottery_pq_extract_and_previous

    // remove
    #undef pottery_pq_remove
    #undef pottery_pq_remove_at
    #undef pottery_pq_remove_key
    #undef pottery_pq_remove_first
    #undef pottery_pq_remove_last
    #undef pottery_pq_remove_and_next
    #undef pottery_pq_remove_and_previous
    #undef pottery_pq_remove_all
    #undef pottery_pq_remove_bulk
    #undef pottery_pq_remove_at_bulk
    #undef pottery_pq_remove_first_bulk
    #undef pottery_pq_remove_last_bulk
    #undef pottery_pq_remove_at_bulk
    #undef pottery_pq_remove_and_next_bulk
    #undef pottery_pq_remove_and_previous_bulk

    // displace
    #undef pottery_pq_displace
    #undef pottery_pq_displace_at
    #undef pottery_pq_displace_key
    #undef pottery_pq_displace_first
    #undef pottery_pq_displace_last
    #undef pottery_pq_displace_and_next
    #undef pottery_pq_displace_and_previous
    #undef pottery_pq_displace_all
    #undef pottery_pq_displace_bulk
    #undef pottery_pq_displace_at_bulk
    #undef pottery_pq_displace_first_bulk
    #undef pottery_pq_displace_last_bulk
    #undef pottery_pq_displace_and_next_bulk
    #undef pottery_pq_displace_and_previous_bulk
//!!! END_AUTOGENERATED



// vector wrappers
#undef pottery_pq_array_t
//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_pq_array
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_pq_array"

    // types
    #undef pottery_pq_array_t
    #undef pottery_pq_array_value_t
    #undef pottery_pq_array_ref_t
    #undef pottery_pq_array_entry_t
    #undef pottery_pq_array_context_t

    // lifecycle operations
    #undef pottery_pq_array_copy
    #undef pottery_pq_array_destroy
    #undef pottery_pq_array_init
    #undef pottery_pq_array_init_copy
    #undef pottery_pq_array_init_steal
    #undef pottery_pq_array_move
    #undef pottery_pq_array_steal
    #undef pottery_pq_array_swap

    // entry operations
    #undef pottery_pq_array_entry_exists
    #undef pottery_pq_array_entry_equal
    #undef pottery_pq_array_entry_value

    // lookup
    #undef pottery_pq_array_count
    #undef pottery_pq_array_is_empty
    #undef pottery_pq_array_at
    #undef pottery_pq_array_find
    #undef pottery_pq_array_contains_key
    #undef pottery_pq_array_first
    #undef pottery_pq_array_last
    #undef pottery_pq_array_any

    // array access
    #undef pottery_pq_array_select
    #undef pottery_pq_array_index
    #undef pottery_pq_array_shift
    #undef pottery_pq_array_offset
    #undef pottery_pq_array_begin
    #undef pottery_pq_array_end
    #undef pottery_pq_array_next
    #undef pottery_pq_array_previous

    // misc
    #undef pottery_pq_array_capacity
    #undef pottery_pq_array_reserve
    #undef pottery_pq_array_shrink

    // insert
    #undef pottery_pq_array_insert
    #undef pottery_pq_array_insert_at
    #undef pottery_pq_array_insert_first
    #undef pottery_pq_array_insert_last
    #undef pottery_pq_array_insert_before
    #undef pottery_pq_array_insert_after
    #undef pottery_pq_array_insert_bulk
    #undef pottery_pq_array_insert_at_bulk
    #undef pottery_pq_array_insert_first_bulk
    #undef pottery_pq_array_insert_last_bulk
    #undef pottery_pq_array_insert_before_bulk
    #undef pottery_pq_array_insert_after_bulk

    // emplace
    #undef pottery_pq_array_emplace
    #undef pottery_pq_array_emplace_at
    #undef pottery_pq_array_emplace_key
    #undef pottery_pq_array_emplace_first
    #undef pottery_pq_array_emplace_last
    #undef pottery_pq_array_emplace_before
    #undef pottery_pq_array_emplace_after
    #undef pottery_pq_array_emplace_bulk
    #undef pottery_pq_array_emplace_at_bulk
    #undef pottery_pq_array_emplace_first_bulk
    #undef pottery_pq_array_emplace_last_bulk
    #undef pottery_pq_array_emplace_before_bulk
    #undef pottery_pq_array_emplace_after_bulk

    // construct
    #undef pottery_pq_array_construct
    #undef pottery_pq_array_construct_at
    #undef pottery_pq_array_construct_key
    #undef pottery_pq_array_construct_first
    #undef pottery_pq_array_construct_last
    #undef pottery_pq_array_construct_before
    #undef pottery_pq_array_construct_after

    // extract
    #undef pottery_pq_array_extract
    #undef pottery_pq_array_extract_at
    #undef pottery_pq_array_extract_key
    #undef pottery_pq_array_extract_first
    #undef pottery_pq_array_extract_first_bulk
    #undef pottery_pq_array_extract_last
    #undef pottery_pq_array_extract_last_bulk
    #undef pottery_pq_array_extract_and_next
    #undef pottery_pq_array_extract_and_previous

    // remove
    #undef pottery_pq_array_remove
    #undef pottery_pq_array_remove_at
    #undef pottery_pq_array_remove_key
    #undef pottery_pq_array_remove_first
    #undef pottery_pq_array_remove_last
    #undef pottery_pq_array_remove_and_next
    #undef pottery_pq_array_remove_and_previous
    #undef pottery_pq_array_remove_all
    #undef pottery_pq_array_remove_bulk
    #undef pottery_pq_array_remove_at_bulk
    #undef pottery_pq_array_remove_first_bulk
    #undef pottery_pq_array_remove_last_bulk
    #undef pottery_pq_array_remove_at_bulk
    #undef pottery_pq_array_remove_and_next_bulk
    #undef pottery_pq_array_remove_and_previous_bulk

    // displace
    #undef pottery_pq_array_displace
    #undef pottery_pq_array_displace_at
    #undef pottery_pq_array_displace_key
    #undef pottery_pq_array_displace_first
    #undef pottery_pq_array_displace_last
    #undef pottery_pq_array_displace_and_next
    #undef pottery_pq_array_displace_and_previous
    #undef pottery_pq_array_displace_all
    #undef pottery_pq_array_displace_bulk
    #undef pottery_pq_array_displace_at_bulk
    #undef pottery_pq_array_displace_first_bulk
    #undef pottery_pq_array_displace_last_bulk
    #undef pottery_pq_array_displace_and_next_bulk
    #undef pottery_pq_array_displace_and_previous_bulk
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
