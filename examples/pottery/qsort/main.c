#include "pottery_qsort.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int strcmp_wrapper(const void* left, const void* right) {
//printf("compare %p %p %p %p\n",left,right, *(const char**)left, *(const char**)right);
    return strcmp(*(const char* const*)left, *(const char* const*)right);
}

static int strcmp_wrapper_gnu(const void* left, const void* right, void* context) {
    bool reverse = (bool)(intptr_t)context;
    return (reverse ? -1 : 1) * strcmp(*(const char* const*)left, *(const char* const*)right);
}

static int strcmp_wrapper_bsd(void* context, const void* left, const void* right) {
    return strcmp_wrapper_gnu(left, right, context);
}

static void test_in_order(bool reverse, const char** strings, size_t count) {
    size_t i;
    for (i = 1; i < count; ++i) {
        //printf("comparing %s to %s\n",strings[i - 1], strings[i]);
        if (0 < (reverse ? -1 : 1) * strcmp(strings[i - 1], strings[i])) {
            fflush(stdout);
            fprintf(stderr, "Strings out of order!\n");
            abort();
        }
    }
}

#define COUNT 5
typedef struct fruits_t {
    const char* array[COUNT];
} fruits_t;

int main(void) {
    fruits_t fruits_original = {{
        "carrot",
        "date",
        "banana",
        "eggplant",
        "apple",
    }};

    printf("Sorting with qsort()\n");
    fruits_t fruits = fruits_original;
//for (size_t i = 0; i < COUNT; ++i)printf("%zi %s %p\n", i, fruits.array[i], (const void*)fruits.array[i]);
    pottery_qsort((void*)fruits.array, COUNT, sizeof(*fruits.array), strcmp_wrapper);
//for (size_t i = 0; i < COUNT; ++i)puts(fruits.array[i]);
    test_in_order(false, fruits.array, COUNT);

    // TODO test these as well
    (void)&strcmp_wrapper_gnu;
    (void)&strcmp_wrapper_bsd;
    return EXIT_SUCCESS;
}
