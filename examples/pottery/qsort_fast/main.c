#include "pottery_qsort_fast.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * This tests sorting an array of strings with Pottery's implementations of
 * qsort(), GNU qsort_r() and BSD qsort_r().
 */


/*
 * qsort() callback functions
 */

static int strcmp_wrapper(const void* left, const void* right) {
    return strcmp(*(const char* const*)left, *(const char* const*)right);
}

static int strcmp_wrapper_gnu(const void* left, const void* right, void* context) {
    bool reverse = (bool)(intptr_t)context;
    return (reverse ? -1 : 1) * strcmp(*(const char* const*)left, *(const char* const*)right);
}

static int strcmp_wrapper_bsd(void* context, const void* left, const void* right) {
    return strcmp_wrapper_gnu(left, right, context);
}


/*
 * An array of strings to be sorted (wrapped in a struct so we can assign it to
 * reset it)
 */

typedef struct fruits_t {
    const char* array[8];
} fruits_t;

static fruits_t fruits_original = {{
    "clementine",
    "date",
    "grapefruit",
    "banana",
    "eggplant",
    "apple",
    "honeydew",
    "fig",
}};

static void test_in_order(bool reverse, const char** strings, size_t count) {
    size_t i;
    for (i = 1; i < count; ++i) {
        if (0 < (reverse ? -1 : 1) * strcmp(strings[i - 1], strings[i])) {
            fflush(stdout);
            fprintf(stderr, "Strings out of order!\n");
            abort();
        }
    }
}


int main(void) {

    size_t count = sizeof(fruits_original.array) / sizeof(*fruits_original.array);


    printf("Sorting with qsort()\n");
    fruits_t fruits = fruits_original;
    pottery_qsort_fast((void*)fruits.array, count, sizeof(*fruits.array), strcmp_wrapper);
    test_in_order(false, fruits.array, count);


    printf("Sorting reverse with gnu_qsort_r()\n");
    fruits = fruits_original;
    pottery_gnu_qsort_r_fast((void*)fruits.array, count, sizeof(*fruits.array),
            strcmp_wrapper_gnu, (void*)true);
    test_in_order(true, fruits.array, count);

    printf("Sorting forward with gnu_qsort_r()\n");
    pottery_gnu_qsort_r_fast((void*)fruits.array, count, sizeof(*fruits.array),
            strcmp_wrapper_gnu, (void*)false);
    test_in_order(false, fruits.array, count);


    printf("Sorting forward with bsd_qsort_r()\n");
    fruits = fruits_original;
    pottery_bsd_qsort_r_fast((void*)fruits.array, count, sizeof(*fruits.array),
            (void*)false, strcmp_wrapper_bsd);
    test_in_order(false, fruits.array, count);

    printf("Sorting reverse with bsd_qsort_r()\n");
    pottery_bsd_qsort_r_fast((void*)fruits.array, count, sizeof(*fruits.array),
            (void*)true, strcmp_wrapper_bsd);
    test_in_order(true, fruits.array, count);


    return EXIT_SUCCESS;
}
