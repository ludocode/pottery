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

#include "pottery/common/test_pottery_ufo.h"
#include "pottery/unit/test_pottery_framework.h"

// Instantiate quick_sort on an int array
#define POTTERY_QUICK_SORT_PREFIX sort_ints
#define POTTERY_QUICK_SORT_VALUE_TYPE int
#define POTTERY_QUICK_SORT_LIFECYCLE_SWAP_BY_VALUE 1 // swap
#define POTTERY_QUICK_SORT_USE_FAT_PARTITION 1 // fat
#define POTTERY_QUICK_SORT_COMPARE_THREE_WAY(x, y) (*x < *y) ? -1 : (*x > *y) ? 1 : 0 // 3way
#define POTTERY_QUICK_SORT_COUNT_LIMIT 0 // never use insertion sort: test small partitions
#include "pottery/quick_sort/pottery_quick_sort_static.t.h"

// Instantiate sort_ints tests
#define POTTERY_TEST_SORT_INT_PREFIX pottery_quick_sort_int_fat_swap_3way
#include "pottery/unit/sort/test_sort_ints.t.h"
