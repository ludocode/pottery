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

#ifndef POTTERY_VECTOR_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_VECTOR_PREFIX
#error "POTTERY_VECTOR_PREFIX is not defined."
#endif

#ifndef POTTERY_VECTOR_VALUE_TYPE
#error "POTTERY_VECTOR_VALUE_TYPE is not defined."
#endif

#ifndef POTTERY_VECTOR_EXTERN
#define POTTERY_VECTOR_EXTERN /*nothing*/
#endif

// Context forwarding
#ifdef POTTERY_VECTOR_CONTEXT_TYPE
    #define POTTERY_VECTOR_CONTEXT_VAL(vector) vector->context,
#else
    #define POTTERY_VECTOR_CONTEXT_VAL(vector) /*nothing*/
#endif

#ifndef POTTERY_VECTOR_DOUBLE_ENDED
    #define POTTERY_VECTOR_DOUBLE_ENDED 0
#endif

#ifndef POTTERY_VECTOR_AUTO_SHRINK
    #define POTTERY_VECTOR_AUTO_SHRINK 1
#endif

#ifndef POTTERY_VECTOR_CXX
    #define POTTERY_VECTOR_CXX 0
#endif



/*
 * Renaming
 */

#define POTTERY_VECTOR_NAME(name) \
    POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, name)

#define pottery_vector_data POTTERY_VECTOR_NAME(_data)
#define pottery_vector_const_data POTTERY_VECTOR_NAME(_const_data)
#define pottery_vector_capacity POTTERY_VECTOR_NAME(_capacity)
#define pottery_vector_minimum_capacity POTTERY_VECTOR_NAME(_minimum_capacity)

#define pottery_vector_insert_vector POTTERY_VECTOR_NAME(_insert_vector)
#define pottery_vector_insert_last_vector POTTERY_VECTOR_NAME(_insert_last_vector)
#define pottery_vector_insert_first_vector POTTERY_VECTOR_NAME(_insert_first_vector)

#define pottery_vector_impl_alloc POTTERY_VECTOR_NAME(_impl_alloc)
#define pottery_vector_impl_free POTTERY_VECTOR_NAME(_impl_free)
#define pottery_vector_impl_clear POTTERY_VECTOR_NAME(_impl_clear)
#define pottery_vector_impl_destroy_all POTTERY_VECTOR_NAME(_impl_destroy_all)
#define pottery_vector_impl_copy POTTERY_VECTOR_NAME(_impl_copy)
#define pottery_vector_impl_reset POTTERY_VECTOR_NAME(_impl_reset)



// dynamic array functions
//!!! AUTOGENERATED:array/rename.m.h SRC:pottery_vector DEST:POTTERY_VECTOR_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from pottery_vector to POTTERY_VECTOR_PREFIX

    // types
    #define pottery_vector_t POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _t)
    #define pottery_vector_value_t POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _value_t)
    #define pottery_vector_ref_t POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _ref_t)
    #define pottery_vector_entry_t POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _entry_t)
    #define pottery_vector_context_t POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _context_t)

    // lifecycle operations
    #define pottery_vector_copy POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _copy)
    #define pottery_vector_destroy POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _destroy)
    #define pottery_vector_init POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _init)
    #define pottery_vector_init_copy POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _init_copy)
    #define pottery_vector_init_steal POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _init_steal)
    #define pottery_vector_move POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _move)
    #define pottery_vector_steal POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _steal)
    #define pottery_vector_swap POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _swap)

    // entry operations
    #define pottery_vector_entry_exists POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _entry_exists)
    #define pottery_vector_entry_equal POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _entry_equal)
    #define pottery_vector_entry_value POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _entry_value)

    // lookup
    #define pottery_vector_count POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _count)
    #define pottery_vector_is_empty POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _is_empty)
    #define pottery_vector_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _at)
    #define pottery_vector_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _first)
    #define pottery_vector_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _last)
    #define pottery_vector_any POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _any)

    // iteration
    #define pottery_vector_select POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _select)
    #define pottery_vector_index POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _index)
    #define pottery_vector_begin POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _begin)
    #define pottery_vector_end POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _end)
    #define pottery_vector_next POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _next)
    #define pottery_vector_previous POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _previous)

    // misc
    #define pottery_vector_reserve POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _reserve)
    #define pottery_vector_shrink POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _shrink)

    // insert
    #define pottery_vector_insert_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_at)
    #define pottery_vector_insert_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_first)
    #define pottery_vector_insert_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_last)
    #define pottery_vector_insert_before POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_before)
    #define pottery_vector_insert_after POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_after)
    #define pottery_vector_insert_at_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_at_bulk)
    #define pottery_vector_insert_first_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_first_bulk)
    #define pottery_vector_insert_last_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_last_bulk)
    #define pottery_vector_insert_before_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_before_bulk)
    #define pottery_vector_insert_after_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _insert_after_bulk)

    // emplace
    #define pottery_vector_emplace_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_at)
    #define pottery_vector_emplace_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_first)
    #define pottery_vector_emplace_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_last)
    #define pottery_vector_emplace_before POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_before)
    #define pottery_vector_emplace_after POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_after)
    #define pottery_vector_emplace_at_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_at_bulk)
    #define pottery_vector_emplace_first_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_first_bulk)
    #define pottery_vector_emplace_last_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_last_bulk)
    #define pottery_vector_emplace_before_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_before_bulk)
    #define pottery_vector_emplace_after_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _emplace_after_bulk)

    // construct
    #define pottery_vector_construct_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _construct_at)
    #define pottery_vector_construct_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _construct_first)
    #define pottery_vector_construct_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _construct_last)
    #define pottery_vector_construct_before POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _construct_before)
    #define pottery_vector_construct_after POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _construct_after)

    // extract
    #define pottery_vector_extract POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract)
    #define pottery_vector_extract_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_at)
    #define pottery_vector_extract_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_first)
    #define pottery_vector_extract_first_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_first_bulk)
    #define pottery_vector_extract_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_last)
    #define pottery_vector_extract_last_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_last_bulk)
    #define pottery_vector_extract_and_next POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_and_next)
    #define pottery_vector_extract_and_previous POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _extract_and_previous)

    // remove
    #define pottery_vector_remove POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove)
    #define pottery_vector_remove_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_at)
    #define pottery_vector_remove_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_first)
    #define pottery_vector_remove_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_last)
    #define pottery_vector_remove_and_next POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_and_next)
    #define pottery_vector_remove_and_previous POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_and_previous)
    #define pottery_vector_remove_all POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_all)
    #define pottery_vector_remove_at_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_at_bulk)
    #define pottery_vector_remove_first_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_first_bulk)
    #define pottery_vector_remove_last_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_last_bulk)
    #define pottery_vector_remove_at_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _remove_at_bulk)

    // displace
    #define pottery_vector_displace POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace)
    #define pottery_vector_displace_at POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_at)
    #define pottery_vector_displace_first POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_first)
    #define pottery_vector_displace_last POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_last)
    #define pottery_vector_displace_and_next POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_and_next)
    #define pottery_vector_displace_and_previous POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_and_previous)
    #define pottery_vector_displace_all POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_all)
    #define pottery_vector_displace_at_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_at_bulk)
    #define pottery_vector_displace_first_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_first_bulk)
    #define pottery_vector_displace_last_bulk POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _displace_last_bulk)
//!!! END_AUTOGENERATED



// lifecycle functions
#define POTTERY_VECTOR_LIFECYCLE_PREFIX POTTERY_VECTOR_NAME(_lifecycle)
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_vector_lifecycle DEST:POTTERY_VECTOR_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_vector_lifecycle to POTTERY_VECTOR_LIFECYCLE_PREFIX

    #define pottery_vector_lifecycle_init_steal POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_vector_lifecycle_init_copy POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_vector_lifecycle_init POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _init)
    #define pottery_vector_lifecycle_destroy POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _destroy)

    #define pottery_vector_lifecycle_swap POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _swap)
    #define pottery_vector_lifecycle_move POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move)
    #define pottery_vector_lifecycle_steal POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _steal)
    #define pottery_vector_lifecycle_copy POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _copy)
    #define pottery_vector_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_vector_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_vector_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_vector_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_vector_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_vector_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_vector_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_vector_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_vector_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_vector_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_vector_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_vector_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_VECTOR_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED

// alloc functions
#define POTTERY_VECTOR_ALLOC_PREFIX POTTERY_CONCAT(POTTERY_VECTOR_PREFIX, _alloc)
//!!! AUTOGENERATED:alloc/rename.m.h SRC:pottery_vector_alloc DEST:POTTERY_VECTOR_ALLOC_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames alloc identifiers from pottery_vector_alloc to POTTERY_VECTOR_ALLOC_PREFIX

    #define pottery_vector_alloc_free POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _free)
    #define pottery_vector_alloc_impl_free_ea POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_free_ea)
    #define pottery_vector_alloc_impl_free_fa POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_free_fa)

    #define pottery_vector_alloc_malloc POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _malloc)
    #define pottery_vector_alloc_impl_malloc_ea POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_ea)
    #define pottery_vector_alloc_impl_malloc_fa POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_fa)

    #define pottery_vector_alloc_malloc_zero POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _malloc_zero)
    #define pottery_vector_alloc_impl_malloc_zero_ea POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_zero_ea)
    #define pottery_vector_alloc_impl_malloc_zero_fa POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_zero_fa)
    #define pottery_vector_alloc_impl_malloc_zero_fa_wrap POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_zero_fa_wrap)

    #define pottery_vector_alloc_malloc_array_at_least POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _malloc_array_at_least)
    #define pottery_vector_alloc_impl_malloc_array_at_least_ea POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_array_at_least_ea)
    #define pottery_vector_alloc_impl_malloc_array_at_least_fa POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _impl_malloc_array_at_least_fa)

    #define pottery_vector_alloc_malloc_array POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _malloc_array)
    #define pottery_vector_alloc_malloc_array_zero POTTERY_CONCAT(POTTERY_VECTOR_ALLOC_PREFIX, _malloc_array_zero)
//!!! END_AUTOGENERATED
