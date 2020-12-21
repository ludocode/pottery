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

#include "pottery_qsort_simple.h"

#include <string.h>

// When using a compare callback without a context, we set user_context to this
// special sentinel address. This lets us detect it in qsort_compare().
static char qsort_compare_without_context;

typedef struct qsort_context_t {
    size_t element_size;
    union {
        int (*with_context)(const void* left, const void* right, void* user_context);
        int (*without_context)(const void* left, const void* right);
    } compare;
    void* user_context;
} qsort_context_t;

static inline
void* qsort_array_access_select(qsort_context_t context, void* base, size_t index) {
    return (char*)base + (index * context.element_size);
}

static inline
size_t qsort_array_access_index(qsort_context_t context, void* base, void* ref) {
    return (size_t)((char*)ref - (char*)base) / context.element_size;
}

static inline
int qsort_compare(qsort_context_t context, void* left, void* right) {
    if (context.user_context == &qsort_compare_without_context)
        return context.compare.without_context(left, right);
    return context.compare.with_context(left, right, context.user_context);
}

static inline
void qsort_swap(qsort_context_t context, void* vleft, void* vright) {
    char* left = (char*)vleft;
    char* right = (char*)vright;
    char* end = right + context.element_size;
    while (right != end) {
        char temp = *left;
        *left = *right;
        *right = temp;
        ++left;
        ++right;
    }
}

#define POTTERY_INTRO_SORT_PREFIX qsort_run
#define POTTERY_INTRO_SORT_REF_TYPE void*
#define POTTERY_INTRO_SORT_CONTEXT_TYPE qsort_context_t
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_SELECT qsort_array_access_select
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_INDEX qsort_array_access_index
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY qsort_compare
#define POTTERY_INTRO_SORT_LIFECYCLE_SWAP qsort_swap
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

void pottery_qsort_r_simple(void* first, size_t count, size_t element_size,
            int (*compare)(const void* left, const void* right, void* user_context),
            void* user_context)
{
    qsort_context_t context;
    context.element_size = element_size;
    context.compare.with_context = compare;
    context.user_context = user_context;
    qsort_run(context, first, count);
}

void pottery_qsort_simple(void* first, size_t count, size_t element_size,
            int (*compare)(const void* left, const void* right))
{
    qsort_context_t context;
    context.element_size = element_size;
    context.compare.without_context = compare;
    context.user_context = &qsort_compare_without_context;
    qsort_run(context, first, count);
}
