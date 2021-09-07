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

#include "pottery_qsort.h"

#include "pottery/pottery_dependencies.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4702) // unreachable code after pottery_unreachable()
#endif

typedef enum pottery_qsort_variant_t {
    pottery_qsort_variant_c,
    pottery_qsort_variant_gnu,
    pottery_qsort_variant_bsd,
    pottery_qsort_variant_windows,
} pottery_qsort_variant_t;

typedef enum pottery_qsort_size_class_t {
    pottery_qsort_size_class_4,
    pottery_qsort_size_class_8,
    pottery_qsort_size_class_any,
} pottery_qsort_size_class_t;

static inline
bool pottery_qsort_is_aligned(void* p, size_t required_align) {
    uintptr_t u = pottery_reinterpret_cast(uintptr_t, p);
    return u == (u & ~required_align);
}
#define pottery_qsort_is_aligned_as(p, T) pottery_qsort_is_aligned((p), pottery_alignof(T))

static inline
pottery_qsort_size_class_t pottery_qsort_resolve_size_class(void* first, size_t element_size) {
    if (element_size == sizeof(uint32_t) && pottery_qsort_is_aligned_as(first, uint32_t))
        return pottery_qsort_size_class_4;
    if (element_size == sizeof(uint64_t) && pottery_qsort_is_aligned_as(first, uint64_t))
        return pottery_qsort_size_class_8;
    return pottery_qsort_size_class_any;
}

typedef struct pottery_qsort_state_t {
    size_t element_size;
    pottery_qsort_size_class_t size_class;

    pottery_qsort_variant_t variant;
    union {
        int (*c)(const void* left, const void* right);
        int (*gnu)(const void* left, const void* right, void* user_context);
        int (*bsd)(void* user_context, const void* left, const void* right);
        int (pottery_qsort_cdecl *windows)(void* user_context, const void* left, const void* right);
    } compare;

    void* user_context;
} pottery_qsort_state_t;

static inline
int pottery_qsort_compare(pottery_qsort_state_t* state, void* left, void* right) {
    switch (state->variant) {
        case pottery_qsort_variant_c:       return state->compare.c(left, right);
        case pottery_qsort_variant_gnu:     return state->compare.gnu(left, right, state->user_context);
        case pottery_qsort_variant_bsd:     return state->compare.bsd(state->user_context, left, right);
        case pottery_qsort_variant_windows: return state->compare.windows(state->user_context, left, right);
    }
    pottery_unreachable();
    return 0;
}

static inline
void pottery_qsort_swap(pottery_qsort_state_t* state, void* vleft, void* vright) {
    switch (state->size_class) {
        case pottery_qsort_size_class_4: {
            uint32_t* left = pottery_cast(uint32_t*, vleft);
            uint32_t* right = pottery_cast(uint32_t*, vright);
            uint32_t temp = *left;
            *left = *right;
            *right = temp;
        } return;
        case pottery_qsort_size_class_8: {
            uint64_t* left = pottery_cast(uint64_t*, vleft);
            uint64_t* right = pottery_cast(uint64_t*, vright);
            uint64_t temp = *left;
            *left = *right;
            *right = temp;
        } return;
        case pottery_qsort_size_class_any: {
            char* left = pottery_cast(char*, vleft);
            char* right = pottery_cast(char*, vright);
            char* end = right + state->element_size;
            while (right != end) {
                char temp = *left;
                *left++ = *right;
                *right++ = temp;
            }
        } return;
    }
    pottery_unreachable();
}

// We will use Pottery's intro_sort template. This will give us very high
// performance with guaranteed O(nlogn) worst-case time complexity.
//
// We want our state struct passed through to our configured expressions so we
// pass it as CONTEXT_TYPE. Our REF_TYPE is void* and we SWAP and COMPARE with
// the above functions.

#define POTTERY_INTRO_SORT_PREFIX pottery_qsort_impl
#define POTTERY_INTRO_SORT_CONTEXT_TYPE pottery_qsort_state_t*
#define POTTERY_INTRO_SORT_REF_TYPE void*
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY pottery_qsort_compare
#define POTTERY_INTRO_SORT_LIFECYCLE_SWAP pottery_qsort_swap

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

void pottery_qsort(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right))
{
    pottery_qsort_state_t state;
    state.element_size = element_size;
    state.size_class = pottery_qsort_resolve_size_class(first, element_size);
    state.variant = pottery_qsort_variant_c;
    state.compare.c = compare;
    pottery_qsort_impl(&state, first, count);
}

void pottery_gnu_qsort_r(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right, void* user_context),
        void* user_context)
{
    pottery_qsort_state_t state;
    state.element_size = element_size;
    state.size_class = pottery_qsort_resolve_size_class(first, element_size);
    state.variant = pottery_qsort_variant_gnu;
    state.compare.gnu = compare;
    state.user_context = user_context;
    pottery_qsort_impl(&state, first, count);
}

void pottery_bsd_qsort_r(void* first, size_t count, size_t element_size,
        void* user_context,
        int (*compare)(void* user_context, const void* left, const void* right))
{
    pottery_qsort_state_t state;
    state.element_size = element_size;
    state.size_class = pottery_qsort_resolve_size_class(first, element_size);
    state.variant = pottery_qsort_variant_bsd;
    state.compare.bsd = compare;
    state.user_context = user_context;
    pottery_qsort_impl(&state, first, count);
}

void pottery_win_qsort_s(void* first, size_t count, size_t element_size,
        int (pottery_qsort_cdecl *compare)(void* user_context, const void* left, const void* right),
        void* user_context)
{
    pottery_qsort_state_t state;
    state.element_size = element_size;
    state.size_class = pottery_qsort_resolve_size_class(first, element_size);
    state.variant = pottery_qsort_variant_windows;
    state.compare.windows = compare;
    state.user_context = user_context;
    pottery_qsort_impl(&state, first, count);
}

#ifdef _MSC_VER
// pop warnings in case this file is included by another
#pragma warning(pop)
#endif
