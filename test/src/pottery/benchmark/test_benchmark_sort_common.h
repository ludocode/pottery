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

#ifndef POTTERY_BENCHMARK_TEST_BENCHMARK_SORT_COMMON_H
#define POTTERY_BENCHMARK_TEST_BENCHMARK_SORT_COMMON_H

// find __GLIBC__
#if __has_include(<features.h>)
#include <features.h>
#endif

__attribute__((__always_inline__))
static inline int int_compare_values(const int a, const int b) {
    #if SAFE_THREE_WAY_COMPARE
    // The ternary is faster than the alternative below, sometimes even faster
    // than the subtraction while being safe, as long as it gets inlined. This
    // is probably because when inlined the compiler can detect when algorithms
    // ignore the greater-than branch.
    return ((a < b) ? -1 : ((a > b) ? 1 : 0));

    // This is significantly slower despite optimizations, even when the
    // comparison function is inlined.
    //return (a > b) - (a < b);
    #else
    return a - b;
    #endif
}

__attribute__((__unused__))
static int int_compare_pointers(const void* left, const void* right) {
    return int_compare_values(
            *(const int*)left,
            *(const int*)right);
}

__attribute__((__unused__))
static int int_compare_pointers_gnu_r(const void* left, const void* right, void* user_context) {
    (void)user_context;
    return int_compare_values(
            *(const int*)left,
            *(const int*)right);
}

__attribute__((__unused__))
static int int_compare_pointers_bsd_r(void* user_context, const void* left, const void* right) {
    (void)user_context;
    return int_compare_values(
            *(const int*)left,
            *(const int*)right);
}

#endif
