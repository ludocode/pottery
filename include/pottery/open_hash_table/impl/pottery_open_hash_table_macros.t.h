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

#ifndef POTTERY_OPEN_HASH_TABLE_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_OPEN_HASH_TABLE_PREFIX
    #error "POTTERY_OPEN_HASH_TABLE_PREFIX is not defined."
#endif

#ifndef POTTERY_OPEN_HASH_TABLE_EXTERN
    #define POTTERY_OPEN_HASH_TABLE_EXTERN /*nothing*/
#endif

// Context forwarding
#ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
    #define POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL context,
#else
    #define POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL /*nothing*/
#endif

// Probing sequence
#ifndef POTTERY_OPEN_HASH_TABLE_QUADRATIC_PROBING
    #define POTTERY_OPEN_HASH_TABLE_QUADRATIC_PROBING 0
#endif
#ifndef POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
    #define POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING 0
#endif
#if !defined(POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING)
    #if !POTTERY_OPEN_HASH_TABLE_QUADRATIC_PROBING && !POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
        // default to linear probing
        #define POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING 1
    #else
        #define POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING 0
    #endif
#endif

// Empty
#ifndef POTTERY_OPEN_HASH_TABLE_EMPTY_IS_ZERO
    #define POTTERY_OPEN_HASH_TABLE_EMPTY_IS_ZERO 0
#endif
#if !defined(POTTERY_OPEN_HASH_TABLE_IS_EMPTY) && !POTTERY_OPEN_HASH_TABLE_EMPTY_IS_ZERO
    #error "You must define IS_EMPTY or EMPTY_IS_ZERO."
#endif
#if !defined(POTTERY_OPEN_HASH_TABLE_SET_EMPTY) && !POTTERY_OPEN_HASH_TABLE_EMPTY_IS_ZERO
    #error "You must define SET_EMPTY or EMPTY_IS_ZERO."
#endif

// Tombstones
#if defined(POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE) || defined(POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE)
    #if !defined(POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE) || !defined(POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE)
        #error "You must define both or neither of SET_TOMBSTONE and IS_TOMBSTONE."
    #endif
    #define POTTERY_OPEN_HASH_TABLE_TOMBSTONES 1
#else
    #if !POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING
        #error "Non-linear probing requires tombstones."
    #endif
    #define POTTERY_OPEN_HASH_TABLE_TOMBSTONES 0
#endif

#if !POTTERY_LIFECYCLE_CAN_MOVE && !POTTERY_OPEN_HASH_TABLE_TOMBSTONES
    #error "Either a move expression or tombstones are required."
#endif



/*
 * Array Access Function Arguments
 */

//!!! AUTOGENERATED:array_access/args.m.h CONFIG:POTTERY_OPEN_HASH_TABLE NAME:pottery_oht
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Defines wrappers for common function arguments for functions that access
    // arrays via the array_access template.

    // ARGS are function arguments
    // VALS are the matching values
    // SOLE means they are the only arguments to the function (so no trailing comma)
    // UNUSED casts them to void to silence unused parameter warnings

    #ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #define POTTERY_OPEN_HASH_TABLE_SOLE_ARGS pottery_oht_context_t context, pottery_oht_ref_t base
            #define POTTERY_OPEN_HASH_TABLE_SOLE_VALS context, base
            #define POTTERY_OPEN_HASH_TABLE_ARGS_UNUSED (void)context, (void)base
        #else
            #define POTTERY_OPEN_HASH_TABLE_SOLE_ARGS pottery_oht_context_t context
            #define POTTERY_OPEN_HASH_TABLE_SOLE_VALS context
            #define POTTERY_OPEN_HASH_TABLE_ARGS_UNUSED (void)context
        #endif
    #else
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #define POTTERY_OPEN_HASH_TABLE_SOLE_ARGS pottery_oht_ref_t base
            #define POTTERY_OPEN_HASH_TABLE_SOLE_VALS base
            #define POTTERY_OPEN_HASH_TABLE_ARGS_UNUSED (void)base
        #else
            #define POTTERY_OPEN_HASH_TABLE_SOLE_ARGS void
            #define POTTERY_OPEN_HASH_TABLE_SOLE_VALS /*nothing*/
            #define POTTERY_OPEN_HASH_TABLE_ARGS_UNUSED /*nothing*/
        #endif
    #endif
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE) || !POTTERY_ARRAY_ACCESS_INHERENT_BASE
        #define POTTERY_OPEN_HASH_TABLE_ARGS POTTERY_OPEN_HASH_TABLE_SOLE_ARGS ,
        #define POTTERY_OPEN_HASH_TABLE_VALS POTTERY_OPEN_HASH_TABLE_SOLE_VALS ,
    #else
        #define POTTERY_OPEN_HASH_TABLE_ARGS /*nothing*/
        #define POTTERY_OPEN_HASH_TABLE_VALS /*nothing*/
    #endif
//!!! END_AUTOGENERATED



// functions

#define POTTERY_OPEN_HASH_TABLE_NAME(name) POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, name)

