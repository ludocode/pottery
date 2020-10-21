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
 * each example. The buildsystem also defines POTTERY_UNIT_TEST_FILENAME to
 * turn the filename into a test name.
 */


#define POTTERY_TEST_EXAMPLE

// We're going to replace printf() so we need to include stdio first.
#include <stdio.h>

#include "pottery/unit/test_pottery_framework.h"



// The examples don't follow the warnings as strictly as the rest of Pottery
// (otherwise the example code would be too ugly.)
#ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #ifdef __cplusplus
        #pragma GCC diagnostic ignored "-Wold-style-cast"
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
    POTTERY_EXAMPLE_TEST_UNUSED(void) {return 0;} \
    \
    static int POTTERY_EXAMPLE_TEST_WRAPPER(void); \
    \
    POTTERY_TEST(POTTERY_EXAMPLE_TEST_NAME) { \
        (void)POTTERY_EXAMPLE_TEST_UNUSED; \
        int ret = POTTERY_EXAMPLE_TEST_WRAPPER(); \
        pottery_assert(ret == 0); \
    } \
    \
    static int POTTERY_EXAMPLE_TEST_WRAPPER

#define POTTERY_EXAMPLE_TEST_NAME POTTERY_CONCAT(pottery, POTTERY_UNIT_TEST_FILENAME) // TODO this line is probably no longer necessary since we're moving into examples/pottery/ folder
#define POTTERY_EXAMPLE_TEST_WRAPPER POTTERY_CONCAT(POTTERY_EXAMPLE_TEST_NAME, impl) // TODO rename POTTERY_UNIT_TEST_FILENAME to POTTERY_EXAMPLE_NAME after above fix
#define POTTERY_EXAMPLE_TEST_UNUSED POTTERY_CONCAT(POTTERY_EXAMPLE_TEST_NAME, unused)



/*
 * The examples print lots of stuff. We don't want them to.
 */
static inline int pottery_example_disable_printf(const char *format, ...) {
    return strlen(format); // close enough
}
#define printf pottery_example_disable_printf



/*
 * We also want the examples to avoid normal assert and abort, instead calling
 * into Pottery so that the unit test harness can catch them.
 */
#define assert pottery_assert
#define abort pottery_abort



#endif
