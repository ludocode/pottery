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

#include "pottery/common/test_pottery_ufo.h"
#include "pottery/unit/test_pottery_framework.h"

// Instantiate a vector of int
#define POTTERY_VECTOR_PREFIX int_array
#define POTTERY_VECTOR_ELEMENT_TYPE int
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"

// Instantiate int_array tests on our int vector
#define POTTERY_TEST_ARRAY_INT_PREFIX pottery_vector_int
#define POTTERY_TEST_ARRAY_INT_HAS_CAPACITY
#include "pottery/unit/array/test_pottery_unit_array_int.t.h"

// Instantiate a vector of ufo_t
#define POTTERY_VECTOR_PREFIX ufo_array
#define POTTERY_VECTOR_INTERNAL_CAPACITY 1
#define POTTERY_VECTOR_ELEMENT_TYPE ufo_t
#define POTTERY_VECTOR_LIFECYCLE_INIT_COPY ufo_init_copy
#define POTTERY_VECTOR_LIFECYCLE_MOVE ufo_move
#define POTTERY_VECTOR_LIFECYCLE_DESTROY ufo_destroy
#include "pottery/vector/pottery_vector_static.t.h"

// Instantiate ufo_array tests on our ufo vector
#define POTTERY_TEST_ARRAY_UFO_PREFIX pottery_vector_ufo
#define POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
#include "pottery/unit/array/test_pottery_unit_array_ufo.t.h"
