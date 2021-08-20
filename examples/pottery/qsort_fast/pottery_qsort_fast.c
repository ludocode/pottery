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

#include "pottery_qsort_fast.h"

#include "pottery/pottery_dependencies.h"



/*
 * Common stuff
 */

// some types
typedef struct {int32_t a[3];} pottery_qsort_i32_3_t;
typedef struct {int64_t a[2];} pottery_qsort_i64_2_t;
typedef struct {int64_t a[3];} pottery_qsort_i64_3_t;
typedef struct {int64_t a[4];} pottery_qsort_i64_4_t;

// little helper to check alignment
static inline
bool pottery_qsort_fast_is_aligned(void* p, size_t required_align) {
    uintptr_t u = pottery_reinterpret_cast(uintptr_t, p);
    return u == (u & ~required_align);
}
#define pottery_qsort_fast_is_aligned_as(p, T) pottery_qsort_fast_is_aligned((p), pottery_alignof(T))

// arbitrary size swap
static void pottery_qsort_fast_swap_any(size_t element_size, void* vleft, void* vright) {
    char* left = pottery_cast(char*, vleft);
    char* right = pottery_cast(char*, vright);
    char* end = right + element_size;
    while (right != end) {
        char temp = *left;
        *left++ = *right;
        *right++ = temp;
    }
}

// Standard C qsort() compare function
typedef int (*pottery_qsort_compare_t)(const void* left, const void* right);

// GNU extension (and C11 Annex K) compare function. The user_context is last.
typedef int (*pottery_gnu_qsort_r_compare_t)(const void* left, const void* right, void* user_context);

// BSD (and MSVC without __cdecl) extension compare function. The user_context is first.
typedef int (*pottery_bsd_qsort_r_compare_t)(void* user_context, const void* left, const void* right);



/*
 * Heap sort fallback
 *
 * We use a common heap_sort fallback across all instantiated quick_sort
 * variants. This minimizes code size while still providing guaranteed O(nlogn)
 * worst case time complexity.
 *
 * This part is nearly identical to the qsort_simple example except we are
 * instantiating heap_sort instead of intro_sort.
 */

typedef enum pottery_qsort_fast_variant_t {
    pottery_qsort_fast_variant_c,
    pottery_qsort_fast_variant_gnu,
    pottery_qsort_fast_variant_bsd,
} pottery_qsort_fast_variant_t;

typedef struct pottery_qsort_fast_heap_sort_state_t {
    size_t element_size;

    pottery_qsort_fast_variant_t variant;

    union {
        int (*c)(const void* left, const void* right);
        int (*gnu)(const void* left, const void* right, void* user_context);
        int (*bsd)(void* user_context, const void* left, const void* right);
    } compare;

    void* user_context;
} pottery_qsort_fast_heap_sort_state_t;

static inline
int pottery_qsort_fast_heap_sort_compare(pottery_qsort_fast_heap_sort_state_t* state, void* left, void* right) {
    switch (state->variant) {
        case pottery_qsort_fast_variant_c:   return state->compare.c(left, right);
        case pottery_qsort_fast_variant_gnu: return state->compare.gnu(left, right, state->user_context);
        case pottery_qsort_fast_variant_bsd: return state->compare.bsd(state->user_context, left, right);
    }
    pottery_unreachable();
    return 0;
}

#define POTTERY_HEAP_SORT_PREFIX pottery_qsort_fast_heap_sort
#define POTTERY_HEAP_SORT_CONTEXT_TYPE pottery_qsort_fast_heap_sort_state_t*
#define POTTERY_HEAP_SORT_REF_TYPE void*
#define POTTERY_HEAP_SORT_COMPARE_THREE_WAY pottery_qsort_fast_heap_sort_compare
#define POTTERY_HEAP_SORT_LIFECYCLE_SWAP(state, left, right) \
        pottery_qsort_fast_swap_any(state->element_size, left, right)

#define POTTERY_HEAP_SORT_ARRAY_ACCESS_SELECT(state, base, index) \
        (char*)base + (index * state->element_size)
#define POTTERY_HEAP_SORT_ARRAY_ACCESS_INDEX(state, base, ref) \
        (size_t)((char*)ref - (char*)base) / state->element_size

#define POTTERY_HEAP_SORT_ARRAY_ACCESS_EQUAL(state, base, left, right) left == right
#define POTTERY_HEAP_SORT_ARRAY_ACCESS_NEXT(state, base, ref) (char*)ref + state->element_size
#define POTTERY_HEAP_SORT_ARRAY_ACCESS_PREVIOUS(state, base, ref) (char*)ref - state->element_size

#include "pottery/heap_sort/pottery_heap_sort_static.t.h"



/*
 * Heap sort fallback variant wrappers
 */

static
void pottery_qsort_fast_heap_sort_c(size_t element_size, pottery_qsort_compare_t compare,
        void* base, size_t offset, size_t count)
{
    pottery_qsort_fast_heap_sort_state_t state;
    state.element_size = element_size;
    state.variant = pottery_qsort_fast_variant_c;
    state.compare.c = compare;
    pottery_qsort_fast_heap_sort_range(&state, base, offset, count);
}

static
void pottery_qsort_fast_heap_sort_gnu(size_t element_size, pottery_gnu_qsort_r_compare_t compare,
        void* user_context, void* base, size_t offset, size_t count)
{
    pottery_qsort_fast_heap_sort_state_t state;
    state.element_size = element_size;
    state.variant = pottery_qsort_fast_variant_gnu;
    state.compare.gnu = compare;
    state.user_context = user_context;
    pottery_qsort_fast_heap_sort_range(&state, base, offset, count);
}

static
void pottery_qsort_fast_heap_sort_bsd(size_t element_size, pottery_bsd_qsort_r_compare_t compare,
        void* user_context, void* base, size_t offset, size_t count)
{
    pottery_qsort_fast_heap_sort_state_t state;
    state.element_size = element_size;
    state.variant = pottery_qsort_fast_variant_bsd;
    state.compare.bsd = compare;
    state.user_context = user_context;
    pottery_qsort_fast_heap_sort_range(&state, base, offset, count);
}



/*
 * pottery_qsort()
 */

// The fixed-size qsort() implementations use the compare function as the
// context directly.

// The variable-sized qsort() context
typedef struct {
    size_t element_size;
    pottery_qsort_compare_t compare;
} pottery_qsort_variable_context_t;

#define POTTERY_QSORT_NAME pottery_qsort_fast
#define POTTERY_QSORT_COMPARE_TYPE pottery_qsort_compare_t
#define POTTERY_QSORT_CONTEXT_FIXED_TYPE pottery_qsort_compare_t
#define POTTERY_QSORT_COMPARE_FIXED(compare, a, b) compare((void*)a, (void*)b)
#define POTTERY_QSORT_CONTEXT_VARIABLE_TYPE pottery_qsort_variable_context_t
#define POTTERY_QSORT_COMPARE_VARIABLE(context, a, b) context.compare((void*)a, (void*)b)
#define POTTERY_QSORT_USER_CONTEXT 0
#define POTTERY_QSORT_COMPARE_ARGS pottery_qsort_compare_t compare
#define POTTERY_QSORT_FIXED_DEPTH_LIMIT_FALLBACK(compare, base, offset, count) \
        pottery_qsort_fast_heap_sort_c(sizeof(POTTERY_QUICK_SORT_VALUE_TYPE), compare, base, offset, count)
#define POTTERY_QSORT_VARIABLE_DEPTH_LIMIT_FALLBACK(context, base, offset, count) \
        pottery_qsort_fast_heap_sort_c(context.element_size, context.compare, base, offset, count)
#include "pottery_qsort_fast_sizes.t.h"



/*
 * pottery_gnu_qsort_r()
 */

// The fixed-size context.
typedef struct {
    pottery_gnu_qsort_r_compare_t compare;
    void* user_context;
} pottery_gnu_qsort_r_context_fixed_t;

// The variable-sized context.
typedef struct {
    size_t element_size;
    pottery_gnu_qsort_r_compare_t compare;
    void* user_context;
} pottery_gnu_qsort_r_context_variable_t;

#define POTTERY_QSORT_NAME pottery_gnu_qsort_r_fast
#define POTTERY_QSORT_COMPARE_TYPE pottery_gnu_qsort_r_compare_t
#define POTTERY_QSORT_CONTEXT_FIXED_TYPE pottery_gnu_qsort_r_context_fixed_t
#define POTTERY_QSORT_COMPARE_FIXED(context, a, b) context.compare((void*)a, (void*)b, context.user_context)
#define POTTERY_QSORT_CONTEXT_VARIABLE_TYPE pottery_gnu_qsort_r_context_variable_t
#define POTTERY_QSORT_COMPARE_VARIABLE(context, a, b) context.compare((void*)a, (void*)b, context.user_context)
#define POTTERY_QSORT_USER_CONTEXT 1
#define POTTERY_QSORT_COMPARE_ARGS pottery_gnu_qsort_r_compare_t compare, void* user_context
#define POTTERY_QSORT_FIXED_DEPTH_LIMIT_FALLBACK(context, base, offset, count) \
        pottery_qsort_fast_heap_sort_gnu(sizeof(POTTERY_QUICK_SORT_VALUE_TYPE), context.compare, \
                context.user_context, base, offset, count)
#define POTTERY_QSORT_VARIABLE_DEPTH_LIMIT_FALLBACK(context, base, offset, count) \
        pottery_qsort_fast_heap_sort_gnu(context.element_size, context.compare, context.user_context, \
                base, offset, count)
#include "pottery_qsort_fast_sizes.t.h"



/*
 * pottery_bsd_qsort_r()
 */

// The fixed-size context.
typedef struct {
    pottery_bsd_qsort_r_compare_t compare;
    void* user_context;
} pottery_bsd_qsort_r_context_fixed_t;

// The variable-size context.
typedef struct {
    size_t element_size;
    pottery_bsd_qsort_r_compare_t compare;
    void* user_context;
} pottery_bsd_qsort_r_context_variable_t;

#define POTTERY_QSORT_NAME pottery_bsd_qsort_r_fast
#define POTTERY_QSORT_COMPARE_TYPE pottery_bsd_qsort_r_compare_t
#define POTTERY_QSORT_CONTEXT_FIXED_TYPE pottery_bsd_qsort_r_context_fixed_t
#define POTTERY_QSORT_COMPARE_FIXED(context, a, b) context.compare(context.user_context, (void*)a, (void*)b)
#define POTTERY_QSORT_CONTEXT_VARIABLE_TYPE pottery_bsd_qsort_r_context_variable_t
#define POTTERY_QSORT_COMPARE_VARIABLE(context, a, b) context.compare(context.user_context, (void*)a, (void*)b)
#define POTTERY_QSORT_USER_CONTEXT 1
#define POTTERY_QSORT_COMPARE_ARGS void* user_context, pottery_bsd_qsort_r_compare_t compare
#define POTTERY_QSORT_FIXED_DEPTH_LIMIT_FALLBACK(context, base, offset, count) \
        pottery_qsort_fast_heap_sort_bsd(sizeof(POTTERY_QUICK_SORT_VALUE_TYPE), context.compare, \
                context.user_context, base, offset, count)
#define POTTERY_QSORT_VARIABLE_DEPTH_LIMIT_FALLBACK(context, base, offset, count) \
        pottery_qsort_fast_heap_sort_bsd(context.element_size, context.compare, context.user_context, \
                base, offset, count)
#include "pottery_qsort_fast_sizes.t.h"



/*
 * pottery_win_qsort_s()
 */

// The compare function in Windows's qsort_s() is the same as BSD's qsort_r(),
// except that it explicitly specifies __cdecl. This is the default on x86 and
// does nothing on other platforms, so we don't bother to instantiate separate
// functions for win_qsort_s(); we just wrap bsd_qsort_r(). If for some reason
// you are compiling x86 with a different default calling convention, you
// should at least get a compiler error from this rather than it crashing at
// runtime.

void pottery_win_qsort_s_fast(void* first, size_t count, size_t element_size,
        int (
            #if defined(_MSC_VER) || defined(__MINGW32__)
            __cdecl
            #endif
            *compare)(void* user_context, const void* left, const void* right),
        void* user_context)
{
    // No cast passing compare. We want a compiler error if the calling
    // convention is different.
    // Note that the order of arguments is different from bsd_qsort_r().
    pottery_bsd_qsort_r_fast(first, count, element_size, user_context, compare);
}



/*
 * pottery_c11_qsort_s()
 */

// C11 Annex K specifies this "safe" sorting function. It provides minimal
// actual safety. There's no way to query the current constraint handler so
// it's not possible for something outside of libc to correctly implement it.
// Instead if a constraint violation happens we just abort. We otherwise wrap
// gnu_qsort_r() since the API is nearly the same.

#if defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)
errno_t pottery_c11_qsort_s_fast(void* first, rsize_t count, rsize_t element_size,
        int (*compare)(const void* left, const void* right, void* user_context),
        void* user_context)
{
    if (pottery_unlikely(first == pottery_null || compare == pottery_null ||
            count > RSIZE_MAX || element_size > RSIZE_MAX))
    {
        // If we could get the constraint handler, we'd call it here.
        abort();
    }

    pottery_gnu_qsort_r(first,
            pottery_cast(size_t, count),
            pottery_cast(size_t, element_size),
            compare,
            user_context);

    return 0;
}
#endif
