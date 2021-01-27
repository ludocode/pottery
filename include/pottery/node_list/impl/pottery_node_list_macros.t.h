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

#ifndef POTTERY_NODE_LIST_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_NODE_LIST_PREFIX
#error "POTTERY_NODE_LIST_PREFIX is not defined."
#endif

#ifndef POTTERY_NODE_LIST_VALUE_TYPE
#error "POTTERY_NODE_LIST_VALUE_TYPE is not defined."
#endif

#ifndef POTTERY_NODE_LIST_EXTERN
#define POTTERY_NODE_LIST_EXTERN /*nothing*/
#endif

// Context forwarding
#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    #define POTTERY_NODE_LIST_CONTEXT_VAL(nl) ((nl)->context),
#else
    #define POTTERY_NODE_LIST_CONTEXT_VAL(nl) /*nothing*/
#endif

// Remap capabilities (the doubly_linked_list defines its own container types)
#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    #define POTTERY_NODE_LIST_HAS_CONTEXT 1
#else
    #define POTTERY_NODE_LIST_HAS_CONTEXT 0
#endif



/*
 * Renaming
 */

#define POTTERY_NODE_LIST_NAME(name) \
    POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, name)

#define pottery_nl_alloc_node POTTERY_NODE_LIST_NAME(_alloc_node)
#define pottery_nl_free_node POTTERY_NODE_LIST_NAME(_free_node)



// container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_nl DEST:POTTERY_NODE_LIST_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_nl to POTTERY_NODE_LIST_PREFIX

    // types
    #define pottery_nl_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _t)
    #define pottery_nl_value_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _value_t)
    #define pottery_nl_ref_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _ref_t)
    #define pottery_nl_entry_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _entry_t)
    #define pottery_nl_context_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _context_t)

    // lifecycle operations
    #define pottery_nl_copy POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _copy)
    #define pottery_nl_destroy POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _destroy)
    #define pottery_nl_init POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _init)
    #define pottery_nl_init_copy POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _init_copy)
    #define pottery_nl_init_steal POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _init_steal)
    #define pottery_nl_move POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _move)
    #define pottery_nl_steal POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _steal)
    #define pottery_nl_swap POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _swap)

    // entry operations
    #define pottery_nl_entry_exists POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _entry_exists)
    #define pottery_nl_entry_equal POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _entry_equal)
    #define pottery_nl_entry_value POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _entry_value)
    #define pottery_nl_entry_ref POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _entry_ref)

    // lookup
    #define pottery_nl_count POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _count)
    #define pottery_nl_is_empty POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _is_empty)
    #define pottery_nl_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _at)
    #define pottery_nl_find POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _find)
    #define pottery_nl_contains_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _contains_key)
    #define pottery_nl_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _first)
    #define pottery_nl_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _last)
    #define pottery_nl_any POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _any)

    // array access
    #define pottery_nl_select POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _select)
    #define pottery_nl_index POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _index)
    #define pottery_nl_shift POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _shift)
    #define pottery_nl_offset POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _offset)
    #define pottery_nl_begin POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _begin)
    #define pottery_nl_end POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _end)
    #define pottery_nl_next POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _next)
    #define pottery_nl_previous POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _previous)

    // misc
    #define pottery_nl_capacity POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _capacity)
    #define pottery_nl_reserve POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _reserve)
    #define pottery_nl_shrink POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _shrink)

    // insert
    #define pottery_nl_insert POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert)
    #define pottery_nl_insert_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_at)
    #define pottery_nl_insert_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_first)
    #define pottery_nl_insert_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_last)
    #define pottery_nl_insert_before POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_before)
    #define pottery_nl_insert_after POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_after)
    #define pottery_nl_insert_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_bulk)
    #define pottery_nl_insert_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_at_bulk)
    #define pottery_nl_insert_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_first_bulk)
    #define pottery_nl_insert_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_last_bulk)
    #define pottery_nl_insert_before_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_before_bulk)
    #define pottery_nl_insert_after_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_nl_emplace POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace)
    #define pottery_nl_emplace_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_at)
    #define pottery_nl_emplace_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_key)
    #define pottery_nl_emplace_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_first)
    #define pottery_nl_emplace_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_last)
    #define pottery_nl_emplace_before POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_before)
    #define pottery_nl_emplace_after POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_after)
    #define pottery_nl_emplace_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_bulk)
    #define pottery_nl_emplace_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_at_bulk)
    #define pottery_nl_emplace_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_first_bulk)
    #define pottery_nl_emplace_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_last_bulk)
    #define pottery_nl_emplace_before_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_before_bulk)
    #define pottery_nl_emplace_after_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_nl_construct POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct)
    #define pottery_nl_construct_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct_at)
    #define pottery_nl_construct_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct_key)
    #define pottery_nl_construct_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct_first)
    #define pottery_nl_construct_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct_last)
    #define pottery_nl_construct_before POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct_before)
    #define pottery_nl_construct_after POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _construct_after)

    // extract
    #define pottery_nl_extract POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract)
    #define pottery_nl_extract_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_at)
    #define pottery_nl_extract_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_key)
    #define pottery_nl_extract_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_first)
    #define pottery_nl_extract_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_first_bulk)
    #define pottery_nl_extract_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_last)
    #define pottery_nl_extract_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_last_bulk)
    #define pottery_nl_extract_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_and_next)
    #define pottery_nl_extract_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _extract_and_previous)

    // remove
    #define pottery_nl_remove POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove)
    #define pottery_nl_remove_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_at)
    #define pottery_nl_remove_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_key)
    #define pottery_nl_remove_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_first)
    #define pottery_nl_remove_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_last)
    #define pottery_nl_remove_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_and_next)
    #define pottery_nl_remove_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_and_previous)
    #define pottery_nl_remove_all POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_all)
    #define pottery_nl_remove_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_bulk)
    #define pottery_nl_remove_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_at_bulk)
    #define pottery_nl_remove_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_first_bulk)
    #define pottery_nl_remove_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_last_bulk)
    #define pottery_nl_remove_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_at_bulk)
    #define pottery_nl_remove_and_next_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_and_next_bulk)
    #define pottery_nl_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_nl_displace POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace)
    #define pottery_nl_displace_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_at)
    #define pottery_nl_displace_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_key)
    #define pottery_nl_displace_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_first)
    #define pottery_nl_displace_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_last)
    #define pottery_nl_displace_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_and_next)
    #define pottery_nl_displace_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_and_previous)
    #define pottery_nl_displace_all POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_all)
    #define pottery_nl_displace_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_bulk)
    #define pottery_nl_displace_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_at_bulk)
    #define pottery_nl_displace_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_first_bulk)
    #define pottery_nl_displace_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_last_bulk)
    #define pottery_nl_displace_and_next_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_and_next_bulk)
    #define pottery_nl_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_nl_link POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _link)
    #define pottery_nl_link_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _link_first)
    #define pottery_nl_link_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _link_last)
    #define pottery_nl_link_before POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _link_before)
    #define pottery_nl_link_after POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _link_after)

    // unlink
    #define pottery_nl_unlink POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink)
    #define pottery_nl_unlink_at POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_at)
    #define pottery_nl_unlink_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_key)
    #define pottery_nl_unlink_first POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_first)
    #define pottery_nl_unlink_last POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_last)
    #define pottery_nl_unlink_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_and_next)
    #define pottery_nl_unlink_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_and_previous)
    #define pottery_nl_unlink_all POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _unlink_all)
//!!! END_AUTOGENERATED



// doubly_linked_list functions
#define POTTERY_NODE_LIST_DLL_PREFIX POTTERY_NODE_LIST_NAME(_dll)
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_nl_dll DEST:POTTERY_NODE_LIST_DLL_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_nl_dll to POTTERY_NODE_LIST_DLL_PREFIX

    // types
    #define pottery_nl_dll_t POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _t)
    #define pottery_nl_dll_value_t POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _value_t)
    #define pottery_nl_dll_ref_t POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _ref_t)
    #define pottery_nl_dll_entry_t POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _entry_t)
    #define pottery_nl_dll_context_t POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _context_t)

    // lifecycle operations
    #define pottery_nl_dll_copy POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _copy)
    #define pottery_nl_dll_destroy POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _destroy)
    #define pottery_nl_dll_init POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _init)
    #define pottery_nl_dll_init_copy POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _init_copy)
    #define pottery_nl_dll_init_steal POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _init_steal)
    #define pottery_nl_dll_move POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _move)
    #define pottery_nl_dll_steal POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _steal)
    #define pottery_nl_dll_swap POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _swap)

    // entry operations
    #define pottery_nl_dll_entry_exists POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _entry_exists)
    #define pottery_nl_dll_entry_equal POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _entry_equal)
    #define pottery_nl_dll_entry_value POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _entry_value)
    #define pottery_nl_dll_entry_ref POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _entry_ref)

    // lookup
    #define pottery_nl_dll_count POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _count)
    #define pottery_nl_dll_is_empty POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _is_empty)
    #define pottery_nl_dll_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _at)
    #define pottery_nl_dll_find POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _find)
    #define pottery_nl_dll_contains_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _contains_key)
    #define pottery_nl_dll_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _first)
    #define pottery_nl_dll_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _last)
    #define pottery_nl_dll_any POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _any)

    // array access
    #define pottery_nl_dll_select POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _select)
    #define pottery_nl_dll_index POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _index)
    #define pottery_nl_dll_shift POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _shift)
    #define pottery_nl_dll_offset POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _offset)
    #define pottery_nl_dll_begin POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _begin)
    #define pottery_nl_dll_end POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _end)
    #define pottery_nl_dll_next POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _next)
    #define pottery_nl_dll_previous POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _previous)

    // misc
    #define pottery_nl_dll_capacity POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _capacity)
    #define pottery_nl_dll_reserve POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _reserve)
    #define pottery_nl_dll_shrink POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _shrink)

    // insert
    #define pottery_nl_dll_insert POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert)
    #define pottery_nl_dll_insert_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_at)
    #define pottery_nl_dll_insert_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_first)
    #define pottery_nl_dll_insert_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_last)
    #define pottery_nl_dll_insert_before POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_before)
    #define pottery_nl_dll_insert_after POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_after)
    #define pottery_nl_dll_insert_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_bulk)
    #define pottery_nl_dll_insert_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_at_bulk)
    #define pottery_nl_dll_insert_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_first_bulk)
    #define pottery_nl_dll_insert_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_last_bulk)
    #define pottery_nl_dll_insert_before_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_before_bulk)
    #define pottery_nl_dll_insert_after_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_nl_dll_emplace POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace)
    #define pottery_nl_dll_emplace_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_at)
    #define pottery_nl_dll_emplace_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_key)
    #define pottery_nl_dll_emplace_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_first)
    #define pottery_nl_dll_emplace_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_last)
    #define pottery_nl_dll_emplace_before POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_before)
    #define pottery_nl_dll_emplace_after POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_after)
    #define pottery_nl_dll_emplace_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_bulk)
    #define pottery_nl_dll_emplace_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_at_bulk)
    #define pottery_nl_dll_emplace_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_first_bulk)
    #define pottery_nl_dll_emplace_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_last_bulk)
    #define pottery_nl_dll_emplace_before_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_before_bulk)
    #define pottery_nl_dll_emplace_after_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_nl_dll_construct POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct)
    #define pottery_nl_dll_construct_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct_at)
    #define pottery_nl_dll_construct_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct_key)
    #define pottery_nl_dll_construct_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct_first)
    #define pottery_nl_dll_construct_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct_last)
    #define pottery_nl_dll_construct_before POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct_before)
    #define pottery_nl_dll_construct_after POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _construct_after)

    // extract
    #define pottery_nl_dll_extract POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract)
    #define pottery_nl_dll_extract_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_at)
    #define pottery_nl_dll_extract_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_key)
    #define pottery_nl_dll_extract_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_first)
    #define pottery_nl_dll_extract_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_first_bulk)
    #define pottery_nl_dll_extract_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_last)
    #define pottery_nl_dll_extract_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_last_bulk)
    #define pottery_nl_dll_extract_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_and_next)
    #define pottery_nl_dll_extract_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _extract_and_previous)

    // remove
    #define pottery_nl_dll_remove POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove)
    #define pottery_nl_dll_remove_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_at)
    #define pottery_nl_dll_remove_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_key)
    #define pottery_nl_dll_remove_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_first)
    #define pottery_nl_dll_remove_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_last)
    #define pottery_nl_dll_remove_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_and_next)
    #define pottery_nl_dll_remove_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_and_previous)
    #define pottery_nl_dll_remove_all POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_all)
    #define pottery_nl_dll_remove_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_bulk)
    #define pottery_nl_dll_remove_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_at_bulk)
    #define pottery_nl_dll_remove_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_first_bulk)
    #define pottery_nl_dll_remove_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_last_bulk)
    #define pottery_nl_dll_remove_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_at_bulk)
    #define pottery_nl_dll_remove_and_next_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_and_next_bulk)
    #define pottery_nl_dll_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_nl_dll_displace POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace)
    #define pottery_nl_dll_displace_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_at)
    #define pottery_nl_dll_displace_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_key)
    #define pottery_nl_dll_displace_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_first)
    #define pottery_nl_dll_displace_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_last)
    #define pottery_nl_dll_displace_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_and_next)
    #define pottery_nl_dll_displace_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_and_previous)
    #define pottery_nl_dll_displace_all POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_all)
    #define pottery_nl_dll_displace_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_bulk)
    #define pottery_nl_dll_displace_at_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_at_bulk)
    #define pottery_nl_dll_displace_first_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_first_bulk)
    #define pottery_nl_dll_displace_last_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_last_bulk)
    #define pottery_nl_dll_displace_and_next_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_and_next_bulk)
    #define pottery_nl_dll_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_nl_dll_link POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _link)
    #define pottery_nl_dll_link_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _link_first)
    #define pottery_nl_dll_link_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _link_last)
    #define pottery_nl_dll_link_before POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _link_before)
    #define pottery_nl_dll_link_after POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _link_after)

    // unlink
    #define pottery_nl_dll_unlink POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink)
    #define pottery_nl_dll_unlink_at POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_at)
    #define pottery_nl_dll_unlink_key POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_key)
    #define pottery_nl_dll_unlink_first POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_first)
    #define pottery_nl_dll_unlink_last POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_last)
    #define pottery_nl_dll_unlink_and_next POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_and_next)
    #define pottery_nl_dll_unlink_and_previous POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_and_previous)
    #define pottery_nl_dll_unlink_all POTTERY_CONCAT(POTTERY_NODE_LIST_DLL_PREFIX, _unlink_all)
//!!! END_AUTOGENERATED



// alloc functions
#define POTTERY_NODE_LIST_ALLOC_PREFIX POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _alloc)
//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_nl_alloc DEST:POTTERY_NODE_LIST_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_nl_alloc to POTTERY_NODE_LIST_ALLOC_PREFIX

    #define pottery_nl_alloc_free POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _free)
    #define pottery_nl_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_nl_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_nl_alloc_malloc POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _malloc)
    #define pottery_nl_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_nl_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_nl_alloc_malloc_zero POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _malloc_zero)
    #define pottery_nl_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_nl_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_nl_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_nl_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_nl_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_nl_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_nl_alloc_malloc_array POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _malloc_array)
    #define pottery_nl_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_NODE_LIST_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED



// container types
// TODO disabled while we figure out how container_types is going to work for
// dynamic containers (type renaming is duplicated between container/rename and
// container_types/rename)
#if 0
#ifdef POTTERY_NODE_LIST_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_NODE_LIST_CONTAINER_TYPES_PREFIX POTTERY_NODE_LIST_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_NODE_LIST_CONTAINER_TYPES_PREFIX POTTERY_NODE_LIST_PREFIX
#endif
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_nl DEST:POTTERY_NODE_LIST_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_nl to POTTERY_NODE_LIST_PREFIX

    #define pottery_nl_value_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _value_t)
    #define pottery_nl_ref_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _ref_t)
    #define pottery_nl_const_ref_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _const_ref_t)
    #define pottery_nl_key_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _key_t)
    #define pottery_nl_context_t POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _context_t)

    #define pottery_nl_ref_key POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _ref_key)
    #define pottery_nl_ref_equal POTTERY_CONCAT(POTTERY_NODE_LIST_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions
#define POTTERY_NODE_LIST_LIFECYCLE_PREFIX POTTERY_NODE_LIST_NAME(_lifecycle)
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_nl_lifecycle DEST:POTTERY_NODE_LIST_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_nl_lifecycle to POTTERY_NODE_LIST_LIFECYCLE_PREFIX

    #define pottery_nl_lifecycle_init_steal POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_nl_lifecycle_init_copy POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_nl_lifecycle_init POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _init)
    #define pottery_nl_lifecycle_destroy POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _destroy)

    #define pottery_nl_lifecycle_swap POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _swap)
    #define pottery_nl_lifecycle_move POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move)
    #define pottery_nl_lifecycle_steal POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _steal)
    #define pottery_nl_lifecycle_copy POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _copy)
    #define pottery_nl_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_nl_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_nl_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_nl_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_nl_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_nl_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_nl_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_nl_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_nl_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_nl_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_nl_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_nl_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_NODE_LIST_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED
