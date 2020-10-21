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

#ifndef POTTERY_PLATFORM_FEATURES_H
#define POTTERY_PLATFORM_FEATURES_H 1



/*
 * Debug
 */

/*
 * POTTERY_DEBUG enables lightweight assertions in Pottery suitable for debug
 * builds of your application. It detects most debug builds automatically.
 *
 * If an assertion in Pottery is triggered, it is more likely that the bug is
 * in the code that uses Pottery rather than in Pottery itself. Some examples:
 *
 * - You may have attempted an out-of-bounds array access;
 *
 * - You may have attemped to dereference a non-existent entry;
 *
 * - You may have corrupted a map element by changing its key;
 *
 * - Your comparison function may be intransitive;
 *
 * - Your hash or comparison function may have changed its results based on
 *   some external context without rebuilding the data structure that uses it.
 *
 * If you are hitting an assertion and you are sure you are using Pottery
 * correctly, please file a bug!
 */
#if !defined(POTTERY_DEBUG)
    #if defined(DEBUG) || defined(_DEBUG)
        #define POTTERY_DEBUG 1
    #else
        #define POTTERY_DEBUG 0
    #endif
#endif



/*
 * Macro manipulation
 */

// Concatenates arguments.
#define POTTERY_CONCAT(prefix, name) POTTERY_CONCAT_IMPL(prefix, name)
#define POTTERY_CONCAT_IMPL(prefix, name) prefix##name

// Expands macro argument (e.g. allowing it to be used with token pasting or
// stringification.)
#define POTTERY_EXPAND(token) POTTERY_EXPAND_IMPL(token)
#define POTTERY_EXPAND_IMPL(token) token

// Stringifies macro argument.
#define POTTERY_STRINGIFY(token) POTTERY_STRINGIFY_IMPL(token)
#define POTTERY_STRINGIFY_IMPL(token) #token



/*
 * exceptions
 */

// exceptions require C++
#ifndef POTTERY_CXX_EXCEPTIONS
    #ifndef __cplusplus
        #define POTTERY_CXX_EXCEPTIONS 0
    #endif
#endif

// GNUC defines __cpp_exceptions to non-zero
#ifndef POTTERY_CXX_EXCEPTIONS
    #ifdef __cpp_exceptions
        #if __cpp_exceptions
            #define POTTERY_CXX_EXCEPTIONS 1
        #else
            #define POTTERY_CXX_EXCEPTIONS 0
        #endif
    #elif defined(__GNUC__)
        #define POTTERY_CXX_EXCEPTIONS 0
    #endif
#endif

// MSVC defines _CPPUNWIND to 1
#ifndef POTTERY_CXX_EXCEPTIONS
    #ifdef _CPPUNWIND
        #if _CPPUNWIND
            #define POTTERY_CXX_EXCEPTIONS 1
        #else
            #define POTTERY_CXX_EXCEPTIONS 0
        #endif
    #elif defined(_MSC_VER)
        #define POTTERY_CXX_EXCEPTIONS 0
    #endif
#endif

// if we haven't found a way to detect them, we've check for C++ above so we
// assume they're on.
#ifndef POTTERY_CXX_EXCEPTIONS
    #define POTTERY_CXX_EXCEPTIONS 1
#endif



// Forward declarations

// Pottery disables this when instantiating a C template into a C++ class
// template because C++ does not allow a prototype declaration of a class
// member function that is also defined inside the definition of the class.
#define POTTERY_FORWARD_DECLARATIONS 1



/*
 * Declare an unconstructed variable
 */

// This defines a temporary variable on the stack for which C++ constructors
// and destructors are not run. This allows us to do moves and swaps with our
// normal move functions.

// We don't have alignas in MSVC in C mode so we use an aligned buffer in C++
// only. In C we just use a normal variable.
#ifdef __cplusplus
    #define POTTERY_DECLARE_UNCONSTRUCTED(Type, name) \
        pottery_alignas(Type) char name##_unconstructed_buffer[sizeof(Type)]; \
        Type* name = pottery_bless(Type, name##_unconstructed_buffer);
#else
    #define POTTERY_DECLARE_UNCONSTRUCTED(Type, name) \
        Type name##_unconstructed_value; \
        Type* name = &name##_unconstructed_value;
#endif



/*
 * Error Codes
 *
 * See docs/features.md for a description of error handling.
 */

typedef int pottery_error_t;

#define POTTERY_OK 0
#define POTTERY_ERROR_ALLOC 1 // malloc() failed or std:bad_alloc was caught.
#define POTTERY_ERROR_OVERFLOW 2 // A math operation overflowed.
#define POTTERY_ERROR_FULL 3 // A size-limited container is full.
#define POTTERY_ERROR_CXX_EXCEPTION 4 // An unrecognized C++ exception was caught.



/*
 * Compound literal
 */

#ifdef __cplusplus
#define pottery_compound_literal(Type, ...) (Type{__VA_ARGS__})
#else
#define pottery_compound_literal(Type, ...) ((Type){__VA_ARGS__})
#endif



#endif
