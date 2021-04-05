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

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_PRIORITY_QUEUE_PREFIX
#error "POTTERY_PRIORITY_QUEUE_PREFIX is not defined."
#endif

#ifndef POTTERY_PRIORITY_QUEUE_EXTERN
    #define POTTERY_PRIORITY_QUEUE_EXTERN /*nothing*/
#endif

#if !POTTERY_COMPARE_CAN_ORDER
    #error "An ordering expression is required. Define at least one of COMPARE_LESS, COMPARE_THREE_WAY, etc."
#endif

// remap some capabilities since we need to cleanup before heap
#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    #define POTTERY_PRIORITY_QUEUE_HAS_CONTEXT 1
    #define POTTERY_PRIORITY_QUEUE_CONTEXT_VAL(priority_queue) priority_queue->context,
#else
    #define POTTERY_PRIORITY_QUEUE_HAS_CONTEXT 0
    #define POTTERY_PRIORITY_QUEUE_CONTEXT_VAL(priority_queue) /*nothing*/
#endif
#if POTTERY_LIFECYCLE_CAN_PASS
    #define POTTERY_PRIORITY_QUEUE_CAN_PASS 1
#else
    #define POTTERY_PRIORITY_QUEUE_CAN_PASS 0
#endif
#if POTTERY_LIFECYCLE_CAN_DESTROY
    #define POTTERY_PRIORITY_QUEUE_CAN_DESTROY 1
#else
    #define POTTERY_PRIORITY_QUEUE_CAN_DESTROY 0
#endif
#if POTTERY_LIFECYCLE_CAN_INIT_COPY
    #define POTTERY_PRIORITY_QUEUE_CAN_INIT_COPY 1
#else
    #define POTTERY_PRIORITY_QUEUE_CAN_INIT_COPY 0
#endif



/*
 * Renaming
 */

#define POTTERY_PRIORITY_QUEUE_NAME(name) POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, name)

// public funcs
#define pottery_pq_repair POTTERY_PRIORITY_QUEUE_NAME(_repair)
#define pottery_pq_insert_broken POTTERY_PRIORITY_QUEUE_NAME(_insert_broken)
#define pottery_pq_insert_broken_bulk POTTERY_PRIORITY_QUEUE_NAME(_insert_broken_bulk)

// internal funcs
#define pottery_pq_set_index POTTERY_PRIORITY_QUEUE_NAME(_set_index)
#define pottery_pq_heap_build POTTERY_PRIORITY_QUEUE_NAME(_heap_build)
#define pottery_pq_heap_expand_bulk POTTERY_PRIORITY_QUEUE_NAME(_heap_expand_bulk)
#define pottery_pq_heap_contract_at POTTERY_PRIORITY_QUEUE_NAME(_heap_contract_at)



