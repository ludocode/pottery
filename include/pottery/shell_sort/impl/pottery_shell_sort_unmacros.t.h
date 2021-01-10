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

#ifndef POTTERY_SHELL_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif



/*
 * Configuration
 */

#undef POTTERY_SHELL_SORT_PREFIX
#undef POTTERY_SHELL_SORT_EXTERN
#undef POTTERY_SHELL_SORT_USE_MOVE
#undef POTTERY_SHELL_SORT_EXTERNAL_LIFECYCLE
#undef POTTERY_SHELL_SORT_EXTERNAL_COMPARE

//!!! AUTOGENERATED:sort/unconfigure.m.h PREFIX:POTTERY_SHELL_SORT
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) common sort configuration on POTTERY_SHELL_SORT
    #undef POTTERY_SHELL_SORT_REF_TYPE
    #undef POTTERY_SHELL_SORT_VALUE_TYPE
    #undef POTTERY_SHELL_SORT_CONTEXT_TYPE
    #undef POTTERY_SHELL_SORT_ACCESS
//!!! END_AUTOGENERATED

#undef POTTERY_SHELL_SORT_CONTEXT_VAL
#undef POTTERY_SHELL_SORT_SOLE_ARGS
#undef POTTERY_SHELL_SORT_SOLE_VALS
#undef POTTERY_SHELL_SORT_ARGS
#undef POTTERY_SHELL_SORT_VALS
#undef POTTERY_SHELL_SORT_ARGS_UNUSED



/*
 * Renaming
 */

#undef POTTERY_SHELL_SORT_NAME

#undef pottery_shell_sort
#undef pottery_shell_sort_insertion_sort
#undef pottery_shell_sort_state_t
#undef pottery_shell_sort_gap_select
#undef pottery_shell_sort_gap_shift
#undef pottery_shell_sort_gap_index
#undef pottery_shell_sort_gap_offset
#undef pottery_shell_sort_wrapped_ref_equal
#undef pottery_shell_sort_wrapped_entry_ref



// container types

#undef POTTERY_SHELL_SORT_CONTAINER_TYPES_PREFIX
//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_shell_sort
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_shell_sort

    #undef pottery_shell_sort_value_t
    #undef pottery_shell_sort_ref_t
    #undef pottery_shell_sort_const_ref_t
    #undef pottery_shell_sort_entry_t
    #undef pottery_shell_sort_key_t
    #undef pottery_shell_sort_context_t

    #undef pottery_shell_sort_entry_ref
    #undef pottery_shell_sort_ref_key
    #undef pottery_shell_sort_ref_equal
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_SHELL_SORT
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_SHELL_SORT

    // types
    #undef POTTERY_SHELL_SORT_VALUE_TYPE
    #undef POTTERY_SHELL_SORT_REF_TYPE
    #undef POTTERY_SHELL_SORT_ENTRY_TYPE
    #undef POTTERY_SHELL_SORT_KEY_TYPE
    #undef POTTERY_SHELL_SORT_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_SHELL_SORT_ENTRY_REF
    #undef POTTERY_SHELL_SORT_REF_KEY
    #undef POTTERY_SHELL_SORT_REF_EQUAL
//!!! END_AUTOGENERATED



// lifecycle

#undef POTTERY_SHELL_SORT_LIFECYCLE_PREFIX

//!!! AUTOGENERATED:lifecycle/unrename.m.h PREFIX:pottery_shell_sort_lifecycle
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames lifecycle identifiers with prefix "pottery_shell_sort_lifecycle"

    #undef pottery_shell_sort_lifecycle_init_steal
    #undef pottery_shell_sort_lifecycle_init_copy
    #undef pottery_shell_sort_lifecycle_init
    #undef pottery_shell_sort_lifecycle_destroy
    #undef pottery_shell_sort_lifecycle_swap
    #undef pottery_shell_sort_lifecycle_move
    #undef pottery_shell_sort_lifecycle_steal
    #undef pottery_shell_sort_lifecycle_copy

    #undef pottery_shell_sort_lifecycle_move_bulk
    #undef pottery_shell_sort_lifecycle_move_bulk_restrict
    #undef pottery_shell_sort_lifecycle_move_bulk_restrict_impl
    #undef pottery_shell_sort_lifecycle_move_bulk_up
    #undef pottery_shell_sort_lifecycle_move_bulk_up_impl
    #undef pottery_shell_sort_lifecycle_move_bulk_down
    #undef pottery_shell_sort_lifecycle_move_bulk_down_impl
    #undef pottery_shell_sort_lifecycle_destroy_bulk
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:lifecycle/unconfigure.m.h PREFIX:POTTERY_SHELL_SORT_LIFECYCLE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.

    // Undefines (forwardable/forwarded) lifecycle configuration on POTTERY_SHELL_SORT_LIFECYCLE

    #undef POTTERY_SHELL_SORT_LIFECYCLE_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_DESTROY
    #undef POTTERY_SHELL_SORT_LIFECYCLE_DESTROY_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_INIT
    #undef POTTERY_SHELL_SORT_LIFECYCLE_INIT_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_INIT_COPY
    #undef POTTERY_SHELL_SORT_LIFECYCLE_INIT_COPY_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_INIT_STEAL
    #undef POTTERY_SHELL_SORT_LIFECYCLE_INIT_STEAL_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_MOVE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_MOVE_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_COPY
    #undef POTTERY_SHELL_SORT_LIFECYCLE_COPY_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_STEAL
    #undef POTTERY_SHELL_SORT_LIFECYCLE_STEAL_BY_VALUE
    #undef POTTERY_SHELL_SORT_LIFECYCLE_SWAP
    #undef POTTERY_SHELL_SORT_LIFECYCLE_SWAP_BY_VALUE
//!!! END_AUTOGENERATED



// compare

#undef POTTERY_SHELL_SORT_COMPARE_PREFIX

//!!! AUTOGENERATED:compare/unrename.m.h PREFIX:pottery_shell_sort_compare
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames compare identifiers with prefix "pottery_shell_sort_compare"

    #undef pottery_shell_sort_compare_three_way
    #undef pottery_shell_sort_compare_equal
    #undef pottery_shell_sort_compare_not_equal
    #undef pottery_shell_sort_compare_less
    #undef pottery_shell_sort_compare_less_or_equal
    #undef pottery_shell_sort_compare_greater
    #undef pottery_shell_sort_compare_greater_or_equal

    #undef pottery_shell_sort_compare_min
    #undef pottery_shell_sort_compare_max
    #undef pottery_shell_sort_compare_clamp
    #undef pottery_shell_sort_compare_median
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:compare/unconfigure.m.h PREFIX:POTTERY_SHELL_SORT_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) compare configuration on POTTERY_SHELL_SORT_COMPARE

    #undef POTTERY_SHELL_SORT_COMPARE_EQUAL
    #undef POTTERY_SHELL_SORT_COMPARE_NOT_EQUAL
    #undef POTTERY_SHELL_SORT_COMPARE_LESS
    #undef POTTERY_SHELL_SORT_COMPARE_LESS_OR_EQUAL
    #undef POTTERY_SHELL_SORT_COMPARE_GREATER
    #undef POTTERY_SHELL_SORT_COMPARE_GREATER_OR_EQUAL
    #undef POTTERY_SHELL_SORT_COMPARE_THREE_WAY
//!!! END_AUTOGENERATED



// array_access

#undef POTTERY_SHELL_SORT_ARRAY_ACCESS_PREFIX

//!!! AUTOGENERATED:array_access/unrename.m.h PREFIX:pottery_shell_sort_array_access
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array_access identifiers with prefix "pottery_shell_sort_array_access"

    #undef pottery_shell_sort_array_access_value_t
    #undef pottery_shell_sort_array_access_ref_t
    #undef pottery_shell_sort_array_access_entry_t
    #undef pottery_shell_sort_array_access_context_t

    #undef pottery_shell_sort_array_access_begin
    #undef pottery_shell_sort_array_access_end
    #undef pottery_shell_sort_array_access_count

    #undef pottery_shell_sort_array_access_select
    #undef pottery_shell_sort_array_access_index

    #undef pottery_shell_sort_array_access_shift
    #undef pottery_shell_sort_array_access_offset
    #undef pottery_shell_sort_array_access_next
    #undef pottery_shell_sort_array_access_previous

    #undef pottery_shell_sort_array_access_ref
    #undef pottery_shell_sort_array_access_equal
    #undef pottery_shell_sort_array_access_exists
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:array_access/unconfigure.m.h PREFIX:POTTERY_SHELL_SORT_ARRAY_ACCESS
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) array_access configuration on POTTERY_SHELL_SORT_ARRAY_ACCESS

    // absolute addressing configs
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_BEGIN
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_END
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_COUNT

    // required configs for non-standard array access
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_SELECT
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_INDEX

    // required config for non-trivial entries
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_EQUAL

    // optional configs
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_SHIFT
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_OFFSET
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_NEXT
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_PREVIOUS
    #undef POTTERY_SHELL_SORT_ARRAY_ACCESS_EXISTS
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
