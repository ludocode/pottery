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

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Standard C qsort() plus some platform-specific extensions.

void pottery_qsort(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right));

void pottery_gnu_qsort_r(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right, void* user_context),
        void* user_context);

void pottery_bsd_qsort_r(void* first, size_t count, size_t element_size,
        void* user_context,
        int (*compare)(void* user_context, const void* left, const void* right));

void pottery_win_qsort_s(void* first, size_t count, size_t element_size,
        int (
            #if defined(_MSC_VER) || defined(__MINGW32__)
            __cdecl
            #endif
            *compare)(void* user_context, const void* left, const void* right),
        void* user_context);

#if defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)
// NOTE: This doesn't call the constraint handler on error because there is no
// way to get the current constraint handler. Instead it just aborts on error.
// This means when it returns, the return value is always 0.
errno_t pottery_c11_qsort_s(void* first, rsize_t count, rsize_t element_size,
        int (*compare)(const void* left, const void* right, void* user_context),
        void* user_context);
#endif

#ifdef __cplusplus
}
#endif
