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

#ifndef POTTERY_RING_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"

#ifndef POTTERY_RING_PREFIX
#error "POTTERY_RING_PREFIX is not defined."
#endif
#ifndef POTTERY_RING_VALUE_TYPE
#error "POTTERY_RING_VALUE_TYPE is not defined."
#endif

#ifndef POTTERY_RING_EXTERN
#define POTTERY_RING_EXTERN /*nothing*/
#endif

#define POTTERY_RING_NAME(name) POTTERY_CONCAT(POTTERY_RING_PREFIX, name)

// ring internal functions
#define pottery_ring_sanity_check POTTERY_RING_NAME(_sanity_check)
#define pottery_ring_grow POTTERY_RING_NAME(_grow)
#define pottery_ring_impl_destroy_all POTTERY_RING_NAME(_impl_destroy_all)

// common array types and functions
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_ring DEST:POTTERY_RING_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_ring to POTTERY_RING_PREFIX

    // types
    #define pottery_ring_t POTTERY_CONCAT(POTTERY_RING_PREFIX, _t)
    #define pottery_ring_value_t POTTERY_CONCAT(POTTERY_RING_PREFIX, _value_t)
    #define pottery_ring_ref_t POTTERY_CONCAT(POTTERY_RING_PREFIX, _ref_t)
    #define pottery_ring_entry_t POTTERY_CONCAT(POTTERY_RING_PREFIX, _entry_t)
    #define pottery_ring_context_t POTTERY_CONCAT(POTTERY_RING_PREFIX, _context_t)

    // lifecycle operations
    #define pottery_ring_copy POTTERY_CONCAT(POTTERY_RING_PREFIX, _copy)
    #define pottery_ring_destroy POTTERY_CONCAT(POTTERY_RING_PREFIX, _destroy)
    #define pottery_ring_init POTTERY_CONCAT(POTTERY_RING_PREFIX, _init)
    #define pottery_ring_init_copy POTTERY_CONCAT(POTTERY_RING_PREFIX, _init_copy)
    #define pottery_ring_init_steal POTTERY_CONCAT(POTTERY_RING_PREFIX, _init_steal)
    #define pottery_ring_move POTTERY_CONCAT(POTTERY_RING_PREFIX, _move)
    #define pottery_ring_steal POTTERY_CONCAT(POTTERY_RING_PREFIX, _steal)
    #define pottery_ring_swap POTTERY_CONCAT(POTTERY_RING_PREFIX, _swap)

    // entry operations
    #define pottery_ring_entry_exists POTTERY_CONCAT(POTTERY_RING_PREFIX, _entry_exists)
    #define pottery_ring_entry_equal POTTERY_CONCAT(POTTERY_RING_PREFIX, _entry_equal)
    #define pottery_ring_entry_value POTTERY_CONCAT(POTTERY_RING_PREFIX, _entry_value)

    // lookup
    #define pottery_ring_count POTTERY_CONCAT(POTTERY_RING_PREFIX, _count)
    #define pottery_ring_is_empty POTTERY_CONCAT(POTTERY_RING_PREFIX, _is_empty)
    #define pottery_ring_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _at)
    #define pottery_ring_find POTTERY_CONCAT(POTTERY_RING_PREFIX, _find)
    #define pottery_ring_contains_key POTTERY_CONCAT(POTTERY_RING_PREFIX, _contains_key)
    #define pottery_ring_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _first)
    #define pottery_ring_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _last)
    #define pottery_ring_any POTTERY_CONCAT(POTTERY_RING_PREFIX, _any)

    // array access
    #define pottery_ring_select POTTERY_CONCAT(POTTERY_RING_PREFIX, _select)
    #define pottery_ring_index POTTERY_CONCAT(POTTERY_RING_PREFIX, _index)
    #define pottery_ring_shift POTTERY_CONCAT(POTTERY_RING_PREFIX, _shift)
    #define pottery_ring_offset POTTERY_CONCAT(POTTERY_RING_PREFIX, _offset)
    #define pottery_ring_begin POTTERY_CONCAT(POTTERY_RING_PREFIX, _begin)
    #define pottery_ring_end POTTERY_CONCAT(POTTERY_RING_PREFIX, _end)
    #define pottery_ring_next POTTERY_CONCAT(POTTERY_RING_PREFIX, _next)
    #define pottery_ring_previous POTTERY_CONCAT(POTTERY_RING_PREFIX, _previous)

    // misc
    #define pottery_ring_capacity POTTERY_CONCAT(POTTERY_RING_PREFIX, _capacity)
    #define pottery_ring_reserve POTTERY_CONCAT(POTTERY_RING_PREFIX, _reserve)
    #define pottery_ring_shrink POTTERY_CONCAT(POTTERY_RING_PREFIX, _shrink)

    // insert
    #define pottery_ring_insert POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert)
    #define pottery_ring_insert_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_at)
    #define pottery_ring_insert_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_first)
    #define pottery_ring_insert_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_last)
    #define pottery_ring_insert_before POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_before)
    #define pottery_ring_insert_after POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_after)
    #define pottery_ring_insert_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_bulk)
    #define pottery_ring_insert_at_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_at_bulk)
    #define pottery_ring_insert_first_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_first_bulk)
    #define pottery_ring_insert_last_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_last_bulk)
    #define pottery_ring_insert_before_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_before_bulk)
    #define pottery_ring_insert_after_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_ring_emplace POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace)
    #define pottery_ring_emplace_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_at)
    #define pottery_ring_emplace_key POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_key)
    #define pottery_ring_emplace_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_first)
    #define pottery_ring_emplace_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_last)
    #define pottery_ring_emplace_before POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_before)
    #define pottery_ring_emplace_after POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_after)
    #define pottery_ring_emplace_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_bulk)
    #define pottery_ring_emplace_at_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_at_bulk)
    #define pottery_ring_emplace_first_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_first_bulk)
    #define pottery_ring_emplace_last_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_last_bulk)
    #define pottery_ring_emplace_before_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_before_bulk)
    #define pottery_ring_emplace_after_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_ring_construct POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct)
    #define pottery_ring_construct_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct_at)
    #define pottery_ring_construct_key POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct_key)
    #define pottery_ring_construct_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct_first)
    #define pottery_ring_construct_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct_last)
    #define pottery_ring_construct_before POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct_before)
    #define pottery_ring_construct_after POTTERY_CONCAT(POTTERY_RING_PREFIX, _construct_after)

    // extract
    #define pottery_ring_extract POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract)
    #define pottery_ring_extract_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_at)
    #define pottery_ring_extract_key POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_key)
    #define pottery_ring_extract_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_first)
    #define pottery_ring_extract_first_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_first_bulk)
    #define pottery_ring_extract_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_last)
    #define pottery_ring_extract_last_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_last_bulk)
    #define pottery_ring_extract_and_next POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_and_next)
    #define pottery_ring_extract_and_previous POTTERY_CONCAT(POTTERY_RING_PREFIX, _extract_and_previous)

    // remove
    #define pottery_ring_remove POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove)
    #define pottery_ring_remove_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_at)
    #define pottery_ring_remove_key POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_key)
    #define pottery_ring_remove_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_first)
    #define pottery_ring_remove_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_last)
    #define pottery_ring_remove_and_next POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_and_next)
    #define pottery_ring_remove_and_previous POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_and_previous)
    #define pottery_ring_remove_all POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_all)
    #define pottery_ring_remove_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_bulk)
    #define pottery_ring_remove_at_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_at_bulk)
    #define pottery_ring_remove_first_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_first_bulk)
    #define pottery_ring_remove_last_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_last_bulk)
    #define pottery_ring_remove_at_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_at_bulk)
    #define pottery_ring_remove_and_next_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_and_next_bulk)
    #define pottery_ring_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_ring_displace POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace)
    #define pottery_ring_displace_at POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_at)
    #define pottery_ring_displace_key POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_key)
    #define pottery_ring_displace_first POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_first)
    #define pottery_ring_displace_last POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_last)
    #define pottery_ring_displace_and_next POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_and_next)
    #define pottery_ring_displace_and_previous POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_and_previous)
    #define pottery_ring_displace_all POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_all)
    #define pottery_ring_displace_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_bulk)
    #define pottery_ring_displace_at_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_at_bulk)
    #define pottery_ring_displace_first_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_first_bulk)
    #define pottery_ring_displace_last_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_last_bulk)
    #define pottery_ring_displace_and_next_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_and_next_bulk)
    #define pottery_ring_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_RING_PREFIX, _displace_and_previous_bulk)
//!!! END_AUTOGENERATED



// lifecycle functions
#ifdef POTTERY_RING_EXTERNAL_LIFECYCLE
    #define POTTERY_RING_LIFECYCLE_PREFIX POTTERY_RING_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_RING_LIFECYCLE_PREFIX POTTERY_RING_NAME(_lifecycle)
#endif
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_ring_lifecycle DEST:POTTERY_RING_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_ring_lifecycle to POTTERY_RING_LIFECYCLE_PREFIX

    #define pottery_ring_lifecycle_init_steal POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_ring_lifecycle_init_copy POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_ring_lifecycle_init POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _init)
    #define pottery_ring_lifecycle_destroy POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _destroy)

    #define pottery_ring_lifecycle_swap POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _swap)
    #define pottery_ring_lifecycle_move POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move)
    #define pottery_ring_lifecycle_steal POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _steal)
    #define pottery_ring_lifecycle_copy POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _copy)
    #define pottery_ring_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_ring_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_ring_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_ring_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_ring_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_ring_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_ring_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_ring_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_ring_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_ring_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_ring_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_ring_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_RING_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED



// alloc functions
#ifdef POTTERY_RING_EXTERNAL_ALLOC
    #define POTTERY_RING_ALLOC_PREFIX POTTERY_RING_EXTERNAL_ALLOC
#else
    #define POTTERY_RING_ALLOC_PREFIX POTTERY_RING_NAME(_alloc)
#endif
//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_ring_alloc DEST:POTTERY_RING_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_ring_alloc to POTTERY_RING_ALLOC_PREFIX

    #define pottery_ring_alloc_free POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _free)
    #define pottery_ring_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_ring_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_ring_alloc_malloc POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _malloc)
    #define pottery_ring_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_ring_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_ring_alloc_malloc_zero POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _malloc_zero)
    #define pottery_ring_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_ring_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_ring_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_ring_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_ring_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_ring_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_ring_alloc_malloc_array POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _malloc_array)
    #define pottery_ring_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_RING_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED
