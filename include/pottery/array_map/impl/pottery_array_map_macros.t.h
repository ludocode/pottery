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

#ifndef POTTERY_ARRAY_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_ARRAY_MAP_PREFIX
#error "POTTERY_ARRAY_MAP_PREFIX is not defined."
#include "pottery_fatal"
#endif

#ifdef POTTERY_ARRAY_MAP_KEY_EQUAL
    // TODO this is temporarily here for backwards compatibility with old
    // HomeFort code. Don't define KEY_EQUAL.
    #define POTTERY_ARRAY_MAP_COMPARE_EQUAL POTTERY_ARRAY_MAP_KEY_EQUAL
#endif

#define POTTERY_ARRAY_MAP_NAME(name) POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, name)

// TODO remove
#define pottery_array_map_place POTTERY_ARRAY_MAP_NAME(_place)
#define pottery_array_map_value_key POTTERY_ARRAY_MAP_NAME(_value_key)

// array_map container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_array_map DEST:POTTERY_ARRAY_MAP_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_array_map to POTTERY_ARRAY_MAP_PREFIX

    // types
    #define pottery_array_map_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _t)
    #define pottery_array_map_entry_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _entry_t)
    #define pottery_array_map_location_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_array_map_copy POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _copy)
    #define pottery_array_map_destroy POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _destroy)
    #define pottery_array_map_init POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _init)
    #define pottery_array_map_init_copy POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _init_copy)
    #define pottery_array_map_init_steal POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _init_steal)
    #define pottery_array_map_move POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _move)
    #define pottery_array_map_steal POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _steal)
    #define pottery_array_map_swap POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _swap)

    // entry operations
    #define pottery_array_map_entry_exists POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _entry_exists)
    #define pottery_array_map_entry_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _entry_equal)
    #define pottery_array_map_entry_ref POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _entry_ref)

    // lookup
    #define pottery_array_map_count POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _count)
    #define pottery_array_map_is_empty POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _is_empty)
    #define pottery_array_map_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _at)
    #define pottery_array_map_find POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _find)
    #define pottery_array_map_contains_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _contains_key)
    #define pottery_array_map_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _first)
    #define pottery_array_map_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _last)
    #define pottery_array_map_any POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _any)

    // array access
    #define pottery_array_map_select POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _select)
    #define pottery_array_map_index POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _index)
    #define pottery_array_map_shift POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _shift)
    #define pottery_array_map_offset POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _offset)
    #define pottery_array_map_begin POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _begin)
    #define pottery_array_map_end POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _end)
    #define pottery_array_map_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _next)
    #define pottery_array_map_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _previous)

    // misc
    #define pottery_array_map_capacity POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _capacity)
    #define pottery_array_map_reserve POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _reserve)
    #define pottery_array_map_shrink POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _shrink)

    // insert
    #define pottery_array_map_insert POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert)
    #define pottery_array_map_insert_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_at)
    #define pottery_array_map_insert_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_first)
    #define pottery_array_map_insert_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_last)
    #define pottery_array_map_insert_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_before)
    #define pottery_array_map_insert_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_after)
    #define pottery_array_map_insert_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_bulk)
    #define pottery_array_map_insert_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_at_bulk)
    #define pottery_array_map_insert_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_first_bulk)
    #define pottery_array_map_insert_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_last_bulk)
    #define pottery_array_map_insert_before_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_before_bulk)
    #define pottery_array_map_insert_after_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_array_map_emplace POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace)
    #define pottery_array_map_emplace_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_at)
    #define pottery_array_map_emplace_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_key)
    #define pottery_array_map_emplace_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_first)
    #define pottery_array_map_emplace_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_last)
    #define pottery_array_map_emplace_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_before)
    #define pottery_array_map_emplace_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_after)
    #define pottery_array_map_emplace_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_bulk)
    #define pottery_array_map_emplace_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_at_bulk)
    #define pottery_array_map_emplace_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_first_bulk)
    #define pottery_array_map_emplace_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_last_bulk)
    #define pottery_array_map_emplace_before_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_before_bulk)
    #define pottery_array_map_emplace_after_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_array_map_construct POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct)
    #define pottery_array_map_construct_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct_at)
    #define pottery_array_map_construct_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct_key)
    #define pottery_array_map_construct_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct_first)
    #define pottery_array_map_construct_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct_last)
    #define pottery_array_map_construct_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct_before)
    #define pottery_array_map_construct_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _construct_after)

    // extract
    #define pottery_array_map_extract POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract)
    #define pottery_array_map_extract_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_at)
    #define pottery_array_map_extract_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_key)
    #define pottery_array_map_extract_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_first)
    #define pottery_array_map_extract_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_first_bulk)
    #define pottery_array_map_extract_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_last)
    #define pottery_array_map_extract_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_last_bulk)
    #define pottery_array_map_extract_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_and_next)
    #define pottery_array_map_extract_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _extract_and_previous)

    // remove
    #define pottery_array_map_remove POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove)
    #define pottery_array_map_remove_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_at)
    #define pottery_array_map_remove_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_key)
    #define pottery_array_map_remove_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_first)
    #define pottery_array_map_remove_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_last)
    #define pottery_array_map_remove_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_and_next)
    #define pottery_array_map_remove_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_and_previous)
    #define pottery_array_map_remove_all POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_all)
    #define pottery_array_map_remove_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_bulk)
    #define pottery_array_map_remove_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_at_bulk)
    #define pottery_array_map_remove_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_first_bulk)
    #define pottery_array_map_remove_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_last_bulk)
    #define pottery_array_map_remove_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_at_bulk)
    #define pottery_array_map_remove_and_next_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_and_next_bulk)
    #define pottery_array_map_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_array_map_displace POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace)
    #define pottery_array_map_displace_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_at)
    #define pottery_array_map_displace_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_key)
    #define pottery_array_map_displace_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_first)
    #define pottery_array_map_displace_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_last)
    #define pottery_array_map_displace_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_and_next)
    #define pottery_array_map_displace_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_and_previous)
    #define pottery_array_map_displace_all POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_all)
    #define pottery_array_map_displace_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_bulk)
    #define pottery_array_map_displace_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_at_bulk)
    #define pottery_array_map_displace_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_first_bulk)
    #define pottery_array_map_displace_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_last_bulk)
    #define pottery_array_map_displace_and_next_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_and_next_bulk)
    #define pottery_array_map_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_array_map_link POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _link)
    #define pottery_array_map_link_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _link_first)
    #define pottery_array_map_link_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _link_last)
    #define pottery_array_map_link_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _link_before)
    #define pottery_array_map_link_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _link_after)

    // unlink
    #define pottery_array_map_unlink POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink)
    #define pottery_array_map_unlink_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_at)
    #define pottery_array_map_unlink_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_key)
    #define pottery_array_map_unlink_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_first)
    #define pottery_array_map_unlink_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_last)
    #define pottery_array_map_unlink_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_and_next)
    #define pottery_array_map_unlink_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_and_previous)
    #define pottery_array_map_unlink_all POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _unlink_all)

    // special links
    #define pottery_array_map_replace POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _replace)
    #define pottery_array_map_find_location POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _find_location)
    #define pottery_array_map_link_location POTTERY_CONCAT(POTTERY_ARRAY_MAP_PREFIX, _link_location)
//!!! END_AUTOGENERATED



// vector wrappers
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_array_map_vector DEST:POTTERY_ARRAY_MAP_NAME(_vector)
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_array_map_vector to POTTERY_ARRAY_MAP_NAME(_vector)

    // types
    #define pottery_array_map_vector_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _t)
    #define pottery_array_map_vector_entry_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _entry_t)
    #define pottery_array_map_vector_location_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_array_map_vector_copy POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _copy)
    #define pottery_array_map_vector_destroy POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _destroy)
    #define pottery_array_map_vector_init POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _init)
    #define pottery_array_map_vector_init_copy POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _init_copy)
    #define pottery_array_map_vector_init_steal POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _init_steal)
    #define pottery_array_map_vector_move POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _move)
    #define pottery_array_map_vector_steal POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _steal)
    #define pottery_array_map_vector_swap POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _swap)

    // entry operations
    #define pottery_array_map_vector_entry_exists POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _entry_exists)
    #define pottery_array_map_vector_entry_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _entry_equal)
    #define pottery_array_map_vector_entry_ref POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _entry_ref)

    // lookup
    #define pottery_array_map_vector_count POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _count)
    #define pottery_array_map_vector_is_empty POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _is_empty)
    #define pottery_array_map_vector_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _at)
    #define pottery_array_map_vector_find POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _find)
    #define pottery_array_map_vector_contains_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _contains_key)
    #define pottery_array_map_vector_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _first)
    #define pottery_array_map_vector_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _last)
    #define pottery_array_map_vector_any POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _any)

    // array access
    #define pottery_array_map_vector_select POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _select)
    #define pottery_array_map_vector_index POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _index)
    #define pottery_array_map_vector_shift POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _shift)
    #define pottery_array_map_vector_offset POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _offset)
    #define pottery_array_map_vector_begin POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _begin)
    #define pottery_array_map_vector_end POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _end)
    #define pottery_array_map_vector_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _next)
    #define pottery_array_map_vector_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _previous)

    // misc
    #define pottery_array_map_vector_capacity POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _capacity)
    #define pottery_array_map_vector_reserve POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _reserve)
    #define pottery_array_map_vector_shrink POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _shrink)

    // insert
    #define pottery_array_map_vector_insert POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert)
    #define pottery_array_map_vector_insert_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_at)
    #define pottery_array_map_vector_insert_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_first)
    #define pottery_array_map_vector_insert_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_last)
    #define pottery_array_map_vector_insert_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_before)
    #define pottery_array_map_vector_insert_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_after)
    #define pottery_array_map_vector_insert_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_bulk)
    #define pottery_array_map_vector_insert_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_at_bulk)
    #define pottery_array_map_vector_insert_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_first_bulk)
    #define pottery_array_map_vector_insert_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_last_bulk)
    #define pottery_array_map_vector_insert_before_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_before_bulk)
    #define pottery_array_map_vector_insert_after_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _insert_after_bulk)

    // emplace
    #define pottery_array_map_vector_emplace POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace)
    #define pottery_array_map_vector_emplace_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_at)
    #define pottery_array_map_vector_emplace_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_key)
    #define pottery_array_map_vector_emplace_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_first)
    #define pottery_array_map_vector_emplace_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_last)
    #define pottery_array_map_vector_emplace_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_before)
    #define pottery_array_map_vector_emplace_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_after)
    #define pottery_array_map_vector_emplace_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_bulk)
    #define pottery_array_map_vector_emplace_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_at_bulk)
    #define pottery_array_map_vector_emplace_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_first_bulk)
    #define pottery_array_map_vector_emplace_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_last_bulk)
    #define pottery_array_map_vector_emplace_before_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_before_bulk)
    #define pottery_array_map_vector_emplace_after_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _emplace_after_bulk)

    // construct
    #define pottery_array_map_vector_construct POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct)
    #define pottery_array_map_vector_construct_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct_at)
    #define pottery_array_map_vector_construct_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct_key)
    #define pottery_array_map_vector_construct_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct_first)
    #define pottery_array_map_vector_construct_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct_last)
    #define pottery_array_map_vector_construct_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct_before)
    #define pottery_array_map_vector_construct_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _construct_after)

    // extract
    #define pottery_array_map_vector_extract POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract)
    #define pottery_array_map_vector_extract_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_at)
    #define pottery_array_map_vector_extract_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_key)
    #define pottery_array_map_vector_extract_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_first)
    #define pottery_array_map_vector_extract_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_first_bulk)
    #define pottery_array_map_vector_extract_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_last)
    #define pottery_array_map_vector_extract_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_last_bulk)
    #define pottery_array_map_vector_extract_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_and_next)
    #define pottery_array_map_vector_extract_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _extract_and_previous)

    // remove
    #define pottery_array_map_vector_remove POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove)
    #define pottery_array_map_vector_remove_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_at)
    #define pottery_array_map_vector_remove_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_key)
    #define pottery_array_map_vector_remove_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_first)
    #define pottery_array_map_vector_remove_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_last)
    #define pottery_array_map_vector_remove_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_and_next)
    #define pottery_array_map_vector_remove_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_and_previous)
    #define pottery_array_map_vector_remove_all POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_all)
    #define pottery_array_map_vector_remove_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_bulk)
    #define pottery_array_map_vector_remove_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_at_bulk)
    #define pottery_array_map_vector_remove_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_first_bulk)
    #define pottery_array_map_vector_remove_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_last_bulk)
    #define pottery_array_map_vector_remove_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_at_bulk)
    #define pottery_array_map_vector_remove_and_next_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_and_next_bulk)
    #define pottery_array_map_vector_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _remove_and_previous_bulk)

    // displace
    #define pottery_array_map_vector_displace POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace)
    #define pottery_array_map_vector_displace_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_at)
    #define pottery_array_map_vector_displace_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_key)
    #define pottery_array_map_vector_displace_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_first)
    #define pottery_array_map_vector_displace_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_last)
    #define pottery_array_map_vector_displace_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_and_next)
    #define pottery_array_map_vector_displace_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_and_previous)
    #define pottery_array_map_vector_displace_all POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_all)
    #define pottery_array_map_vector_displace_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_bulk)
    #define pottery_array_map_vector_displace_at_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_at_bulk)
    #define pottery_array_map_vector_displace_first_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_first_bulk)
    #define pottery_array_map_vector_displace_last_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_last_bulk)
    #define pottery_array_map_vector_displace_and_next_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_and_next_bulk)
    #define pottery_array_map_vector_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _displace_and_previous_bulk)

    // link
    #define pottery_array_map_vector_link POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _link)
    #define pottery_array_map_vector_link_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _link_first)
    #define pottery_array_map_vector_link_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _link_last)
    #define pottery_array_map_vector_link_before POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _link_before)
    #define pottery_array_map_vector_link_after POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _link_after)

    // unlink
    #define pottery_array_map_vector_unlink POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink)
    #define pottery_array_map_vector_unlink_at POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_at)
    #define pottery_array_map_vector_unlink_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_key)
    #define pottery_array_map_vector_unlink_first POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_first)
    #define pottery_array_map_vector_unlink_last POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_last)
    #define pottery_array_map_vector_unlink_and_next POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_and_next)
    #define pottery_array_map_vector_unlink_and_previous POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_and_previous)
    #define pottery_array_map_vector_unlink_all POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _unlink_all)

    // special links
    #define pottery_array_map_vector_replace POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _replace)
    #define pottery_array_map_vector_find_location POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _find_location)
    #define pottery_array_map_vector_link_location POTTERY_CONCAT(POTTERY_ARRAY_MAP_NAME(_vector), _link_location)
//!!! END_AUTOGENERATED



// compare functions
#ifdef POTTERY_ARRAY_MAP_EXTERNAL_COMPARE
    #define POTTERY_ARRAY_MAP_COMPARE_PREFIX POTTERY_ARRAY_MAP_EXTERNAL_COMPARE
#else
    #define POTTERY_ARRAY_MAP_COMPARE_PREFIX POTTERY_ARRAY_MAP_NAME(_compare)
#endif
//!!! AUTOGENERATED:compare/rename.m.h SRC:pottery_array_map_compare DEST:POTTERY_ARRAY_MAP_COMPARE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames compare identifiers from pottery_array_map_compare to POTTERY_ARRAY_MAP_COMPARE_PREFIX

    #define pottery_array_map_compare_three_way POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _three_way)
    #define pottery_array_map_compare_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _equal)
    #define pottery_array_map_compare_not_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _not_equal)
    #define pottery_array_map_compare_less POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _less)
    #define pottery_array_map_compare_less_or_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _less_or_equal)
    #define pottery_array_map_compare_greater POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _greater)
    #define pottery_array_map_compare_greater_or_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _greater_or_equal)

    #define pottery_array_map_compare_min POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _min)
    #define pottery_array_map_compare_max POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _max)
    #define pottery_array_map_compare_clamp POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _clamp)
    #define pottery_array_map_compare_median POTTERY_CONCAT(POTTERY_ARRAY_MAP_COMPARE_PREFIX, _median)
//!!! END_AUTOGENERATED



// container types
#ifdef POTTERY_ARRAY_MAP_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX POTTERY_ARRAY_MAP_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX POTTERY_ARRAY_MAP_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_array_map DEST:POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_array_map to POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX

    #define pottery_array_map_value_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_array_map_ref_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_array_map_const_ref_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_array_map_key_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_array_map_context_t POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_array_map_ref_key POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_array_map_ref_equal POTTERY_CONCAT(POTTERY_ARRAY_MAP_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions
#ifdef POTTERY_ARRAY_MAP_EXTERNAL_LIFECYCLE
    #define POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX POTTERY_ARRAY_MAP_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX POTTERY_ARRAY_MAP_NAME(_lifecycle)
#endif
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_array_map_lifecycle DEST:POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_array_map_lifecycle to POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX

    #define pottery_array_map_lifecycle_init_steal POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_array_map_lifecycle_init_copy POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_array_map_lifecycle_init POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _init)
    #define pottery_array_map_lifecycle_destroy POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _destroy)

    #define pottery_array_map_lifecycle_swap POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _swap)
    #define pottery_array_map_lifecycle_move POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move)
    #define pottery_array_map_lifecycle_steal POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _steal)
    #define pottery_array_map_lifecycle_copy POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _copy)
    #define pottery_array_map_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_array_map_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_array_map_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_array_map_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_array_map_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_array_map_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_array_map_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_array_map_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_array_map_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_array_map_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_array_map_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_array_map_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_ARRAY_MAP_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED
