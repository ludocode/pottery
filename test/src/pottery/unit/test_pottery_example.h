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

#ifndef POTTERY_TEST_POTTERY_EXAMPLE_H
#define POTTERY_TEST_POTTERY_EXAMPLE_H 1

/*
 * This header converts a Pottery example's main() function into a unit test
 * declared with POTTERY_TEST().
 *
 * This header is force-included by the unit test buildsystem when compiling
 * each example. The buildsystem also defines POTTERY_EXAMPLE_NAME to
 * turn the filename into a test name.
 */



#define POTTERY_TEST_EXAMPLE

// We want strdup() and other POSIX/GNU extensions available in example code
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

// We want strcpy() and other standard functions available under MSVC in example code
//#define _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER
#include <string.h>
static inline char* strcpy_workaround(char* to, const char* from) {
    #pragma warning(push)
    #pragma warning(disable:4996)
    return strcpy(to, from);
    #pragma warning(pop)
}
#define strcpy strcpy_workaround
#endif

// We're going to replace printf() so we need to include stdio first.
#include <stdio.h>

#include "pottery/unit/test_pottery_framework.h"



// For examples that use <assert.h> we want to keep assertions enabled.
#undef NDEBUG



// The examples don't follow the warnings as strictly as the rest of Pottery
// (otherwise the example code would be too ugly.)
#ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #ifdef __cplusplus
        #pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
        #pragma GCC diagnostic ignored "-Wold-style-cast"
    #else
        #pragma GCC diagnostic ignored "-Wmissing-prototypes"
    #endif
#endif



/*
 * We need to replace `int main(void)` with POTTERY_TEST(x)` where x is a
 * generated test name based on the path.
 *
 * The first bit gets rid of the `int` by attaching it to another unused
 * function.
 *
 * The rest handles the `(void)` by attaching it to a wrapped function
 * that we forward-declare. (This means examples cannot currently take
 * command-line arguments.)
 */
#define main \
    POTTERY_EXAMPLE_TEST_UNUSED(void); \
    int POTTERY_EXAMPLE_TEST_UNUSED(void) {return 0;} \
    \
    static int POTTERY_EXAMPLE_TEST_WRAPPER(void); \
    \
    POTTERY_TEST(POTTERY_EXAMPLE_NAME) { \
        (void)&POTTERY_EXAMPLE_TEST_UNUSED; \
        int ret = POTTERY_EXAMPLE_TEST_WRAPPER(); \
        pottery_assert(ret == 0); \
    } \
    \
    static int POTTERY_EXAMPLE_TEST_WRAPPER

#define POTTERY_EXAMPLE_TEST_WRAPPER POTTERY_CONCAT(POTTERY_EXAMPLE_NAME, _impl)
#define POTTERY_EXAMPLE_TEST_UNUSED POTTERY_CONCAT(POTTERY_EXAMPLE_NAME, _unused)



/*
 * The examples print lots of stuff. We don't want them to.
 */

#if 1
#ifdef __CLR_VER
// C++/CLR doesn't support varargs functions but we can do this hack instead.
#define printf(...) ( __VA_ARGS__ , (void)0)
#else
static inline int pottery_example_disable_printf(const char *format, ...) {
    return (int)strlen(format); // close enough
}
#define printf pottery_example_disable_printf
#endif

static inline int pottery_example_disable_puts(const char* s) {
    (void)s;
    return 1;
}
#define puts pottery_example_disable_puts
#endif



/*
 * We also want the examples to avoid normal assert and abort, instead calling
 * into Pottery so that the unit test harness can catch them.
 */
#undef assert
#define assert pottery_assert
#undef abort
#define abort pottery_abort



/*
 * MSVC workarounds
 */
#ifdef _MSC_VER
#define strdup _strdup
#endif



#endif
