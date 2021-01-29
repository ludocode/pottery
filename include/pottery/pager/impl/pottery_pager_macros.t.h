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

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"

#ifndef POTTERY_PAGER_PREFIX
#error "POTTERY_PAGER_PREFIX is not defined."
#endif
#ifndef POTTERY_PAGER_VALUE_TYPE
#error "POTTERY_PAGER_VALUE_TYPE is not defined."
#endif

#ifndef POTTERY_PAGER_EXTERN
#define POTTERY_PAGER_EXTERN /*nothing*/
#endif

#define POTTERY_PAGER_NAME(name) POTTERY_CONCAT(POTTERY_PAGER_PREFIX, name)

// public pager-specific functions
#define pottery_pager_next_bulk POTTERY_PAGER_NAME(_next_bulk)
#define pottery_pager_previous_bulk POTTERY_PAGER_NAME(_previous_bulk)

// internal types and functions
#define pottery_pager_page_t POTTERY_PAGER_NAME(_page_t)
#define pottery_pager_entry_make POTTERY_PAGER_NAME(_entry_make)
#define pottery_pager_per_page POTTERY_PAGER_NAME(_per_page)
#define pottery_pager_sanity_check POTTERY_PAGER_NAME(_sanity_check)
#define pottery_pager_acquire_page POTTERY_PAGER_NAME(_acquire_page)
#define pottery_pager_release_page POTTERY_PAGER_NAME(_release_page)
#define pottery_pager_destroy_all POTTERY_PAGER_NAME(_destroy_all)
#define pottery_pager_set_clear POTTERY_PAGER_NAME(_set_clear)

// common array types and functions
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_pager DEST:POTTERY_PAGER_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_pager to POTTERY_PAGER_PREFIX

    // types
    #define pottery_pager_t POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _t)
    #define pottery_pager_entry_t POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _entry_t)

    // lifecycle operations
    #define pottery_pager_copy POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _copy)
    #define pottery_pager_destroy POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _destroy)
    #define pottery_pager_init POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _init)
    #define pottery_pager_init_copy POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _init_copy)
    #define pottery_pager_init_steal POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _init_steal)
    #define pottery_pager_move POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _move)
    #define pottery_pager_steal POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _steal)
    #define pottery_pager_swap POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _swap)

    // entry operations
    #define pottery_pager_entry_exists POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _entry_exists)
    #define pottery_pager_entry_equal POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _entry_equal)
    #define pottery_pager_entry_ref POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _entry_ref)

    // lookup
    #define pottery_pager_count POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _count)
    #define pottery_pager_is_empty POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _is_empty)
    #define pottery_pager_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _at)
    #define pottery_pager_find POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _find)
    #define pottery_pager_contains_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _contains_key)
    #define pottery_pager_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _first)
    #define pottery_pager_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _last)
    #define pottery_pager_any POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _any)

    // array access
    #define pottery_pager_select POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _select)
    #define pottery_pager_index POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _index)
    #define pottery_pager_shift POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _shift)
    #define pottery_pager_offset POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _offset)
    #define pottery_pager_begin POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _begin)
    #define pottery_pager_end POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _end)
    #define pottery_pager_next POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _next)
    #define pottery_pager_previous POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _previous)

    // misc
    #define pottery_pager_capacity POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _capacity)
    #define pottery_pager_reserve POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _reserve)
    #define pottery_pager_shrink POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _shrink)

    // insert
    #define pottery_pager_insert POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert)
    #define pottery_pager_insert_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_at)
    #define pottery_pager_insert_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_first)
    #define pottery_pager_insert_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_last)
    #define pottery_pager_insert_before POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_before)
    #define pottery_pager_insert_after POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_after)
    #define pottery_pager_insert_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_bulk)
    #define pottery_pager_insert_at_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_at_bulk)
    #define pottery_pager_insert_first_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_first_bulk)
    #define pottery_pager_insert_last_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_last_bulk)
    #define pottery_pager_insert_before_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_before_bulk)
    #define pottery_pager_insert_after_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_pager_emplace POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace)
    #define pottery_pager_emplace_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_at)
    #define pottery_pager_emplace_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_key)
    #define pottery_pager_emplace_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_first)
    #define pottery_pager_emplace_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_last)
    #define pottery_pager_emplace_before POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_before)
    #define pottery_pager_emplace_after POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_after)
    #define pottery_pager_emplace_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_bulk)
    #define pottery_pager_emplace_at_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_at_bulk)
    #define pottery_pager_emplace_first_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_first_bulk)
    #define pottery_pager_emplace_last_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_last_bulk)
    #define pottery_pager_emplace_before_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_before_bulk)
    #define pottery_pager_emplace_after_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_pager_construct POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct)
    #define pottery_pager_construct_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct_at)
    #define pottery_pager_construct_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct_key)
    #define pottery_pager_construct_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct_first)
    #define pottery_pager_construct_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct_last)
    #define pottery_pager_construct_before POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct_before)
    #define pottery_pager_construct_after POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _construct_after)

    // extract
    #define pottery_pager_extract POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract)
    #define pottery_pager_extract_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_at)
    #define pottery_pager_extract_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_key)
    #define pottery_pager_extract_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_first)
    #define pottery_pager_extract_first_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_first_bulk)
    #define pottery_pager_extract_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_last)
    #define pottery_pager_extract_last_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_last_bulk)
    #define pottery_pager_extract_and_next POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_and_next)
    #define pottery_pager_extract_and_previous POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _extract_and_previous)

    // remove
    #define pottery_pager_remove POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove)
    #define pottery_pager_remove_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_at)
    #define pottery_pager_remove_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_key)
    #define pottery_pager_remove_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_first)
    #define pottery_pager_remove_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_last)
    #define pottery_pager_remove_and_next POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_and_next)
    #define pottery_pager_remove_and_previous POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_and_previous)
    #define pottery_pager_remove_all POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_all)
    #define pottery_pager_remove_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_bulk)
    #define pottery_pager_remove_at_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_at_bulk)
    #define pottery_pager_remove_first_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_first_bulk)
    #define pottery_pager_remove_last_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_last_bulk)
    #define pottery_pager_remove_at_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_at_bulk)
    #define pottery_pager_remove_and_next_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_and_next_bulk)
    #define pottery_pager_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_pager_displace POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace)
    #define pottery_pager_displace_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_at)
    #define pottery_pager_displace_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_key)
    #define pottery_pager_displace_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_first)
    #define pottery_pager_displace_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_last)
    #define pottery_pager_displace_and_next POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_and_next)
    #define pottery_pager_displace_and_previous POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_and_previous)
    #define pottery_pager_displace_all POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_all)
    #define pottery_pager_displace_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_bulk)
    #define pottery_pager_displace_at_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_at_bulk)
    #define pottery_pager_displace_first_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_first_bulk)
    #define pottery_pager_displace_last_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_last_bulk)
    #define pottery_pager_displace_and_next_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_and_next_bulk)
    #define pottery_pager_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_pager_link POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _link)
    #define pottery_pager_link_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _link_first)
    #define pottery_pager_link_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _link_last)
    #define pottery_pager_link_before POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _link_before)
    #define pottery_pager_link_after POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _link_after)

    // unlink
    #define pottery_pager_unlink POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink)
    #define pottery_pager_unlink_at POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_at)
    #define pottery_pager_unlink_key POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_key)
    #define pottery_pager_unlink_first POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_first)
    #define pottery_pager_unlink_last POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_last)
    #define pottery_pager_unlink_and_next POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_and_next)
    #define pottery_pager_unlink_and_previous POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_and_previous)
    #define pottery_pager_unlink_all POTTERY_CONCAT(POTTERY_PAGER_PREFIX, _unlink_all)
//!!! END_AUTOGENERATED


// forward ring
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_pager_page_ring DEST:POTTERY_PAGER_NAME(_page_ring)
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_pager_page_ring to POTTERY_PAGER_NAME(_page_ring)

    // types
    #define pottery_pager_page_ring_t POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _t)
    #define pottery_pager_page_ring_entry_t POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _entry_t)

    // lifecycle operations
    #define pottery_pager_page_ring_copy POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _copy)
    #define pottery_pager_page_ring_destroy POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _destroy)
    #define pottery_pager_page_ring_init POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _init)
    #define pottery_pager_page_ring_init_copy POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _init_copy)
    #define pottery_pager_page_ring_init_steal POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _init_steal)
    #define pottery_pager_page_ring_move POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _move)
    #define pottery_pager_page_ring_steal POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _steal)
    #define pottery_pager_page_ring_swap POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _swap)

    // entry operations
    #define pottery_pager_page_ring_entry_exists POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _entry_exists)
    #define pottery_pager_page_ring_entry_equal POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _entry_equal)
    #define pottery_pager_page_ring_entry_ref POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _entry_ref)

    // lookup
    #define pottery_pager_page_ring_count POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _count)
    #define pottery_pager_page_ring_is_empty POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _is_empty)
    #define pottery_pager_page_ring_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _at)
    #define pottery_pager_page_ring_find POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _find)
    #define pottery_pager_page_ring_contains_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _contains_key)
    #define pottery_pager_page_ring_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _first)
    #define pottery_pager_page_ring_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _last)
    #define pottery_pager_page_ring_any POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _any)

    // array access
    #define pottery_pager_page_ring_select POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _select)
    #define pottery_pager_page_ring_index POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _index)
    #define pottery_pager_page_ring_shift POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _shift)
    #define pottery_pager_page_ring_offset POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _offset)
    #define pottery_pager_page_ring_begin POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _begin)
    #define pottery_pager_page_ring_end POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _end)
    #define pottery_pager_page_ring_next POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _next)
    #define pottery_pager_page_ring_previous POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _previous)

    // misc
    #define pottery_pager_page_ring_capacity POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _capacity)
    #define pottery_pager_page_ring_reserve POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _reserve)
    #define pottery_pager_page_ring_shrink POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _shrink)

    // insert
    #define pottery_pager_page_ring_insert POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert)
    #define pottery_pager_page_ring_insert_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_at)
    #define pottery_pager_page_ring_insert_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_first)
    #define pottery_pager_page_ring_insert_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_last)
    #define pottery_pager_page_ring_insert_before POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_before)
    #define pottery_pager_page_ring_insert_after POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_after)
    #define pottery_pager_page_ring_insert_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_bulk)
    #define pottery_pager_page_ring_insert_at_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_at_bulk)
    #define pottery_pager_page_ring_insert_first_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_first_bulk)
    #define pottery_pager_page_ring_insert_last_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_last_bulk)
    #define pottery_pager_page_ring_insert_before_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_before_bulk)
    #define pottery_pager_page_ring_insert_after_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _insert_after_bulk)

    // emplace
    #define pottery_pager_page_ring_emplace POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace)
    #define pottery_pager_page_ring_emplace_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_at)
    #define pottery_pager_page_ring_emplace_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_key)
    #define pottery_pager_page_ring_emplace_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_first)
    #define pottery_pager_page_ring_emplace_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_last)
    #define pottery_pager_page_ring_emplace_before POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_before)
    #define pottery_pager_page_ring_emplace_after POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_after)
    #define pottery_pager_page_ring_emplace_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_bulk)
    #define pottery_pager_page_ring_emplace_at_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_at_bulk)
    #define pottery_pager_page_ring_emplace_first_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_first_bulk)
    #define pottery_pager_page_ring_emplace_last_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_last_bulk)
    #define pottery_pager_page_ring_emplace_before_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_before_bulk)
    #define pottery_pager_page_ring_emplace_after_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _emplace_after_bulk)

    // construct
    #define pottery_pager_page_ring_construct POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct)
    #define pottery_pager_page_ring_construct_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct_at)
    #define pottery_pager_page_ring_construct_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct_key)
    #define pottery_pager_page_ring_construct_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct_first)
    #define pottery_pager_page_ring_construct_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct_last)
    #define pottery_pager_page_ring_construct_before POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct_before)
    #define pottery_pager_page_ring_construct_after POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _construct_after)

    // extract
    #define pottery_pager_page_ring_extract POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract)
    #define pottery_pager_page_ring_extract_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_at)
    #define pottery_pager_page_ring_extract_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_key)
    #define pottery_pager_page_ring_extract_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_first)
    #define pottery_pager_page_ring_extract_first_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_first_bulk)
    #define pottery_pager_page_ring_extract_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_last)
    #define pottery_pager_page_ring_extract_last_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_last_bulk)
    #define pottery_pager_page_ring_extract_and_next POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_and_next)
    #define pottery_pager_page_ring_extract_and_previous POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _extract_and_previous)

    // remove
    #define pottery_pager_page_ring_remove POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove)
    #define pottery_pager_page_ring_remove_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_at)
    #define pottery_pager_page_ring_remove_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_key)
    #define pottery_pager_page_ring_remove_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_first)
    #define pottery_pager_page_ring_remove_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_last)
    #define pottery_pager_page_ring_remove_and_next POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_and_next)
    #define pottery_pager_page_ring_remove_and_previous POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_and_previous)
    #define pottery_pager_page_ring_remove_all POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_all)
    #define pottery_pager_page_ring_remove_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_bulk)
    #define pottery_pager_page_ring_remove_at_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_at_bulk)
    #define pottery_pager_page_ring_remove_first_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_first_bulk)
    #define pottery_pager_page_ring_remove_last_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_last_bulk)
    #define pottery_pager_page_ring_remove_at_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_at_bulk)
    #define pottery_pager_page_ring_remove_and_next_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_and_next_bulk)
    #define pottery_pager_page_ring_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _remove_and_previous_bulk)

    // displace
    #define pottery_pager_page_ring_displace POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace)
    #define pottery_pager_page_ring_displace_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_at)
    #define pottery_pager_page_ring_displace_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_key)
    #define pottery_pager_page_ring_displace_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_first)
    #define pottery_pager_page_ring_displace_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_last)
    #define pottery_pager_page_ring_displace_and_next POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_and_next)
    #define pottery_pager_page_ring_displace_and_previous POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_and_previous)
    #define pottery_pager_page_ring_displace_all POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_all)
    #define pottery_pager_page_ring_displace_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_bulk)
    #define pottery_pager_page_ring_displace_at_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_at_bulk)
    #define pottery_pager_page_ring_displace_first_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_first_bulk)
    #define pottery_pager_page_ring_displace_last_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_last_bulk)
    #define pottery_pager_page_ring_displace_and_next_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_and_next_bulk)
    #define pottery_pager_page_ring_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _displace_and_previous_bulk)

    // link
    #define pottery_pager_page_ring_link POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _link)
    #define pottery_pager_page_ring_link_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _link_first)
    #define pottery_pager_page_ring_link_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _link_last)
    #define pottery_pager_page_ring_link_before POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _link_before)
    #define pottery_pager_page_ring_link_after POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _link_after)

    // unlink
    #define pottery_pager_page_ring_unlink POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink)
    #define pottery_pager_page_ring_unlink_at POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_at)
    #define pottery_pager_page_ring_unlink_key POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_key)
    #define pottery_pager_page_ring_unlink_first POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_first)
    #define pottery_pager_page_ring_unlink_last POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_last)
    #define pottery_pager_page_ring_unlink_and_next POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_and_next)
    #define pottery_pager_page_ring_unlink_and_previous POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_and_previous)
    #define pottery_pager_page_ring_unlink_all POTTERY_CONCAT(POTTERY_PAGER_NAME(_page_ring), _unlink_all)
//!!! END_AUTOGENERATED



// container_types
#ifdef POTTERY_PAGER_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_PAGER_CONTAINER_TYPES_PREFIX POTTERY_PAGER_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_PAGER_CONTAINER_TYPES_PREFIX POTTERY_PAGER_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_pager DEST:POTTERY_PAGER_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_pager to POTTERY_PAGER_CONTAINER_TYPES_PREFIX

    #define pottery_pager_value_t POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_pager_ref_t POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_pager_const_ref_t POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_pager_key_t POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_pager_context_t POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_pager_ref_key POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_pager_ref_equal POTTERY_CONCAT(POTTERY_PAGER_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions
#ifdef POTTERY_PAGER_EXTERNAL_LIFECYCLE
    #define POTTERY_PAGER_LIFECYCLE_PREFIX POTTERY_PAGER_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_PAGER_LIFECYCLE_PREFIX POTTERY_PAGER_NAME(_lifecycle)
#endif
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_pager_lifecycle DEST:POTTERY_PAGER_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_pager_lifecycle to POTTERY_PAGER_LIFECYCLE_PREFIX

    #define pottery_pager_lifecycle_init_steal POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_pager_lifecycle_init_copy POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_pager_lifecycle_init POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _init)
    #define pottery_pager_lifecycle_destroy POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _destroy)

    #define pottery_pager_lifecycle_swap POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _swap)
    #define pottery_pager_lifecycle_move POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move)
    #define pottery_pager_lifecycle_steal POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _steal)
    #define pottery_pager_lifecycle_copy POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _copy)
    #define pottery_pager_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_pager_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_pager_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_pager_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_pager_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_pager_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_pager_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_pager_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_pager_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_pager_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_pager_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_pager_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_PAGER_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED



// alloc functions
#ifdef POTTERY_PAGER_EXTERNAL_ALLOC
    #define POTTERY_PAGER_ALLOC_PREFIX POTTERY_PAGER_EXTERNAL_ALLOC
#else
    #define POTTERY_PAGER_ALLOC_PREFIX POTTERY_PAGER_NAME(_alloc)
#endif
//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_pager_alloc DEST:POTTERY_PAGER_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_pager_alloc to POTTERY_PAGER_ALLOC_PREFIX

    #define pottery_pager_alloc_free POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _free)
    #define pottery_pager_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_pager_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_pager_alloc_malloc POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _malloc)
    #define pottery_pager_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_pager_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_pager_alloc_malloc_zero POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _malloc_zero)
    #define pottery_pager_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_pager_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_pager_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_pager_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_pager_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_pager_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_pager_alloc_malloc_array POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _malloc_array)
    #define pottery_pager_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_PAGER_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED
