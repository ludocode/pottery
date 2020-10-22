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

// c
void pottery_benchmark_shell_sort_wrapper(int* ints, size_t count);
void pottery_benchmark_quick_sort_wrapper(int* ints, size_t count);
void pottery_benchmark_intro_sort_wrapper(int* ints, size_t count);
void pottery_benchmark_heap_sort_wrapper(int* ints, size_t count);
void stb_sort_wrapper(int* ints, size_t count);
void qsort_wrapper(int* ints, size_t count);
void pottery_qsort_simple_wrapper(int* ints, size_t count);
void pqsort_wrapper(int* ints, size_t count);
void justinow_introsort_c_wrapper(int* ints, size_t count);
void swenson_timsort_wrapper(int* ints, size_t count);
void swenson_quicksort_wrapper(int* ints, size_t count);

// c++
void std_sort_wrapper(int* ints, size_t count);
#if __has_include(<boost/sort/sort.hpp>)
void boost_pdqsort_wrapper(int* ints, size_t count);
void boost_spinsort_wrapper(int* ints, size_t count);
void boost_flat_stable_sort_wrapper(int* ints, size_t count);
#endif

typedef struct result_t {
    double duration;
    const char* name;
} result_t;

#define POTTERY_VECTOR_PREFIX results
#define POTTERY_VECTOR_ELEMENT_TYPE result_t
#define POTTERY_VECTOR_ELEMENT_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"

#define POTTERY_INSERTION_SORT_PREFIX sort_results
#define POTTERY_INSERTION_SORT_VALUE_TYPE result_t
#define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_INSERTION_SORT_COMPARE_LESS(x, y) x->duration < y->duration
#include "pottery/insertion_sort/pottery_insertion_sort_static.t.h"

static results_t results;

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

static void benchmark_sorts() {
    results_init(&results);

    // generate some random ints with a fixed seed for reproducibility
    srand(0);
    size_t count = 10*1000*1000;
    int* ints = pottery_cast(int*, malloc(sizeof(int) * count));
    size_t i;
    for (i = 0; i < count; ++i) {
        #if 0
        // RAND_MAX is typically only around 65536, so use this to generate
        // tons of duplicates to see how well algorithms handle them.
        ints[i] = rand();
        #else
        // This generates mostly unique numbers.
        ints[i] = (rand() << 16) ^ rand();
        #endif
    }

    printf("\nSorting %zi random ints\n", count);

    // This one is buggy, skip it
    //benchmark_sort(ints, count, justinow_introsort_c_wrapper, "justinow/introsort-c");

    // run benchmarks
    #if 1&&__has_include(<boost/sort/sort.hpp>)
    benchmark_sort(ints, count, boost_pdqsort_wrapper, "boost::pdqsort");
    benchmark_sort(ints, count, boost_spinsort_wrapper, "boost::spinsort");
    benchmark_sort(ints, count, boost_flat_stable_sort_wrapper, "boost::flat_stable_sort");
    #endif
    benchmark_sort(ints, count, pottery_benchmark_quick_sort_wrapper, "pottery_quick_sort");
    benchmark_sort(ints, count, pottery_benchmark_intro_sort_wrapper, "pottery_intro_sort");
    benchmark_sort(ints, count, pottery_qsort_simple_wrapper, "pottery_qsort_simple");
    benchmark_sort(ints, count, stb_sort_wrapper, "nothings/stb quicksort");
    benchmark_sort(ints, count, qsort_wrapper, "qsort");
    benchmark_sort(ints, count, std_sort_wrapper, "std::sort");
    #if 1
    benchmark_sort(ints, count, pottery_benchmark_shell_sort_wrapper, "pottery_shell_sort");
    benchmark_sort(ints, count, pottery_benchmark_heap_sort_wrapper, "pottery_heap_sort");
    benchmark_sort(ints, count, pqsort_wrapper, "Freaky/pqsort");
    benchmark_sort(ints, count, swenson_quicksort_wrapper, "swenson/sort quicksort");
    benchmark_sort(ints, count, swenson_timsort_wrapper, "swenson/sort timsort");
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



// all benchmarks

int main(void) {
    benchmark_sorts();
}
