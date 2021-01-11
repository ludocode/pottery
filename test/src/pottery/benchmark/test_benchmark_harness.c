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

// Enable qsort_r() function if available. We check for __GLIBC__ below but we
// can't check it before headers are included.
#ifdef __linux__
    #define _GNU_SOURCE
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "pottery/benchmark/test_benchmark_sort_common.h"


// qsort wrappers
// These aren't allowed to inline the comparator so we define the wrappers
// here. The benchmark implementation files compile the standalone qsort()
// functions.

__attribute__((__unused__))
static void platform_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

#if defined(__GLIBC__) || defined(__UCLIBC__)
__attribute__((__unused__))
static void platform_gnu_qsort_r_wrapper(benchmark_int_t* ints, size_t count) {
    qsort_r(ints, count, sizeof(benchmark_int_t), benchmark_int_compare_gnu_r, NULL);
}
#endif

#if defined(__FreeBSD__) || defined(__APPLE__)
__attribute__((__unused__))
static void platform_bsd_qsort_r_wrapper(benchmark_int_t* ints, size_t count) {
    qsort_r(ints, count, sizeof(benchmark_int_t), NULL, benchmark_int_compare_bsd_r);
}
#endif

#include "pottery/qsort/pottery_qsort.h"
__attribute__((__unused__))
static void pottery_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}
__attribute__((__unused__))
static void pottery_gnu_qsort_r_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_gnu_qsort_r(ints, count, sizeof(benchmark_int_t), benchmark_int_compare_gnu_r, NULL);
}
__attribute__((__unused__))
static void pottery_bsd_qsort_r_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_bsd_qsort_r(ints, count, sizeof(benchmark_int_t), NULL, benchmark_int_compare_bsd_r);
}

#include "pottery/qsort_simple/pottery_simple_qsort.h"
__attribute__((__unused__))
static void pottery_simple_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_simple_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}
__attribute__((__unused__))
static void pottery_simple_gnu_qsort_r_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_simple_gnu_qsort_r(ints, count, sizeof(benchmark_int_t), benchmark_int_compare_gnu_r, NULL);
}
__attribute__((__unused__))
static void pottery_simple_bsd_qsort_r_wrapper(benchmark_int_t* ints, size_t count) {
    pottery_simple_bsd_qsort_r(ints, count, sizeof(benchmark_int_t), NULL, benchmark_int_compare_bsd_r);
}

