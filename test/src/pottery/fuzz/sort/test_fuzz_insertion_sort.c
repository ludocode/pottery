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

#define POTTERY_INSERTION_SORT_PREFIX sort_ufos_by_string
#define POTTERY_INSERTION_SORT_VALUE_TYPE ufo_t
#define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE ufo_move
#define POTTERY_INSERTION_SORT_COMPARE_THREE_WAY ufo_compare_by_string
#include "pottery/insertion_sort/pottery_insertion_sort_static.t.h"

// small limits otherwise it's way too slow
#ifdef TEST_POTTERY_FUZZ_MAIN
    #define FUZZ_SORT_UFO_COUNT_LIMIT 500
#else
    #define FUZZ_SORT_UFO_COUNT_LIMIT 50
#endif

#define FUZZ_SORT_IS_STABLE 1
#define TEST_POTTERY_FUZZ_SORT_UFO_PREFIX pottery_insertion_sort_fuzz_ufo
#include "pottery/fuzz/sort/test_fuzz_sort_ufos.t.h"
