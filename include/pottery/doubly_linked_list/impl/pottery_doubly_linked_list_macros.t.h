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

#ifndef POTTERY_DOUBLY_LINKED_LIST_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"

// configs
#ifndef POTTERY_DOUBLY_LINKED_LIST_PREFIX
    #error "POTTERY_DOUBLY_LINKED_LIST_PREFIX is not defined."
#endif

#ifdef POTTERY_DOUBLY_LINKED_LIST_NULL
    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
        // NULL is a function-like macro that takes the context.
        #define pottery_dll_null(ll) POTTERY_DOUBLY_LINKED_LIST_NULL(((ll)->context))
    #else
        // NULL is not a function-like macro; there are no parentheses after it.
        #define pottery_dll_null(ll) (POTTERY_DOUBLY_LINKED_LIST_NULL)
    #endif
#else
    // By default null is just a null pointer.
    #define pottery_dll_null(ll) (pottery_null)
#endif

#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    #define POTTERY_DLL_CONTEXT_VAL(ll) ll->context,
#else
    #define POTTERY_DLL_CONTEXT_VAL(ll) /*nothing*/
#endif


// naming macros
#define POTTERY_DOUBLY_LINKED_LIST_NAME(name) POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, name)

// internal functions
#define pottery_dll_next_ref POTTERY_DOUBLY_LINKED_LIST_NAME(_next_ref)
#define pottery_dll_previous_ref POTTERY_DOUBLY_LINKED_LIST_NAME(_previous_ref)
#define pottery_dll_set_next_ref POTTERY_DOUBLY_LINKED_LIST_NAME(_set_next_ref)
#define pottery_dll_set_previous_ref POTTERY_DOUBLY_LINKED_LIST_NAME(_set_previous_ref)
#define pottery_dll_sanity_check POTTERY_DOUBLY_LINKED_LIST_NAME(_sanity_check)
#define pottery_dll_ref_is_null POTTERY_DOUBLY_LINKED_LIST_NAME(_ref_is_null)



// container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_dll DEST:POTTERY_DOUBLY_LINKED_LIST_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_dll to POTTERY_DOUBLY_LINKED_LIST_PREFIX

    // types
    #define pottery_dll_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _t)
    #define pottery_dll_entry_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _entry_t)

    // lifecycle operations
    #define pottery_dll_copy POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _copy)
    #define pottery_dll_destroy POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _destroy)
    #define pottery_dll_init POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _init)
    #define pottery_dll_init_copy POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _init_copy)
    #define pottery_dll_init_steal POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _init_steal)
    #define pottery_dll_move POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _move)
    #define pottery_dll_steal POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _steal)
    #define pottery_dll_swap POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _swap)

    // entry operations
    #define pottery_dll_entry_exists POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _entry_exists)
    #define pottery_dll_entry_equal POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _entry_equal)
    #define pottery_dll_entry_ref POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _entry_ref)

    // lookup
    #define pottery_dll_count POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _count)
    #define pottery_dll_is_empty POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _is_empty)
    #define pottery_dll_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _at)
    #define pottery_dll_find POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _find)
    #define pottery_dll_contains_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _contains_key)
    #define pottery_dll_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _first)
    #define pottery_dll_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _last)
    #define pottery_dll_any POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _any)

    // array access
    #define pottery_dll_select POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _select)
    #define pottery_dll_index POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _index)
    #define pottery_dll_shift POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _shift)
    #define pottery_dll_offset POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _offset)
    #define pottery_dll_begin POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _begin)
    #define pottery_dll_end POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _end)
    #define pottery_dll_next POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _next)
    #define pottery_dll_previous POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _previous)

    // misc
    #define pottery_dll_capacity POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _capacity)
    #define pottery_dll_reserve POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _reserve)
    #define pottery_dll_shrink POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _shrink)

    // insert
    #define pottery_dll_insert POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert)
    #define pottery_dll_insert_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_at)
    #define pottery_dll_insert_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_first)
    #define pottery_dll_insert_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_last)
    #define pottery_dll_insert_before POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_before)
    #define pottery_dll_insert_after POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_after)
    #define pottery_dll_insert_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_bulk)
    #define pottery_dll_insert_at_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_at_bulk)
    #define pottery_dll_insert_first_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_first_bulk)
    #define pottery_dll_insert_last_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_last_bulk)
    #define pottery_dll_insert_before_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_before_bulk)
    #define pottery_dll_insert_after_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_dll_emplace POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace)
    #define pottery_dll_emplace_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_at)
    #define pottery_dll_emplace_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_key)
    #define pottery_dll_emplace_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_first)
    #define pottery_dll_emplace_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_last)
    #define pottery_dll_emplace_before POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_before)
    #define pottery_dll_emplace_after POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_after)
    #define pottery_dll_emplace_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_bulk)
    #define pottery_dll_emplace_at_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_at_bulk)
    #define pottery_dll_emplace_first_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_first_bulk)
    #define pottery_dll_emplace_last_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_last_bulk)
    #define pottery_dll_emplace_before_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_before_bulk)
    #define pottery_dll_emplace_after_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_dll_construct POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct)
    #define pottery_dll_construct_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct_at)
    #define pottery_dll_construct_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct_key)
    #define pottery_dll_construct_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct_first)
    #define pottery_dll_construct_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct_last)
    #define pottery_dll_construct_before POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct_before)
    #define pottery_dll_construct_after POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _construct_after)

    // extract
    #define pottery_dll_extract POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract)
    #define pottery_dll_extract_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_at)
    #define pottery_dll_extract_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_key)
    #define pottery_dll_extract_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_first)
    #define pottery_dll_extract_first_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_first_bulk)
    #define pottery_dll_extract_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_last)
    #define pottery_dll_extract_last_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_last_bulk)
    #define pottery_dll_extract_and_next POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_and_next)
    #define pottery_dll_extract_and_previous POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _extract_and_previous)

    // remove
    #define pottery_dll_remove POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove)
    #define pottery_dll_remove_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_at)
    #define pottery_dll_remove_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_key)
    #define pottery_dll_remove_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_first)
    #define pottery_dll_remove_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_last)
    #define pottery_dll_remove_and_next POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_and_next)
    #define pottery_dll_remove_and_previous POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_and_previous)
    #define pottery_dll_remove_all POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_all)
    #define pottery_dll_remove_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_bulk)
    #define pottery_dll_remove_at_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_at_bulk)
    #define pottery_dll_remove_first_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_first_bulk)
    #define pottery_dll_remove_last_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_last_bulk)
    #define pottery_dll_remove_at_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_at_bulk)
    #define pottery_dll_remove_and_next_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_and_next_bulk)
    #define pottery_dll_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_dll_displace POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace)
    #define pottery_dll_displace_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_at)
    #define pottery_dll_displace_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_key)
    #define pottery_dll_displace_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_first)
    #define pottery_dll_displace_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_last)
    #define pottery_dll_displace_and_next POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_and_next)
    #define pottery_dll_displace_and_previous POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_and_previous)
    #define pottery_dll_displace_all POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_all)
    #define pottery_dll_displace_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_bulk)
    #define pottery_dll_displace_at_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_at_bulk)
    #define pottery_dll_displace_first_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_first_bulk)
    #define pottery_dll_displace_last_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_last_bulk)
    #define pottery_dll_displace_and_next_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_and_next_bulk)
    #define pottery_dll_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_dll_link POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _link)
    #define pottery_dll_link_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _link_first)
    #define pottery_dll_link_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _link_last)
    #define pottery_dll_link_before POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _link_before)
    #define pottery_dll_link_after POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _link_after)

    // unlink
    #define pottery_dll_unlink POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink)
    #define pottery_dll_unlink_at POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_at)
    #define pottery_dll_unlink_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_key)
    #define pottery_dll_unlink_first POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_first)
    #define pottery_dll_unlink_last POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_last)
    #define pottery_dll_unlink_and_next POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_and_next)
    #define pottery_dll_unlink_and_previous POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_and_previous)
    #define pottery_dll_unlink_all POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_PREFIX, _unlink_all)
//!!! END_AUTOGENERATED



// container_types
#ifdef POTTERY_DOUBLY_LINKED_LIST_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX POTTERY_DOUBLY_LINKED_LIST_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX POTTERY_DOUBLY_LINKED_LIST_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_dll DEST:POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_dll to POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX

    #define pottery_dll_value_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_dll_ref_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_dll_const_ref_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_dll_key_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_dll_context_t POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_dll_ref_key POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_dll_ref_equal POTTERY_CONCAT(POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED