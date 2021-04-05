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

#ifndef POTTERY_RED_BLACK_TREE_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"

// configs
#ifndef POTTERY_RED_BLACK_TREE_PREFIX
    #error "POTTERY_RED_BLACK_TREE_PREFIX is not defined."
#endif
#if !POTTERY_COMPARE_CAN_ORDER
    #error "An ordering comparison expression is required."
#endif

#ifdef POTTERY_RED_BLACK_TREE_NULL
    #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
        // NULL is a function-like macro that takes the context.
        #define pottery_rbt_null(ll) POTTERY_RED_BLACK_TREE_NULL(((ll)->context))
    #else
        // NULL is not a function-like macro; there are no parentheses after it.
        #define pottery_rbt_null(ll) (POTTERY_RED_BLACK_TREE_NULL)
    #endif
#else
    // By default null is just a null pointer.
    #define pottery_rbt_null(ll) (pottery_null)
#endif

// TODO this should probably come from container_types
#ifdef POTTERY_RED_BLACK_TREE_CONTEXT_TYPE
    #define POTTERY_RED_BLACK_TREE_HAS_CONTEXT 1
#else
    #define POTTERY_RED_BLACK_TREE_HAS_CONTEXT 0
#endif

#if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
    #define POTTERY_RBT_CONTEXT_VAL(ll) ll->context,
#else
    #define POTTERY_RBT_CONTEXT_VAL(ll) /*nothing*/
#endif


// naming macros
#define POTTERY_RED_BLACK_TREE_NAME(name) POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, name)

// internal functions
// TODO lots missing! this has next/previous, not *_child/parent/is_red/etc, plus more e.g. link_root
#define pottery_rbt_next_ref POTTERY_RED_BLACK_TREE_NAME(_next_ref)
#define pottery_rbt_previous_ref POTTERY_RED_BLACK_TREE_NAME(_previous_ref)
#define pottery_rbt_set_next_ref POTTERY_RED_BLACK_TREE_NAME(_set_next_ref)
#define pottery_rbt_set_previous_ref POTTERY_RED_BLACK_TREE_NAME(_set_previous_ref)
#define pottery_rbt_sanity_check POTTERY_RED_BLACK_TREE_NAME(_sanity_check)
#define pottery_rbt_sanity_check_node POTTERY_RED_BLACK_TREE_NAME(_sanity_check_node)
#define pottery_rbt_ref_is_null POTTERY_RED_BLACK_TREE_NAME(_ref_is_null)



// container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_rbt DEST:POTTERY_RED_BLACK_TREE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_rbt to POTTERY_RED_BLACK_TREE_PREFIX

    // types
    #define pottery_rbt_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _t)
    #define pottery_rbt_entry_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _entry_t)

    // lifecycle operations
    #define pottery_rbt_copy POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _copy)
    #define pottery_rbt_destroy POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _destroy)
    #define pottery_rbt_init POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _init)
    #define pottery_rbt_init_copy POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _init_copy)
    #define pottery_rbt_init_steal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _init_steal)
    #define pottery_rbt_move POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _move)
    #define pottery_rbt_steal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _steal)
    #define pottery_rbt_swap POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _swap)

    // entry operations
    #define pottery_rbt_entry_exists POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _entry_exists)
    #define pottery_rbt_entry_equal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _entry_equal)
    #define pottery_rbt_entry_ref POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _entry_ref)

    // lookup
    #define pottery_rbt_count POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _count)
    #define pottery_rbt_is_empty POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _is_empty)
    #define pottery_rbt_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _at)
    #define pottery_rbt_find POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _find)
    #define pottery_rbt_contains_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _contains_key)
    #define pottery_rbt_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _first)
    #define pottery_rbt_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _last)
    #define pottery_rbt_any POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _any)

    // array access
    #define pottery_rbt_select POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _select)
    #define pottery_rbt_index POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _index)
    #define pottery_rbt_shift POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _shift)
    #define pottery_rbt_offset POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _offset)
    #define pottery_rbt_begin POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _begin)
    #define pottery_rbt_end POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _end)
    #define pottery_rbt_next POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _next)
    #define pottery_rbt_previous POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _previous)

    // misc
    #define pottery_rbt_capacity POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _capacity)
    #define pottery_rbt_reserve POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _reserve)
    #define pottery_rbt_shrink POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _shrink)

    // insert
    #define pottery_rbt_insert POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert)
    #define pottery_rbt_insert_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_at)
    #define pottery_rbt_insert_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_first)
    #define pottery_rbt_insert_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_last)
    #define pottery_rbt_insert_before POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_before)
    #define pottery_rbt_insert_after POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_after)
    #define pottery_rbt_insert_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_bulk)
    #define pottery_rbt_insert_at_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_at_bulk)
    #define pottery_rbt_insert_first_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_first_bulk)
    #define pottery_rbt_insert_last_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_last_bulk)
    #define pottery_rbt_insert_before_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_before_bulk)
    #define pottery_rbt_insert_after_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_rbt_emplace POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace)
    #define pottery_rbt_emplace_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_at)
    #define pottery_rbt_emplace_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_key)
    #define pottery_rbt_emplace_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_first)
    #define pottery_rbt_emplace_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_last)
    #define pottery_rbt_emplace_before POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_before)
    #define pottery_rbt_emplace_after POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_after)
    #define pottery_rbt_emplace_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_bulk)
    #define pottery_rbt_emplace_at_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_at_bulk)
    #define pottery_rbt_emplace_first_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_first_bulk)
    #define pottery_rbt_emplace_last_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_last_bulk)
    #define pottery_rbt_emplace_before_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_before_bulk)
    #define pottery_rbt_emplace_after_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_rbt_construct POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct)
    #define pottery_rbt_construct_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct_at)
    #define pottery_rbt_construct_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct_key)
    #define pottery_rbt_construct_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct_first)
    #define pottery_rbt_construct_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct_last)
    #define pottery_rbt_construct_before POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct_before)
    #define pottery_rbt_construct_after POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _construct_after)

    // extract
    #define pottery_rbt_extract POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract)
    #define pottery_rbt_extract_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_at)
    #define pottery_rbt_extract_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_key)
    #define pottery_rbt_extract_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_first)
    #define pottery_rbt_extract_first_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_first_bulk)
    #define pottery_rbt_extract_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_last)
    #define pottery_rbt_extract_last_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_last_bulk)
    #define pottery_rbt_extract_and_next POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_and_next)
    #define pottery_rbt_extract_and_previous POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _extract_and_previous)

    // remove
    #define pottery_rbt_remove POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove)
    #define pottery_rbt_remove_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_at)
    #define pottery_rbt_remove_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_key)
    #define pottery_rbt_remove_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_first)
    #define pottery_rbt_remove_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_last)
    #define pottery_rbt_remove_and_next POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_and_next)
    #define pottery_rbt_remove_and_previous POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_and_previous)
    #define pottery_rbt_remove_all POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_all)
    #define pottery_rbt_remove_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_bulk)
    #define pottery_rbt_remove_at_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_at_bulk)
    #define pottery_rbt_remove_first_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_first_bulk)
    #define pottery_rbt_remove_last_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_last_bulk)
    #define pottery_rbt_remove_at_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_at_bulk)
    #define pottery_rbt_remove_and_next_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_and_next_bulk)
    #define pottery_rbt_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_rbt_displace POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace)
    #define pottery_rbt_displace_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_at)
    #define pottery_rbt_displace_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_key)
    #define pottery_rbt_displace_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_first)
    #define pottery_rbt_displace_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_last)
    #define pottery_rbt_displace_and_next POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_and_next)
    #define pottery_rbt_displace_and_previous POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_and_previous)
    #define pottery_rbt_displace_all POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_all)
    #define pottery_rbt_displace_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_bulk)
    #define pottery_rbt_displace_at_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_at_bulk)
    #define pottery_rbt_displace_first_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_first_bulk)
    #define pottery_rbt_displace_last_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_last_bulk)
    #define pottery_rbt_displace_and_next_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_and_next_bulk)
    #define pottery_rbt_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_rbt_link POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _link)
    #define pottery_rbt_link_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _link_first)
    #define pottery_rbt_link_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _link_last)
    #define pottery_rbt_link_before POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _link_before)
    #define pottery_rbt_link_after POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _link_after)

    // unlink
    #define pottery_rbt_unlink POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink)
    #define pottery_rbt_unlink_at POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_at)
    #define pottery_rbt_unlink_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_key)
    #define pottery_rbt_unlink_first POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_first)
    #define pottery_rbt_unlink_last POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_last)
    #define pottery_rbt_unlink_and_next POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_and_next)
    #define pottery_rbt_unlink_and_previous POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_and_previous)
    #define pottery_rbt_unlink_all POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_PREFIX, _unlink_all)
//!!! END_AUTOGENERATED



// container_types
#ifdef POTTERY_RED_BLACK_TREE_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX POTTERY_RED_BLACK_TREE_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX POTTERY_RED_BLACK_TREE_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_rbt DEST:POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_rbt to POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX

    #define pottery_rbt_value_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_rbt_ref_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_rbt_const_ref_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_rbt_key_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_rbt_context_t POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_rbt_ref_key POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_rbt_ref_equal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// compare functions
#ifdef POTTERY_RED_BLACK_TREE_EXTERNAL_COMPARE
    #define POTTERY_RED_BLACK_TREE_COMPARE_PREFIX POTTERY_RED_BLACK_TREE_EXTERNAL_COMPARE
#else
    #define POTTERY_RED_BLACK_TREE_COMPARE_PREFIX POTTERY_RED_BLACK_TREE_NAME(_compare)
#endif
//!!! AUTOGENERATED:compare/rename.m.h SRC:pottery_rbt_compare DEST:POTTERY_RED_BLACK_TREE_COMPARE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames compare identifiers from pottery_rbt_compare to POTTERY_RED_BLACK_TREE_COMPARE_PREFIX

    #define pottery_rbt_compare_three_way POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _three_way)
    #define pottery_rbt_compare_equal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _equal)
    #define pottery_rbt_compare_not_equal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _not_equal)
    #define pottery_rbt_compare_less POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _less)
    #define pottery_rbt_compare_less_or_equal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _less_or_equal)
    #define pottery_rbt_compare_greater POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _greater)
    #define pottery_rbt_compare_greater_or_equal POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _greater_or_equal)

    #define pottery_rbt_compare_min POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _min)
    #define pottery_rbt_compare_max POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _max)
    #define pottery_rbt_compare_clamp POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _clamp)
    #define pottery_rbt_compare_median POTTERY_CONCAT(POTTERY_RED_BLACK_TREE_COMPARE_PREFIX, _median)
//!!! END_AUTOGENERATED
