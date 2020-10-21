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

#ifdef POTTERY_INSERTION_SORT_VALUE_TYPE
typedef POTTERY_INSERTION_SORT_VALUE_TYPE pottery_insertion_sort_value_t;
typedef pottery_insertion_sort_value_t* pottery_insertion_sort_ref_t;
#else
typedef POTTERY_INSERTION_SORT_REF_TYPE pottery_insertion_sort_ref_t;
#endif

#ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
typedef POTTERY_INSERTION_SORT_CONTEXT_TYPE pottery_insertion_sort_context_t;
#else
typedef pottery_insertion_sort_ref_t pottery_insertion_sort_context_t;
#endif

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_INSERTION_SORT_EXTERN
void pottery_insertion_sort(
        pottery_insertion_sort_context_t context,
        size_t count
        #if POTTERY_INSERTION_SORT_SEPARATE_LIFECYCLE_CONTEXT
        , pottery_insertion_sort_lifecycle_context_t lifecycle_context
        #endif
        #if POTTERY_INSERTION_SORT_SEPARATE_COMPARE_CONTEXT
        , pottery_insertion_sort_compare_context_t compare_context
        #endif
        );
#endif
