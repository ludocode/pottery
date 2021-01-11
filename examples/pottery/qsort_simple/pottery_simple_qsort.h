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

#ifndef POTTERY_QSORT_SIMPLE_H
#define POTTERY_QSORT_SIMPLE_H 1

#include <stddef.h>

/**
 * Sorts an array with a custom comparator.
 *
 * This is the standard C qsort() function implemented with Pottery.
 */
void pottery_simple_qsort(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right));

/**
 * Sorts an array with a custom comparator that takes a user context, where the
 * context parameter comes last.
 *
 * This is GNU-style qsort_r() implemented with Pottery.
 */
void pottery_simple_gnu_qsort_r(void* first, size_t count, size_t element_size,
        int (*compare)(const void* left, const void* right, void* user_context),
        void* user_context);

/**
 * Sorts an array with a custom comparator that takes a user context, where the
 * context parameter comes first.
 *
 * This is BSD-style qsort_r() implemented with Pottery.
 */
void pottery_simple_bsd_qsort_r(void* first, size_t count, size_t element_size,
        void* user_context,
        int (*compare)(void* user_context, const void* left, const void* right));

#endif
