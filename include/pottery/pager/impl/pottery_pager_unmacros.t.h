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

#ifndef POTTERY_PAGER_IMPL
#error "This is an internal header. Do not include it."
#endif

#undef POTTERY_PAGER_NAME
#undef POTTERY_PAGER_PREFIX
#undef POTTERY_PAGER_VALUE_TYPE
#undef POTTERY_PAGER_EXTERN

// public pager-specific functions
#undef pottery_pager_next_bulk
#undef pottery_pager_previous_bulk

// internal types and functions
#undef pottery_pager_page_t
#undef pottery_pager_ref_make
#undef pottery_pager_per_page
#undef pottery_pager_sanity_check
#undef pottery_pager_acquire_page
#undef pottery_pager_release_page
#undef pottery_pager_destroy_all
#undef pottery_pager_set_clear

// common array types and functions
//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_pager
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_pager"

    // types
    #undef pottery_pager_t
    #undef pottery_pager_value_t
    #undef pottery_pager_ref_t
    #undef pottery_pager_entry_t
    #undef pottery_pager_context_t

    // lifecycle operations
    #undef pottery_pager_copy
    #undef pottery_pager_destroy
    #undef pottery_pager_init
    #undef pottery_pager_init_copy
    #undef pottery_pager_init_steal
    #undef pottery_pager_move
    #undef pottery_pager_steal
    #undef pottery_pager_swap

    // entry operations
    #undef pottery_pager_entry_exists
    #undef pottery_pager_entry_equal
    #undef pottery_pager_entry_value

    // lookup
    #undef pottery_pager_count
    #undef pottery_pager_is_empty
    #undef pottery_pager_at
    #undef pottery_pager_find
    #undef pottery_pager_contains_key
    #undef pottery_pager_first
    #undef pottery_pager_last
    #undef pottery_pager_any

    // array access
    #undef pottery_pager_select
    #undef pottery_pager_index
    #undef pottery_pager_shift
    #undef pottery_pager_offset
    #undef pottery_pager_begin
    #undef pottery_pager_end
    #undef pottery_pager_next
    #undef pottery_pager_previous

    // misc
    #undef pottery_pager_capacity
    #undef pottery_pager_reserve
    #undef pottery_pager_shrink

    // insert
    #undef pottery_pager_insert
    #undef pottery_pager_insert_at
    #undef pottery_pager_insert_first
    #undef pottery_pager_insert_last
    #undef pottery_pager_insert_before
    #undef pottery_pager_insert_after
    #undef pottery_pager_insert_bulk
    #undef pottery_pager_insert_at_bulk
    #undef pottery_pager_insert_first_bulk
    #undef pottery_pager_insert_last_bulk
    #undef pottery_pager_insert_before_bulk
    #undef pottery_pager_insert_after_bulk

    // emplace
    #undef pottery_pager_emplace
    #undef pottery_pager_emplace_at
    #undef pottery_pager_emplace_key
    #undef pottery_pager_emplace_first
    #undef pottery_pager_emplace_last
    #undef pottery_pager_emplace_before
    #undef pottery_pager_emplace_after
    #undef pottery_pager_emplace_bulk
    #undef pottery_pager_emplace_at_bulk
    #undef pottery_pager_emplace_first_bulk
    #undef pottery_pager_emplace_last_bulk
    #undef pottery_pager_emplace_before_bulk
    #undef pottery_pager_emplace_after_bulk

    // construct
    #undef pottery_pager_construct
    #undef pottery_pager_construct_at
    #undef pottery_pager_construct_key
    #undef pottery_pager_construct_first
    #undef pottery_pager_construct_last
    #undef pottery_pager_construct_before
    #undef pottery_pager_construct_after

    // extract
    #undef pottery_pager_extract
    #undef pottery_pager_extract_at
    #undef pottery_pager_extract_key
    #undef pottery_pager_extract_first
    #undef pottery_pager_extract_first_bulk
    #undef pottery_pager_extract_last
    #undef pottery_pager_extract_last_bulk
    #undef pottery_pager_extract_and_next
    #undef pottery_pager_extract_and_previous

    // remove
    #undef pottery_pager_remove
    #undef pottery_pager_remove_at
    #undef pottery_pager_remove_key
    #undef pottery_pager_remove_first
    #undef pottery_pager_remove_last
    #undef pottery_pager_remove_and_next
    #undef pottery_pager_remove_and_previous
    #undef pottery_pager_remove_all
    #undef pottery_pager_remove_bulk
    #undef pottery_pager_remove_at_bulk
    #undef pottery_pager_remove_first_bulk
    #undef pottery_pager_remove_last_bulk
    #undef pottery_pager_remove_at_bulk
    #undef pottery_pager_remove_and_next_bulk
    #undef pottery_pager_remove_and_previous_bulk

    // displace
    #undef pottery_pager_displace
    #undef pottery_pager_displace_at
    #undef pottery_pager_displace_key
    #undef pottery_pager_displace_first
    #undef pottery_pager_displace_last
    #undef pottery_pager_displace_and_next
    #undef pottery_pager_displace_and_previous
    #undef pottery_pager_displace_all
    #undef pottery_pager_displace_bulk
    #undef pottery_pager_displace_at_bulk
    #undef pottery_pager_displace_first_bulk
    #undef pottery_pager_displace_last_bulk
    #undef pottery_pager_displace_and_next_bulk
    #undef pottery_pager_displace_and_previous_bulk
//!!! END_AUTOGENERATED


// ring
#undef pottery_pager_page_ring_t

