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

#ifndef POTTERY_TEST_POTTERY_UNIT_TEST_FRAMEWORK_H
#define POTTERY_TEST_POTTERY_UNIT_TEST_FRAMEWORK_H 1

#include "pottery/pottery_dependencies.h"

#include <stdio.h>

/*
 * This is a minimal unit testing framework. I couldn't find an existing one
 * that would do what I need. Here are Pottery's requirements:
 *
 * - Supports TinyCC (so no __attribute__((constructor)))
 * - Supports MSVC in C mode (so no C++ static constructors)
 * - Auto-registers tests, despite the above two limitations
 * - Supports gnu89, C11 and C++11 (so no designated initializers among other things)
 * - Runs correctly and quickly under Valgrind (so probably no forking)
 * - Runs correctly with Address Sanitizer and other sanitizers
 * - Supports failure tests (at least without leaking memory)
 * - Can run specific tests by command-line option
 * - Can turn the main() functions in example code into unit tests
 * - Can be overridden by another framework to include tests externally
 *
 * Here are some non-requirements:
 *
 * - Gathering useless failure statistics. This framework stops immediately on
 *   the first error.
 *
 * - Forking. Since we're stopping immediately on error, it's not critical to
 *   run each test in its own sandbox; we just have to override memory
 *   allocation functions to clean up memory when we longjmp out of failure
 *   tests.
 *
 * This will probably eventually be moved out into a separate library.
 * (Actually it already was, but I moved some of this into Pottery
 * because my unit test framework is not ready to be released yet.)
 */

#ifdef __cplusplus
extern "C" {
#endif



static inline void* pottery_test_malloc(size_t size) {
    return malloc(size);
}

static inline void pottery_test_free(void* p) {
    free(p);
}

static inline char* pottery_test_strdup(const char* str) {
    size_t size = strlen(str) + 1;
    char* ret = pottery_cast(char*, pottery_test_malloc(size));
    if (ret)
        memcpy(ret, str, size);
    return ret;
}



/*
 * POTTERY_CUSTOM_TEST overrides our unit test framework with another.
 *
 * e.g. To make Pottery use Google Test (say you want to include Pottery's
 * tests in your library or app's unit test suite, or just run it in a more
 * featureful runner), build Pottery's tests (as C++) with command-line
 * options:
 *
 *     -DPOTTERY_CUSTOM_TEST_HEADER=gtest/gtest.h
 *     -DPOTTERY_CUSTOM_TEST(x)=TEST(Pottery,x)
 *
 * (and observe the 50x slowdown in compilation speed. add it to the pile of
 * reasons why not to use C++)
 */
#ifdef POTTERY_CUSTOM_TEST
#include POTTERY_STRINGIFY(POTTERY_CUSTOM_TEST_HEADER)
#endif



/*
 * test assert
 */
#define pottery_test_assert(x) (!(x) ? ( \
        fflush(stdout), \
        fprintf(stderr, "\n\nTest failed! %s:%i : %s\n\n", __FILE__, __LINE__, #x), \
        pottery_abort()) : ((void)0))



#ifdef __cplusplus
#define POTTERY_EXTERN_C extern "C"
#else
#define POTTERY_EXTERN_C /*nothing*/
#endif

typedef struct pottery_test_case_t {
    const char* name;
    void (*function)(void);
    struct pottery_test_case_t* next;
    bool registered;
} pottery_test_case_t;

POTTERY_EXTERN_C
void pottery_unit_test_register(pottery_test_case_t* test_case);

#define POTTERY_TEST_CASE_IMPL(name) \
    \
    static void pottery_unit_test_##name(void); \
    \
    static pottery_test_case_t pottery_unit_test_case_##name = { \
        #name, \
        &pottery_unit_test_##name, \
        pottery_null, \
        false, \
    };



/*
 * With some compilers (e.g. TinyCC) we cannot register the test automatically.
 * In this case we create a registration function but we give it external
 * linkage.
 *
 * A separate script parses the compiled object files, pulls out the
 * registration functions, and calls them in a special registration function
 * that gets called on startup.
 */

#if defined(POTTERY_MANUAL_UNIT_TEST_REGISTRATION)

void pottery_register_unit_tests(void);

#define POTTERY_TEST_IMPL(name) \
    POTTERY_TEST_CASE_IMPL(name) \
    \
    POTTERY_EXTERN_C void pottery_unit_test_register_##name(void);  \
    \
    POTTERY_EXTERN_C void pottery_unit_test_register_##name(void) { \
        pottery_unit_test_register(&pottery_unit_test_case_##name); \
    } \
    \
    static void pottery_unit_test_##name(void)



/*
 * In C++ we use the constructor of an object in static storage duration to
 * register the test.
 */

#elif defined(__cplusplus)

#define POTTERY_TEST_IMPL(name) \
    POTTERY_TEST_CASE_IMPL(name) \
    \
    static struct pottery_unit_test_class_##name { \
        pottery_unit_test_class_##name() { \
            pottery_unit_test_register(&pottery_unit_test_case_##name); \
        } \
    } pottery_unit_test_instance_##name; \
    \
    static void pottery_unit_test_##name(void)



/*
 * With GCC and friends we use __attribute__((__constructor__)) to register the
 * test.
 */

#elif defined(__GNUC__)

// workaround for a cparser bug, it ignores constructor on static functions.
//     https://github.com/libfirm/cparser/issues/54
#ifdef __CPARSER__
#define POTTERY_TEST_CONSTRUCTOR_STATIC /*nothing*/
#else
#define POTTERY_TEST_CONSTRUCTOR_STATIC static
#endif

#define POTTERY_TEST_IMPL(name) \
    POTTERY_TEST_CASE_IMPL(name) \
    \
    __attribute__((__constructor__)) POTTERY_TEST_CONSTRUCTOR_STATIC \
    void pottery_unit_test_register_##name(void) { \
        pottery_unit_test_register(&pottery_unit_test_case_##name); \
    } \
    \
    static void pottery_unit_test_##name(void)



/*
 * With MSVC in C mode we place a pointer to the function in the user
 * initializer section to get it called during static initialization, and we
 * use a linker pragma to prevent the function from being stripped by
 * whole-program optimization.
 *
 * https://stackoverflow.com/a/2390626
 */

#elif defined(_MSC_VER)
#pragma section(".CRT$XCU",read)

#ifdef _WIN64
#define POTTERY_UNIT_TEST_CRTXCU_PREFIX ""
#else
#define POTTERY_UNIT_TEST_CRTXCU_PREFIX "_"
#endif

#define POTTERY_TEST_IMPL(name) \
    POTTERY_TEST_CASE_IMPL(name) \
    \
    static void pottery_unit_test_register_##name(void) { \
        pottery_unit_test_register(&pottery_unit_test_case_##name); \
    } \
    \
    __declspec(allocate(".CRT$XCU")) \
            void (*pottery_unit_test_crtxcu_##name)(void) = \
                pottery_unit_test_register_##name; \
    \
    __pragma(comment(linker, "/include:" POTTERY_UNIT_TEST_CRTXCU_PREFIX \
               "pottery_unit_test_crtxcu_" #name)) \
    \
    static void pottery_unit_test_##name(void)



/*
 * Otherwise we have no way to automatically register unit tests.
 */

#else
#error "No unit test registration mechanism configured!"

#endif



#ifndef POTTERY_CUSTOM_TEST
/*
 * Defines a test case.
 *
 * If possible, the test case is registered automatically by declaring a helper
 * function as a constructor (e.g. `__attribute__((constructor))`). This causes
 * it to be run automatically before main().
 *
 * If the test case cannot be registered automatically (e.g. when compiling
 * under TinyCC), a script can pull the registration functions out of the
 * compiled object files and generate a function to call them.
 */
#define POTTERY_TEST(name) POTTERY_TEST_IMPL(name)
#else
#define POTTERY_TEST POTTERY_CUSTOM_TEST
#endif



// While unit testing, assert() is disabled in both Pottery code and unit tests
#if defined(__GNUC__) && !defined(POTTERY_TEST_EXAMPLE)
#undef assert
#pragma GCC poison assert
#endif


#ifdef __cplusplus
}
#endif

#endif
