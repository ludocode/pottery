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

#ifndef POTTERY_TREE_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_TREE_MAP_PREFIX
#error "POTTERY_TREE_MAP_PREFIX is not defined."
#endif

#ifndef POTTERY_TREE_MAP_VALUE_TYPE
#error "POTTERY_TREE_MAP_VALUE_TYPE is not defined."
#endif

#ifndef POTTERY_TREE_MAP_EXTERN
#define POTTERY_TREE_MAP_EXTERN /*nothing*/
#endif

// Context forwarding
#ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
    #define POTTERY_TREE_MAP_CONTEXT_VAL context,
#else
    #define POTTERY_TREE_MAP_CONTEXT_VAL /*nothing*/
#endif

// We need to store certain lifecycle and container_types capabilities because
// we clean them up before instantiating our red_black_tree.
#if POTTERY_LIFECYCLE_CAN_PASS
    #define POTTERY_TREE_MAP_CAN_PASS 1
#else
    #define POTTERY_TREE_MAP_CAN_PASS 0
#endif
#if POTTERY_LIFECYCLE_CAN_DESTROY
    #define POTTERY_TREE_MAP_CAN_DESTROY 1
#else
    #define POTTERY_TREE_MAP_CAN_DESTROY 0
#endif



/*
 * Renaming
 */

#define POTTERY_TREE_MAP_NAME(name) \
    POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, name)

// TODO fix
#define pottery_tree_map_bucket_count POTTERY_TREE_MAP_NAME(_bucket_count)
#define pottery_tree_map_impl_values POTTERY_TREE_MAP_NAME(_impl_values)
#define pottery_tree_map_impl_alloc POTTERY_TREE_MAP_NAME(_impl_alloc)
#define pottery_tree_map_impl_free POTTERY_TREE_MAP_NAME(_impl_free)
#define pottery_tree_map_impl_free_allocs POTTERY_TREE_MAP_NAME(_impl_free_allocs)
#define pottery_tree_map_grow_if_needed POTTERY_TREE_MAP_NAME(_grow_if_needed)
#define pottery_tree_map_shrink_if_needed POTTERY_TREE_MAP_NAME(_shrink_if_needed)
#define pottery_tree_map_rehash POTTERY_TREE_MAP_NAME(_rehash)
#define pottery_tree_map_load POTTERY_TREE_MAP_NAME(_load)
#define pottery_tree_map_init_impl POTTERY_TREE_MAP_NAME(_init_impl)



// container funcs
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_tree_map DEST:POTTERY_TREE_MAP_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_tree_map to POTTERY_TREE_MAP_PREFIX

    // types
    #define pottery_tree_map_t POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _t)
    #define pottery_tree_map_entry_t POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _entry_t)
    #define pottery_tree_map_location_t POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_tree_map_copy POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _copy)
    #define pottery_tree_map_destroy POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _destroy)
    #define pottery_tree_map_init POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _init)
    #define pottery_tree_map_init_copy POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _init_copy)
    #define pottery_tree_map_init_steal POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _init_steal)
    #define pottery_tree_map_move POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _move)
    #define pottery_tree_map_steal POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _steal)
    #define pottery_tree_map_swap POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _swap)

    // entry operations
    #define pottery_tree_map_entry_exists POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _entry_exists)
    #define pottery_tree_map_entry_equal POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _entry_equal)
    #define pottery_tree_map_entry_ref POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _entry_ref)

    // lookup
    #define pottery_tree_map_count POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _count)
    #define pottery_tree_map_is_empty POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _is_empty)
    #define pottery_tree_map_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _at)
    #define pottery_tree_map_find POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _find)
    #define pottery_tree_map_contains_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _contains_key)
    #define pottery_tree_map_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _first)
    #define pottery_tree_map_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _last)
    #define pottery_tree_map_any POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _any)

    // array access
    #define pottery_tree_map_select POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _select)
    #define pottery_tree_map_index POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _index)
    #define pottery_tree_map_shift POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _shift)
    #define pottery_tree_map_offset POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _offset)
    #define pottery_tree_map_begin POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _begin)
    #define pottery_tree_map_end POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _end)
    #define pottery_tree_map_next POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _next)
    #define pottery_tree_map_previous POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _previous)

    // misc
    #define pottery_tree_map_capacity POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _capacity)
    #define pottery_tree_map_reserve POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _reserve)
    #define pottery_tree_map_shrink POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _shrink)

    // insert
    #define pottery_tree_map_insert POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert)
    #define pottery_tree_map_insert_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_at)
    #define pottery_tree_map_insert_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_first)
    #define pottery_tree_map_insert_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_last)
    #define pottery_tree_map_insert_before POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_before)
    #define pottery_tree_map_insert_after POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_after)
    #define pottery_tree_map_insert_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_bulk)
    #define pottery_tree_map_insert_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_at_bulk)
    #define pottery_tree_map_insert_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_first_bulk)
    #define pottery_tree_map_insert_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_last_bulk)
    #define pottery_tree_map_insert_before_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_before_bulk)
    #define pottery_tree_map_insert_after_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_tree_map_emplace POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace)
    #define pottery_tree_map_emplace_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_at)
    #define pottery_tree_map_emplace_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_key)
    #define pottery_tree_map_emplace_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_first)
    #define pottery_tree_map_emplace_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_last)
    #define pottery_tree_map_emplace_before POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_before)
    #define pottery_tree_map_emplace_after POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_after)
    #define pottery_tree_map_emplace_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_bulk)
    #define pottery_tree_map_emplace_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_at_bulk)
    #define pottery_tree_map_emplace_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_first_bulk)
    #define pottery_tree_map_emplace_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_last_bulk)
    #define pottery_tree_map_emplace_before_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_before_bulk)
    #define pottery_tree_map_emplace_after_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_tree_map_construct POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct)
    #define pottery_tree_map_construct_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct_at)
    #define pottery_tree_map_construct_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct_key)
    #define pottery_tree_map_construct_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct_first)
    #define pottery_tree_map_construct_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct_last)
    #define pottery_tree_map_construct_before POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct_before)
    #define pottery_tree_map_construct_after POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _construct_after)

    // extract
    #define pottery_tree_map_extract POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract)
    #define pottery_tree_map_extract_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_at)
    #define pottery_tree_map_extract_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_key)
    #define pottery_tree_map_extract_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_first)
    #define pottery_tree_map_extract_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_first_bulk)
    #define pottery_tree_map_extract_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_last)
    #define pottery_tree_map_extract_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_last_bulk)
    #define pottery_tree_map_extract_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_and_next)
    #define pottery_tree_map_extract_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _extract_and_previous)

    // remove
    #define pottery_tree_map_remove POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove)
    #define pottery_tree_map_remove_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_at)
    #define pottery_tree_map_remove_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_key)
    #define pottery_tree_map_remove_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_first)
    #define pottery_tree_map_remove_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_last)
    #define pottery_tree_map_remove_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_and_next)
    #define pottery_tree_map_remove_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_and_previous)
    #define pottery_tree_map_remove_all POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_all)
    #define pottery_tree_map_remove_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_bulk)
    #define pottery_tree_map_remove_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_at_bulk)
    #define pottery_tree_map_remove_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_first_bulk)
    #define pottery_tree_map_remove_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_last_bulk)
    #define pottery_tree_map_remove_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_at_bulk)
    #define pottery_tree_map_remove_and_next_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_and_next_bulk)
    #define pottery_tree_map_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_tree_map_displace POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace)
    #define pottery_tree_map_displace_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_at)
    #define pottery_tree_map_displace_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_key)
    #define pottery_tree_map_displace_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_first)
    #define pottery_tree_map_displace_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_last)
    #define pottery_tree_map_displace_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_and_next)
    #define pottery_tree_map_displace_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_and_previous)
    #define pottery_tree_map_displace_all POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_all)
    #define pottery_tree_map_displace_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_bulk)
    #define pottery_tree_map_displace_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_at_bulk)
    #define pottery_tree_map_displace_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_first_bulk)
    #define pottery_tree_map_displace_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_last_bulk)
    #define pottery_tree_map_displace_and_next_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_and_next_bulk)
    #define pottery_tree_map_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_tree_map_link POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _link)
    #define pottery_tree_map_link_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _link_first)
    #define pottery_tree_map_link_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _link_last)
    #define pottery_tree_map_link_before POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _link_before)
    #define pottery_tree_map_link_after POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _link_after)

    // unlink
    #define pottery_tree_map_unlink POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink)
    #define pottery_tree_map_unlink_at POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_at)
    #define pottery_tree_map_unlink_key POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_key)
    #define pottery_tree_map_unlink_first POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_first)
    #define pottery_tree_map_unlink_last POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_last)
    #define pottery_tree_map_unlink_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_and_next)
    #define pottery_tree_map_unlink_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_and_previous)
    #define pottery_tree_map_unlink_all POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _unlink_all)

    // special links
    #define pottery_tree_map_replace POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _replace)
    #define pottery_tree_map_find_location POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _find_location)
    #define pottery_tree_map_link_location POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _link_location)
