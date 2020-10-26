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



/*
 * MSVC
 */

#ifdef _MSC_VER
#pragma warning(push)

// MSVC doesn't support [[maybe_unused]] before C++17 so it prints warnings
// about our unused static functions. In this case we disable the warning in
// our templates. We do want the warning in C++17 though to make sure we've
// correctly decorated all our potentially unused functions.
#ifdef __cplusplus
    #if __cplusplus < 201703L
        #pragma warning(disable:4505)
    #endif
#endif

// MSVC warns on constant expressions in if statements, e.g.
//
//     if (std::is_trivial<T>::value) { /*something*/ }
//
// We could potentially fix this with "if constexpr" but I haven't bothered
// yet. Like above it would only work in C++17 anyway.
#pragma warning(disable:4127)

// MSVC warns about being unable to force-inline some functions (in particular
// for Pottery, functions with try/catch blocks.)
#pragma warning(disable:4714)

// MSVC warns about unreachable code. We sometimes use assert(false) with some
// recovery code for release builds, so we disable this warning in debug.
#if POTTERY_DEBUG
    #pragma warning(disable:4702)
#endif

#endif


/*
 * GNUC
 */

#ifdef __GNUC__
//#pragma GCC diagnostic push

// Nothing yet! I do wish we could enable -Wfatal-errors here though, that way
// it's enabled only during the instantiation of a Pottery template. That would
// make Pottery error messages worlds better.

#endif
