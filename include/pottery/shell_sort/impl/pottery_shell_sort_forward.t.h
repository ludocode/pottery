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

typedef struct pottery_shell_sort_state_t {
    #if POTTERY_SHELL_SORT_HAS_CONTEXT
    pottery_shell_sort_context_t context;
    #endif
    #if !POTTERY_SHELL_SORT_INHERENT_BASE
    pottery_shell_sort_ref_t base;
    #endif
    size_t gap;
} pottery_shell_sort_state_t;

#if POTTERY_FORWARD_DECLARATIONS
static inline
pottery_shell_sort_ref_t pottery_shell_sort_wrapped_entry_ref(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t entry);

static inline
bool pottery_shell_sort_wrapped_ref_equal(
        pottery_shell_sort_state_t state,
        pottery_shell_sort_ref_t left, pottery_shell_sort_ref_t right);

static inline
pottery_shell_sort_entry_t pottery_shell_sort_gap_select(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t base, size_t index);

static inline
pottery_shell_sort_entry_t pottery_shell_sort_gap_shift(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t base,
        pottery_shell_sort_entry_t entry, ptrdiff_t offset);

static inline
size_t pottery_shell_sort_gap_index(pottery_shell_sort_state_t state,
        pottery_shell_sort_entry_t base, pottery_shell_sort_entry_t entry);

static inline
ptrdiff_t pottery_shell_sort_gap_offset(
        pottery_shell_sort_state_t state, pottery_shell_sort_entry_t base,
        pottery_shell_sort_entry_t first, pottery_shell_sort_entry_t second);
#endif
