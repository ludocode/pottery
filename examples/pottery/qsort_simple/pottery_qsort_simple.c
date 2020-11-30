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

typedef struct qsort_context_t {
    size_t element_size;
    int (*compare)(const void* left, const void* right);
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
    return context.compare(left, right);
}

static
void qsort_swap(qsort_context_t context, void* vleft, void* vright) {
    char* left = (char*)vleft;
    char* right = (char*)vright;
    size_t remaining = context.element_size;
    char buffer[128];

    while (remaining > sizeof(buffer)) {
        memcpy(buffer, left, sizeof(buffer));
        memcpy(left, right, sizeof(buffer));
        memcpy(right, buffer, sizeof(buffer));
        left += sizeof(buffer);
        right += sizeof(buffer);
        remaining -= sizeof(buffer);
    }

    memcpy(buffer, left, remaining);
    memcpy(left, right, remaining);
    memcpy(right, buffer, remaining);
}

#define POTTERY_INTRO_SORT_PREFIX qsort_run
#define POTTERY_INTRO_SORT_REF_TYPE void*
#define POTTERY_INTRO_SORT_CONTEXT_TYPE qsort_context_t
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_SELECT qsort_array_access_select
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_INDEX qsort_array_access_index
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY qsort_compare
#define POTTERY_INTRO_SORT_LIFECYCLE_SWAP qsort_swap
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

void pottery_qsort_simple(void* first, size_t count, size_t element_size,
            int (*compare)(const void* left, const void* right))
{
    qsort_context_t context = {element_size, compare};
    qsort_run(context, first, count);
}
