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

#ifndef POTTERY_PLATFORM_CXX_HXX
#define POTTERY_PLATFORM_CXX_HXX

#ifndef __cplusplus
#error
#endif

#ifndef POTTERY_GENERATE_CODE
// std::move
#include <utility>

// std::swap (may be here or in utility above)
#include <algorithm>

// std::bad_alloc
#include <new>

// std::is_trivially_copyable
#include <type_traits>
#endif

pottery_noreturn static pottery_always_inline
void pottery_terminate(void) {
    #if POTTERY_CXX_EXCEPTIONS
    std::terminate();
    #else
    // We call pottery_abort() instead of std::terminate() because the latter
    // requires linking the C++ STL. Pottery should work without it. We use
    // std::terminate() if we're using exceptions because they require the STL.
    pottery_abort();
    #endif
}

static pottery_always_inline
void pottery_error_to_exception(pottery_error_t error) {
    if (error == POTTERY_OK)
        return;

    #if POTTERY_CXX_EXCEPTIONS
    if (error == POTTERY_ERROR_ALLOC)
        throw std::bad_alloc();
    throw std::exception();
    #else
    pottery_terminate();
    #endif
}

namespace pottery {

/**
 * True if it is safe to use memcpy() to move (or "relocate" in C++ terms) a
 * C++ object of type T, without running the constructor of the destination
 * object and without running the destructor of the source object.
 *
 * You can specialize this to enable bitwise move for a non-trivial C++ type.
 *
 * If this is enabled, swap will be done bitwise as well, so a custom swap()
 * function will not be called. If this is disabled, Pottery will move by move
 * constructing the destination following by destructing the source.
 *
 * Another name for this property is "trivially relocatable". This could use
 * std::is_trivially_relocatable<> if it ever gets added to C++. For now we
 * enable this by default if the type is trivially copyable.
 *
 *     https://quuxplusone.github.io/blog/2018/10/04/trivially-relocatable-faq/
 *     http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1144r0.html
 */
template<typename T>
struct is_bitwise_movable : std::is_trivially_copyable<T> {};

/**
 * True if it is safe to use memcpy() to copy construct or copy assign a C++
 * object of type T.
 *
 * You can specialize this to enable bitwise copy for a non-trivial C++ type.
 */
template<typename T>
struct is_bitwise_copyable : std::is_trivially_copyable<T> {};

} // namespace pottery

#endif
