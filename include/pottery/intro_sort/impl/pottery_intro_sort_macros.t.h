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

#ifndef POTTERY_INTRO_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_INTRO_SORT_PREFIX
    #error "POTTERY_INTRO_SORT_PREFIX is not defined."
#endif



/*
 * Renaming
 */

#define pottery_intro_sort_entry_t pottery_intro_sort_array_access_entry_t
#define pottery_intro_sort_entry_ref pottery_intro_sort_array_access_entry_ref

#define POTTERY_INTRO_SORT_NAME(name) \
    POTTERY_CONCAT(POTTERY_INTRO_SORT_PREFIX, name)

#define pottery_intro_sort POTTERY_INTRO_SORT_PREFIX
#define pottery_intro_sort_range POTTERY_INTRO_SORT_NAME(_range)
#define pottery_intro_sort_quick_sort_range POTTERY_INTRO_SORT_NAME(_quick_sort_range)



/*
 * Array Access Function Arguments
 */

//!!! AUTOGENERATED:array_access/args.m.h CONFIG:POTTERY_INTRO_SORT NAME:pottery_intro_sort
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Defines wrappers for common function arguments for functions that access
    // arrays via the array_access template.

    // ARGS are function arguments
    // VALS are the matching values
    // SOLE means they are the only arguments to the function (so no trailing comma)
    // UNUSED casts them to void to silence unused parameter warnings

    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #define POTTERY_INTRO_SORT_SOLE_ARGS \
                    pottery_intro_sort_context_t context, pottery_intro_sort_entry_t base
            #define POTTERY_INTRO_SORT_SOLE_VALS context, base
            #define POTTERY_INTRO_SORT_ARGS_UNUSED (void)context, (void)base
        #else
            #define POTTERY_INTRO_SORT_SOLE_ARGS pottery_intro_sort_context_t context
            #define POTTERY_INTRO_SORT_SOLE_VALS context
            #define POTTERY_INTRO_SORT_ARGS_UNUSED (void)context
        #endif
    #else
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #define POTTERY_INTRO_SORT_SOLE_ARGS pottery_intro_sort_entry_t base
            #define POTTERY_INTRO_SORT_SOLE_VALS base
            #define POTTERY_INTRO_SORT_ARGS_UNUSED (void)base
        #else
            #define POTTERY_INTRO_SORT_SOLE_ARGS void
            #define POTTERY_INTRO_SORT_SOLE_VALS /*nothing*/
            #define POTTERY_INTRO_SORT_ARGS_UNUSED /*nothing*/
        #endif
    #endif
    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT || !POTTERY_ARRAY_ACCESS_INHERENT_BASE
        #define POTTERY_INTRO_SORT_ARGS POTTERY_INTRO_SORT_SOLE_ARGS ,
        #define POTTERY_INTRO_SORT_VALS POTTERY_INTRO_SORT_SOLE_VALS ,
    #else
        #define POTTERY_INTRO_SORT_ARGS /*nothing*/
        #define POTTERY_INTRO_SORT_VALS /*nothing*/
    #endif

    #undef POTTERY_ARRAY_ACCESS_ARGS_HACK_HAS_CONTEXT
//!!! END_AUTOGENERATED



// container_types
#define POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX POTTERY_INTRO_SORT_PREFIX
//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_intro_sort DEST:POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_intro_sort to POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX

    #define pottery_intro_sort_value_t POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_intro_sort_ref_t POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_intro_sort_const_ref_t POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_intro_sort_key_t POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_intro_sort_context_t POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_intro_sort_ref_key POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_intro_sort_ref_equal POTTERY_CONCAT(POTTERY_INTRO_SORT_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED



// lifecycle functions
#define POTTERY_INTRO_SORT_LIFECYCLE_PREFIX POTTERY_INTRO_SORT_NAME(_lifecycle)
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_intro_sort_lifecycle DEST:POTTERY_INTRO_SORT_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_intro_sort_lifecycle to POTTERY_INTRO_SORT_LIFECYCLE_PREFIX

    #define pottery_intro_sort_lifecycle_init_steal POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_intro_sort_lifecycle_init_copy POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_intro_sort_lifecycle_init POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _init)
    #define pottery_intro_sort_lifecycle_destroy POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _destroy)

    #define pottery_intro_sort_lifecycle_swap POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _swap)
    #define pottery_intro_sort_lifecycle_move POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move)
    #define pottery_intro_sort_lifecycle_steal POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _steal)
    #define pottery_intro_sort_lifecycle_copy POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _copy)
    #define pottery_intro_sort_lifecycle_swap_restrict POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _swap_restrict)
    #define pottery_intro_sort_lifecycle_move_restrict POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_restrict)
    #define pottery_intro_sort_lifecycle_steal_restrict POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _steal_restrict)
    #define pottery_intro_sort_lifecycle_copy_restrict POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _copy_restrict)

    #define pottery_intro_sort_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_intro_sort_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_intro_sort_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_intro_sort_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_intro_sort_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_intro_sort_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_intro_sort_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_intro_sort_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_INTRO_SORT_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED



// compare functions
#define POTTERY_INTRO_SORT_COMPARE_PREFIX POTTERY_INTRO_SORT_NAME(_compare)
//!!! AUTOGENERATED:compare/rename.m.h SRC:pottery_intro_sort_compare DEST:POTTERY_INTRO_SORT_COMPARE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames compare identifiers from pottery_intro_sort_compare to POTTERY_INTRO_SORT_COMPARE_PREFIX

    #define pottery_intro_sort_compare_three_way POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _three_way)
    #define pottery_intro_sort_compare_equal POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _equal)
    #define pottery_intro_sort_compare_not_equal POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _not_equal)
    #define pottery_intro_sort_compare_less POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _less)
    #define pottery_intro_sort_compare_less_or_equal POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _less_or_equal)
    #define pottery_intro_sort_compare_greater POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _greater)
    #define pottery_intro_sort_compare_greater_or_equal POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _greater_or_equal)

    #define pottery_intro_sort_compare_min POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _min)
    #define pottery_intro_sort_compare_max POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _max)
    #define pottery_intro_sort_compare_clamp POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _clamp)
    #define pottery_intro_sort_compare_median POTTERY_CONCAT(POTTERY_INTRO_SORT_COMPARE_PREFIX, _median)
//!!! END_AUTOGENERATED



// array_access functions
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX POTTERY_INTRO_SORT_NAME(_array_access)
//!!! AUTOGENERATED:array_access/rename.m.h SRC:pottery_intro_sort_array_access DEST:POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array_access identifiers from pottery_intro_sort_array_access to POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX

    #define pottery_intro_sort_array_access_entry_t POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _entry_t)
    #define pottery_intro_sort_array_access_entry_ref POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _entry_ref)

    #define pottery_intro_sort_array_access_begin POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _begin)
    #define pottery_intro_sort_array_access_end POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _end)
    #define pottery_intro_sort_array_access_count POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _count)

    #define pottery_intro_sort_array_access_select POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _select)
    #define pottery_intro_sort_array_access_index POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _index)

    #define pottery_intro_sort_array_access_shift POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _shift)
    #define pottery_intro_sort_array_access_offset POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _offset)
    #define pottery_intro_sort_array_access_next POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _next)
    #define pottery_intro_sort_array_access_previous POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _previous)

    #define pottery_intro_sort_array_access_ref POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _ref)
    #define pottery_intro_sort_array_access_equal POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _equal)
    #define pottery_intro_sort_array_access_exists POTTERY_CONCAT(POTTERY_INTRO_SORT_ARRAY_ACCESS_PREFIX, _exists)
//!!! END_AUTOGENERATED
