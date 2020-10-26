#include "pottery_qsort.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int strcmp_wrapper(const void* left, const void* right) {
//printf("compare %p %p %p %p\n",left,right, *(const char**)left, *(const char**)right);
    return strcmp(*(const char**)left, *(const char**)right);
}

static int strcmp_wrapper_first(void* context, const void* left, const void* right, void* user_context) {
    bool reverse = (bool)context;
    return (reverse ? -1 : 1) * strcmp(*(const char**)left, *(const char**)right);
}

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
//for (size_t i = 0; i < COUNT; ++i)printf("%zi %s %p\n", i, fruits.array[i], fruits.array[i]);
    pottery_qsort(fruits.array, COUNT, sizeof(*fruits.array), strcmp_wrapper);
//for (size_t i = 0; i < COUNT; ++i)puts(fruits.array[i]);
    test_in_order(false, fruits.array, COUNT);

    (void)strcmp_wrapper_first;
}
