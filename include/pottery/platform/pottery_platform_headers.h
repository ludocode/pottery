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

#ifndef POTTERY_PLATFORM_HEADERS_H
#define POTTERY_PLATFORM_HEADERS_H 1

#ifndef POTTERY_PLATFORM_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

// In C++ we verify here that the dependencies were correctly included at file
// scope, outside of any namespace or extern "C" block. The below system
// includes cannot be included outside of file scope.
#ifdef __cplusplus
static inline bool POTTERY_YOU_MUST_INCLUDE_DEPENDENCIES_OUTSIDE_OF_NAMESPACES_OR_EXTERN_C(void) {
    return true;
}
// No wrapping here so that the comment ends up in compiler error output,
// whether it's the template error due to extern "C" or the undeclared function
// error due to inner scope.
template <class T> static inline bool POTTERY_SCOPE_ASSERTION(void) { return ::POTTERY_YOU_MUST_INCLUDE_DEPENDENCIES_OUTSIDE_OF_NAMESPACES_OR_EXTERN_C(); /* IMPORTANT: This compiler error means Pottery's dependencies were not included at file scope. You must #include "pottery/pottery_dependencies.h" outside of any class or namespace and outside of any extern "C" block before including pottery template headers within a class, namespace, or extern "C" block. */ }
#endif

// We disable some of the below headers to make manually preprocessed code
// legible.
#ifndef POTTERY_GENERATE_CODE

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#if defined(__unix__) || defined(__APPLE__)
// This will help us detect POSIX support
#include <unistd.h>
#endif

#endif

#endif
