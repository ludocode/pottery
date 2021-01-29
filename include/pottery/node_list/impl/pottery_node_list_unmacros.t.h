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



/*
 * Forwarded configurations
 */

//!!! AUTOGENERATED:alloc/unconfigure.m.h PREFIX:POTTERY_NODE_LIST_ALLOC
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) alloc configuration on POTTERY_NODE_LIST_ALLOC
    #undef POTTERY_NODE_LIST_ALLOC_CONTEXT_TYPE

    #undef POTTERY_NODE_LIST_ALLOC_FREE
    #undef POTTERY_NODE_LIST_ALLOC_MALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ZALLOC
    #undef POTTERY_NODE_LIST_ALLOC_REALLOC
    #undef POTTERY_NODE_LIST_ALLOC_MALLOC_GOOD_SIZE

    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_FREE
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_MALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_ZALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_REALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_MALLOC_GOOD_SIZE
//!!! END_AUTOGENERATED



/*
 * Configuration
 */

// internal

#undef POTTERY_NODE_LIST_PREFIX
#undef POTTERY_NODE_LIST_NAME
#undef POTTERY_NODE_LIST_EXTERN
#undef POTTERY_NODE_LIST_CONTEXT_VAL
#undef POTTERY_NODE_LIST_HAS_CONTEXT


/*
 * Renaming
 */

#undef pottery_nl_entry_t

#undef POTTERY_NODE_LIST_NAME

#undef pottery_nl_alloc_node
#undef pottery_nl_free_node



//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_nl
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_nl"

    // types
    #undef pottery_nl_t
    #undef pottery_nl_entry_t

    // lifecycle operations
    #undef pottery_nl_copy
    #undef pottery_nl_destroy
    #undef pottery_nl_init
    #undef pottery_nl_init_copy
    #undef pottery_nl_init_steal
    #undef pottery_nl_move
    #undef pottery_nl_steal
    #undef pottery_nl_swap

    // entry operations
    #undef pottery_nl_entry_exists
    #undef pottery_nl_entry_equal
    #undef pottery_nl_entry_value

    // lookup
    #undef pottery_nl_count
    #undef pottery_nl_is_empty
    #undef pottery_nl_at
    #undef pottery_nl_find
    #undef pottery_nl_contains_key
    #undef pottery_nl_first
    #undef pottery_nl_last
    #undef pottery_nl_any

    // array access
    #undef pottery_nl_select
    #undef pottery_nl_index
    #undef pottery_nl_shift
    #undef pottery_nl_offset
    #undef pottery_nl_begin
    #undef pottery_nl_end
    #undef pottery_nl_next
    #undef pottery_nl_previous

    // misc
    #undef pottery_nl_capacity
    #undef pottery_nl_reserve
    #undef pottery_nl_shrink

    // insert
    #undef pottery_nl_insert
    #undef pottery_nl_insert_at
    #undef pottery_nl_insert_first
    #undef pottery_nl_insert_last
    #undef pottery_nl_insert_before
    #undef pottery_nl_insert_after
    #undef pottery_nl_insert_bulk
    #undef pottery_nl_insert_at_bulk
    #undef pottery_nl_insert_first_bulk
    #undef pottery_nl_insert_last_bulk
    #undef pottery_nl_insert_before_bulk
    #undef pottery_nl_insert_after_bulk

    // emplace
    #undef pottery_nl_emplace
    #undef pottery_nl_emplace_at
    #undef pottery_nl_emplace_key
    #undef pottery_nl_emplace_first
    #undef pottery_nl_emplace_last
    #undef pottery_nl_emplace_before
    #undef pottery_nl_emplace_after
    #undef pottery_nl_emplace_bulk
    #undef pottery_nl_emplace_at_bulk
    #undef pottery_nl_emplace_first_bulk
    #undef pottery_nl_emplace_last_bulk
    #undef pottery_nl_emplace_before_bulk
    #undef pottery_nl_emplace_after_bulk

    // construct
    #undef pottery_nl_construct
    #undef pottery_nl_construct_at
    #undef pottery_nl_construct_key
    #undef pottery_nl_construct_first
    #undef pottery_nl_construct_last
    #undef pottery_nl_construct_before
    #undef pottery_nl_construct_after

    // extract
    #undef pottery_nl_extract
    #undef pottery_nl_extract_at
    #undef pottery_nl_extract_key
    #undef pottery_nl_extract_first
    #undef pottery_nl_extract_first_bulk
    #undef pottery_nl_extract_last
    #undef pottery_nl_extract_last_bulk
    #undef pottery_nl_extract_and_next
    #undef pottery_nl_extract_and_previous

    // remove
    #undef pottery_nl_remove
    #undef pottery_nl_remove_at
    #undef pottery_nl_remove_key
    #undef pottery_nl_remove_first
    #undef pottery_nl_remove_last
    #undef pottery_nl_remove_and_next
    #undef pottery_nl_remove_and_previous
    #undef pottery_nl_remove_all
    #undef pottery_nl_remove_bulk
    #undef pottery_nl_remove_at_bulk
    #undef pottery_nl_remove_first_bulk
    #undef pottery_nl_remove_last_bulk
    #undef pottery_nl_remove_at_bulk
    #undef pottery_nl_remove_and_next_bulk
    #undef pottery_nl_remove_and_previous_bulk

    // displace
    #undef pottery_nl_displace
    #undef pottery_nl_displace_at
    #undef pottery_nl_displace_key
    #undef pottery_nl_displace_first
    #undef pottery_nl_displace_last
    #undef pottery_nl_displace_and_next
    #undef pottery_nl_displace_and_previous
    #undef pottery_nl_displace_all
    #undef pottery_nl_displace_bulk
    #undef pottery_nl_displace_at_bulk
    #undef pottery_nl_displace_first_bulk
    #undef pottery_nl_displace_last_bulk
    #undef pottery_nl_displace_and_next_bulk
    #undef pottery_nl_displace_and_previous_bulk

    // link
    #undef pottery_nl_link
    #undef pottery_nl_link_first
    #undef pottery_nl_link_last
    #undef pottery_nl_link_before
    #undef pottery_nl_link_after

    // unlink
    #undef pottery_nl_unlink
    #undef pottery_nl_unlink_at
    #undef pottery_nl_unlink_key
    #undef pottery_nl_unlink_first
    #undef pottery_nl_unlink_last
    #undef pottery_nl_unlink_and_next
    #undef pottery_nl_unlink_and_previous
    #undef pottery_nl_unlink_all
//!!! END_AUTOGENERATED



// doubly_linked_list

#undef POTTERY_NODE_LIST_DLL_PREFIX

//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_nl_dll
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_nl_dll"

    // types
    #undef pottery_nl_dll_t
    #undef pottery_nl_dll_entry_t

    // lifecycle operations
    #undef pottery_nl_dll_copy
    #undef pottery_nl_dll_destroy
    #undef pottery_nl_dll_init
    #undef pottery_nl_dll_init_copy
    #undef pottery_nl_dll_init_steal
    #undef pottery_nl_dll_move
    #undef pottery_nl_dll_steal
    #undef pottery_nl_dll_swap

    // entry operations
    #undef pottery_nl_dll_entry_exists
    #undef pottery_nl_dll_entry_equal
    #undef pottery_nl_dll_entry_value

    // lookup
    #undef pottery_nl_dll_count
    #undef pottery_nl_dll_is_empty
    #undef pottery_nl_dll_at
    #undef pottery_nl_dll_find
    #undef pottery_nl_dll_contains_key
    #undef pottery_nl_dll_first
    #undef pottery_nl_dll_last
    #undef pottery_nl_dll_any

    // array access
    #undef pottery_nl_dll_select
    #undef pottery_nl_dll_index
    #undef pottery_nl_dll_shift
    #undef pottery_nl_dll_offset
    #undef pottery_nl_dll_begin
    #undef pottery_nl_dll_end
    #undef pottery_nl_dll_next
    #undef pottery_nl_dll_previous

    // misc
    #undef pottery_nl_dll_capacity
    #undef pottery_nl_dll_reserve
    #undef pottery_nl_dll_shrink

    // insert
    #undef pottery_nl_dll_insert
    #undef pottery_nl_dll_insert_at
    #undef pottery_nl_dll_insert_first
    #undef pottery_nl_dll_insert_last
    #undef pottery_nl_dll_insert_before
    #undef pottery_nl_dll_insert_after
    #undef pottery_nl_dll_insert_bulk
    #undef pottery_nl_dll_insert_at_bulk
    #undef pottery_nl_dll_insert_first_bulk
    #undef pottery_nl_dll_insert_last_bulk
    #undef pottery_nl_dll_insert_before_bulk
    #undef pottery_nl_dll_insert_after_bulk

    // emplace
    #undef pottery_nl_dll_emplace
    #undef pottery_nl_dll_emplace_at
    #undef pottery_nl_dll_emplace_key
    #undef pottery_nl_dll_emplace_first
    #undef pottery_nl_dll_emplace_last
    #undef pottery_nl_dll_emplace_before
    #undef pottery_nl_dll_emplace_after
    #undef pottery_nl_dll_emplace_bulk
    #undef pottery_nl_dll_emplace_at_bulk
    #undef pottery_nl_dll_emplace_first_bulk
    #undef pottery_nl_dll_emplace_last_bulk
    #undef pottery_nl_dll_emplace_before_bulk
    #undef pottery_nl_dll_emplace_after_bulk

    // construct
    #undef pottery_nl_dll_construct
    #undef pottery_nl_dll_construct_at
    #undef pottery_nl_dll_construct_key
    #undef pottery_nl_dll_construct_first
    #undef pottery_nl_dll_construct_last
    #undef pottery_nl_dll_construct_before
    #undef pottery_nl_dll_construct_after

    // extract
    #undef pottery_nl_dll_extract
    #undef pottery_nl_dll_extract_at
    #undef pottery_nl_dll_extract_key
    #undef pottery_nl_dll_extract_first
    #undef pottery_nl_dll_extract_first_bulk
    #undef pottery_nl_dll_extract_last
    #undef pottery_nl_dll_extract_last_bulk
    #undef pottery_nl_dll_extract_and_next
    #undef pottery_nl_dll_extract_and_previous

    // remove
    #undef pottery_nl_dll_remove
    #undef pottery_nl_dll_remove_at
    #undef pottery_nl_dll_remove_key
    #undef pottery_nl_dll_remove_first
    #undef pottery_nl_dll_remove_last
    #undef pottery_nl_dll_remove_and_next
    #undef pottery_nl_dll_remove_and_previous
    #undef pottery_nl_dll_remove_all
    #undef pottery_nl_dll_remove_bulk
    #undef pottery_nl_dll_remove_at_bulk
    #undef pottery_nl_dll_remove_first_bulk
    #undef pottery_nl_dll_remove_last_bulk
    #undef pottery_nl_dll_remove_at_bulk
    #undef pottery_nl_dll_remove_and_next_bulk
    #undef pottery_nl_dll_remove_and_previous_bulk

    // displace
    #undef pottery_nl_dll_displace
    #undef pottery_nl_dll_displace_at
    #undef pottery_nl_dll_displace_key
    #undef pottery_nl_dll_displace_first
    #undef pottery_nl_dll_displace_last
    #undef pottery_nl_dll_displace_and_next
    #undef pottery_nl_dll_displace_and_previous
    #undef pottery_nl_dll_displace_all
    #undef pottery_nl_dll_displace_bulk
    #undef pottery_nl_dll_displace_at_bulk
    #undef pottery_nl_dll_displace_first_bulk
    #undef pottery_nl_dll_displace_last_bulk
    #undef pottery_nl_dll_displace_and_next_bulk
    #undef pottery_nl_dll_displace_and_previous_bulk

    // link
    #undef pottery_nl_dll_link
    #undef pottery_nl_dll_link_first
    #undef pottery_nl_dll_link_last
    #undef pottery_nl_dll_link_before
    #undef pottery_nl_dll_link_after

    // unlink
    #undef pottery_nl_dll_unlink
    #undef pottery_nl_dll_unlink_at
    #undef pottery_nl_dll_unlink_key
    #undef pottery_nl_dll_unlink_first
    #undef pottery_nl_dll_unlink_last
    #undef pottery_nl_dll_unlink_and_next
    #undef pottery_nl_dll_unlink_and_previous
    #undef pottery_nl_dll_unlink_all
//!!! END_AUTOGENERATED



// container types

#undef POTTERY_NODE_LIST_CONTAINER_TYPES_PREFIX
//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_nl
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_nl

    #undef pottery_nl_value_t
    #undef pottery_nl_ref_t
    #undef pottery_nl_const_ref_t
    #undef pottery_nl_key_t
    #undef pottery_nl_context_t

    #undef pottery_nl_ref_key
    #undef pottery_nl_ref_equal
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_NODE_LIST
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_NODE_LIST

    // types
    #undef POTTERY_NODE_LIST_VALUE_TYPE
    #undef POTTERY_NODE_LIST_REF_TYPE
    #undef POTTERY_NODE_LIST_ENTRY_TYPE
    #undef POTTERY_NODE_LIST_KEY_TYPE
    #undef POTTERY_NODE_LIST_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_NODE_LIST_ENTRY_REF
    #undef POTTERY_NODE_LIST_REF_KEY
    #undef POTTERY_NODE_LIST_REF_EQUAL
//!!! END_AUTOGENERATED



// lifecycle

#undef POTTERY_NODE_LIST_LIFECYCLE_PREFIX

//!!! AUTOGENERATED:lifecycle/unrename.m.h PREFIX:pottery_nl_lifecycle
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames lifecycle identifiers with prefix "pottery_nl_lifecycle"

    #undef pottery_nl_lifecycle_init_steal
    #undef pottery_nl_lifecycle_init_copy
    #undef pottery_nl_lifecycle_init
    #undef pottery_nl_lifecycle_destroy
    #undef pottery_nl_lifecycle_swap
    #undef pottery_nl_lifecycle_move
    #undef pottery_nl_lifecycle_steal
    #undef pottery_nl_lifecycle_copy

    #undef pottery_nl_lifecycle_move_bulk
    #undef pottery_nl_lifecycle_move_bulk_restrict
    #undef pottery_nl_lifecycle_move_bulk_restrict_impl
    #undef pottery_nl_lifecycle_move_bulk_up
    #undef pottery_nl_lifecycle_move_bulk_up_impl
    #undef pottery_nl_lifecycle_move_bulk_down
    #undef pottery_nl_lifecycle_move_bulk_down_impl
    #undef pottery_nl_lifecycle_destroy_bulk
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:lifecycle/unconfigure.m.h PREFIX:POTTERY_NODE_LIST_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Undefines (forwardable/forwarded) lifecycle configuration on POTTERY_NODE_LIST_LIFECYCLE

    #undef POTTERY_NODE_LIST_LIFECYCLE_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_DESTROY
    #undef POTTERY_NODE_LIST_LIFECYCLE_DESTROY_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_INIT
    #undef POTTERY_NODE_LIST_LIFECYCLE_INIT_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_INIT_COPY
    #undef POTTERY_NODE_LIST_LIFECYCLE_INIT_COPY_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_INIT_STEAL
    #undef POTTERY_NODE_LIST_LIFECYCLE_INIT_STEAL_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_MOVE
    #undef POTTERY_NODE_LIST_LIFECYCLE_MOVE_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_COPY
    #undef POTTERY_NODE_LIST_LIFECYCLE_COPY_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_STEAL
    #undef POTTERY_NODE_LIST_LIFECYCLE_STEAL_BY_VALUE
    #undef POTTERY_NODE_LIST_LIFECYCLE_SWAP
    #undef POTTERY_NODE_LIST_LIFECYCLE_SWAP_BY_VALUE
//!!! END_AUTOGENERATED



// alloc

#undef POTTERY_NODE_LIST_ALLOC_PREFIX

//!!! AUTOGENERATED:alloc/unrename.m.h PREFIX:pottery_nl_alloc
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames alloc identifiers with prefix "pottery_nl_alloc"

    #undef pottery_nl_alloc_free
    #undef pottery_nl_alloc_impl_free_ea
    #undef pottery_nl_alloc_impl_free_fa

    #undef pottery_nl_alloc_malloc
    #undef pottery_nl_alloc_impl_malloc_ea
    #undef pottery_nl_alloc_impl_malloc_fa

    #undef pottery_nl_alloc_malloc_zero
    #undef pottery_nl_alloc_impl_malloc_zero_ea
    #undef pottery_nl_alloc_impl_malloc_zero_fa
    #undef pottery_nl_alloc_impl_malloc_zero_fa_wrap

    #undef pottery_nl_alloc_malloc_array_at_least
    #undef pottery_nl_alloc_impl_malloc_array_at_least_ea
    #undef pottery_nl_alloc_impl_malloc_array_at_least_fa

    #undef pottery_nl_alloc_malloc_array
    #undef pottery_nl_alloc_malloc_array_zero
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:alloc/unconfigure.m.h PREFIX:POTTERY_NODE_LIST_ALLOC
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) alloc configuration on POTTERY_NODE_LIST_ALLOC
    #undef POTTERY_NODE_LIST_ALLOC_CONTEXT_TYPE

    #undef POTTERY_NODE_LIST_ALLOC_FREE
    #undef POTTERY_NODE_LIST_ALLOC_MALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ZALLOC
    #undef POTTERY_NODE_LIST_ALLOC_REALLOC
    #undef POTTERY_NODE_LIST_ALLOC_MALLOC_GOOD_SIZE

    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_FREE
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_MALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_ZALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_REALLOC
    #undef POTTERY_NODE_LIST_ALLOC_ALIGNED_MALLOC_GOOD_SIZE
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