//!!! END_AUTOGENERATED



// red_black_tree functions
#define POTTERY_TREE_MAP_RBT_PREFIX POTTERY_TREE_MAP_NAME(_tree)
//!!! AUTOGENERATED:container/rename.m.h SRC:pottery_tree_map_tree DEST:POTTERY_TREE_MAP_RBT_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_tree_map_tree to POTTERY_TREE_MAP_RBT_PREFIX

    // types
    #define pottery_tree_map_tree_t POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _t)
    #define pottery_tree_map_tree_entry_t POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _entry_t)
    #define pottery_tree_map_tree_location_t POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _location_t) // currently only used for intrusive maps

    // lifecycle operations
    #define pottery_tree_map_tree_copy POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _copy)
    #define pottery_tree_map_tree_destroy POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _destroy)
    #define pottery_tree_map_tree_init POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _init)
    #define pottery_tree_map_tree_init_copy POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _init_copy)
    #define pottery_tree_map_tree_init_steal POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _init_steal)
    #define pottery_tree_map_tree_move POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _move)
    #define pottery_tree_map_tree_steal POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _steal)
    #define pottery_tree_map_tree_swap POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _swap)

    // entry operations
    #define pottery_tree_map_tree_entry_exists POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _entry_exists)
    #define pottery_tree_map_tree_entry_equal POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _entry_equal)
    #define pottery_tree_map_tree_entry_ref POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _entry_ref)

    // lookup
    #define pottery_tree_map_tree_count POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _count)
    #define pottery_tree_map_tree_is_empty POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _is_empty)
    #define pottery_tree_map_tree_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _at)
    #define pottery_tree_map_tree_find POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _find)
    #define pottery_tree_map_tree_contains_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _contains_key)
    #define pottery_tree_map_tree_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _first)
    #define pottery_tree_map_tree_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _last)
    #define pottery_tree_map_tree_any POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _any)

    // array access
    #define pottery_tree_map_tree_select POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _select)
    #define pottery_tree_map_tree_index POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _index)
    #define pottery_tree_map_tree_shift POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _shift)
    #define pottery_tree_map_tree_offset POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _offset)
    #define pottery_tree_map_tree_begin POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _begin)
    #define pottery_tree_map_tree_end POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _end)
    #define pottery_tree_map_tree_next POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _next)
    #define pottery_tree_map_tree_previous POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _previous)

    // misc
    #define pottery_tree_map_tree_capacity POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _capacity)
    #define pottery_tree_map_tree_reserve POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _reserve)
    #define pottery_tree_map_tree_shrink POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _shrink)

    // insert
    #define pottery_tree_map_tree_insert POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert)
    #define pottery_tree_map_tree_insert_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_at)
    #define pottery_tree_map_tree_insert_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_first)
    #define pottery_tree_map_tree_insert_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_last)
    #define pottery_tree_map_tree_insert_before POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_before)
    #define pottery_tree_map_tree_insert_after POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_after)
    #define pottery_tree_map_tree_insert_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_bulk)
    #define pottery_tree_map_tree_insert_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_at_bulk)
    #define pottery_tree_map_tree_insert_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_first_bulk)
    #define pottery_tree_map_tree_insert_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_last_bulk)
    #define pottery_tree_map_tree_insert_before_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_before_bulk)
    #define pottery_tree_map_tree_insert_after_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_tree_map_tree_emplace POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace)
    #define pottery_tree_map_tree_emplace_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_at)
    #define pottery_tree_map_tree_emplace_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_key)
    #define pottery_tree_map_tree_emplace_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_first)
    #define pottery_tree_map_tree_emplace_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_last)
    #define pottery_tree_map_tree_emplace_before POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_before)
    #define pottery_tree_map_tree_emplace_after POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_after)
    #define pottery_tree_map_tree_emplace_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_bulk)
    #define pottery_tree_map_tree_emplace_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_at_bulk)
    #define pottery_tree_map_tree_emplace_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_first_bulk)
    #define pottery_tree_map_tree_emplace_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_last_bulk)
    #define pottery_tree_map_tree_emplace_before_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_before_bulk)
    #define pottery_tree_map_tree_emplace_after_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_tree_map_tree_construct POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct)
    #define pottery_tree_map_tree_construct_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct_at)
    #define pottery_tree_map_tree_construct_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct_key)
    #define pottery_tree_map_tree_construct_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct_first)
    #define pottery_tree_map_tree_construct_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct_last)
    #define pottery_tree_map_tree_construct_before POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct_before)
    #define pottery_tree_map_tree_construct_after POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _construct_after)

    // extract
    #define pottery_tree_map_tree_extract POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract)
    #define pottery_tree_map_tree_extract_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_at)
    #define pottery_tree_map_tree_extract_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_key)
    #define pottery_tree_map_tree_extract_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_first)
    #define pottery_tree_map_tree_extract_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_first_bulk)
    #define pottery_tree_map_tree_extract_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_last)
    #define pottery_tree_map_tree_extract_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_last_bulk)
    #define pottery_tree_map_tree_extract_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_and_next)
    #define pottery_tree_map_tree_extract_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _extract_and_previous)

    // remove
    #define pottery_tree_map_tree_remove POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove)
    #define pottery_tree_map_tree_remove_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_at)
    #define pottery_tree_map_tree_remove_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_key)
    #define pottery_tree_map_tree_remove_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_first)
    #define pottery_tree_map_tree_remove_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_last)
    #define pottery_tree_map_tree_remove_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_and_next)
    #define pottery_tree_map_tree_remove_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_and_previous)
    #define pottery_tree_map_tree_remove_all POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_all)
    #define pottery_tree_map_tree_remove_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_bulk)
    #define pottery_tree_map_tree_remove_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_at_bulk)
    #define pottery_tree_map_tree_remove_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_first_bulk)
    #define pottery_tree_map_tree_remove_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_last_bulk)
    #define pottery_tree_map_tree_remove_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_at_bulk)
    #define pottery_tree_map_tree_remove_and_next_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_and_next_bulk)
    #define pottery_tree_map_tree_remove_and_previous_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _remove_and_previous_bulk)

    // displace
    #define pottery_tree_map_tree_displace POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace)
    #define pottery_tree_map_tree_displace_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_at)
    #define pottery_tree_map_tree_displace_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_key)
    #define pottery_tree_map_tree_displace_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_first)
    #define pottery_tree_map_tree_displace_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_last)
    #define pottery_tree_map_tree_displace_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_and_next)
    #define pottery_tree_map_tree_displace_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_and_previous)
    #define pottery_tree_map_tree_displace_all POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_all)
    #define pottery_tree_map_tree_displace_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_bulk)
    #define pottery_tree_map_tree_displace_at_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_at_bulk)
    #define pottery_tree_map_tree_displace_first_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_first_bulk)
    #define pottery_tree_map_tree_displace_last_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_last_bulk)
    #define pottery_tree_map_tree_displace_and_next_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_and_next_bulk)
    #define pottery_tree_map_tree_displace_and_previous_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _displace_and_previous_bulk)

    // link
    #define pottery_tree_map_tree_link POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _link)
    #define pottery_tree_map_tree_link_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _link_first)
    #define pottery_tree_map_tree_link_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _link_last)
    #define pottery_tree_map_tree_link_before POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _link_before)
    #define pottery_tree_map_tree_link_after POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _link_after)

    // unlink
    #define pottery_tree_map_tree_unlink POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink)
    #define pottery_tree_map_tree_unlink_at POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_at)
    #define pottery_tree_map_tree_unlink_key POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_key)
    #define pottery_tree_map_tree_unlink_first POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_first)
    #define pottery_tree_map_tree_unlink_last POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_last)
    #define pottery_tree_map_tree_unlink_and_next POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_and_next)
    #define pottery_tree_map_tree_unlink_and_previous POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_and_previous)
    #define pottery_tree_map_tree_unlink_all POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _unlink_all)

    // special links
    #define pottery_tree_map_tree_replace POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _replace)
    #define pottery_tree_map_tree_find_location POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _find_location)
    #define pottery_tree_map_tree_link_location POTTERY_CONCAT(POTTERY_TREE_MAP_RBT_PREFIX, _link_location)
//!!! END_AUTOGENERATED



// alloc functions
#define POTTERY_TREE_MAP_ALLOC_PREFIX POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _alloc)
//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_tree_map_alloc DEST:POTTERY_TREE_MAP_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_tree_map_alloc to POTTERY_TREE_MAP_ALLOC_PREFIX

    #define pottery_tree_map_alloc_free POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _free)
    #define pottery_tree_map_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_tree_map_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_tree_map_alloc_malloc POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _malloc)
    #define pottery_tree_map_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_tree_map_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_tree_map_alloc_malloc_zero POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _malloc_zero)
    #define pottery_tree_map_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_tree_map_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_tree_map_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_tree_map_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_tree_map_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_tree_map_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_tree_map_alloc_malloc_array POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _malloc_array)
    #define pottery_tree_map_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_TREE_MAP_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED



// container types
#ifdef POTTERY_TREE_MAP_EXTERNAL_CONTAINER_TYPES
    #define POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX POTTERY_TREE_MAP_EXTERNAL_CONTAINER_TYPES
#else
    #define POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX POTTERY_TREE_MAP_PREFIX
#endif
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_tree_map DEST:POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_tree_map to POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX

    #define pottery_tree_map_value_t POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_tree_map_ref_t POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_tree_map_const_ref_t POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_tree_map_key_t POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_tree_map_context_t POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_tree_map_ref_key POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_tree_map_ref_equal POTTERY_CONCAT(POTTERY_TREE_MAP_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions
#define POTTERY_TREE_MAP_LIFECYCLE_PREFIX POTTERY_TREE_MAP_NAME(_lifecycle)
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_tree_map_lifecycle DEST:POTTERY_TREE_MAP_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_tree_map_lifecycle to POTTERY_TREE_MAP_LIFECYCLE_PREFIX

    #define pottery_tree_map_lifecycle_init_steal POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_tree_map_lifecycle_init_copy POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_tree_map_lifecycle_init POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _init)
    #define pottery_tree_map_lifecycle_destroy POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _destroy)

    #define pottery_tree_map_lifecycle_swap POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _swap)
    #define pottery_tree_map_lifecycle_move POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move)
    #define pottery_tree_map_lifecycle_steal POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _steal)
    #define pottery_tree_map_lifecycle_copy POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _copy)
    #define pottery_tree_map_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_tree_map_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_tree_map_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_tree_map_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_tree_map_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_tree_map_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_tree_map_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_tree_map_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_tree_map_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_tree_map_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_tree_map_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_tree_map_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_TREE_MAP_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED