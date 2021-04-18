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

#include "pottery_simple_qsort.h"

#include "pottery/pottery_dependencies.h"

typedef enum variant_t {
    variant_c,
    variant_gnu,
    variant_bsd,
} variant_t;

typedef struct state_t {
    size_t element_size;

    variant_t variant;

    union {
        int (*c)(const void* left, const void* right);
        int (*gnu)(const void* left, const void* right, void* user_context);
        int (*bsd)(void* user_context, const void* left, const void* right);
    } compare;

    void* user_context;
} state_t;

static inline
int qsort_compare(state_t* state, void* left, void* right) {
    switch (state->variant) {
        case variant_c:   return state->compare.c(left, right);
        case variant_gnu: return state->compare.gnu(left, right, state->user_context);
        case variant_bsd: return state->compare.bsd(state->user_context, left, right);
    }
    // unreachable
    return 0;
}

static inline
void qsort_swap(state_t* state, void* vleft, void* vright) {
    char* left = pottery_cast(char*, vleft);
    char* right = pottery_cast(char*, vright);
    char* end = right + state->element_size;
    while (right != end) {
        char temp = *left;
        *left++ = *right;
        *right++ = temp;
    }
}

// We will use Pottery's intro_sort template. This will give us very high
// performance with guaranteed O(nlogn) worst-case time complexity.
//
// We want our state struct passed through to our configured expressions so we
// pass it as CONTEXT_TYPE. Our REF_TYPE is void* and we SWAP and COMPARE with
// the above functions.

#define POTTERY_INTRO_SORT_PREFIX qsort_impl
#define POTTERY_INTRO_SORT_CONTEXT_TYPE state_t*
#define POTTERY_INTRO_SORT_REF_TYPE void*
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY qsort_compare
#define POTTERY_INTRO_SORT_LIFECYCLE_SWAP qsort_swap

// SELECT and INDEX are used to access into the array. We offset pointers
// by the user's element size.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_SELECT(state, base, index) \
        pottery_cast(char*, base) + (index * state->element_size)
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_INDEX(state, base, ref) \
        pottery_cast(size_t, pottery_cast(char*, ref) - pottery_cast(char*, base)) / state->element_size

// EQUAL, NEXT and PREVIOUS are not required but they significantly improve
// performance. EQUAL is especially important because without it entries can
// only be compared by index.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_EQUAL(state, base, left, right) left == right
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_NEXT(state, base, ref) pottery_cast(char*, ref) + state->element_size
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_PREVIOUS(state, base, ref) pottery_cast(char*, ref) - state->element_size

#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

void pottery_simple_qsort(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right))
{
    state_t state;
    state.element_size = element_size;
    state.variant = variant_c;
    state.compare.c = compare;
    qsort_impl(&state, first, count);
}

void pottery_simple_gnu_qsort_r(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right, void* user_context),
        void* user_context)
{
    state_t state;
    state.element_size = element_size;
    state.variant = variant_gnu;
    state.compare.gnu = compare;
    state.user_context = user_context;
    qsort_impl(&state, first, count);
}

void pottery_simple_bsd_qsort_r(void* first, size_t count, size_t element_size,
        void* user_context,
        int (*compare)(void* user_context, const void* left, const void* right))
{
    state_t state;
    state.element_size = element_size;
    state.variant = variant_bsd;
    state.compare.bsd = compare;
    state.user_context = user_context;
    qsort_impl(&state, first, count);
}
