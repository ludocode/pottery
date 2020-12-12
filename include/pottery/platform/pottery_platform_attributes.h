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

#ifndef POTTERY_PLATFORM_ATTRIBUTES_H
#define POTTERY_PLATFORM_ATTRIBUTES_H 1

#ifndef POTTERY_PLATFORM_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif



/*
 * Attributes, and other function-like macro stuff like casts, etc.
 *
 * For attributes, we prefer the new [[attribute]] syntax where available. This
 * syntax is significantly more strict than __attribute__(()); for example on
 * functions, it must come before linkage specifiers like static and inline.
 * It's important to run the unit tests as C++17 to make sure we're placing
 * them all correctly.
 */



// noexcept

#ifdef __cplusplus
    #define pottery_noexcept noexcept
#else
    #define pottery_noexcept /*nothing*/
#endif



// restrict

#ifndef pottery_restrict
    #ifdef __STDC_VERSION__
        #if __STDC_VERSION__ >= 201112L
            #define pottery_restrict restrict
        #endif
    #endif
#endif

#ifndef pottery_restrict
    #if defined(_MSC_VER)
        #define pottery_restrict __restrict
    #elif defined(__GNUC__)
        // https://gcc.gnu.org/onlinedocs/gcc/Restricted-Pointers.html
        #define pottery_restrict __restrict__
    #endif
#endif

#ifndef pottery_restrict
    #define pottery_restrict /*nothing*/
#endif



// likely/unlikely

#ifdef __GNUC__
    #define pottery_likely(x) (__builtin_expect(!!(x), 1))
    #define pottery_unlikely(x) (__builtin_expect(!!(x), 0))
#else
    #define pottery_likely(x) (x)
    #define pottery_unlikely(x) (x)
#endif



// always_inline

#if defined(_MSC_VER)
    #define pottery_always_inline __forceinline
#elif defined(__GNUC__)
    // We need inline also to avoid warning:
    // "always_inline function might not be inlinable [-Werror=attributes]"
    #define pottery_always_inline inline __attribute__((__always_inline__))
#else
    #define pottery_always_inline inline
#endif



// noinline

#if defined(_MSC_VER)
    #define pottery_noinline __declspec(noinline)
#elif defined(__GNUC__)
    #define pottery_noinline __attribute__((__noinline__))
#else
    #define pottery_noinline /*nothing*/
#endif



// nodiscard

#if !defined(pottery_nodiscard) && !defined(nodiscard)
    #ifdef __cplusplus
        #if __cplusplus >= 201703L
            #define pottery_nodiscard [[nodiscard]]
        #endif
    #endif
#endif

#ifndef pottery_nodiscard
    #if defined(_MSC_VER)
        #define pottery_nodiscard _Check_return_
    #endif
#endif

#ifndef pottery_nodiscard
    #if defined(__GNUC__)
        #define pottery_nodiscard __attribute__((__warn_unused_result__))
    #endif
#endif

#ifndef pottery_nodiscard
    #define pottery_nodiscard /* nothing */
#endif



// maybe_unused

#if !defined(pottery_maybe_unused) && !defined(maybe_unused)
    #if defined(__cplusplus)
        #if __cplusplus >= 201703L
            #define pottery_maybe_unused [[maybe_unused]]
        #endif
    #endif
#endif

#ifndef pottery_maybe_unused
    #if defined(__GNUC__)
        #define pottery_maybe_unused __attribute__((__unused__))
    #endif
#endif

#ifndef pottery_maybe_unused
    #define pottery_maybe_unused /* nothing */
#endif



/*
 * Alignment
 */

/*
 * C11 and C++11 allow alignas to take either a numerical expression or a type.
 * We separate these two concepts for better portability. We therefore
 * define the following macros:
 *
 * - pottery_alignof(T) -- the minimum necessary alignment of the given type
 * - pottery_align(n)   -- aligns a declaration to the given numerical expression
 * - pottery_alignas(T) -- aligns a declaration as the given type
 * - pottery_alignmax   -- the maximum alignment needed for any scalar type
 *
 * Alignment keywords are not available in gnu89 or C99 but Pottery will still
 * work as long as we have compiler instrinsics for them.
 */