__attribute__((__unused__))
static void openbsd_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void openbsd_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    openbsd_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void netbsd_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void netbsd_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    netbsd_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void uclibc_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void uclibc_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    uclibc_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void newlib_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void newlib_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    newlib_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void musl_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void musl_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    musl_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void wine_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void wine_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    wine_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void dragonflybsd_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void dragonflybsd_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    dragonflybsd_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void freebsd_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void freebsd_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    freebsd_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void glibc_qsort_wrapper(benchmark_int_t* ints, size_t count) {
    extern void glibc_qsort(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    glibc_qsort(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}

__attribute__((__unused__))
static void justinow_introsort_c_wrapper(benchmark_int_t* ints, size_t count) {
    extern void justinow_introsort_c(void *base, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *));
    justinow_introsort_c(ints, count, sizeof(benchmark_int_t), benchmark_int_compare);
}


// pottery
void pottery_benchmark_shell_sort_wrapper(benchmark_int_t* ints, size_t count);
void pottery_benchmark_quick_sort_wrapper(benchmark_int_t* ints, size_t count);
void pottery_benchmark_intro_sort_wrapper(benchmark_int_t* ints, size_t count);
void pottery_benchmark_heap_sort_wrapper(benchmark_int_t* ints, size_t count);

// c, c++
void std_sort_wrapper(benchmark_int_t* ints, size_t count);

// boost
#if __has_include(<boost/sort/sort.hpp>)
void boost_pdqsort_wrapper(benchmark_int_t* ints, size_t count);
void boost_spinsort_wrapper(benchmark_int_t* ints, size_t count);
void boost_flat_stable_sort_wrapper(benchmark_int_t* ints, size_t count);
#endif

// miscellaneous
void stb_sort_wrapper(benchmark_int_t* ints, size_t count);
void pqsort_wrapper(benchmark_int_t* ints, size_t count);
void swenson_timsort_wrapper(benchmark_int_t* ints, size_t count);
void swenson_quicksort_wrapper(benchmark_int_t* ints, size_t count);
void svpv_qsort_wrapper(benchmark_int_t* ints, size_t count);

typedef struct result_t {
    double duration;
    const char* name;
} result_t;

#define POTTERY_VECTOR_PREFIX results
#define POTTERY_VECTOR_VALUE_TYPE result_t
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"

#define POTTERY_INSERTION_SORT_PREFIX sort_results
#define POTTERY_INSERTION_SORT_VALUE_TYPE result_t
#define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_INSERTION_SORT_COMPARE_LESS(x, y) x->duration < y->duration
#include "pottery/insertion_sort/pottery_insertion_sort_static.t.h"

static results_t results;

pottery_noinline
static void benchmark_sort(benchmark_int_t* ref_ints, size_t count,
        void (*sort)(benchmark_int_t*, size_t), const char* name)
{
    benchmark_int_t* ints = pottery_cast(benchmark_int_t*, malloc(sizeof(benchmark_int_t) * count));
    memcpy(ints, ref_ints, sizeof(benchmark_int_t) * count);

    printf("Running %s\n", name);

    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    sort(ints, count);

    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    size_t i;
    for (i = 1; i < count; ++i) {
        if (ints[i] < ints[i - 1]) {
            printf("%s failed to sort correctly!\n", name);
            pottery_abort();
        }
    }

    double duration = (end_time.tv_sec - start_time.tv_sec) +
            (end_time.tv_nsec - start_time.tv_nsec) / 1.e9;
    results_insert_last(&results, pottery_compound_literal(result_t, duration, name));

    free(ints);
}

typedef enum variant_t {
    variant_mostly_sorted,
    variant_duplicates,
    variant_random,
} variant_t;

static void benchmark_sorts_variant(size_t count, variant_t variant) {
    results_init(&results);

    // generate some random ints with a fixed seed for reproducibility.
    srand(0);
    benchmark_int_t* ints = pottery_cast(benchmark_int_t*, malloc(sizeof(benchmark_int_t) * count));
    size_t i;
    for (i = 0; i < count; ++i) {

        // In the mostly_sorted variant, we only use a random number in range
        // a small percentage of the time, and the array index otherwise.
        if (variant == variant_mostly_sorted) {
            if (/*true||*/rand() % 100 != 0) {
                ints[i] = pottery_cast(benchmark_int_t, i);
            } else {
                // RAND_MAX is typically only around 65536 so we call it twice.
                // Use a random number less than count so that random numbers
                // are mixed throughout the range.
                ints[i] = pottery_cast(benchmark_int_t,
                        ((pottery_cast(uint32_t, rand()) << 16) ^
                            (pottery_cast(uint32_t, rand()))) % count);
            }
            continue;
        }

        // In the duplicates variant, we'll select randomly from a small number
        // of possible values.
        if (variant == variant_duplicates) {
            ints[i] = rand() % (count/1000);
            //ints[i]=4;
            continue;
        }

        // RAND_MAX is typically only around 65536 so we call it twice. This
        // means there will be very few or no duplicates.
        ints[i] = (rand() << 16) ^ rand();
    }

    if (variant == variant_mostly_sorted) {
        printf("\nSorting %zi ints which are already mostly sorted\n", count);
    } else if (variant == variant_duplicates) {
        printf("\nSorting %zi random ints with many duplicates\n", count);
    } else {
        printf("\nSorting %zi random ints\n", count);
    }

    // run benchmarks

    // pottery
    #if 1
    benchmark_sort(ints, count, pottery_benchmark_quick_sort_wrapper, "pottery_quick_sort");
    benchmark_sort(ints, count, pottery_benchmark_intro_sort_wrapper, "pottery_intro_sort");
    #endif
    #if 1
    benchmark_sort(ints, count, pottery_qsort_wrapper, "pottery_qsort()");
    #if 1
    benchmark_sort(ints, count, pottery_gnu_qsort_r_wrapper, "pottery_gnu_qsort_r()");
    benchmark_sort(ints, count, pottery_bsd_qsort_r_wrapper, "pottery_bsd_qsort_r()");
    benchmark_sort(ints, count, pottery_simple_qsort_wrapper, "pottery_simple_qsort()");
    benchmark_sort(ints, count, pottery_simple_gnu_qsort_r_wrapper, "pottery_simple_gnu_qsort_r()");
    benchmark_sort(ints, count, pottery_simple_bsd_qsort_r_wrapper, "pottery_simple_bsd_qsort_r()");
    #endif
    #endif
    #if 1
    benchmark_sort(ints, count, pottery_benchmark_shell_sort_wrapper, "pottery_shell_sort");
    benchmark_sort(ints, count, pottery_benchmark_heap_sort_wrapper, "pottery_heap_sort");
    #endif

    // platform
    #if 1
    benchmark_sort(ints, count, std_sort_wrapper, "platform std::sort");
    #endif
    #if 1
    benchmark_sort(ints, count, platform_qsort_wrapper, "platform qsort");
    #if defined(__GLIBC__) || defined(__UCLIBC__)
    benchmark_sort(ints, count, platform_gnu_qsort_r_wrapper, "platform qsort_r (GNU-style)");
    #endif
    #if defined(__FreeBSD__) || defined(__APPLE__)
    benchmark_sort(ints, count, platform_bsd_qsort_r_wrapper, "platform qsort_r (BSD-style)");
    #endif
    #endif

    // boost
    #if 1&&__has_include(<boost/sort/sort.hpp>)
    benchmark_sort(ints, count, boost_pdqsort_wrapper, "boost::pdqsort");
    benchmark_sort(ints, count, boost_spinsort_wrapper, "boost::spinsort");
    benchmark_sort(ints, count, boost_flat_stable_sort_wrapper, "boost::flat_stable_sort");
    #endif

    // various libc qsort()
    //
    // One of these probably matches the platform qsort() but it's likely to
    // have different performance because we're compiling with our own flags
    // plus some hacks to get them to compile standalone.
    #if 1
    benchmark_sort(ints, count, newlib_qsort_wrapper, "newlib qsort()");
    benchmark_sort(ints, count, musl_qsort_wrapper, "musl qsort()");
    benchmark_sort(ints, count, uclibc_qsort_wrapper, "uClibc qsort()");
    benchmark_sort(ints, count, wine_qsort_wrapper, "Wine qsort()");
    benchmark_sort(ints, count, glibc_qsort_wrapper, "glibc qsort()");
    benchmark_sort(ints, count, freebsd_qsort_wrapper, "FreeBSD qsort() (also macOS, Bionic)");
    benchmark_sort(ints, count, openbsd_qsort_wrapper, "OpenBSD qsort()");
    benchmark_sort(ints, count, netbsd_qsort_wrapper, "NetBSD qsort()");
    benchmark_sort(ints, count, dragonflybsd_qsort_wrapper, "DragonFlyBSD qsort() (also ReactOS)");
    #endif

    // miscellaneous
    #if 1
    benchmark_sort(ints, count, pqsort_wrapper, "Freaky/pqsort");
    benchmark_sort(ints, count, swenson_quicksort_wrapper, "swenson/sort quicksort");
    benchmark_sort(ints, count, swenson_timsort_wrapper, "swenson/sort timsort");
    benchmark_sort(ints, count, svpv_qsort_wrapper, "svpv/qsort");
    benchmark_sort(ints, count, stb_sort_wrapper, "nothings/stb quicksort");
    benchmark_sort(ints, count, justinow_introsort_c_wrapper, "justinow/introsort-c");
    #endif

    // sort results
    sort_results(results_begin(&results), results_count(&results));

    // print results
    for (size_t i = 0; i < results_count(&results); ++i) {
        result_t* result = results_at(&results, i);
        printf("% 12.6f seconds for %s\n", result->duration, result->name);
    }

    free(ints);
    results_destroy(&results);
}

static void benchmark_sorts(size_t count) {
    benchmark_sorts_variant(count, variant_mostly_sorted);
    benchmark_sorts_variant(count, variant_duplicates);
    benchmark_sorts_variant(count, variant_random);
}

static void benchmark_all_sorts(void) {
    size_t count = 100*1000;
    benchmark_sorts(count);
    count *= 10;
    benchmark_sorts(count);
    count *= 10;
    benchmark_sorts(count);
}


// all benchmarks

int main(void) {
    benchmark_all_sorts();

    // so far we only have a simple benchmark for sorting algorithms. :(
}
