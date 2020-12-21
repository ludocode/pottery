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

/*
 * A template that generates size specializations for a particular variant of
 * qsort(). This is instantiated for each of:
 *
 * - pottery_qsort()
 * - pottery_gnu_qsort_r()
 * - pottery_bsd_qsort_r()
 */

#define POTTERY_QSORT_RENAME(x) POTTERY_CONCAT(POTTERY_QSORT_NAME, x)

// int16_t
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i16)
#define POTTERY_INTRO_SORT_VALUE_TYPE int16_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// int32_t
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i32)
#define POTTERY_INTRO_SORT_VALUE_TYPE int32_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// int64_t
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i64)
#define POTTERY_INTRO_SORT_VALUE_TYPE int64_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// int32_t[3]
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i32_3)
#define POTTERY_INTRO_SORT_VALUE_TYPE pottery_i32_3_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// int64_t[2]
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i64_2)
#define POTTERY_INTRO_SORT_VALUE_TYPE pottery_i64_2_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// int64_t[3]
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i64_3)
#define POTTERY_INTRO_SORT_VALUE_TYPE pottery_i64_3_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// int64_t[4]
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_i64_4)
#define POTTERY_INTRO_SORT_VALUE_TYPE pottery_i64_4_t
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_FIXED_TYPE
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_FIXED
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

// any
#define POTTERY_INTRO_SORT_PREFIX POTTERY_QSORT_RENAME(_any)
#define POTTERY_INTRO_SORT_REF_TYPE void*
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY POTTERY_QSORT_COMPARE_VARIABLE
#define POTTERY_INTRO_SORT_CONTEXT_TYPE POTTERY_QSORT_CONTEXT_VARIABLE_TYPE
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_SELECT(context, base, offset) (char*)base + offset * context.element_size
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_INDEX(context, base, ref) (size_t)((char*)ref - (char*)base) / context.element_size
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_NEXT(context, base, ref) (char*)ref + context.element_size
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_PREVIOUS(context, base, ref) (char*)ref - context.element_size
#define POTTERY_INTRO_SORT_LIFECYCLE_SWAP(context, a, b) pottery_qsort_swap_any(context.element_size, a, b)
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

void POTTERY_QSORT_NAME(void* first, size_t count, size_t element_size,
        POTTERY_QSORT_COMPARE_ARGS
) {
    #if POTTERY_QSORT_USER_CONTEXT
    POTTERY_QSORT_CONTEXT_FIXED_TYPE fixed_context;
    fixed_context.compare = compare;
    fixed_context.user_context = user_context;
    #else
    // without a user context, the compare itself is the fixed context
    POTTERY_QSORT_COMPARE_TYPE fixed_context = compare;
    #endif

    if (element_size == sizeof(int16_t) && pottery_qsort_is_aligned_as(first, int16_t)) {
        POTTERY_QSORT_RENAME(_i16)(fixed_context, (int16_t*)first, count);
        return;
    }

    if (element_size == sizeof(int32_t) && pottery_qsort_is_aligned_as(first, int32_t)) {
        POTTERY_QSORT_RENAME(_i32)(fixed_context, (int32_t*)first, count);
        return;
    }

    if (element_size == sizeof(int64_t) && pottery_qsort_is_aligned_as(first, int64_t)) {
        POTTERY_QSORT_RENAME(_i64)(fixed_context, (int64_t*)first, count);
        return;
    }

    if (element_size == sizeof(pottery_i32_3_t) && pottery_qsort_is_aligned_as(first, pottery_i32_3_t)) {
        POTTERY_QSORT_RENAME(_i32_3)(fixed_context, (pottery_i32_3_t*)first, count);
        return;
    }

    if (element_size == sizeof(pottery_i64_2_t) && pottery_qsort_is_aligned_as(first, pottery_i64_2_t)) {
        POTTERY_QSORT_RENAME(_i64_2)(fixed_context, (pottery_i64_2_t*)first, count);
        return;
    }

    if (element_size == sizeof(pottery_i64_3_t) && pottery_qsort_is_aligned_as(first, pottery_i64_3_t)) {
        POTTERY_QSORT_RENAME(_i64_3)(fixed_context, (pottery_i64_3_t*)first, count);
        return;
    }

    if (element_size == sizeof(pottery_i64_4_t) && pottery_qsort_is_aligned_as(first, pottery_i64_4_t)) {
        POTTERY_QSORT_RENAME(_i64_4)(fixed_context, (pottery_i64_4_t*)first, count);
        return;
    }

    POTTERY_QSORT_CONTEXT_VARIABLE_TYPE variable_context = {
        element_size,
        compare,
        #if POTTERY_QSORT_USER_CONTEXT
        user_context,
        #endif
    };

    POTTERY_QSORT_RENAME(_any)(variable_context, first, count);
}

#undef POTTERY_QSORT_NAME
#undef POTTERY_QSORT_RENAME
#undef POTTERY_QSORT_CONTEXT_FIXED_TYPE
#undef POTTERY_QSORT_COMPARE_FIXED
#undef POTTERY_QSORT_CONTEXT_VARIABLE_TYPE
#undef POTTERY_QSORT_COMPARE_VARIABLE
#undef POTTERY_QSORT_COMPARE_TYPE
#undef POTTERY_QSORT_COMPARE_ARGS
#undef POTTERY_QSORT_USER_CONTEXT