// priority_queue container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_pq DEST:POTTERY_PRIORITY_QUEUE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_pq to POTTERY_PRIORITY_QUEUE_PREFIX

    // types
    #define pottery_pq_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _t)
    #define pottery_pq_entry_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _entry_t)
    #define pottery_pq_location_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_pq_copy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _copy)
    #define pottery_pq_destroy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _destroy)
    #define pottery_pq_init POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _init)
    #define pottery_pq_init_copy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _init_copy)
    #define pottery_pq_init_steal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _init_steal)
    #define pottery_pq_move POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _move)
    #define pottery_pq_steal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _steal)
    #define pottery_pq_swap POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _swap)

    // entry operations
    #define pottery_pq_entry_exists POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _entry_exists)
    #define pottery_pq_entry_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _entry_equal)
    #define pottery_pq_entry_ref POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _entry_ref)

    // lookup
    #define pottery_pq_count POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _count)
    #define pottery_pq_is_empty POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _is_empty)
    #define pottery_pq_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _at)
    #define pottery_pq_find POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _find)
    #define pottery_pq_contains_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _contains_key)
    #define pottery_pq_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _first)
    #define pottery_pq_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _last)
    #define pottery_pq_any POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _any)

    // array access
    #define pottery_pq_select POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _select)
    #define pottery_pq_index POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _index)
    #define pottery_pq_shift POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _shift)
    #define pottery_pq_offset POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _offset)
    #define pottery_pq_begin POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _begin)
    #define pottery_pq_end POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _end)
    #define pottery_pq_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _next)
    #define pottery_pq_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _previous)

    // misc
    #define pottery_pq_capacity POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _capacity)
    #define pottery_pq_reserve POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _reserve)
    #define pottery_pq_shrink POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _shrink)

    // insert
    #define pottery_pq_insert POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert)
    #define pottery_pq_insert_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_at)
    #define pottery_pq_insert_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_first)
    #define pottery_pq_insert_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_last)
    #define pottery_pq_insert_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_before)
    #define pottery_pq_insert_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_after)
    #define pottery_pq_insert_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_bulk)
    #define pottery_pq_insert_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_at_bulk)
    #define pottery_pq_insert_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_first_bulk)
    #define pottery_pq_insert_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_last_bulk)
    #define pottery_pq_insert_before_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_before_bulk)
    #define pottery_pq_insert_after_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_pq_emplace POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace)
    #define pottery_pq_emplace_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_at)
    #define pottery_pq_emplace_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_key)
    #define pottery_pq_emplace_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_first)
    #define pottery_pq_emplace_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_last)
    #define pottery_pq_emplace_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_before)
    #define pottery_pq_emplace_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_after)
    #define pottery_pq_emplace_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_bulk)
    #define pottery_pq_emplace_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_at_bulk)
    #define pottery_pq_emplace_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_first_bulk)
    #define pottery_pq_emplace_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_last_bulk)
    #define pottery_pq_emplace_before_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_before_bulk)
    #define pottery_pq_emplace_after_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_pq_construct POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct)
    #define pottery_pq_construct_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct_at)
    #define pottery_pq_construct_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct_key)
    #define pottery_pq_construct_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct_first)
    #define pottery_pq_construct_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct_last)
    #define pottery_pq_construct_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct_before)
    #define pottery_pq_construct_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _construct_after)

    // extract
    #define pottery_pq_extract POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract)
    #define pottery_pq_extract_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_at)
    #define pottery_pq_extract_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_key)
    #define pottery_pq_extract_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_first)
    #define pottery_pq_extract_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_first_bulk)
    #define pottery_pq_extract_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_last)
    #define pottery_pq_extract_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_last_bulk)
    #define pottery_pq_extract_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_and_next)
    #define pottery_pq_extract_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _extract_and_previous)

    // remove
    #define pottery_pq_remove POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove)
    #define pottery_pq_remove_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_at)
    #define pottery_pq_remove_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_key)
    #define pottery_pq_remove_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_first)
    #define pottery_pq_remove_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_last)
    #define pottery_pq_remove_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_and_next)
    #define pottery_pq_remove_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_and_previous)
    #define pottery_pq_remove_all POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_all)
    #define pottery_pq_remove_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_bulk)
    #define pottery_pq_remove_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_at_bulk)
    #define pottery_pq_remove_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_first_bulk)
    #define pottery_pq_remove_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_last_bulk)
    #define pottery_pq_remove_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_at_bulk)
    #define pottery_pq_remove_and_next_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_and_next_bulk)
    #define pottery_pq_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_pq_displace POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace)
    #define pottery_pq_displace_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_at)
    #define pottery_pq_displace_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_key)
    #define pottery_pq_displace_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_first)
    #define pottery_pq_displace_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_last)
    #define pottery_pq_displace_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_and_next)
    #define pottery_pq_displace_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_and_previous)
    #define pottery_pq_displace_all POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_all)
    #define pottery_pq_displace_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_bulk)
    #define pottery_pq_displace_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_at_bulk)
    #define pottery_pq_displace_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_first_bulk)
    #define pottery_pq_displace_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_last_bulk)
    #define pottery_pq_displace_and_next_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_and_next_bulk)
    #define pottery_pq_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_pq_link POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _link)
    #define pottery_pq_link_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _link_first)
    #define pottery_pq_link_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _link_last)
    #define pottery_pq_link_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _link_before)
    #define pottery_pq_link_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _link_after)

    // unlink
    #define pottery_pq_unlink POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink)
    #define pottery_pq_unlink_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_at)
    #define pottery_pq_unlink_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_key)
    #define pottery_pq_unlink_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_first)
    #define pottery_pq_unlink_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_last)
    #define pottery_pq_unlink_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_and_next)
    #define pottery_pq_unlink_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_and_previous)
    #define pottery_pq_unlink_all POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _unlink_all)

    // special links
    #define pottery_pq_replace POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _replace)
    #define pottery_pq_find_location POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _find_location)
    #define pottery_pq_link_location POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_PREFIX, _link_location)
//!!! END_AUTOGENERATED



// container_types
#ifdef POTTERY_PRIORITY_QUEUE_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX POTTERY_PRIORITY_QUEUE_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX POTTERY_PRIORITY_QUEUE_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_pq DEST:POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_pq to POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX

    #define pottery_pq_value_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_pq_ref_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_pq_const_ref_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_pq_key_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_pq_context_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_pq_ref_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_pq_ref_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions
#ifdef POTTERY_PRIORITY_QUEUE_EXTERNAL_LIFECYCLE
    #define POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX POTTERY_PRIORITY_QUEUE_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX POTTERY_PRIORITY_QUEUE_NAME(_lifecycle)
#endif
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_pq_lifecycle DEST:POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_pq_lifecycle to POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX

    #define pottery_pq_lifecycle_init_steal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_pq_lifecycle_init_copy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_pq_lifecycle_init POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _init)
    #define pottery_pq_lifecycle_destroy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _destroy)

    #define pottery_pq_lifecycle_swap POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _swap)
    #define pottery_pq_lifecycle_move POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move)
    #define pottery_pq_lifecycle_steal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _steal)
    #define pottery_pq_lifecycle_copy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _copy)
    #define pottery_pq_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_pq_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_pq_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_pq_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_pq_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_pq_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_pq_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_pq_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_pq_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_pq_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_pq_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_pq_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED



// compare functions
#ifdef POTTERY_PRIORITY_QUEUE_EXTERNAL_COMPARE
    #define POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX POTTERY_PRIORITY_QUEUE_EXTERNAL_COMPARE
#else
    #define POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX POTTERY_PRIORITY_QUEUE_NAME(_compare)
#endif
//!!! AUTOGENERATED:compare/rename.m.h SRC:pottery_pq_compare DEST:POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames compare identifiers from pottery_pq_compare to POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX

    #define pottery_pq_compare_three_way POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _three_way)
    #define pottery_pq_compare_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _equal)
    #define pottery_pq_compare_not_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _not_equal)
    #define pottery_pq_compare_less POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _less)
    #define pottery_pq_compare_less_or_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _less_or_equal)
    #define pottery_pq_compare_greater POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _greater)
    #define pottery_pq_compare_greater_or_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _greater_or_equal)

    #define pottery_pq_compare_min POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _min)
    #define pottery_pq_compare_max POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _max)
    #define pottery_pq_compare_clamp POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _clamp)
    #define pottery_pq_compare_median POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_COMPARE_PREFIX, _median)
//!!! END_AUTOGENERATED



