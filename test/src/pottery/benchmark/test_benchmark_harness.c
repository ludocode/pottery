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

#include <time.h>
#include <stdio.h>

#include "pottery/benchmark/test_benchmark_sort_common.h"

// pottery
void pottery_benchmark_shell_sort_wrapper(int* ints, size_t count);
void pottery_benchmark_quick_sort_wrapper(int* ints, size_t count);
void pottery_benchmark_intro_sort_wrapper(int* ints, size_t count);
void pottery_benchmark_heap_sort_wrapper(int* ints, size_t count);
void pottery_qsort_simple_wrapper(int* ints, size_t count);
void pottery_qsort_r_simple_wrapper(int* ints, size_t count);
void pottery_qsort_wrapper(int* ints, size_t count);
void pottery_gnu_qsort_r_wrapper(int* ints, size_t count);

// c, c++
void std_sort_wrapper(int* ints, size_t count);
void qsort_wrapper(int* ints, size_t count);
#if defined(__GLIBC__) || defined(__UCLIBC__)
void gnu_qsort_r_wrapper(int* ints, size_t count);
#endif
#if defined(__FreeBSD__) || defined(__APPLE__)
void bsd_qsort_r_wrapper(int* ints, size_t count);
#endif

// boost
#if __has_include(<boost/sort/sort.hpp>)
void boost_pdqsort_wrapper(int* ints, size_t count);
void boost_spinsort_wrapper(int* ints, size_t count);
void boost_flat_stable_sort_wrapper(int* ints, size_t count);
#endif

// miscellaneous
void musl_qsort_wrapper(int* ints, size_t count);
void glibc_qsort_wrapper(int* ints, size_t count);
void freebsd_qsort_wrapper(int* ints, size_t count);
void stb_sort_wrapper(int* ints, size_t count);
void pqsort_wrapper(int* ints, size_t count);
void swenson_timsort_wrapper(int* ints, size_t count);
void swenson_quicksort_wrapper(int* ints, size_t count);
void svpv_qsort_wrapper(int* ints, size_t count);
//void justinow_introsort_c_wrapper(int* ints, size_t count);

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
static void benchmark_sort(int* ref_ints, size_t count,
        void (*sort)(int*, size_t), const char* name)
{
    int* ints = pottery_cast(int*, malloc(sizeof(int) * count));
    memcpy(ints, ref_ints, sizeof(int) * count);

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
    int* ints = pottery_cast(int*, malloc(sizeof(int) * count));
    size_t i;
    for (i = 0; i < count; ++i) {

        // In the mostly_sorted variant, we only use a random number 10% of
        // the time, and the array index otherwise.
        if (variant == variant_mostly_sorted) {
            if (rand() % 10 != 0) {
                ints[i] = pottery_cast(int, i);
            } else {
                // RAND_MAX is typically only around 65536 so we call it twice.
                // Use a random number less than count so that random numbers
                // are mixed throughout the range.
                ints[i] = pottery_cast(int,
                        pottery_cast(uint32_t, (rand() << 16) ^ rand()) % count);
            }
            continue;
        }

        // In the duplicates variant, we'll select randomly from 100 numbers.
        if (variant == variant_duplicates) {
            ints[i] = rand() % 100;
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

    // This one is buggy, skip it
    //benchmark_sort(ints, count, justinow_introsort_c_wrapper, "justinow/introsort-c");

    // run benchmarks

    // pottery
    benchmark_sort(ints, count, pottery_benchmark_quick_sort_wrapper, "pottery_quick_sort");
    benchmark_sort(ints, count, pottery_benchmark_intro_sort_wrapper, "pottery_intro_sort");
    benchmark_sort(ints, count, pottery_qsort_wrapper, "pottery_qsort");
    benchmark_sort(ints, count, pottery_qsort_simple_wrapper, "pottery_qsort_simple");
    benchmark_sort(ints, count, pottery_qsort_r_simple_wrapper, "pottery_qsort_r_simple");
    benchmark_sort(ints, count, pottery_gnu_qsort_r_wrapper, "pottery_gnu_qsort_r");
    #if 1
    benchmark_sort(ints, count, pottery_benchmark_shell_sort_wrapper, "pottery_shell_sort");
    benchmark_sort(ints, count, pottery_benchmark_heap_sort_wrapper, "pottery_heap_sort");
    #endif

    // platform
    benchmark_sort(ints, count, std_sort_wrapper, "platform std::sort");
    benchmark_sort(ints, count, qsort_wrapper, "platform qsort");
    #if defined(__GLIBC__) || defined(__UCLIBC__)
    benchmark_sort(ints, count, gnu_qsort_r_wrapper, "platform qsort_r (GNU-style)");
    #endif
    #if defined(__FreeBSD__) || defined(__APPLE__)
    benchmark_sort(ints, count, bsd_qsort_r_wrapper, "platform qsort_r (BSD-style)");
    #endif

    // boost
    #if 1&&__has_include(<boost/sort/sort.hpp>)
    benchmark_sort(ints, count, boost_pdqsort_wrapper, "boost::pdqsort");
    benchmark_sort(ints, count, boost_spinsort_wrapper, "boost::spinsort");
    benchmark_sort(ints, count, boost_flat_stable_sort_wrapper, "boost::flat_stable_sort");
    #endif

    // various libc qsort()
    // One of these probably matches the platform qsort() but it's likely to
    // have different performance because we're compiling with our own flags
    // plus some hacks to get them to compile standalone.
    #if 1
    benchmark_sort(ints, count, musl_qsort_wrapper, "musl qsort");
    benchmark_sort(ints, count, glibc_qsort_wrapper, "glibc qsort");
    benchmark_sort(ints, count, freebsd_qsort_wrapper, "FreeBSD qsort (macOS, Bionic)");
    #endif

    // miscellaneous
    #if 1
    benchmark_sort(ints, count, pqsort_wrapper, "Freaky/pqsort");
    benchmark_sort(ints, count, swenson_quicksort_wrapper, "swenson/sort quicksort");
    benchmark_sort(ints, count, swenson_timsort_wrapper, "swenson/sort timsort");
    benchmark_sort(ints, count, svpv_qsort_wrapper, "svpv/qsort");
    benchmark_sort(ints, count, stb_sort_wrapper, "nothings/stb quicksort");
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
