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


__attribute__((__always_inline__))
static inline int int_compare_values(const int a, const int b) {
    #if SAFE_THREE_WAY_COMPARE
    // The ternary is faster than the alternative below, sometimes even faster
    // than the subtraction while being safe, as long as it gets inlined. This
    // is probably because when inlined the compiler can detect when algorithms
    // ignore the greater-than branch.
    return ((a < b) ? -1 : ((a > b) ? 1 : 0));

    // This is significantly slower despite optimizations, even when the
    // comparison function is inlined.
    //return (a > b) - (a < b);
    #else
    return a - b;
    #endif
}



// pottery quick sort

#define POTTERY_QUICK_SORT_PREFIX pottery_benchmark_quick_sort
#define POTTERY_QUICK_SORT_VALUE_TYPE int
#define POTTERY_QUICK_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_QUICK_SORT_COMPARE_BY_VALUE 1
#include "pottery/quick_sort/pottery_quick_sort_static.t.h"

void pottery_benchmark_quick_sort_wrapper(int* ints, size_t count) {
    pottery_benchmark_quick_sort(ints, count);
}



// pottery intro sort

#define POTTERY_INTRO_SORT_PREFIX pottery_benchmark_intro_sort
#define POTTERY_INTRO_SORT_VALUE_TYPE int
#define POTTERY_INTRO_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_INTRO_SORT_COMPARE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

void pottery_benchmark_intro_sort_wrapper(int* ints, size_t count) {
    pottery_benchmark_intro_sort(ints, count);
}



// pottery shell sort

#define POTTERY_SHELL_SORT_PREFIX pottery_benchmark_shell_sort
#define POTTERY_SHELL_SORT_VALUE_TYPE int
#define POTTERY_SHELL_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_SHELL_SORT_COMPARE_BY_VALUE 1
#include "pottery/shell_sort/pottery_shell_sort_static.t.h"

void pottery_benchmark_shell_sort_wrapper(int* ints, size_t count) {
    pottery_benchmark_shell_sort(ints, count);
}



// pottery heap sort

#define POTTERY_HEAP_SORT_PREFIX pottery_benchmark_heap_sort
#define POTTERY_HEAP_SORT_VALUE_TYPE int
#define POTTERY_HEAP_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_HEAP_SORT_COMPARE_BY_VALUE 1
#include "pottery/heap_sort/pottery_heap_sort_static.t.h"

void pottery_benchmark_heap_sort_wrapper(int* ints, size_t count) {
    pottery_benchmark_heap_sort(ints, count);
}



// qsort()
static int int_compare_pointers(const void* left, const void* right) {
    return int_compare_values(
            *pottery_reinterpret_cast(const int*, left),
            *pottery_reinterpret_cast(const int*, right));
}

void qsort_wrapper(int* ints, size_t count) {
    qsort(ints, count, sizeof(int), int_compare_pointers);
}



// pottery_qsort_simple

#include "pottery/qsort_simple/pottery_qsort_simple.c"

void pottery_qsort_simple_wrapper(int* ints, size_t count) {
    pottery_qsort_simple(ints, count, sizeof(int), int_compare_pointers);
}




// Freaky/pqsort

#include "pqsort/pqsort.h"

define_pqsort(ints, int, int_compare_values)

void pqsort_wrapper(int* ints, size_t count) {
    ints_pqsort(ints, pottery_cast(int, count), 0, pottery_cast(int, count));
}



// swenson/sort

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"

#define SORT_NAME benchmark_swenson
#define SORT_TYPE int
#define SORT_CMP int_compare_values
#include "swenson_sort/sort.h"

void swenson_timsort_wrapper(int* ints, size_t count) {
    benchmark_swenson_tim_sort(ints, count);
}

void swenson_quicksort_wrapper(int* ints, size_t count) {
    benchmark_swenson_quick_sort(ints, count);
}

#pragma GCC diagnostic pop



// This one is buggy.
#if 0

// justinow/introsort-c

// hack around use of variable-length arrays
#define WINDOWS 1
#define _malloca alloca
#define _freea(x) /*nothing*/

#include "justinow_introsort_c/sort.c"

void justinow_introsort_c_wrapper(int* ints, size_t count) {
    sort(ints, count, sizeof(int), int_compare_pointers);
}

#endif
