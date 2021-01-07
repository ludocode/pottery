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

#ifndef POTTERY_BENCHMARK_TEST_BENCHMARK_SORT_COMMON_H
#define POTTERY_BENCHMARK_TEST_BENCHMARK_SORT_COMMON_H

// find __GLIBC__
#if __has_include(<features.h>)
#include <features.h>
#endif

#include <stdint.h>

// int type to sort
typedef intptr_t benchmark_int_t;

// The rules for comparator inlining are:
//
// - qsort() implementations that take a function pointer comparator are not
// allowed to inline it. This matches their behaviour in the real world.
//
// - Template implementations that take a function (such as swenson/sort) are
// allowed to inline the comparator function because they take it as a
// compile-time template argument, not as a function pointer parameter. An
// inline definition is provided below for these templates.
//
// - Pottery's sort algorithms as well as all C++ templates use the default
// comparators. They don't take a comparator function at all.
//
// We declare here some non-inline comparison functions for use in the various
// qsort() wrappers and we don't build the benchmark suite with link-time
// optimization. This prevents the compiler from optimizing away the function
// pointer indirection.

// Three-way value comparator, inline for use in templates
static inline
int benchmark_int_compare_values(const benchmark_int_t a, const benchmark_int_t b) {

    // The ternary is faster than the alternative below, sometimes even faster
    // than the subtraction while being safe, as long as it gets inlined. This
    // is probably because when inlined the compiler can detect when algorithms
    // ignore the greater-than branch.
    return ((a < b) ? -1 : ((a > b) ? 1 : 0));

    // This is significantly slower despite optimizations, even when the
    // comparison function is inlined.
    //return (a > b) - (a < b);

    // This is an unsafe three-way comparison of ints. It gives wrong results
    // on large values. A safe comparison instead uses ordinary comparison
    // operators as above.
    //
    // Ironically, when the comparison function is inlined, the safe comparison is
    // marginally faster than the unsafe comparison (like swenson/sort quicksort).
    // However the unsafe comparison is faster on algorithms that cannot inline it
    // (like qsort() and pottery_qsort().)
    //
    // This does not affect algorithms that do not need to use three-way
    // comparisons and do not use this function (like Pottery templated sorts
    // and C++ sorts.)
    //return a - b;
}

// Non-inline qsort() comparators taking void pointers
int benchmark_int_compare(const void* left, const void* right);
int benchmark_int_compare_gnu_r(const void* left, const void* right, void* user_context);
int benchmark_int_compare_bsd_r(void* user_context, const void* left, const void* right);

#endif
