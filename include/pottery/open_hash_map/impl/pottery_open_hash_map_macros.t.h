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
#if !defined(POTTERY_OPEN_HASH_MAP_IS_EMPTY) && !defined(POTTERY_OPEN_HASH_MAP_SET_EMPTY)
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

// We pass our map as the context to table functions, but only if necessary
// (i.e. if we have a user context or metadata.)

#if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE) || POTTERY_OPEN_HASH_MAP_HAS_METADATA
    #define POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG pottery_ohm_t* map,
    #define POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map) map,
    #define POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED (void)map
    #define POTTERY_OPEN_HASH_MAP_HAS_TABLE_CONTEXT 1
#else
    #define POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG /*nothing*/
    #define POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map) /*nothing*/
    #define POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED /*nothing*/
    #define POTTERY_OPEN_HASH_MAP_HAS_TABLE_CONTEXT 0
#endif



/*
 * Renaming
 */

#define POTTERY_OPEN_HASH_MAP_NAME(name) \
    POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, name)

#define pottery_ohm_t POTTERY_OPEN_HASH_MAP_NAME(_t)
#define pottery_ohm_ref_t POTTERY_OPEN_HASH_MAP_NAME(_ref_t)
#define pottery_ohm_value_t POTTERY_OPEN_HASH_MAP_NAME(value_t)
#define pottery_ohm_key_t POTTERY_OPEN_HASH_MAP_NAME(_key_t)
#define pottery_ohm_context_t POTTERY_OPEN_HASH_MAP_NAME(_context_t)

#define pottery_ohm_init POTTERY_OPEN_HASH_MAP_NAME(_init)
#define pottery_ohm_destroy POTTERY_OPEN_HASH_MAP_NAME(_destroy)
#define pottery_ohm_count POTTERY_OPEN_HASH_MAP_NAME(_count)
#define pottery_ohm_capacity POTTERY_OPEN_HASH_MAP_NAME(_capacity)
#define pottery_ohm_is_empty POTTERY_OPEN_HASH_MAP_NAME(_is_empty)
#define pottery_ohm_emplace POTTERY_OPEN_HASH_MAP_NAME(_emplace)
#define pottery_ohm_find POTTERY_OPEN_HASH_MAP_NAME(_find)
#define pottery_ohm_displace POTTERY_OPEN_HASH_MAP_NAME(_displace)
#define pottery_ohm_remove POTTERY_OPEN_HASH_MAP_NAME(_remove)
#define pottery_ohm_remove_key POTTERY_OPEN_HASH_MAP_NAME(_remove_key)
#define pottery_ohm_contains_key POTTERY_OPEN_HASH_MAP_NAME(_contains_key)

#define pottery_ohm_begin POTTERY_OPEN_HASH_MAP_NAME(_begin)
#define pottery_ohm_end POTTERY_OPEN_HASH_MAP_NAME(_end)
#define pottery_ohm_first POTTERY_OPEN_HASH_MAP_NAME(_first)
#define pottery_ohm_last POTTERY_OPEN_HASH_MAP_NAME(_last)
#define pottery_ohm_next POTTERY_OPEN_HASH_MAP_NAME(_next)
#define pottery_ohm_previous POTTERY_OPEN_HASH_MAP_NAME(_previous)

#define pottery_ohm_ref_exists POTTERY_OPEN_HASH_MAP_NAME(_ref_exists)

#define pottery_ohm_impl_alloc POTTERY_OPEN_HASH_MAP_NAME(_impl_alloc)
#define pottery_ohm_impl_free POTTERY_OPEN_HASH_MAP_NAME(_impl_free)
#define pottery_ohm_grow_if_needed POTTERY_OPEN_HASH_MAP_NAME(_grow_if_needed)
#define pottery_ohm_shrink_if_needed POTTERY_OPEN_HASH_MAP_NAME(_shrink_if_needed)
#define pottery_ohm_rehash POTTERY_OPEN_HASH_MAP_NAME(_rehash)



// open_hash_table functions
#define POTTERY_OPEN_HASH_MAP_OHT_PREFIX POTTERY_OPEN_HASH_MAP_NAME(_table)
//!!! AUTOGENERATED:open_hash_table/rename.m.h SRC:pottery_ohm_table DEST:POTTERY_OPEN_HASH_MAP_OHT_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames open_hash_table identifiers from pottery_ohm_table to POTTERY_OPEN_HASH_MAP_OHT_PREFIX

    #define pottery_ohm_table_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_t)
    #define pottery_ohm_table_value_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _value_t)
    #define pottery_ohm_table_key_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key_t)
    #define pottery_ohm_table_context_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _context_t)

    #define pottery_ohm_table_emplace POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _emplace)
    #define pottery_ohm_table_insert POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _insert)
    #define pottery_ohm_table_find POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _find)
    #define pottery_ohm_table_displace POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _displace)
    #define pottery_ohm_table_remove POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _remove)
    #define pottery_ohm_table_remove_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _remove_key)
    #define pottery_ohm_table_contains_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _contains_key)

    #define pottery_ohm_table_begin POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _begin)
    #define pottery_ohm_table_end POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _end)
    #define pottery_ohm_table_first POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _first)
    #define pottery_ohm_table_last POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _last)
    #define pottery_ohm_table_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _next)
    #define pottery_ohm_table_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _previous)

    #define pottery_ohm_table_access_at POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _access_at)
    #define pottery_ohm_table_access_index POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _access_index)
    #define pottery_ohm_table_access_next POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _access_next)
    #define pottery_ohm_table_access_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _access_previous)

    #define pottery_ohm_table_key POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key)
    #define pottery_ohm_table_key_hash POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key_hash)
    #define pottery_ohm_table_key_equal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _key_equal)
    #define pottery_ohm_table_double_hash_interval POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _double_hash_interval)
    #define pottery_ohm_table_ref_is_tombstone POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_is_tombstone)
    #define pottery_ohm_table_ref_set_tombstone POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_set_tombstone)
    #define pottery_ohm_table_ref_is_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_is_empty)
    #define pottery_ohm_table_ref_set_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_set_empty)
    #define pottery_ohm_table_ref_is_element POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_is_element)
    #define pottery_ohm_table_ref_in_use POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_in_use)
    #define pottery_ohm_table_ref_exists POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_OHT_PREFIX, _ref_exists)

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



// lifecycle functions (wrapping those of the open_hash_table)
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX POTTERY_OPEN_HASH_MAP_NAME(_lifecycle)
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_ohm_lifecycle DEST:POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_ohm_lifecycle to POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX

    // types
    #define pottery_ohm_lifecycle_value_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _value_t)
    #define pottery_ohm_lifecycle_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _ref_t)
    #define pottery_ohm_lifecycle_const_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _const_ref_t)
    #define pottery_ohm_lifecycle_context_t POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _context_t)

    // fundamental operations
    #define pottery_ohm_lifecycle_init_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_ohm_lifecycle_init_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_ohm_lifecycle_init POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _init)
    #define pottery_ohm_lifecycle_destroy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _destroy)
    #define pottery_ohm_lifecycle_swap POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _swap)
    #define pottery_ohm_lifecycle_move POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move)
    #define pottery_ohm_lifecycle_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _steal)
    #define pottery_ohm_lifecycle_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _copy)

    // bulk operations
    #define pottery_ohm_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_ohm_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_ohm_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_ohm_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_ohm_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_ohm_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_ohm_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_ohm_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED
