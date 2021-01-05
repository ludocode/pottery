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

#include "pottery/benchmark/test_benchmark_sort_common.h"

// In order to match the definitions of the other qsort() algorithms the
// pottery_qsort() code is inlined into this file. This doesn't appear to make
// a difference but we want to be consistent.
#define POTTERY_QSORT_BENCHMARK_INCLUDE_INLINE

#ifdef POTTERY_QSORT_BENCHMARK_INCLUDE_INLINE
#define pottery_qsort pottery_qsort_benchmark_qsort
#define pottery_gnu_qsort_r pottery_qsort_benchmark_gnu_qsort_r
#define pottery_bsd_qsort_r pottery_qsort_benchmark_bsd_qsort_r
#define pottery_win_qsort_s pottery_qsort_benchmark_win_qsort_s
#include "pottery/qsort/pottery_qsort.c"
#else
#include "pottery/qsort/pottery_qsort.h"
#endif

void pottery_qsort_wrapper(int* ints, size_t count) {
    #ifdef POTTERY_QSORT_BENCHMARK_INCLUDE_INLINE
    pottery_qsort_benchmark_qsort
    #else
    pottery_qsort
    #endif
        (ints, count, sizeof(int), int_compare_pointers);
}
