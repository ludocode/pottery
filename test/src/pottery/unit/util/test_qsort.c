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

#include "pottery/qsort/pottery_qsort.h"

#include "pottery/unit/test_pottery_framework.h"

static int strcmp_wrapper(const void* left, const void* right) {
    return strcmp(
            *pottery_cast(const char* const*, left),
            *pottery_cast(const char* const*, right));
}

static int compare_int16(const void* vleft, const void* vright) {
    int16_t left = *pottery_cast(const int16_t*, vleft);
    int16_t right = *pottery_cast(const int16_t*, vright);
    return (left < right) ? -1 : ((left > right) ? 1 : 0);
}

static int compare_int32_gnu(const void* vleft, const void* vright, void* context) {
    pottery_test_assert(*pottery_cast(const int32_t*, context) == 0x12345678);
    int32_t left = *pottery_cast(const int32_t*, vleft);
    int32_t right = *pottery_cast(const int32_t*, vright);
    return (left < right) ? -1 : ((left > right) ? 1 : 0);
}

static int compare_int64_bsd(void* context, const void* vleft, const void* vright) {
    pottery_test_assert(*pottery_cast(const int32_t*, context) == 0x0abcdef0);
    int64_t left = *pottery_cast(const int64_t*, vleft);
    int64_t right = *pottery_cast(const int64_t*, vright);
    return (left < right) ? -1 : ((left > right) ? 1 : 0);
}

POTTERY_TEST(pottery_qsort_strings) {
    const char* players[] = {
        "fred", "quincy", "alice", "eve", "zack", "ned", "paul", "bob", "gary",
        "ursula", "yves", "carl", "olivia", "steve", "rob", "mike", "wade", "dave",
        "jake", "helen", "xavier", "karen", "tammy", "laura", "isaac", "vick",
    };

    pottery_qsort(pottery_cast(void*, players), pottery_array_count(players),
            sizeof(players[0]), strcmp_wrapper);

    size_t i;
    for (i = 1; i < pottery_array_count(players); ++i)
        pottery_test_assert(strcmp(players[i-1], players[i]) <= 0);
}

POTTERY_TEST(pottery_qsort_int16) {
    int16_t ints[] = {-1, 32, 16, -124, 0, -133, 4, 4, 17, 4, 4, 4, 28, INT16_MAX, INT16_MIN};

    pottery_qsort(pottery_cast(void*, ints), pottery_array_count(ints),
            sizeof(ints[0]), compare_int16);

    size_t i;
    for (i = 1; i < pottery_array_count(ints); ++i)
        pottery_test_assert(ints[i-1] <= ints[i]);
}

POTTERY_TEST(pottery_qsort_int32) {
    int32_t ints[] = {-1, 32, 16, -124, 0, -133, 4, 4, 17, 4, 4, 4, 28, INT32_MAX, INT32_MIN};
    int32_t context = 0x12345678;

    pottery_gnu_qsort_r(pottery_cast(void*, ints), pottery_array_count(ints),
            sizeof(ints[0]), compare_int32_gnu, &context);

    size_t i;
    for (i = 1; i < pottery_array_count(ints); ++i)
        pottery_test_assert(ints[i-1] <= ints[i]);
}

POTTERY_TEST(pottery_qsort_int64) {
    int64_t ints[] = {-1, 32, 16, -124, 0, -133, 4, 4, 17, 4, 4, 4, 28, INT64_MAX, INT64_MIN};
    int32_t context = 0x0abcdef0;

    pottery_bsd_qsort_r(pottery_cast(void*, ints), pottery_array_count(ints),
            sizeof(ints[0]), &context, compare_int64_bsd);

    size_t i;
    for (i = 1; i < pottery_array_count(ints); ++i)
        pottery_test_assert(ints[i-1] <= ints[i]);
}
