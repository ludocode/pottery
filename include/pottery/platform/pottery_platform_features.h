/*
 * MIT License
 *
 * Copyright (c) 2020-2021 11037650 Canada Inc.
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

#ifndef POTTERY_PLATFORM_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif



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
 * - You may have attemped to dereference a non-existent ref;
 *
 * - You may have corrupted a map element by changing its key;
 *
 * - Your comparison function may be intransitive or otherwise buggy;
 *
 * - Your hash or comparison function may have changed its results based on
 *   some external context without rebuilding the data structure that uses it.
 *
 * If you are hitting an assertion and you are sure you are using Pottery
 * correctly, please file a bug!
 */
#ifndef POTTERY_DEBUG
    #if defined(DEBUG) || defined(_DEBUG)
        #define POTTERY_DEBUG 1
    #else
        #define POTTERY_DEBUG 0
    #endif
#endif

/*
 * POTTERY_LEAK_CHECK additionally enables extra leak check allocations for
 * objects that require explicit destruction. This can be useful to detect when
 * you've forgotten to destroy objects that might not otherwise cause an
 * external leak detector to notice.
 *
 * For example if you forget to destroy a vector that was only using internal
 * space, you didn't technically leak memory so a leak detector like ASAN or
 * Valgrind won't detect it, but it'll start leaking as soon as you put enough
 * stuff in it to make it allocate.
 *
 * This is off by default because it requires ordinary malloc() and free()
 * (since we need it before we've set up alloc contexts), it can be annoying
 * (seeing tons of 1 byte allocations in your memory tools), and it can slow
 * down debug builds too much.
 *
 * A better way might be to instrument the code to inform your tool directly
 * when Pottery's objects are created and destroyed. We don't really want to
 * instrument Pottery with every tool under the sun, but it might make sense
 * for some (and it would be nice if it detected more stuff, e.g. out-of-bounds
 * accesses for unused capacity in a vector.) In the meantime this is a bit of
 * a blunt hammer that can detect mismatched init/destroy with any memory
 * checker.
 */
#ifndef POTTERY_LEAK_CHECK
    #define POTTERY_LEAK_CHECK 0
#endif
#if POTTERY_LEAK_CHECK && !POTTERY_DEBUG
    #error "POTTERY_LEAK_CHECK requires POTTERY_DEBUG."
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

#ifndef POTTERY_GENERATE_CODE
typedef int pottery_error_t;
#endif

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



// This doesn't really belong in "platform" but we don't have a better place
// for this yet. These headers need to be moved around, probably renaming
// "platform" to "common".

/*
 * POTTERY_FOR_EACH()
 *
 * The for_each loop loops over all entries in the container. It starts with
 * prefix##_begin() and loops with prefix##_next() until
 * prefix##_entry_exists() returns false, assigning prefix##_entry_ref() to
 * your ref in each iteration of the loop.
 *
 * @param ref An l-value of type prefix##_ref_t (which is a pointer to value
 *            for concrete value types.) This will be assigned in each loop
 *            iteration.
 * @param prefix The pottery PREFIX of the container.
 * @param container A pointer to the container.
 *
 * @warning The ref and container arguments may be evaluated multiple times.
 *
 * @note In ANSI C/gnu89 (i.e. pre-C99), POTTERY_FOR_EACH() only supports
 *       containers where the ref and entry types are the same because an entry
 *       cannot be defined within the for loop. Usage is the same where
 *       supported so POTTERY_FOR_EACH() in gnu89 is forward-compatible.
 */

// First determine whether we can suppor the full implementation of for-each
// (we need support for C99/C++-style definitions in for loops)

#ifndef POTTERY_HAS_FULL_FOR_EACH
    #ifdef __STDC_VERSION__
        #if __STDC_VERSION__ >= 199901L
            #define POTTERY_HAS_FULL_FOR_EACH 1
        #endif
    #endif
#endif

#ifndef POTTERY_HAS_FULL_FOR_EACH
    #ifdef __cplusplus
        #define POTTERY_HAS_FULL_FOR_EACH 1
    #endif
#endif

#ifndef POTTERY_HAS_FULL_FOR_EACH
    #define POTTERY_HAS_FULL_FOR_EACH 0
#endif

// If possible, POTTERY_FOR_EACH() defines an entry iterator and performs
// correct ref conversions. This means it works on any container type.

#ifndef POTTERY_FOR_EACH
    #if POTTERY_HAS_FULL_FOR_EACH
        #define POTTERY_FOR_EACH(ref, prefix, container) \
            for (POTTERY_CONCAT(prefix, _entry_t) POTTERY_CONCAT(prefix, _for_each_entry) = \
                        POTTERY_CONCAT(prefix, _begin)((container)); \
                    (POTTERY_CONCAT(prefix, _entry_exists)((container), POTTERY_CONCAT(prefix, _for_each_entry)) ? \
                        (((ref) = POTTERY_CONCAT(prefix, _entry_ref)((container), POTTERY_CONCAT(prefix, _for_each_entry))), true) : \
                        false); \
                    POTTERY_CONCAT(prefix, _for_each_entry) = \
                        POTTERY_CONCAT(prefix, _next)((container), POTTERY_CONCAT(prefix, _for_each_entry)))
    #endif
#endif

// POTTERY_FOR_EACH() cannot declare an entry iterator pre-C99 so in this case
// it only works on containers where the entry and ref types are the same.

// If possible we add a static assert to ensure that the types are the same
// (otherwise if e.g. the ref type is void* and the entry type is some other
// pointer, they are implicitly convertible so this would silently compile to
// nonsense.) (This causes warnings if you're compiling under gnu89 with
// -Wpedantic since ANSI C doesn't have statement expressions or _Static_assert,
// but so does everything else in Pottery like these // comments.)
#ifndef POTTERY_FOR_EACH
    #if defined(__GNUC__) && defined(__has_builtin)
        #if __has_builtin(__builtin_types_compatible_p)
            #define POTTERY_FOR_EACH(ref, prefix, container) \
                for ((ref) = ({ \
                            _Static_assert(__builtin_types_compatible_p(POTTERY_CONCAT(prefix, _entry_t), POTTERY_CONCAT(prefix, _ref_t)), \
                                    "Entry and ref types do not match! POTTERY_FOR_EACH() cannot be used under gnu89 on this container."); \
                            POTTERY_CONCAT(prefix, _begin)((container)); \
                        }); \
                        POTTERY_CONCAT(prefix, _entry_exists)((container), (ref)); \
                        (ref) = POTTERY_CONCAT(prefix, _next)((container), (ref)))
        #endif
    #endif
#endif

// This is the same as the previous one, just without the static assert.
#ifndef POTTERY_FOR_EACH
    #define POTTERY_FOR_EACH(ref, prefix, container) \
        for ((ref) = POTTERY_CONCAT(prefix, _begin)((container)); \
                POTTERY_CONCAT(prefix, _entry_exists)((container), (ref)); \
                (ref) = POTTERY_CONCAT(prefix, _next)((container), (ref)))
#endif



#endif