// array wrappers
// TODO fix external array support
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_pq_array DEST:POTTERY_PRIORITY_QUEUE_NAME(_array)
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_pq_array to POTTERY_PRIORITY_QUEUE_NAME(_array)

    // types
    #define pottery_pq_array_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _t)
    #define pottery_pq_array_entry_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _entry_t)
    #define pottery_pq_array_location_t POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_pq_array_copy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _copy)
    #define pottery_pq_array_destroy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _destroy)
    #define pottery_pq_array_init POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _init)
    #define pottery_pq_array_init_copy POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _init_copy)
    #define pottery_pq_array_init_steal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _init_steal)
    #define pottery_pq_array_move POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _move)
    #define pottery_pq_array_steal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _steal)
    #define pottery_pq_array_swap POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _swap)

    // entry operations
    #define pottery_pq_array_entry_exists POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _entry_exists)
    #define pottery_pq_array_entry_equal POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _entry_equal)
    #define pottery_pq_array_entry_ref POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _entry_ref)

    // lookup
    #define pottery_pq_array_count POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _count)
    #define pottery_pq_array_is_empty POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _is_empty)
    #define pottery_pq_array_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _at)
    #define pottery_pq_array_find POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _find)
    #define pottery_pq_array_contains_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _contains_key)
    #define pottery_pq_array_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _first)
    #define pottery_pq_array_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _last)
    #define pottery_pq_array_any POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _any)

    // array access
    #define pottery_pq_array_select POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _select)
    #define pottery_pq_array_index POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _index)
    #define pottery_pq_array_shift POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _shift)
    #define pottery_pq_array_offset POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _offset)
    #define pottery_pq_array_begin POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _begin)
    #define pottery_pq_array_end POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _end)
    #define pottery_pq_array_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _next)
    #define pottery_pq_array_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _previous)

    // misc
    #define pottery_pq_array_capacity POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _capacity)
    #define pottery_pq_array_reserve POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _reserve)
    #define pottery_pq_array_shrink POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _shrink)

    // insert
    #define pottery_pq_array_insert POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert)
    #define pottery_pq_array_insert_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_at)
    #define pottery_pq_array_insert_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_first)
    #define pottery_pq_array_insert_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_last)
    #define pottery_pq_array_insert_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_before)
    #define pottery_pq_array_insert_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_after)
    #define pottery_pq_array_insert_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_bulk)
    #define pottery_pq_array_insert_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_at_bulk)
    #define pottery_pq_array_insert_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_first_bulk)
    #define pottery_pq_array_insert_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_last_bulk)
    #define pottery_pq_array_insert_before_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_before_bulk)
    #define pottery_pq_array_insert_after_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _insert_after_bulk)

    // emplace
    #define pottery_pq_array_emplace POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace)
    #define pottery_pq_array_emplace_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_at)
    #define pottery_pq_array_emplace_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_key)
    #define pottery_pq_array_emplace_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_first)
    #define pottery_pq_array_emplace_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_last)
    #define pottery_pq_array_emplace_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_before)
    #define pottery_pq_array_emplace_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_after)
    #define pottery_pq_array_emplace_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_bulk)
    #define pottery_pq_array_emplace_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_at_bulk)
    #define pottery_pq_array_emplace_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_first_bulk)
    #define pottery_pq_array_emplace_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_last_bulk)
    #define pottery_pq_array_emplace_before_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_before_bulk)
    #define pottery_pq_array_emplace_after_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _emplace_after_bulk)

    // construct
    #define pottery_pq_array_construct POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct)
    #define pottery_pq_array_construct_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct_at)
    #define pottery_pq_array_construct_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct_key)
    #define pottery_pq_array_construct_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct_first)
    #define pottery_pq_array_construct_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct_last)
    #define pottery_pq_array_construct_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct_before)
    #define pottery_pq_array_construct_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _construct_after)

    // extract
    #define pottery_pq_array_extract POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract)
    #define pottery_pq_array_extract_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_at)
    #define pottery_pq_array_extract_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_key)
    #define pottery_pq_array_extract_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_first)
    #define pottery_pq_array_extract_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_first_bulk)
    #define pottery_pq_array_extract_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_last)
    #define pottery_pq_array_extract_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_last_bulk)
    #define pottery_pq_array_extract_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_and_next)
    #define pottery_pq_array_extract_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _extract_and_previous)

    // remove
    #define pottery_pq_array_remove POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove)
    #define pottery_pq_array_remove_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_at)
    #define pottery_pq_array_remove_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_key)
    #define pottery_pq_array_remove_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_first)
    #define pottery_pq_array_remove_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_last)
    #define pottery_pq_array_remove_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_and_next)
    #define pottery_pq_array_remove_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_and_previous)
    #define pottery_pq_array_remove_all POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_all)
    #define pottery_pq_array_remove_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_bulk)
    #define pottery_pq_array_remove_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_at_bulk)
    #define pottery_pq_array_remove_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_first_bulk)
    #define pottery_pq_array_remove_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_last_bulk)
    #define pottery_pq_array_remove_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_at_bulk)
    #define pottery_pq_array_remove_and_next_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_and_next_bulk)
    #define pottery_pq_array_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _remove_and_previous_bulk)

    // displace
    #define pottery_pq_array_displace POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace)
    #define pottery_pq_array_displace_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_at)
    #define pottery_pq_array_displace_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_key)
    #define pottery_pq_array_displace_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_first)
    #define pottery_pq_array_displace_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_last)
    #define pottery_pq_array_displace_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_and_next)
    #define pottery_pq_array_displace_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_and_previous)
    #define pottery_pq_array_displace_all POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_all)
    #define pottery_pq_array_displace_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_bulk)
    #define pottery_pq_array_displace_at_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_at_bulk)
    #define pottery_pq_array_displace_first_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_first_bulk)
    #define pottery_pq_array_displace_last_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_last_bulk)
    #define pottery_pq_array_displace_and_next_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_and_next_bulk)
    #define pottery_pq_array_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _displace_and_previous_bulk)

    // link
    #define pottery_pq_array_link POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _link)
    #define pottery_pq_array_link_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _link_first)
    #define pottery_pq_array_link_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _link_last)
    #define pottery_pq_array_link_before POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _link_before)
    #define pottery_pq_array_link_after POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _link_after)

    // unlink
    #define pottery_pq_array_unlink POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink)
    #define pottery_pq_array_unlink_at POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_at)
    #define pottery_pq_array_unlink_key POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_key)
    #define pottery_pq_array_unlink_first POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_first)
    #define pottery_pq_array_unlink_last POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_last)
    #define pottery_pq_array_unlink_and_next POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_and_next)
    #define pottery_pq_array_unlink_and_previous POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_and_previous)
    #define pottery_pq_array_unlink_all POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _unlink_all)

    // special links
    #define pottery_pq_array_replace POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _replace)
    #define pottery_pq_array_find_location POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _find_location)
    #define pottery_pq_array_link_location POTTERY_CONCAT(POTTERY_PRIORITY_QUEUE_NAME(_array), _link_location)
//!!! END_AUTOGENERATED
