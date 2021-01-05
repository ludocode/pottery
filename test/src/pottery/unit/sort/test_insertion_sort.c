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

// Instantiate insertion_sort on an int array
#define POTTERY_INSERTION_SORT_PREFIX sort_ints
#define POTTERY_INSERTION_SORT_VALUE_TYPE int
#define POTTERY_INSERTION_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_INSERTION_SORT_COMPARE_BY_VALUE 1
#include "pottery/insertion_sort/pottery_insertion_sort_static.t.h"

// Instantiate sort_ints tests
#define POTTERY_TEST_SORT_INT_PREFIX pottery_insertion_sort_int
#include "pottery/unit/sort/test_sort_ints.t.h"

#if 0
// TODO implement sort on ufos
// Instantiate a vector of ufo_t
#define POTTERY_INSERTION_SORT_PREFIX ufo_array
#define POTTERY_INSERTION_SORT_VALUE_TYPE ufo_t
#define POTTERY_INSERTION_SORT_LIFECYCLE_INIT_COPY ufo_init_copy
#define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE ufo_move
#define POTTERY_INSERTION_SORT_DESTROY ufo_destroy
#include "pottery/vector/pottery_vector_static.t.h"

// Instantiate ufo_array tests on our ufo vector
#define POTTERY_TEST_ARRAY_UFO_PREFIX pottery_vector_ufo
#define POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
#include "pottery/unit/test_pottery_unit_array_ufo.t.h"
#endif