//!!! AUTOGENERATED:open_hash_table/rename.m.h SRC:pottery_oht DEST:POTTERY_OPEN_HASH_TABLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames open_hash_table identifiers from pottery_oht to POTTERY_OPEN_HASH_TABLE_PREFIX

    #define pottery_oht_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_t)
    #define pottery_oht_value_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _value_t)
    #define pottery_oht_key_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _key_t)
    #define pottery_oht_context_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _context_t)

    #define pottery_oht_emplace POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _emplace)
    #define pottery_oht_insert POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _insert)
    #define pottery_oht_find POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _find)
    #define pottery_oht_displace POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _displace)
    #define pottery_oht_displace_all POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _displace_all)
    #define pottery_oht_remove POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _remove)
    #define pottery_oht_remove_key POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _remove_key)
    #define pottery_oht_remove_all POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _remove_all)
    #define pottery_oht_destroy_all POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _destroy_all)
    #define pottery_oht_contains_key POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _contains_key)

    #define pottery_oht_begin POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _begin)
    #define pottery_oht_end POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _end)
    #define pottery_oht_first POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _first)
    #define pottery_oht_last POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _last)
    #define pottery_oht_next POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _next)
    #define pottery_oht_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _previous)

    #define pottery_oht_access_at POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _access_at)
    #define pottery_oht_access_index POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _access_index)
    #define pottery_oht_access_next POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _access_next)
    #define pottery_oht_access_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _access_previous)

    #define pottery_oht_key POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _key)
    #define pottery_oht_key_hash POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _key_hash)
    #define pottery_oht_key_equal POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _key_equal)
    #define pottery_oht_double_hash_interval POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _double_hash_interval)
    #define pottery_oht_ref_is_tombstone POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_is_tombstone)
    #define pottery_oht_ref_set_tombstone POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_set_tombstone)
    #define pottery_oht_ref_is_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_is_empty)
    #define pottery_oht_ref_set_empty POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_set_empty)
    #define pottery_oht_ref_is_element POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_is_element)
    #define pottery_oht_ref_in_use POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_in_use)
    #define pottery_oht_ref_exists POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _ref_exists)

    #define pottery_oht_bucket_for_hash POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _bucket_for_hash)
    #define pottery_oht_next_probe POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _next_probe)
    #define pottery_oht_probe POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_PREFIX, _probe)
//!!! END_AUTOGENERATED



// lifecycle functions
#ifdef POTTERY_OPEN_HASH_TABLE_EXTERNAL_LIFECYCLE
    #define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX POTTERY_OPEN_HASH_TABLE_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX POTTERY_OPEN_HASH_TABLE_NAME(_lifecycle)
#endif
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_oht_lifecycle DEST:POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_oht_lifecycle to POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX

    // types
    #define pottery_oht_lifecycle_value_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _value_t)
    #define pottery_oht_lifecycle_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _ref_t)
    #define pottery_oht_lifecycle_const_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _const_ref_t)
    #define pottery_oht_lifecycle_context_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _context_t)

    // fundamental operations
    #define pottery_oht_lifecycle_init_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_oht_lifecycle_init_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_oht_lifecycle_init POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _init)
    #define pottery_oht_lifecycle_destroy POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _destroy)
    #define pottery_oht_lifecycle_swap POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _swap)
    #define pottery_oht_lifecycle_move POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move)
    #define pottery_oht_lifecycle_steal POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _steal)
    #define pottery_oht_lifecycle_copy POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _copy)

    // bulk operations
    #define pottery_oht_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_oht_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_oht_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_oht_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_oht_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_oht_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_oht_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_oht_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED



// array_access functions
#ifdef POTTERY_OPEN_HASH_TABLE_EXTERNAL_ARRAY_ACCESS
    #define POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX POTTERY_OPEN_HASH_TABLE_EXTERNAL_ARRAY_ACCESS
#else
    #define POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX POTTERY_OPEN_HASH_TABLE_NAME(_array_access)
#endif
//!!! AUTOGENERATED:array_access/rename.m.h SRC:pottery_oht_array_access DEST:POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array_access identifiers from pottery_oht_array_access to POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX

    #define pottery_oht_array_access_ref_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _ref_t)
    #define pottery_oht_array_access_value_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _value_t)
    #define pottery_oht_array_access_context_t POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _context_t)

    #define pottery_oht_array_access_begin POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _begin)
    #define pottery_oht_array_access_end POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _end)
    #define pottery_oht_array_access_count POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _count)

    #define pottery_oht_array_access_select POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _select)
    #define pottery_oht_array_access_index POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _index)

    #define pottery_oht_array_access_shift POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _shift)
    #define pottery_oht_array_access_offset POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _offset)
    #define pottery_oht_array_access_next POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _next)
    #define pottery_oht_array_access_previous POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _previous)

    #define pottery_oht_array_access_value POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _value)
    #define pottery_oht_array_access_equal POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _equal)
    #define pottery_oht_array_access_exists POTTERY_CONCAT(POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_PREFIX, _exists)
//!!! END_AUTOGENERATED
