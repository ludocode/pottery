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

#include "test_pottery_framework.h"

#include <time.h>

#ifdef __GLIBC__
#include <gnu/libc-version.h>
#endif

static pottery_test_case_t* test_case_first = NULL;

#ifndef POTTERY_CUSTOM_TEST
static void seed_random(void) {
    unsigned int seed = pottery_cast(unsigned int, time(NULL));
    printf("Random seed: %u", seed);
    #ifdef __GLIBC__
    printf(" for glibc %s", gnu_get_libc_version());
    #endif
    putchar('\n');
    srand(seed);
}

int main(void) {
    seed_random();

    #ifdef POTTERY_MANUAL_UNIT_TEST_REGISTRATION
    // This function must be defined externally by a script that has parsed
    // registration functions out of object files.
    pottery_register_unit_tests();
    #endif

    int count = 0;
    printf("Running unit tests\n");

    pottery_test_case_t* test_case = test_case_first;
    for (; test_case != NULL; test_case = test_case->next) {
        ++count;

        // print the test being run (and flush in case it crashes)
        printf("Running %s   ", test_case->name);
        fflush(stdout);

        // run the test case
        test_case->function();

        // erase the name of the test case being run
        size_t length = strlen("Running ") + strlen(test_case->name) + strlen("   ");
        putchar('\r');
        size_t i;
        for (i = 0; i < length; ++i)
            putchar(' ');
        putchar('\r');
    }

    printf("Done. %i tests passed.\n", count);
    return EXIT_SUCCESS;
}
#endif

void pottery_unit_test_register(pottery_test_case_t* test_case) {

    // this is a safety check against duplicate registration. this can happen
    // due to incorrect parsing of register functions when doing a non-clean
    // build with tcc
    if (test_case->registered) {
        printf("Duplicate registration! %s\n", test_case->name);
        abort();
    }

    test_case->next = test_case_first;
    test_case_first = test_case;
}
