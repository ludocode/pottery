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



/**
 * Sorts a sub-range of elements within an array.
 */
#if POTTERY_FORWARD_DECLARATIONS
POTTERY_SHELL_SORT_EXTERN
void pottery_shell_sort_range(
        POTTERY_SHELL_SORT_ARGS
        size_t offset,
        size_t range_count);
#endif

/**
 * Sorts an array.
 */
static inline
void pottery_shell_sort(
        #if POTTERY_SHELL_SORT_INHERENT_COUNT
        POTTERY_SHELL_SORT_SOLE_ARGS
        #else
        POTTERY_SHELL_SORT_ARGS
        size_t total_count
        #endif
) {
    #if POTTERY_SHELL_SORT_INHERENT_COUNT
    size_t total_count = pottery_shell_sort_array_access_count(
            POTTERY_SHELL_SORT_SOLE_VALS);
    #endif

    pottery_shell_sort_range(POTTERY_SHELL_SORT_VALS
            0, total_count);
}



/*
 * Context wrappers for insertion sort
 */

static inline
pottery_shell_sort_ref_t pottery_shell_sort_wrapped_entry_ref(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t entry)
{
    (void)state;
    return pottery_shell_sort_entry_ref(
            #if POTTERY_SHELL_SORT_HAS_CONTEXT
            state.context,
            #endif
            entry);
}

static inline
bool pottery_shell_sort_wrapped_ref_equal(
        pottery_shell_sort_state_t state,
        pottery_shell_sort_ref_t left, pottery_shell_sort_ref_t right)
{
    (void)state;
    return pottery_shell_sort_ref_equal(
            #if POTTERY_SHELL_SORT_HAS_CONTEXT
            state.context,
            #endif
            left, right);
}



/*
 * Gap sequence wrappers for insertion sort.
 *
 * These translate array_access calls from insertion_sort to shell_sort,
 * multiplying and dividing by the gap sequence.
 */

// The first entry for an insertion_sort step is already shifted by the gap
// sequence offset. We therefore use shift for everything, not select, because
// insertion_sort's indices are relative to the first offset entry, not the
// true base of the sort. Despite this, we still have to pass the real base to
// array_access, so it's stored in the shell_sort state.

static inline
pottery_shell_sort_entry_t pottery_shell_sort_gap_select(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t base, size_t index)
{
    return pottery_shell_sort_array_access_shift(
            #if POTTERY_SHELL_SORT_HAS_CONTEXT
            state.context,
            #endif
            #if !POTTERY_SHELL_SORT_INHERENT_BASE
            state.base,
            #endif
            base,
            pottery_cast(ptrdiff_t, index * state.gap));
}

static inline
pottery_shell_sort_entry_t pottery_shell_sort_gap_shift(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t base,
        pottery_shell_sort_entry_t entry, ptrdiff_t offset)
{
    (void)base;
    return pottery_shell_sort_array_access_shift(
            #if POTTERY_SHELL_SORT_HAS_CONTEXT
            state.context,
            #endif
            #if !POTTERY_SHELL_SORT_INHERENT_BASE
            state.base,
            #endif
            entry,
            offset * pottery_cast(ptrdiff_t, state.gap));
}

static inline
size_t pottery_shell_sort_gap_index(pottery_shell_sort_state_t state,
        pottery_shell_sort_entry_t base, pottery_shell_sort_entry_t entry)
{
    ptrdiff_t offset = pottery_shell_sort_array_access_offset(
            #if POTTERY_SHELL_SORT_HAS_CONTEXT
            state.context,
            #endif
            #if !POTTERY_SHELL_SORT_INHERENT_BASE
            state.base,
            #endif
            base,
            entry);
    return pottery_cast(size_t, offset / pottery_cast(ptrdiff_t, state.gap));
}

static inline
ptrdiff_t pottery_shell_sort_gap_offset(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t base,
        pottery_shell_sort_entry_t first, pottery_shell_sort_entry_t second)
{
    (void)base;
    return pottery_shell_sort_array_access_offset(
            #if POTTERY_SHELL_SORT_HAS_CONTEXT
            state.context,
            #endif
            #if !POTTERY_SHELL_SORT_INHERENT_BASE
            state.base,
            #endif
            first,
            second) / pottery_cast(ptrdiff_t, state.gap);
}
