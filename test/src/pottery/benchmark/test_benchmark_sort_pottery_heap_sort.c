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

#define POTTERY_HEAP_SORT_PREFIX pottery_benchmark_heap_sort
#define POTTERY_HEAP_SORT_VALUE_TYPE benchmark_int_t
#define POTTERY_HEAP_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_HEAP_SORT_COMPARE_BY_VALUE 1
#include "pottery/heap_sort/pottery_heap_sort_static.t.h"

void pottery_benchmark_heap_sort_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_benchmark_heap_sort(ints, count);
}
