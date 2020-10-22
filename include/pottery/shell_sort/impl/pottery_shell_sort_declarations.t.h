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
#error "This is an internal header. Do not include it."
#endif

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_SHELL_SORT_EXTERN
void pottery_shell_sort(
        #ifdef POTTERY_SHELL_SORT_CONTEXT_TYPE
        pottery_shell_sort_context_t context,
        #endif
        pottery_shell_sort_ref_t first,
        size_t count);
#endif

// Gets the ref for the element at index based on the gap sequence.
// Our embedded insertion_sort uses this as its accessor function.
static inline
pottery_shell_sort_ref_t pottery_shell_sort_access(pottery_shell_sort_state_t state,
        pottery_shell_sort_ref_t first, size_t index)
{
    size_t real_index = state.offset + index * state.gap;
    #ifndef POTTERY_SHELL_SORT_ACCESS
        // With no defined access expression, it's a simple array.
        return first + real_index;
    #else
        return POTTERY_SHELL_SORT_ACCESS(state.context, first, real_index);
    #endif
}
