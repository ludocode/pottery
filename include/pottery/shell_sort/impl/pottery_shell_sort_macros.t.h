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

#include "pottery/pottery_dependencies.h"



/*
 * Configuration
 */

#ifndef POTTERY_SHELL_SORT_EXTERN
    #define POTTERY_SHELL_SORT_EXTERN /*nothing*/
#endif

// See if we should accept a separate lifecycle context parameter
#ifndef POTTERY_SHELL_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT
    #define POTTERY_SHELL_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT 0
#endif
#if defined(POTTERY_SHELL_SORT_LIFECYCLE_CONTEXT_TYPE) && !POTTERY_SHELL_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT
    #define POTTERY_SHELL_SORT_SEPARATE_LIFECYCLE_CONTEXT 1
#else
    #define POTTERY_SHELL_SORT_SEPARATE_LIFECYCLE_CONTEXT 0
#endif

// See if we should accept a separate compare context parameter
#ifndef POTTERY_SHELL_SORT_CONTEXT_IS_COMPARE_CONTEXT
    #define POTTERY_SHELL_SORT_CONTEXT_IS_COMPARE_CONTEXT 0
#endif
#if defined(POTTERY_SHELL_SORT_COMPARE_CONTEXT_TYPE) && !POTTERY_SHELL_SORT_CONTEXT_IS_COMPARE_CONTEXT
    #define POTTERY_SHELL_SORT_SEPARATE_COMPARE_CONTEXT 1
#else
    #define POTTERY_SHELL_SORT_SEPARATE_COMPARE_CONTEXT 0
#endif



/*
 * Renaming
 */

#define POTTERY_SHELL_SORT_NAME(name) \
    POTTERY_CONCAT(POTTERY_SHELL_SORT_PREFIX, name)

#define pottery_shell_sort_ref_t POTTERY_SHELL_SORT_NAME(_ref_t)
#define pottery_shell_sort_value_t POTTERY_SHELL_SORT_NAME(_value_t)
#define pottery_shell_sort_context_t POTTERY_SHELL_SORT_NAME(_context_t)
#define pottery_shell_sort_lifecycle_context_t POTTERY_SHELL_SORT_NAME(_lifecycle_context_t)
#define pottery_shell_sort_compare_context_t POTTERY_SHELL_SORT_NAME(_compare_context_t)
#define pottery_shell_sort POTTERY_SHELL_SORT_NAME()
#define pottery_shell_sort_insertion_sort POTTERY_SHELL_SORT_NAME(_insertion_sort)
#define pottery_shell_sort_access POTTERY_SHELL_SORT_NAME(_access)
#define pottery_shell_sort_state_t POTTERY_SHELL_SORT_NAME(_state_t)
