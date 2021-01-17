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

// Instantiate a vector of ufo_t with a custom context
#define POTTERY_VECTOR_PREFIX ufo_array
#define POTTERY_VECTOR_INTERNAL_CAPACITY 1
#define POTTERY_VECTOR_VALUE_TYPE ufo_t
#define POTTERY_VECTOR_CONTEXT_TYPE uint32_t
// custom allocators, verifying context
#define POTTERY_VECTOR_ALLOC_MALLOC(context, size) \
    (pottery_assert(context == 0xDEADC0DE), malloc(size))
#define POTTERY_VECTOR_ALLOC_FREE(context, p) \
    (pottery_assert(context == 0xDEADC0DE), free(p))
// custom lifecycle functions, verifying context
#define POTTERY_VECTOR_LIFECYCLE_INIT_COPY(context, to, from) \
    (pottery_assert(context == 0xDEADC0DE), ufo_init_copy(to, from))
#define POTTERY_VECTOR_LIFECYCLE_MOVE(context, to, from) \
    (pottery_assert(context == 0xDEADC0DE), ufo_move(to, from))
#define POTTERY_VECTOR_LIFECYCLE_DESTROY(context, ufo) \
    (pottery_assert(context == 0xDEADC0DE), ufo_destroy(ufo))
#include "pottery/vector/pottery_vector_static.t.h"

// Instantiate ufo_array tests on our ufo vector
#define POTTERY_TEST_ARRAY_UFO_PREFIX pottery_vector_ufo
#define POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
#define POTTERY_TEST_ARRAY_UFO_CONTEXT_VAL ,0xDEADC0DE
#include "pottery/unit/array/test_pottery_unit_array_ufo.t.h"
