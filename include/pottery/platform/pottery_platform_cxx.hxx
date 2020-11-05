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

#ifndef POTTERY_PLATFORM_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#ifndef __cplusplus
#error "This header should only be included in C++."
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

namespace pottery {

pottery_noreturn static pottery_always_inline
void terminate(void) {
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
void error_to_exception(pottery_error_t error) {
    if (pottery_likely(error == POTTERY_OK))
        return;

    #if POTTERY_CXX_EXCEPTIONS
    if (error == POTTERY_ERROR_ALLOC)
        throw std::bad_alloc();
    throw std::exception();
    #else
    pottery::terminate();
    #endif
}

/**
 * construct()
 *
 * This is used in C++ to run the constructor of an object in arbitrary
 * (properly typed) storage and convert exceptions to error codes.
 *
 * This can take arbitrary arguments in C++ so it can run default/copy/move
 * constructors or any user-defined constructors to allow C++-style
 * object emplacement in Pottery's dynamic containers.
 *
 * This catches exceptions and converts them to Pottery error codes. If
 * anything other than POTTERY_OK is returned, the object was not constructed.
 */
template<class Value, class... ConstructArgs>
pottery_nodiscard
pottery_always_inline static
pottery_error_t construct(Value* target,
        ConstructArgs&&... args) pottery_noexcept
{
    #if POTTERY_CXX_EXCEPTIONS
    try {
    #endif
        new (target) Value(std::forward<ConstructArgs>(args)...);
    #if POTTERY_CXX_EXCEPTIONS
    } catch (const std::bad_alloc&) {
        return POTTERY_ERROR_ALLOC;
    } catch (...) {
        return POTTERY_ERROR_CXX_EXCEPTION;
    }
    #endif
    return POTTERY_OK;
}

/**
 * assign()
 *
 * This is used in C++ to assign to an object (to run copy-assignment or
 * move-assignment operators) and convert exceptions to error codes.
 */
template<class Value, class AssignSource>
pottery_nodiscard
pottery_always_inline static
pottery_error_t assign(Value* target,
        AssignSource&& source) pottery_noexcept
{
    #if POTTERY_CXX_EXCEPTIONS
    try {
    #endif
        *target = std::forward<AssignSource>(source);
    #if POTTERY_CXX_EXCEPTIONS
    } catch (const std::bad_alloc&) {
        return POTTERY_ERROR_ALLOC;
    } catch (...) {
        return POTTERY_ERROR_CXX_EXCEPTION;
    }
    #endif
    return POTTERY_OK;
}

// workaround for GCC <5 not supporting std::is_trivially_copyable
#if defined(__GNUC__) && !defined(__clang__) && defined(__cplusplus)
    #if __GNUC__ < 5
        #define POTTERY_IS_BITWISE_COPYABLE_DEFINED
        template<typename T>
        struct is_bitwise_copyable : std::is_trivial<T> {};
    #endif
#endif

#ifndef POTTERY_IS_BITWISE_COPYABLE_DEFINED
/**
 * True if it is safe to use memcpy() to copy construct or copy assign a C++
 * object of type T.
 *
 * You can specialize this to enable bitwise copy for a non-trivial C++ type.
 */
template<typename T>
struct is_bitwise_copyable : std::is_trivially_copyable<T> {};
#endif

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
struct is_bitwise_movable : pottery::is_bitwise_copyable<T> {};

} // namespace pottery

// TODO backward compatibility
constexpr auto pottery_terminate = pottery::terminate;
constexpr auto pottery_error_to_exception = pottery::error_to_exception;

#endif
