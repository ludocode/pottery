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

#ifndef POTTERY_INTRO_SORT_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"

#define POTTERY_QUICK_SORT_PREFIX POTTERY_CONCAT(POTTERY_INTRO_SORT_PREFIX, _quick_sort)

// We'll be using our own compare and lifecycle templates
#define POTTERY_QUICK_SORT_EXTERNAL_LIFECYCLE POTTERY_CONCAT(POTTERY_INTRO_SORT_PREFIX, _lifecycle)
#define POTTERY_QUICK_SORT_EXTERNAL_COMPARE POTTERY_CONCAT(POTTERY_INTRO_SORT_PREFIX, _compare)

// Switch to insertion_sort when partition is too small
#define POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK POTTERY_CONCAT(POTTERY_INTRO_SORT_PREFIX, _insertion_sort)

// Switch to heap_sort when recursion is too deep
#define POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK POTTERY_CONCAT(POTTERY_INTRO_SORT_PREFIX, _heap_sort)

// Forward the ref or value type
#ifdef POTTERY_INTRO_SORT_REF_TYPE
    #define POTTERY_QUICK_SORT_REF_TYPE POTTERY_INTRO_SORT_REF_TYPE
#endif
#ifdef POTTERY_INTRO_SORT_VALUE_TYPE
    #define POTTERY_QUICK_SORT_VALUE_TYPE POTTERY_INTRO_SORT_VALUE_TYPE
#endif

// Forward the context configurations
#ifdef POTTERY_INTRO_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT
    #define  POTTERY_QUICK_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT POTTERY_INTRO_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT
#endif
#ifdef POTTERY_INTRO_SORT_LIFECYCLE_CONTEXT_TYPE
    #define POTTERY_QUICK_SORT_LIFECYCLE_CONTEXT_TYPE POTTERY_INTRO_SORT_LIFECYCLE_CONTEXT_TYPE
#endif
#ifdef POTTERY_INTRO_SORT_CONTEXT_IS_COMPARE_CONTEXT
    #define POTTERY_QUICK_SORT_CONTEXT_IS_COMPARE_CONTEXT POTTERY_INTRO_SORT_CONTEXT_IS_COMPARE_CONTEXT
#endif
#ifdef POTTERY_INTRO_SORT_COMPARE_CONTEXT_TYPE
    #define POTTERY_QUICK_SORT_COMPARE_CONTEXT_TYPE POTTERY_INTRO_SORT_COMPARE_CONTEXT_TYPE
#endif
