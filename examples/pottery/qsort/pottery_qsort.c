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

#include "pottery_qsort.h"

#include "pottery/pottery_dependencies.h"



/*
 * common stuff
 */

// some types
typedef struct {int32_t a[3];} pottery_i32_3_t;
typedef struct {int64_t a[2];} pottery_i64_2_t;
typedef struct {int64_t a[3];} pottery_i64_3_t;
typedef struct {int64_t a[4];} pottery_i64_4_t;

// little helper to check alignment
static inline bool pottery_qsort_is_aligned(void* p, size_t required_align) {
    uintptr_t u = pottery_reinterpret_cast(uintptr_t, p);
    return u == (u & ~required_align);
}
#define pottery_qsort_is_aligned_as(p, T) pottery_qsort_is_aligned((p), pottery_alignof(T))

// arbitrary size swap
static void pottery_qsort_swap_any(size_t element_size, void* vleft, void* vright) {
    char* left = pottery_cast(char*, vleft);
    char* right = pottery_cast(char*, vright);
    char* end = right + element_size;
    while (right != end) {
        char temp = *left;
        *left = *right;
        *right = temp;
        ++left;
        ++right;
    }
}



/*
 * pottery_qsort()
 */

// qsort() compare function type. The fixed-size qsort() implementations use
// the compare function as the context directly.
typedef int (*pottery_qsort_compare_t)(const void* left, const void* right);

// The variable-sized qsort() context.
typedef struct {
    size_t element_size;
    pottery_qsort_compare_t compare;
} pottery_qsort_variable_context_t;

#define POTTERY_QSORT_NAME pottery_qsort
#define POTTERY_QSORT_COMPARE_TYPE pottery_qsort_compare_t
#define POTTERY_QSORT_CONTEXT_FIXED_TYPE pottery_qsort_compare_t
#define POTTERY_QSORT_COMPARE_FIXED(compare, a, b) compare((void*)a, (void*)b)
#define POTTERY_QSORT_CONTEXT_VARIABLE_TYPE pottery_qsort_variable_context_t
#define POTTERY_QSORT_COMPARE_VARIABLE(context, a, b) context.compare((void*)a, (void*)b)
#define POTTERY_QSORT_USER_CONTEXT 0
#define POTTERY_QSORT_COMPARE_ARGS pottery_qsort_compare_t compare
#include "pottery_qsort_sizes.t.h"



/*
 * pottery_gnu_qsort_r()
 */

// GNU extension (and C11 Annex K) compare function. The user_context is last.
typedef int (*pottery_gnu_qsort_r_compare_t)(const void* left, const void* right, void* user_context);

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

#define POTTERY_QSORT_NAME pottery_gnu_qsort_r
#define POTTERY_QSORT_COMPARE_TYPE pottery_gnu_qsort_r_compare_t
#define POTTERY_QSORT_CONTEXT_FIXED_TYPE pottery_gnu_qsort_r_context_fixed_t
#define POTTERY_QSORT_COMPARE_FIXED(context, a, b) context.compare((void*)a, (void*)b, context.user_context)
#define POTTERY_QSORT_CONTEXT_VARIABLE_TYPE pottery_gnu_qsort_r_context_variable_t
#define POTTERY_QSORT_COMPARE_VARIABLE(context, a, b) context.compare((void*)a, (void*)b, context.user_context)
#define POTTERY_QSORT_USER_CONTEXT 1
#define POTTERY_QSORT_COMPARE_ARGS pottery_gnu_qsort_r_compare_t compare, void* user_context
#include "pottery_qsort_sizes.t.h"



/*
 * pottery_bsd_qsort_r()
 */

// BSD (and MSVC without __cdecl) extension compare function. The user_context is first.
typedef int (*pottery_bsd_qsort_r_compare_t)(void* user_context, const void* left, const void* right);

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

#define POTTERY_QSORT_NAME pottery_bsd_qsort_r
#define POTTERY_QSORT_COMPARE_TYPE pottery_bsd_qsort_r_compare_t
#define POTTERY_QSORT_CONTEXT_FIXED_TYPE pottery_bsd_qsort_r_context_fixed_t
#define POTTERY_QSORT_COMPARE_FIXED(context, a, b) context.compare(context.user_context, (void*)a, (void*)b)
#define POTTERY_QSORT_CONTEXT_VARIABLE_TYPE pottery_bsd_qsort_r_context_variable_t
#define POTTERY_QSORT_COMPARE_VARIABLE(context, a, b) context.compare(context.user_context, (void*)a, (void*)b)
#define POTTERY_QSORT_USER_CONTEXT 1
#define POTTERY_QSORT_COMPARE_ARGS void* user_context, pottery_bsd_qsort_r_compare_t compare
#include "pottery_qsort_sizes.t.h"



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

void pottery_win_qsort_s(void* first, size_t count, size_t element_size,
        int (
            #if defined(_MSC_VER) || defined(__MINGW32__)
            __cdecl
            #endif
            *compare)(void* user_context, const void* left, const void* right),
        void* user_context)
{
    // No cast passing compare. We want a compiler error if the calling
    // convention is different.
    // Note that the order of arguments is different from bsd_qsort_r():
    // user_context comes before compare.
    pottery_bsd_qsort_r(first, count, element_size, user_context, compare);
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
errno_t pottery_c11_qsort_s(void* first, rsize_t count, rsize_t element_size,
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
