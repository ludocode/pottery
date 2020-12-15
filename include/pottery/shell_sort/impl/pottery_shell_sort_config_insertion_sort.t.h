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

#ifndef POTTERY_SHELL_SORT_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"

#define POTTERY_INSERTION_SORT_PREFIX POTTERY_CONCAT(POTTERY_SHELL_SORT_PREFIX, _insertion_sort)



// Manually forward the ref or value type. We use our own context and entry.
#ifdef POTTERY_SHELL_SORT_REF_TYPE
    #define POTTERY_INSERTION_SORT_REF_TYPE POTTERY_SHELL_SORT_REF_TYPE
#endif
#ifdef POTTERY_SHELL_SORT_VALUE_TYPE
    #define POTTERY_INSERTION_SORT_VALUE_TYPE POTTERY_SHELL_SORT_VALUE_TYPE
#endif



// We pass our own state as the insertion_sort context and configure its
// array_access to use our gap sequence.
#define POTTERY_INSERTION_SORT_CONTEXT_TYPE pottery_shell_sort_state_t
#define POTTERY_INSERTION_SORT_ARRAY_ACCESS_SELECT pottery_shell_sort_gap_select
#define POTTERY_INSERTION_SORT_ARRAY_ACCESS_INDEX pottery_shell_sort_gap_index
#define POTTERY_INSERTION_SORT_ARRAY_ACCESS_SHIFT pottery_shell_sort_gap_shift
#define POTTERY_INSERTION_SORT_ARRAY_ACCESS_OFFSET pottery_shell_sort_gap_offset

// Since we're using our own context, insertion_sort needs to generate its
// own lifecycle and compare functions that wrap ours.
#if defined(POTTERY_SHELL_SORT_LIFECYCLE_MOVE) || \
        defined(POTTERY_SHELL_SORT_LIFECYCLE_MOVE_BY_VALUE) || \
        defined(POTTERY_SHELL_SORT_LIFECYCLE_BY_VALUE)
    #define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE(state, a, b) \
            pottery_shell_sort_lifecycle_move(POTTERY_SHELL_SORT_CONTEXT_VAL(state) a, b)
#endif
#define POTTERY_INSERTION_SORT_LIFECYCLE_SWAP(state, a, b) \
        pottery_shell_sort_lifecycle_swap(POTTERY_SHELL_SORT_CONTEXT_VAL(state) a, b)
#define POTTERY_INSERTION_SORT_COMPARE_LESS(state, a, b) \
        pottery_shell_sort_compare_less(POTTERY_SHELL_SORT_CONTEXT_VAL(state) a, b)
#define POTTERY_INSERTION_SORT_COMPARE_THREE_WAY(state, a, b) \
        pottery_shell_sort_compare_three_way(POTTERY_SHELL_SORT_CONTEXT_VAL(state) a, b)