// C11 _Alignof, _Alignas
#ifdef __STDC_VERSION__
    #if __STDC_VERSION__ >= 201112L
        #ifndef pottery_alignof
            // https://en.cppreference.com/w/c/language/_Alignof
            #define pottery_alignof(T) _Alignof(T)
        #endif
        #ifndef pottery_align
            // https://en.cppreference.com/w/c/language/_Alignas
            #define pottery_align(n) _Alignas(n)
        #endif
        #ifndef pottery_alignas
            #define pottery_alignas(T) _Alignas(T)
        #endif
        #ifndef pottery_alignmax
            // https://en.cppreference.com/w/c/types/max_align_t
            #define pottery_alignmax _Alignof(max_align_t)
        #endif
    #endif
#endif

// C++11 alignof, alignas
#ifdef __cplusplus
    #if __cplusplus >= 201103L || defined(_MSC_VER)
        #ifndef pottery_alignof
            // https://en.cppreference.com/w/cpp/language/alignof
            #define pottery_alignof(T) alignof(T)
        #endif
        #ifndef pottery_align
            // https://en.cppreference.com/w/cpp/language/alignas
            #define pottery_align(n) alignas(n)
        #endif
        #ifndef pottery_alignas
            #define pottery_alignas(T) alignas(T)
        #endif
        #ifndef pottery_alignmax
            // https://en.cppreference.com/w/cpp/types/max_align_t
            #define pottery_alignmax alignof(max_align_t)
        #endif
    #endif
#endif

// GNU-style __alignof__, __attribute__((__aligned__(n)))
#if defined(__GNUC__) || defined(__TINYC__)
    #ifndef pottery_alignof
        // This actually returns preferred alignment rather than minimum as of
        // GCC 8, but without a standard alignof it's the best we've got:
        //     https://www.gnu.org/software/gcc/gcc-8/porting_to.html
        #define pottery_alignof(T) __alignof__(T)
    #endif
    #ifndef pottery_align
        // https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html
        #define pottery_align(n) __attribute__((__aligned__(n)))
    #endif
    #ifndef pottery_alignas
        #define pottery_alignas(T) __attribute__((__aligned__(__alignof__(T))))
    #endif
#endif

// MSVC __alignof, __declspec(align(n))
#ifdef _MSC_VER
    #ifndef pottery_alignof
        // https://docs.microsoft.com/en-us/cpp/cpp/alignof-operator#microsoft-specific
        #define pottery_alignof(T) __alignof(T)
    #endif
    #ifndef pottery_align
        // https://docs.microsoft.com/en-us/cpp/cpp/align-cpp
        #define pottery_align(n) __declspec(align(n))
    #endif
    #ifndef pottery_alignas
        // This doesn't actually work in MSVC!
        //#define pottery_alignas(T) __declspec(align(__alignof(T)))
        //
        // It looks like this bug/limitation has been around forever:
        //
        //     https://godbolt.org/z/1f7oWj
        //     https://stackoverflow.com/questions/5134217/aligning-data-on-the-stack-c
        //
        // Since we can't do this we disable it. For now we only require
        // alignas(type) in C++ mode anyway.
    #endif
#endif

#ifndef pottery_alignmax

    // GNU-style __BIGGEST_ALIGNMENT__
    #ifdef __BIGGEST_ALIGNMENT__
        #define pottery_alignmax __BIGGEST_ALIGNMENT__

    // TinyCC defines max_align_t as the larger of long long and long double
    // but it's not available by default since TinyCC defaults to C99 mode.
    // We supply a similar definition here as a fallback.
    // Note that GCC and friends define __BIGGEST_ALIGNMENT__ as 16 on both
    // 32-bit and 64-bit. On 64-bit this is also 16, but on 32-bit this is 4.
    // This may mean that translation units compiled with different compilers
    // may cause problems when linked together if they disagree on whether
    // they should use extended alignment allocation.
    #elif defined(__TINYC__)
        #define pottery_alignmax pottery_alignof(union {long long a; long double b;})

    // MSVC hardcoded values
    // See "fundamental alignment" here:
    //     https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=vs-2019
    #elif defined(_WIN32) && (defined(_M_X64) || defined(_M_ARM64))
        #define pottery_alignmax 16
    #elif defined(_WIN32) && (defined(_M_IX86) || defined(_M_ARM))
        #define pottery_alignmax 8

    #endif
#endif

// Alignment specifiers are (mostly) required. This is one reason why we don't
// technically support C99 without compiler extensions.
#ifndef pottery_alignof
    #error "alignof or equivalent is required."
#endif
#ifndef pottery_align
    #error "alignas (value) or equivalent is required."
#endif
#if !defined(pottery_alignas) && defined(__cplusplus)
    #error "alignas (type) or equivalent is required in C++."
#endif
#ifndef pottery_alignmax
    #error "alignof(max_align_t) or equivalent is required."
#endif



/*
 * Casting
 */

/*
 * We use these wrappers instead of raw C casts so that the code compiles
 * correctly as C++ under GCC's -Wold-style-cast.
 *
 * Don't worry, we don't actually cast very often in Pottery because everything
 * is strongly typed. These don't show up much.
 */
#ifdef __cplusplus
    #define pottery_static_cast(Type, value) (static_cast<Type>(value))
    #define pottery_reinterpret_cast(Type, value) (reinterpret_cast<Type>(value))
    #define pottery_const_cast(Type, value) (const_cast<Type>(value))
#else
    #define pottery_static_cast(Type, value) ((Type)(value))
    #define pottery_reinterpret_cast(Type, value) ((Type)(value))
    #define pottery_const_cast(Type, value) ((Type)(uintptr_t)(const void*)(value))
#endif

/*
 * We usually want a static cast, so we alias this for convenience.
 */
#define pottery_cast pottery_static_cast

/*
 * This is a form of void pointer cast used to explicitly create objects in
 * untyped memory. (Not run constructors, or do anything actually useful; just
 * "create" the object, telling the compiler to allow it to exist.) Eventually
 * this will wrap std::start_lifetime_as<>.
 *
 * You see, C++ is nowhere near complicated enough, so it is absolutely
 * critical that everything anyone does in practice remains undefined behaviour
 * to preserve the ability of future compiler optimizations to break your code.
 *
 * This isn't standardized yet but we need to stay ahead of it.
 */
#ifdef __cplusplus
    // disabled until approved for C++23
    //#if __cplusplus > 202002L
    //    #define pottery_bless(T, p) std::start_lifetime_as<T>(p)
    //#endif
#endif
#ifndef pottery_bless
    #define pottery_bless(T, p) pottery_reinterpret_cast(T*, p)
#endif

/*
 * Similar to pottery_bless(), here's a wrapper for std::launder(), which we
 * try to use whenever we re-use a pointer after running a C++ constructor.
 *
 * There are some arcane reachability rules with std::launder() that trigger
 * undefined behaviour. I don't really understand them. It's highly likely that
 * I'm using this wrong or not using it enough so please help me fix it.
 */
#ifdef __cplusplus
    #if __cplusplus >= 201703L
        #define pottery_launder(p) (std::launder(p))
    #endif
#endif
#ifndef pottery_launder
    #define pottery_launder(p) (p)
#endif

/*
 * Wrap an expression in std::move() if C++
 */
#ifdef __cplusplus
    #define pottery_move_if_cxx(x) (std::move(x))
#else
    #define pottery_move_if_cxx(x) (x)
#endif

/**
 * Use move construction with placement new if C++
 */
#ifdef __cplusplus
    #define pottery_move_construct(T, x, y) (new (&(x)) T(std::move(y)))
#else
    #define pottery_move_construct(T, x, y) ((x) = (y))
#endif



/*
 * no return
 */

#ifndef pottery_noreturn
    #ifdef __STDC_VERSION__
        #if __STDC_VERSION__ >= 201112L
            #define pottery_noreturn _Noreturn
        #endif
    #endif
#endif

#ifndef pottery_noreturn
    #if defined(__cplusplus)
        #if __cplusplus >= 201103L
            #define pottery_noreturn [[noreturn]]
        #endif
    #endif
#endif

#ifndef pottery_noreturn
    #if defined(_MSC_VER)
        #define pottery_noreturn __declspec(noreturn)
    #elif defined(__GNUC__)
        #define pottery_noreturn __attribute__((__noreturn__))
    #else
        #define pottery_noreturn /* nothing */
    #endif
#endif



#endif
