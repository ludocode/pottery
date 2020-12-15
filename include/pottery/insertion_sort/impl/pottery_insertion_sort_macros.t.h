/*
 * MIT License
 *
 * Copyright (c) 2020 Nicholas Fraser
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

#ifndef POTTERY_INSERTION_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"



/*
 * Configuration
 */

#ifndef POTTERY_INSERTION_SORT_PREFIX
    #error "POTTERY_INSERTION_SORT_PREFIX is not defined."
#endif

#ifndef POTTERY_INSERTION_SORT_EXTERN
    #define POTTERY_INSERTION_SORT_EXTERN /*nothing*/
#endif

// Context forwarding
#ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
    #define POTTERY_INSERTION_SORT_CONTEXT_VAL context,
#else
    #define POTTERY_INSERTION_SORT_CONTEXT_VAL /*nothing*/
#endif



/*
 * Array Access Function Arguments
 */

//!!! AUTOGENERATED:array_access/args.m.h CONFIG:POTTERY_INSERTION_SORT NAME:pottery_insertion_sort
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Defines wrappers for common function arguments for functions that access
    // arrays via the array_access template.

    // ARGS are function arguments
    // VALS are the matching values
    // SOLE means they are the only arguments to the function (so no trailing comma)
    // UNUSED casts them to void to silence unused parameter warnings

    #ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #define POTTERY_INSERTION_SORT_SOLE_ARGS pottery_insertion_sort_context_t context, pottery_insertion_sort_entry_t base
            #define POTTERY_INSERTION_SORT_SOLE_VALS context, base
            #define POTTERY_INSERTION_SORT_ARGS_UNUSED (void)context, (void)base
        #else
            #define POTTERY_INSERTION_SORT_SOLE_ARGS pottery_insertion_sort_context_t context
            #define POTTERY_INSERTION_SORT_SOLE_VALS context
            #define POTTERY_INSERTION_SORT_ARGS_UNUSED (void)context
        #endif
    #else
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #define POTTERY_INSERTION_SORT_SOLE_ARGS pottery_insertion_sort_entry_t base
            #define POTTERY_INSERTION_SORT_SOLE_VALS base
            #define POTTERY_INSERTION_SORT_ARGS_UNUSED (void)base
        #else
            #define POTTERY_INSERTION_SORT_SOLE_ARGS void
            #define POTTERY_INSERTION_SORT_SOLE_VALS /*nothing*/
            #define POTTERY_INSERTION_SORT_ARGS_UNUSED /*nothing*/
        #endif
    #endif
    #if defined(POTTERY_INSERTION_SORT_CONTEXT_TYPE) || !POTTERY_ARRAY_ACCESS_INHERENT_BASE
        #define POTTERY_INSERTION_SORT_ARGS POTTERY_INSERTION_SORT_SOLE_ARGS ,
        #define POTTERY_INSERTION_SORT_VALS POTTERY_INSERTION_SORT_SOLE_VALS ,
    #else
        #define POTTERY_INSERTION_SORT_ARGS /*nothing*/
        #define POTTERY_INSERTION_SORT_VALS /*nothing*/
    #endif
//!!! END_AUTOGENERATED



/*
 * Renaming
 */

//!!! AUTOGENERATED:insertion_sort/rename.m.h SRC:pottery_insertion_sort DEST:POTTERY_INSERTION_SORT_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames insertion_sort identifiers from pottery_insertion_sort to POTTERY_INSERTION_SORT_PREFIX

    #define pottery_insertion_sort_value_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _value_t)
    #define pottery_insertion_sort_ref_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _ref_t)
    #define pottery_insertion_sort_entry_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _entry_t)
    #define pottery_insertion_sort_context_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _context_t)

    #define pottery_insertion_sort POTTERY_INSERTION_SORT_PREFIX
    #define pottery_insertion_sort_range POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _range)
    #define pottery_insertion_sort_access POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _access)
    #define pottery_insertion_sort_impl POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _impl)
//!!! END_AUTOGENERATED



// lifecycle functions
#ifdef POTTERY_INSERTION_SORT_EXTERNAL_LIFECYCLE
    #define POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX POTTERY_INSERTION_SORT_EXTERNAL_LIFECYCLE
#else
    #define POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _lifecycle)
#endif
//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_insertion_sort_lifecycle DEST:POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_insertion_sort_lifecycle to POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX

    // types
    #define pottery_insertion_sort_lifecycle_value_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _value_t)
    #define pottery_insertion_sort_lifecycle_ref_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _ref_t)
    #define pottery_insertion_sort_lifecycle_entry_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _entry_t)
    #define pottery_insertion_sort_lifecycle_context_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _context_t)

    #define pottery_insertion_sort_lifecycle_const_ref_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _const_ref_t)

    // fundamental operations
    #define pottery_insertion_sort_lifecycle_init_steal POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_insertion_sort_lifecycle_init_copy POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_insertion_sort_lifecycle_init POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _init)
    #define pottery_insertion_sort_lifecycle_destroy POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _destroy)
    #define pottery_insertion_sort_lifecycle_swap POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _swap)
    #define pottery_insertion_sort_lifecycle_move POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move)
    #define pottery_insertion_sort_lifecycle_steal POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _steal)
    #define pottery_insertion_sort_lifecycle_copy POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _copy)

    // bulk operations
    #define pottery_insertion_sort_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_insertion_sort_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_insertion_sort_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_insertion_sort_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_insertion_sort_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_insertion_sort_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_insertion_sort_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_insertion_sort_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_INSERTION_SORT_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED



// compare functions
#ifdef POTTERY_INSERTION_SORT_EXTERNAL_COMPARE
    #define POTTERY_INSERTION_SORT_COMPARE_PREFIX POTTERY_INSERTION_SORT_EXTERNAL_COMPARE
#else
    #define POTTERY_INSERTION_SORT_COMPARE_PREFIX POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _compare)
#endif
//!!! AUTOGENERATED:compare/rename.m.h SRC:pottery_insertion_sort_compare DEST:POTTERY_INSERTION_SORT_COMPARE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames compare identifiers from pottery_insertion_sort_compare to POTTERY_INSERTION_SORT_COMPARE_PREFIX

    #define pottery_insertion_sort_compare_value_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _value_t)
    #define pottery_insertion_sort_compare_ref_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _ref_t)
    #define pottery_insertion_sort_compare_entry_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _entry_t)
    #define pottery_insertion_sort_compare_context_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _context_t)

    #define pottery_insertion_sort_compare_value POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _value)

    #define pottery_insertion_sort_compare_three_way POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _three_way)
    #define pottery_insertion_sort_compare_equal POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _equal)
    #define pottery_insertion_sort_compare_not_equal POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _not_equal)
    #define pottery_insertion_sort_compare_less POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _less)
    #define pottery_insertion_sort_compare_less_or_equal POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _less_or_equal)
    #define pottery_insertion_sort_compare_greater POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _greater)
    #define pottery_insertion_sort_compare_greater_or_equal POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _greater_or_equal)

    #define pottery_insertion_sort_compare_min POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _min)
    #define pottery_insertion_sort_compare_max POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _max)
    #define pottery_insertion_sort_compare_clamp POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _clamp)
    #define pottery_insertion_sort_compare_median POTTERY_CONCAT(POTTERY_INSERTION_SORT_COMPARE_PREFIX, _median)
//!!! END_AUTOGENERATED



// array_access functions
#ifdef POTTERY_INSERTION_SORT_EXTERNAL_ARRAY_ACCESS
    #define POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX POTTERY_INSERTION_SORT_EXTERNAL_ARRAY_ACCESS
#else
    #define POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX POTTERY_CONCAT(POTTERY_INSERTION_SORT_PREFIX, _array_access)
#endif
//!!! AUTOGENERATED:array_access/rename.m.h SRC:pottery_insertion_sort_array_access DEST:POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array_access identifiers from pottery_insertion_sort_array_access to POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX

    #define pottery_insertion_sort_array_access_value_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _value_t)
    #define pottery_insertion_sort_array_access_ref_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _ref_t)
    #define pottery_insertion_sort_array_access_entry_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _entry_t)
    #define pottery_insertion_sort_array_access_context_t POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _context_t)

    #define pottery_insertion_sort_array_access_begin POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _begin)
    #define pottery_insertion_sort_array_access_end POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _end)
    #define pottery_insertion_sort_array_access_count POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _count)

    #define pottery_insertion_sort_array_access_select POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _select)
    #define pottery_insertion_sort_array_access_index POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _index)

    #define pottery_insertion_sort_array_access_shift POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _shift)
    #define pottery_insertion_sort_array_access_offset POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _offset)
    #define pottery_insertion_sort_array_access_next POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _next)
    #define pottery_insertion_sort_array_access_previous POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _previous)

    #define pottery_insertion_sort_array_access_value POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _value)
    #define pottery_insertion_sort_array_access_equal POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _equal)
    #define pottery_insertion_sort_array_access_exists POTTERY_CONCAT(POTTERY_INSERTION_SORT_ARRAY_ACCESS_PREFIX, _exists)
//!!! END_AUTOGENERATED
