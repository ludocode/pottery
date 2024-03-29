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

#ifndef POTTERY_OPEN_HASH_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_OPEN_HASH_MAP_PREFIX
#error "POTTERY_OPEN_HASH_MAP_PREFIX is not defined."
#endif

#ifndef POTTERY_OPEN_HASH_MAP_VALUE_TYPE
#error "POTTERY_OPEN_HASH_MAP_VALUE_TYPE is not defined."
#endif

#ifndef POTTERY_OPEN_HASH_MAP_EXTERN
#define POTTERY_OPEN_HASH_MAP_EXTERN /*nothing*/
#endif

// Probing sequence
// This is mostly duplicated from open_hash_table. We need to figure out the
// probing sequence to decide whether we'll store tombstones.
#ifndef POTTERY_OPEN_HASH_MAP_QUADRATIC_PROBING
    #define POTTERY_OPEN_HASH_MAP_QUADRATIC_PROBING 0
#endif
#ifndef POTTERY_OPEN_HASH_MAP_DOUBLE_HASHING
    #define POTTERY_OPEN_HASH_MAP_DOUBLE_HASHING 0
#endif
#if !defined(POTTERY_OPEN_HASH_MAP_LINEAR_PROBING)
    #if !POTTERY_OPEN_HASH_MAP_QUADRATIC_PROBING && !POTTERY_OPEN_HASH_MAP_DOUBLE_HASHING
        // default to linear probing
        #define POTTERY_OPEN_HASH_MAP_LINEAR_PROBING 1
    #else
        #define POTTERY_OPEN_HASH_MAP_LINEAR_PROBING 0
    #endif
#endif

// Empty
#if !defined(POTTERY_OPEN_HASH_MAP_IS_EMPTY) && \
        !defined(POTTERY_OPEN_HASH_MAP_SET_EMPTY) && \
        !defined(POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO)
    #define POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY 1
#else
    #define POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY 0
#endif
#ifndef POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO
    #define POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO 0
#endif

// Tombstones
#if !defined(POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE) && \
        !defined(POTTERY_OPEN_HASH_MAP_SET_TOMBSTONE) && \
        !POTTERY_OPEN_HASH_MAP_LINEAR_PROBING
    #define POTTERY_OPEN_HASH_MAP_TOMBSTONES 1
    #define POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE 1
#elif defined(POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE)
    #define POTTERY_OPEN_HASH_MAP_TOMBSTONES 1
    #define POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE 0
#else
    #define POTTERY_OPEN_HASH_MAP_TOMBSTONES 0
    #define POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE 0
#endif

// Metadata
#if POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY || \
        (!POTTERY_OPEN_HASH_MAP_LINEAR_PROBING && POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE)
    #define POTTERY_OPEN_HASH_MAP_HAS_METADATA 1
#else
    #define POTTERY_OPEN_HASH_MAP_HAS_METADATA 0
#endif

// Context forwarding
#ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
    #define POTTERY_OPEN_HASH_MAP_CONTEXT_VAL context,
#else
    #define POTTERY_OPEN_HASH_MAP_CONTEXT_VAL /*nothing*/
#endif

// We need to store certain lifecycle and container_types capabilities because
// we clean them up before instantiating our open_hash_table.
#if POTTERY_LIFECYCLE_CAN_DESTROY
    #define POTTERY_OPEN_HASH_MAP_CAN_DESTROY 1
#else
    #define POTTERY_OPEN_HASH_MAP_CAN_DESTROY 0
#endif



/*
 * Renaming
 */

#define POTTERY_OPEN_HASH_MAP_NAME(name) \
    POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, name)

#define pottery_ohm_bucket_count POTTERY_OPEN_HASH_MAP_NAME(_bucket_count)
#define pottery_ohm_impl_values POTTERY_OPEN_HASH_MAP_NAME(_impl_values)
#define pottery_ohm_impl_alloc POTTERY_OPEN_HASH_MAP_NAME(_impl_alloc)
#define pottery_ohm_impl_free POTTERY_OPEN_HASH_MAP_NAME(_impl_free)
#define pottery_ohm_impl_free_allocs POTTERY_OPEN_HASH_MAP_NAME(_impl_free_allocs)
#define pottery_ohm_impl_alloc_metadata POTTERY_OPEN_HASH_MAP_NAME(_impl_alloc_metadata)
#define pottery_ohm_impl_free_metadata POTTERY_OPEN_HASH_MAP_NAME(_impl_free_metadata)
#define pottery_ohm_grow_if_needed POTTERY_OPEN_HASH_MAP_NAME(_grow_if_needed)
#define pottery_ohm_shrink_if_needed POTTERY_OPEN_HASH_MAP_NAME(_shrink_if_needed)
#define pottery_ohm_rehash POTTERY_OPEN_HASH_MAP_NAME(_rehash)
#define pottery_ohm_load POTTERY_OPEN_HASH_MAP_NAME(_load)
#define pottery_ohm_init_impl POTTERY_OPEN_HASH_MAP_NAME(_init_impl)
#define pottery_ohm_entry_move POTTERY_OPEN_HASH_MAP_NAME(_entry_move)
#define pottery_ohm_entry_key POTTERY_OPEN_HASH_MAP_NAME(_entry_key)
#define pottery_ohm_entry_key_equal POTTERY_OPEN_HASH_MAP_NAME(_entry_key_equal)
#define pottery_ohm_entry_key_hash POTTERY_OPEN_HASH_MAP_NAME(_entry_key_hash)
#define pottery_ohm_entry_is_empty POTTERY_OPEN_HASH_MAP_NAME(_entry_is_empty)
#define pottery_ohm_entry_is_value POTTERY_OPEN_HASH_MAP_NAME(_entry_is_value)
#define pottery_ohm_entry_set_empty POTTERY_OPEN_HASH_MAP_NAME(_entry_set_empty)
#define pottery_ohm_entry_set_other POTTERY_OPEN_HASH_MAP_NAME(_entry_set_other)
#define pottery_ohm_entry_metadata POTTERY_OPEN_HASH_MAP_NAME(_entry_metadata)
#define pottery_ohm_rehash POTTERY_OPEN_HASH_MAP_NAME(_rehash)
#define pottery_ohm_bucket_state_t POTTERY_OPEN_HASH_MAP_NAME(_bucket_state_t)
#define pottery_ohm_bucket_state_empty POTTERY_OPEN_HASH_MAP_NAME(_bucket_state_empty)
#define pottery_ohm_bucket_state_other POTTERY_OPEN_HASH_MAP_NAME(_bucket_state_other)
#define pottery_ohm_bucket_state_tombstone POTTERY_OPEN_HASH_MAP_NAME(_bucket_state_tombstone)



// container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_ohm DEST:POTTERY_OPEN_HASH_MAP_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_ohm to POTTERY_OPEN_HASH_MAP_PREFIX

    // types
    #define pottery_ohm_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _t)
    #define pottery_ohm_entry_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _entry_t)
    #define pottery_ohm_location_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_ohm_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _copy)
    #define pottery_ohm_destroy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _destroy)
    #define pottery_ohm_init POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _init)
    #define pottery_ohm_init_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _init_copy)
    #define pottery_ohm_init_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _init_steal)
    #define pottery_ohm_move POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _move)
    #define pottery_ohm_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _steal)
    #define pottery_ohm_swap POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _swap)

    // entry operations
    #define pottery_ohm_entry_exists POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _entry_exists)
    #define pottery_ohm_entry_equal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _entry_equal)
    #define pottery_ohm_entry_ref POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _entry_ref)

    // lookup
    #define pottery_ohm_count POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _count)
    #define pottery_ohm_is_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _is_empty)
    #define pottery_ohm_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _at)
    #define pottery_ohm_find POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _find)
    #define pottery_ohm_contains_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _contains_key)
    #define pottery_ohm_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _first)
    #define pottery_ohm_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _last)
    #define pottery_ohm_any POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _any)

    // array access
    #define pottery_ohm_select POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _select)
    #define pottery_ohm_index POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _index)
    #define pottery_ohm_shift POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _shift)
    #define pottery_ohm_offset POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _offset)
    #define pottery_ohm_begin POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _begin)
    #define pottery_ohm_end POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _end)
    #define pottery_ohm_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _next)
    #define pottery_ohm_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _previous)

    // misc
    #define pottery_ohm_capacity POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _capacity)
    #define pottery_ohm_reserve POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _reserve)
    #define pottery_ohm_shrink POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _shrink)

    // insert
    #define pottery_ohm_insert POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert)
    #define pottery_ohm_insert_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_at)
    #define pottery_ohm_insert_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_first)
    #define pottery_ohm_insert_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_last)
    #define pottery_ohm_insert_before POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_before)
    #define pottery_ohm_insert_after POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_after)
    #define pottery_ohm_insert_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_bulk)
    #define pottery_ohm_insert_at_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_at_bulk)
    #define pottery_ohm_insert_first_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_first_bulk)
    #define pottery_ohm_insert_last_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_last_bulk)
    #define pottery_ohm_insert_before_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_before_bulk)
    #define pottery_ohm_insert_after_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_ohm_emplace POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace)
    #define pottery_ohm_emplace_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_at)
    #define pottery_ohm_emplace_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_key)
    #define pottery_ohm_emplace_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_first)
    #define pottery_ohm_emplace_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_last)
    #define pottery_ohm_emplace_before POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_before)
    #define pottery_ohm_emplace_after POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_after)
    #define pottery_ohm_emplace_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_bulk)
    #define pottery_ohm_emplace_at_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_at_bulk)
    #define pottery_ohm_emplace_first_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_first_bulk)
    #define pottery_ohm_emplace_last_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_last_bulk)
    #define pottery_ohm_emplace_before_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_before_bulk)
    #define pottery_ohm_emplace_after_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_ohm_construct POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct)
    #define pottery_ohm_construct_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct_at)
    #define pottery_ohm_construct_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct_key)
    #define pottery_ohm_construct_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct_first)
    #define pottery_ohm_construct_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct_last)
    #define pottery_ohm_construct_before POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct_before)
    #define pottery_ohm_construct_after POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _construct_after)

    // extract
    #define pottery_ohm_extract POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract)
    #define pottery_ohm_extract_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_at)
    #define pottery_ohm_extract_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_key)
    #define pottery_ohm_extract_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_first)
    #define pottery_ohm_extract_first_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_first_bulk)
    #define pottery_ohm_extract_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_last)
    #define pottery_ohm_extract_last_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_last_bulk)
    #define pottery_ohm_extract_and_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_and_next)
    #define pottery_ohm_extract_and_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _extract_and_previous)

    // remove
    #define pottery_ohm_remove POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove)
    #define pottery_ohm_remove_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_at)
    #define pottery_ohm_remove_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_key)
    #define pottery_ohm_remove_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_first)
    #define pottery_ohm_remove_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_last)
    #define pottery_ohm_remove_and_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_and_next)
    #define pottery_ohm_remove_and_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_and_previous)
    #define pottery_ohm_remove_all POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_all)
    #define pottery_ohm_remove_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_bulk)
    #define pottery_ohm_remove_at_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_at_bulk)
    #define pottery_ohm_remove_first_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_first_bulk)
    #define pottery_ohm_remove_last_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_last_bulk)
    #define pottery_ohm_remove_at_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_at_bulk)
    #define pottery_ohm_remove_and_next_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_and_next_bulk)
    #define pottery_ohm_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_ohm_displace POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace)
    #define pottery_ohm_displace_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_at)
    #define pottery_ohm_displace_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_key)
    #define pottery_ohm_displace_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_first)
    #define pottery_ohm_displace_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_last)
    #define pottery_ohm_displace_and_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_and_next)
    #define pottery_ohm_displace_and_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_and_previous)
    #define pottery_ohm_displace_all POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_all)
    #define pottery_ohm_displace_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_bulk)
    #define pottery_ohm_displace_at_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_at_bulk)
    #define pottery_ohm_displace_first_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_first_bulk)
    #define pottery_ohm_displace_last_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_last_bulk)
    #define pottery_ohm_displace_and_next_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_and_next_bulk)
    #define pottery_ohm_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_ohm_link POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _link)
    #define pottery_ohm_link_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _link_first)
    #define pottery_ohm_link_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _link_last)
    #define pottery_ohm_link_before POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _link_before)
    #define pottery_ohm_link_after POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _link_after)

    // unlink
    #define pottery_ohm_unlink POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink)
    #define pottery_ohm_unlink_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_at)
    #define pottery_ohm_unlink_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_key)
    #define pottery_ohm_unlink_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_first)
    #define pottery_ohm_unlink_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_last)
    #define pottery_ohm_unlink_and_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_and_next)
    #define pottery_ohm_unlink_and_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_and_previous)
    #define pottery_ohm_unlink_all POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _unlink_all)

    // special links
    #define pottery_ohm_replace POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _replace)
    #define pottery_ohm_find_location POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _find_location)
    #define pottery_ohm_link_location POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _link_location)
//!!! END_AUTOGENERATED



// open_hash_table functions
#define POTTERY_OPEN_HASH_MAP_OHT_PREFIX POTTERY_OPEN_HASH_MAP_NAME(_table)
//!!! AUTOGENERATED:open_hash_table/rename.m.h SRC:pottery_ohm_table DEST:POTTERY_OPEN_HASH_MAP_OHT_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames open_hash_table identifiers from pottery_ohm_table to POTTERY_OPEN_HASH_MAP_OHT_PREFIX

    #define pottery_ohm_table_emplace_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _emplace_key)
    #define pottery_ohm_table_insert POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _insert)
    #define pottery_ohm_table_find POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _find)
    #define pottery_ohm_table_displace POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _displace)
    #define pottery_ohm_table_displace_all POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _displace_all)
    #define pottery_ohm_table_remove POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _remove)
    #define pottery_ohm_table_remove_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _remove_key)
    #define pottery_ohm_table_remove_all POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _remove_all)
    #define pottery_ohm_table_destroy_all POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _destroy_all)
    #define pottery_ohm_table_contains_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _contains_key)

    #define pottery_ohm_table_begin POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _begin)
    #define pottery_ohm_table_end POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _end)
    #define pottery_ohm_table_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _first)
    #define pottery_ohm_table_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _last)
    #define pottery_ohm_table_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _next)
    #define pottery_ohm_table_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _previous)

    #define pottery_ohm_table_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key)
    #define pottery_ohm_table_key_hash POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key_hash)
    #define pottery_ohm_table_key_equal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key_equal)
    #define pottery_ohm_table_double_hash_interval POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _double_hash_interval)
    #define pottery_ohm_table_entry_is_tombstone POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_is_tombstone)
    #define pottery_ohm_table_entry_set_tombstone POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_set_tombstone)
    #define pottery_ohm_table_entry_is_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_is_empty)
    #define pottery_ohm_table_entry_set_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_set_empty)
    #define pottery_ohm_table_entry_is_element POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_is_element)
    #define pottery_ohm_table_entry_in_use POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_in_use)
    #define pottery_ohm_table_entry_exists POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _entry_exists)

    #define pottery_ohm_table_bucket_for_hash POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _bucket_for_hash)
    #define pottery_ohm_table_next_probe POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _next_probe)
    #define pottery_ohm_table_probe POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _probe)
//!!! END_AUTOGENERATED



// alloc functions
#define POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _alloc)
//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_ohm_alloc DEST:POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_ohm_alloc to POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX

    #define pottery_ohm_alloc_free POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _free)
    #define pottery_ohm_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_ohm_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_ohm_alloc_malloc POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _malloc)
    #define pottery_ohm_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_ohm_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_ohm_alloc_malloc_zero POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _malloc_zero)
    #define pottery_ohm_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_ohm_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_ohm_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_ohm_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_ohm_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_ohm_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_ohm_alloc_malloc_array POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _malloc_array)
    #define pottery_ohm_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED



// container types
#ifdef POTTERY_OPEN_HASH_MAP_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX POTTERY_OPEN_HASH_MAP_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX POTTERY_OPEN_HASH_MAP_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_ohm DEST:POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_ohm to POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX

    #define pottery_ohm_value_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_ohm_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_ohm_const_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_ohm_key_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_ohm_context_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_ohm_ref_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_ohm_ref_equal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions (wrapping those of the open_hash_table)
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX POTTERY_OPEN_HASH_MAP_NAME(_lifecycle)
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_ohm_lifecycle DEST:POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_ohm_lifecycle to POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX

    #define pottery_ohm_lifecycle_init_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_ohm_lifecycle_init_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_ohm_lifecycle_init POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _init)
    #define pottery_ohm_lifecycle_destroy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _destroy)

    #define pottery_ohm_lifecycle_swap POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _swap)
    #define pottery_ohm_lifecycle_move POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move)
    #define pottery_ohm_lifecycle_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _steal)
    #define pottery_ohm_lifecycle_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _copy)
    #define pottery_ohm_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_ohm_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_ohm_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_ohm_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_ohm_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_ohm_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_ohm_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_ohm_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_ohm_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_ohm_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_ohm_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_ohm_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED
