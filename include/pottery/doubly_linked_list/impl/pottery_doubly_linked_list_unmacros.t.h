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

// types
#undef pottery_dll_t
#undef pottery_dll_entry_t

// public functions
#undef pottery_dll_init
#undef pottery_dll_destroy
#undef pottery_dll_count
#undef pottery_dll_is_empty
#undef pottery_dll_clear
#undef pottery_dll_swap
#undef pottery_dll_link_first
#undef pottery_dll_link_last
#undef pottery_dll_unlink_first
#undef pottery_dll_unlink_last
#undef pottery_dll_link_before
#undef pottery_dll_link_after
#undef pottery_dll_unlink
#undef pottery_dll_first
#undef pottery_dll_next
#undef pottery_dll_last
#undef pottery_dll_previous
#undef pottery_dll_entry_exists
#undef pottery_dll_entry_ref

// internal functions
#undef pottery_dll_next_ref
#undef pottery_dll_previous_ref
#undef pottery_dll_set_next_ref
#undef pottery_dll_set_previous_ref
#undef pottery_dll_sanity_check
#undef pottery_dll_ref_is_null

// naming macros
#undef POTTERY_DOUBLY_LINKED_LIST_NAME

// configs
#undef POTTERY_DOUBLY_LINKED_LIST_PREFIX
#undef POTTERY_DOUBLY_LINKED_LIST_ENTRY_TYPE
#undef POTTERY_DOUBLY_LINKED_LIST_NEXT
#undef POTTERY_DOUBLY_LINKED_LIST_PREVIOUS
#undef POTTERY_DOUBLY_LINKED_LIST_SET_NEXT
#undef POTTERY_DOUBLY_LINKED_LIST_SET_PREVIOUS

#undef POTTERY_DOUBLY_LINKED_LIST_EXTERN
#undef POTTERY_DLL_CONTEXT_VAL



//!!! AUTOGENERATED:container/unrename.m.h PREFIX:pottery_dll
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array identifiers with prefix "pottery_dll"

    // types
    #undef pottery_dll_t
    #undef pottery_dll_value_t
    #undef pottery_dll_ref_t
    #undef pottery_dll_entry_t
    #undef pottery_dll_context_t

    // lifecycle operations
    #undef pottery_dll_copy
    #undef pottery_dll_destroy
    #undef pottery_dll_init
    #undef pottery_dll_init_copy
    #undef pottery_dll_init_steal
    #undef pottery_dll_move
    #undef pottery_dll_steal
    #undef pottery_dll_swap

    // entry operations
    #undef pottery_dll_entry_exists
    #undef pottery_dll_entry_equal
    #undef pottery_dll_entry_value

    // lookup
    #undef pottery_dll_count
    #undef pottery_dll_is_empty
    #undef pottery_dll_at
    #undef pottery_dll_find
    #undef pottery_dll_contains_key
    #undef pottery_dll_first
    #undef pottery_dll_last
    #undef pottery_dll_any

    // array access
    #undef pottery_dll_select
    #undef pottery_dll_index
    #undef pottery_dll_shift
    #undef pottery_dll_offset
    #undef pottery_dll_begin
    #undef pottery_dll_end
    #undef pottery_dll_next
    #undef pottery_dll_previous

    // misc
    #undef pottery_dll_capacity
    #undef pottery_dll_reserve
    #undef pottery_dll_shrink

    // insert
    #undef pottery_dll_insert
    #undef pottery_dll_insert_at
    #undef pottery_dll_insert_first
    #undef pottery_dll_insert_last
    #undef pottery_dll_insert_before
    #undef pottery_dll_insert_after
    #undef pottery_dll_insert_bulk
    #undef pottery_dll_insert_at_bulk
    #undef pottery_dll_insert_first_bulk
    #undef pottery_dll_insert_last_bulk
    #undef pottery_dll_insert_before_bulk
    #undef pottery_dll_insert_after_bulk

    // emplace
    #undef pottery_dll_emplace
    #undef pottery_dll_emplace_at
    #undef pottery_dll_emplace_key
    #undef pottery_dll_emplace_first
    #undef pottery_dll_emplace_last
    #undef pottery_dll_emplace_before
    #undef pottery_dll_emplace_after
    #undef pottery_dll_emplace_bulk
    #undef pottery_dll_emplace_at_bulk
    #undef pottery_dll_emplace_first_bulk
    #undef pottery_dll_emplace_last_bulk
    #undef pottery_dll_emplace_before_bulk
    #undef pottery_dll_emplace_after_bulk

    // construct
    #undef pottery_dll_construct
    #undef pottery_dll_construct_at
    #undef pottery_dll_construct_key
    #undef pottery_dll_construct_first
    #undef pottery_dll_construct_last
    #undef pottery_dll_construct_before
    #undef pottery_dll_construct_after

    // extract
    #undef pottery_dll_extract
    #undef pottery_dll_extract_at
    #undef pottery_dll_extract_key
    #undef pottery_dll_extract_first
    #undef pottery_dll_extract_first_bulk
    #undef pottery_dll_extract_last
    #undef pottery_dll_extract_last_bulk
    #undef pottery_dll_extract_and_next
    #undef pottery_dll_extract_and_previous

    // remove
    #undef pottery_dll_remove
    #undef pottery_dll_remove_at
    #undef pottery_dll_remove_key
    #undef pottery_dll_remove_first
    #undef pottery_dll_remove_last
    #undef pottery_dll_remove_and_next
    #undef pottery_dll_remove_and_previous
    #undef pottery_dll_remove_all
    #undef pottery_dll_remove_bulk
    #undef pottery_dll_remove_at_bulk
    #undef pottery_dll_remove_first_bulk
    #undef pottery_dll_remove_last_bulk
    #undef pottery_dll_remove_at_bulk
    #undef pottery_dll_remove_and_next_bulk
    #undef pottery_dll_remove_and_previous_bulk

    // displace
    #undef pottery_dll_displace
    #undef pottery_dll_displace_at
    #undef pottery_dll_displace_key
    #undef pottery_dll_displace_first
    #undef pottery_dll_displace_last
    #undef pottery_dll_displace_and_next
    #undef pottery_dll_displace_and_previous
    #undef pottery_dll_displace_all
    #undef pottery_dll_displace_bulk
    #undef pottery_dll_displace_at_bulk
    #undef pottery_dll_displace_first_bulk
    #undef pottery_dll_displace_last_bulk
    #undef pottery_dll_displace_and_next_bulk
    #undef pottery_dll_displace_and_previous_bulk

    // link
    #undef pottery_dll_link
    #undef pottery_dll_link_first
    #undef pottery_dll_link_last
    #undef pottery_dll_link_before
    #undef pottery_dll_link_after

    // unlink
    #undef pottery_dll_unlink
    #undef pottery_dll_unlink_at
    #undef pottery_dll_unlink_key
    #undef pottery_dll_unlink_first
    #undef pottery_dll_unlink_last
    #undef pottery_dll_unlink_and_next
    #undef pottery_dll_unlink_and_previous
    #undef pottery_dll_unlink_all
//!!! END_AUTOGENERATED



// container_types

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_DOUBLY_LINKED_LIST
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_DOUBLY_LINKED_LIST

    // types
    #undef POTTERY_DOUBLY_LINKED_LIST_VALUE_TYPE
    #undef POTTERY_DOUBLY_LINKED_LIST_REF_TYPE
    #undef POTTERY_DOUBLY_LINKED_LIST_ENTRY_TYPE
    #undef POTTERY_DOUBLY_LINKED_LIST_KEY_TYPE
    #undef POTTERY_DOUBLY_LINKED_LIST_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_DOUBLY_LINKED_LIST_ENTRY_REF
    #undef POTTERY_DOUBLY_LINKED_LIST_REF_KEY
    #undef POTTERY_DOUBLY_LINKED_LIST_REF_EQUAL
//!!! END_AUTOGENERATED

#undef POTTERY_DOUBLY_LINKED_LIST_CONTAINER_TYPES_PREFIX
//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_dll
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_dll

    #undef pottery_dll_value_t
    #undef pottery_dll_ref_t
    #undef pottery_dll_const_ref_t
    #undef pottery_dll_key_t
    #undef pottery_dll_context_t

    #undef pottery_dll_ref_key
    #undef pottery_dll_ref_equal
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