//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_pager_page_ring
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_pager_page_ring"

    // types
    #undef pottery_pager_page_ring_t
    #undef pottery_pager_page_ring_value_t
    #undef pottery_pager_page_ring_ref_t
    #undef pottery_pager_page_ring_entry_t
    #undef pottery_pager_page_ring_context_t

    // lifecycle operations
    #undef pottery_pager_page_ring_copy
    #undef pottery_pager_page_ring_destroy
    #undef pottery_pager_page_ring_init
    #undef pottery_pager_page_ring_init_copy
    #undef pottery_pager_page_ring_init_steal
    #undef pottery_pager_page_ring_move
    #undef pottery_pager_page_ring_steal
    #undef pottery_pager_page_ring_swap

    // entry operations
    #undef pottery_pager_page_ring_entry_exists
    #undef pottery_pager_page_ring_entry_equal
    #undef pottery_pager_page_ring_entry_value

    // lookup
    #undef pottery_pager_page_ring_count
    #undef pottery_pager_page_ring_is_empty
    #undef pottery_pager_page_ring_at
    #undef pottery_pager_page_ring_find
    #undef pottery_pager_page_ring_contains_key
    #undef pottery_pager_page_ring_first
    #undef pottery_pager_page_ring_last
    #undef pottery_pager_page_ring_any

    // array access
    #undef pottery_pager_page_ring_select
    #undef pottery_pager_page_ring_index
    #undef pottery_pager_page_ring_shift
    #undef pottery_pager_page_ring_offset
    #undef pottery_pager_page_ring_begin
    #undef pottery_pager_page_ring_end
    #undef pottery_pager_page_ring_next
    #undef pottery_pager_page_ring_previous

    // misc
    #undef pottery_pager_page_ring_capacity
    #undef pottery_pager_page_ring_reserve
    #undef pottery_pager_page_ring_shrink

    // insert
    #undef pottery_pager_page_ring_insert
    #undef pottery_pager_page_ring_insert_at
    #undef pottery_pager_page_ring_insert_first
    #undef pottery_pager_page_ring_insert_last
    #undef pottery_pager_page_ring_insert_before
    #undef pottery_pager_page_ring_insert_after
    #undef pottery_pager_page_ring_insert_bulk
    #undef pottery_pager_page_ring_insert_at_bulk
    #undef pottery_pager_page_ring_insert_first_bulk
    #undef pottery_pager_page_ring_insert_last_bulk
    #undef pottery_pager_page_ring_insert_before_bulk
    #undef pottery_pager_page_ring_insert_after_bulk

    // emplace
    #undef pottery_pager_page_ring_emplace
    #undef pottery_pager_page_ring_emplace_at
    #undef pottery_pager_page_ring_emplace_key
    #undef pottery_pager_page_ring_emplace_first
    #undef pottery_pager_page_ring_emplace_last
    #undef pottery_pager_page_ring_emplace_before
    #undef pottery_pager_page_ring_emplace_after
    #undef pottery_pager_page_ring_emplace_bulk
    #undef pottery_pager_page_ring_emplace_at_bulk
    #undef pottery_pager_page_ring_emplace_first_bulk
    #undef pottery_pager_page_ring_emplace_last_bulk
    #undef pottery_pager_page_ring_emplace_before_bulk
    #undef pottery_pager_page_ring_emplace_after_bulk

    // construct
    #undef pottery_pager_page_ring_construct
    #undef pottery_pager_page_ring_construct_at
    #undef pottery_pager_page_ring_construct_key
    #undef pottery_pager_page_ring_construct_first
    #undef pottery_pager_page_ring_construct_last
    #undef pottery_pager_page_ring_construct_before
    #undef pottery_pager_page_ring_construct_after

    // extract
    #undef pottery_pager_page_ring_extract
    #undef pottery_pager_page_ring_extract_at
    #undef pottery_pager_page_ring_extract_key
    #undef pottery_pager_page_ring_extract_first
    #undef pottery_pager_page_ring_extract_first_bulk
    #undef pottery_pager_page_ring_extract_last
    #undef pottery_pager_page_ring_extract_last_bulk
    #undef pottery_pager_page_ring_extract_and_next
    #undef pottery_pager_page_ring_extract_and_previous

    // remove
    #undef pottery_pager_page_ring_remove
    #undef pottery_pager_page_ring_remove_at
    #undef pottery_pager_page_ring_remove_key
    #undef pottery_pager_page_ring_remove_first
    #undef pottery_pager_page_ring_remove_last
    #undef pottery_pager_page_ring_remove_and_next
    #undef pottery_pager_page_ring_remove_and_previous
    #undef pottery_pager_page_ring_remove_all
    #undef pottery_pager_page_ring_remove_bulk
    #undef pottery_pager_page_ring_remove_at_bulk
    #undef pottery_pager_page_ring_remove_first_bulk
    #undef pottery_pager_page_ring_remove_last_bulk
    #undef pottery_pager_page_ring_remove_at_bulk
    #undef pottery_pager_page_ring_remove_and_next_bulk
    #undef pottery_pager_page_ring_remove_and_previous_bulk

    // displace
    #undef pottery_pager_page_ring_displace
    #undef pottery_pager_page_ring_displace_at
    #undef pottery_pager_page_ring_displace_key
    #undef pottery_pager_page_ring_displace_first
    #undef pottery_pager_page_ring_displace_last
    #undef pottery_pager_page_ring_displace_and_next
    #undef pottery_pager_page_ring_displace_and_previous
    #undef pottery_pager_page_ring_displace_all
    #undef pottery_pager_page_ring_displace_bulk
    #undef pottery_pager_page_ring_displace_at_bulk
    #undef pottery_pager_page_ring_displace_first_bulk
    #undef pottery_pager_page_ring_displace_last_bulk
    #undef pottery_pager_page_ring_displace_and_next_bulk
    #undef pottery_pager_page_ring_displace_and_previous_bulk
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
