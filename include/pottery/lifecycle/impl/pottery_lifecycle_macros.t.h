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

#ifndef POTTERY_LIFECYCLE_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"

#ifdef POTTERY_LIFECYCLE_CAN_DESTROY
#error "A previous instantiation of the Lifecycle template was not cleaned up."
#endif



// configuration



// collapse configuration options

#ifndef POTTERY_LIFECYCLE_BY_VALUE
    #define POTTERY_LIFECYCLE_BY_VALUE 0
#endif

#if POTTERY_LIFECYCLE_BY_VALUE && !defined(POTTERY_LIFECYCLE_VALUE_TYPE)
    #error "BY_VALUE lifecycle operations require a VALUE_TYPE."
#endif

#if POTTERY_LIFECYCLE_BY_VALUE
    #ifndef POTTERY_LIFECYCLE_MOVE_BY_VALUE
        #define POTTERY_LIFECYCLE_MOVE_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_DESTROY_BY_VALUE
        #define POTTERY_LIFECYCLE_DESTROY_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_INIT_BY_VALUE
        #define POTTERY_LIFECYCLE_INIT_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE
        #define POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_COPY_BY_VALUE
        #define POTTERY_LIFECYCLE_COPY_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE
        #define POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_STEAL_BY_VALUE
        #define POTTERY_LIFECYCLE_STEAL_BY_VALUE 1
    #endif
    #ifndef POTTERY_LIFECYCLE_SWAP_BY_VALUE
        #define POTTERY_LIFECYCLE_SWAP_BY_VALUE 1
    #endif
#endif

#ifndef POTTERY_LIFECYCLE_MOVE_BY_VALUE
    #define POTTERY_LIFECYCLE_MOVE_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_DESTROY_BY_VALUE
    #define POTTERY_LIFECYCLE_DESTROY_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_INIT_BY_VALUE
    #define POTTERY_LIFECYCLE_INIT_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE
    #define POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_COPY_BY_VALUE
    #define POTTERY_LIFECYCLE_COPY_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE
    #define POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_STEAL_BY_VALUE
    #define POTTERY_LIFECYCLE_STEAL_BY_VALUE 0
#endif
#ifndef POTTERY_LIFECYCLE_SWAP_BY_VALUE
    #define POTTERY_LIFECYCLE_SWAP_BY_VALUE 0
#endif



// config error checks

// ensure we have at least the required configs

#ifndef POTTERY_LIFECYCLE_PREFIX
#error "POTTERY_LIFECYCLE_PREFIX is not defined."
#endif

// must have one of ref or value type
#if !defined(POTTERY_LIFECYCLE_REF_TYPE) && !defined(POTTERY_LIFECYCLE_VALUE_TYPE)
#error "One of POTTERY_LIFECYCLE_REF_TYPE or POTTERY_LIFECYCLE_VALUE_TYPE must be defined."
#endif



// default features

#ifndef POTTERY_LIFECYCLE_CONSTRUCT
#define POTTERY_LIFECYCLE_CONSTRUCT 0
#endif

#ifndef POTTERY_LIFECYCLE_BULK
#define POTTERY_LIFECYCLE_BULK 0
#endif



// declare available functionality
//
// these figure out what we can synthesize based on the available configuration
// options. the below match the preproc structure of the functions that
// implement them.
//
// the order they're defined in is important: later functions depend on earlier
// ones.

/*
 * destroy()
 */
#if defined(POTTERY_LIFECYCLE_DESTROY)
    // destroy by given expression
    #define POTTERY_LIFECYCLE_CAN_DESTROY 1
#elif POTTERY_LIFECYCLE_DESTROY_BY_VALUE
    // destroy trivially (with destructor)
    #define POTTERY_LIFECYCLE_CAN_DESTROY 1
#else
    // no destroy
    #define POTTERY_LIFECYCLE_CAN_DESTROY 0
#endif

/*
 * move()
 */
#if defined(POTTERY_LIFECYCLE_MOVE)
    // move by given expression
    #define POTTERY_LIFECYCLE_CAN_MOVE 1
#elif POTTERY_LIFECYCLE_MOVE_BY_VALUE || \
        POTTERY_LIFECYCLE_COPY_BY_VALUE
    // move bitwise or by copy construction or simple assignment then destroy
    #define POTTERY_LIFECYCLE_CAN_MOVE 1
#else
    // no move
    #define POTTERY_LIFECYCLE_CAN_MOVE 0
#endif

/*
 * swap()
 */
#if defined(POTTERY_LIFECYCLE_SWAP)
    // swap by given expression
    #define POTTERY_LIFECYCLE_CAN_SWAP 1
#elif POTTERY_LIFECYCLE_SWAP_BY_VALUE
    // swap bitwise or by ADL-swap() or std::swap()
    #define POTTERY_LIFECYCLE_CAN_SWAP 1
#elif POTTERY_LIFECYCLE_CAN_MOVE && defined(POTTERY_LIFECYCLE_VALUE_TYPE)
    // swap by move through a temporary
    #define POTTERY_LIFECYCLE_CAN_SWAP 1
#else
    #define POTTERY_LIFECYCLE_CAN_SWAP 0
#endif

/*
 * steal()
 */
#if defined(POTTERY_LIFECYCLE_STEAL)
    // steal by given expression
    #define POTTERY_LIFECYCLE_CAN_STEAL 1
#elif POTTERY_LIFECYCLE_STEAL_BY_VALUE
    // steal bitwise or by move assignment or simple assignment
    #define POTTERY_LIFECYCLE_CAN_STEAL 1
#elif POTTERY_LIFECYCLE_CAN_SWAP
    // synthesize steal by swap
    #define POTTERY_LIFECYCLE_CAN_STEAL 1
#else
    // no steal
    #define POTTERY_LIFECYCLE_CAN_STEAL 0
#endif

/*
 * init()
 */
#if defined(POTTERY_LIFECYCLE_INIT)
    // init by given expression
    #define POTTERY_LIFECYCLE_CAN_INIT 1
#elif POTTERY_LIFECYCLE_INIT_BY_VALUE
    // init trivially (with default constructor)
    #define POTTERY_LIFECYCLE_CAN_INIT 1
#else
    // no init
    #define POTTERY_LIFECYCLE_CAN_INIT 0
#endif

/*
 * copy()
 */
#if defined(POTTERY_LIFECYCLE_COPY)
    // copy by given expression
    #define POTTERY_LIFECYCLE_CAN_COPY 1
#elif POTTERY_LIFECYCLE_COPY_BY_VALUE
    // copy bitwise or by copy assignment or simple assignment
    #define POTTERY_LIFECYCLE_CAN_COPY 1
#else
    // no copy
    #define POTTERY_LIFECYCLE_CAN_COPY 0
#endif

/*
 * init_copy()
 */
#if defined(POTTERY_LIFECYCLE_INIT_COPY)
    // init_copy by given expression
    #define POTTERY_LIFECYCLE_CAN_INIT_COPY 1
#elif POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE
    // init_copy by copy construction or simple assignment
    #define POTTERY_LIFECYCLE_CAN_INIT_COPY 1
#elif POTTERY_LIFECYCLE_CAN_INIT && POTTERY_LIFECYCLE_CAN_COPY
    // synthesize init_copy by init then copy
    #define POTTERY_LIFECYCLE_CAN_INIT_COPY 1
#else
    // no init_copy
    #define POTTERY_LIFECYCLE_CAN_INIT_COPY 0
#endif

/*
 * init_steal()
 */
#if defined(POTTERY_LIFECYCLE_INIT_STEAL)
    // init_steal by given expression
    #define POTTERY_LIFECYCLE_CAN_INIT_STEAL 1
#elif POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE
    // init_steal by move construction or simple assignment
    #define POTTERY_LIFECYCLE_CAN_INIT_STEAL 1
#elif POTTERY_LIFECYCLE_CAN_INIT && POTTERY_LIFECYCLE_CAN_SWAP
    // synthesize init_steal by init then swap
    #define POTTERY_LIFECYCLE_CAN_INIT_STEAL 1
#elif POTTERY_LIFECYCLE_CAN_INIT_COPY
    // synthesize init_steal by init_copy
    #define POTTERY_LIFECYCLE_CAN_INIT_STEAL 1
#else
    // no init_steal
    #define POTTERY_LIFECYCLE_CAN_INIT_STEAL 0
#endif

/*
 * copy by passing by value (e.g. as argument of insert() or return value of extract())
 * TODO should actually be move by value?
 */
#if POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE && POTTERY_LIFECYCLE_CAN_DESTROY
    #define POTTERY_LIFECYCLE_CAN_PASS 1
#else
    #define POTTERY_LIFECYCLE_CAN_PASS 0
#endif



// function attributes

#ifndef POTTERY_LIFECYCLE_ATTRIBUTES_EXTERNAL
#define POTTERY_LIFECYCLE_ATTRIBUTES_EXTERNAL /*nothing*/
#endif

// Context

#ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
#define POTTERY_LIFECYCLE_CONTEXT_ARG pottery_lifecycle_context_t context,
#define POTTERY_LIFECYCLE_CONTEXT_VAL context,
#define POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED (void)context
#else
#define POTTERY_LIFECYCLE_CONTEXT_ARG    /* nothing */
#define POTTERY_LIFECYCLE_CONTEXT_VAL    /* nothing */
#define POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED /* nothing */
#endif



/*
 * Renaming
 */

//!!! AUTOGENERATED:lifecycle/rename.m.h SRC:pottery_lifecycle DEST:POTTERY_LIFECYCLE_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames lifecycle identifiers from pottery_lifecycle to POTTERY_LIFECYCLE_PREFIX

    // types
    #define pottery_lifecycle_value_t POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _value_t)
    #define pottery_lifecycle_ref_t POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _ref_t)
    #define pottery_lifecycle_const_ref_t POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _const_ref_t)
    #define pottery_lifecycle_context_t POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _context_t)

    // fundamental operations
    #define pottery_lifecycle_init_steal POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _init_steal)
    #define pottery_lifecycle_init_copy POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _init_copy)
    #define pottery_lifecycle_init POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _init)
    #define pottery_lifecycle_destroy POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _destroy)
    #define pottery_lifecycle_swap POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _swap)
    #define pottery_lifecycle_move POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move)
    #define pottery_lifecycle_steal POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _steal)
    #define pottery_lifecycle_copy POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _copy)

    // bulk operations
    #define pottery_lifecycle_destroy_bulk POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _destroy_bulk)
    #define pottery_lifecycle_move_bulk POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk)
    #define pottery_lifecycle_move_bulk_restrict POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk_restrict)
    #define pottery_lifecycle_move_bulk_restrict_impl POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk_restrict_impl)
    #define pottery_lifecycle_move_bulk_up POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk_up)
    #define pottery_lifecycle_move_bulk_up_impl POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk_up_impl)
    #define pottery_lifecycle_move_bulk_down POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk_down)
    #define pottery_lifecycle_move_bulk_down_impl POTTERY_CONCAT(POTTERY_LIFECYCLE_PREFIX, _move_bulk_down_impl)
//!!! END_AUTOGENERATED

/*
 * The C++ functions above are separate partially to wrap them up for C (so
 * that we can call e.g. {PREFIX}_destruct() in C and it just does nothing),
 * but also because they may need an alternate implementation of a compound
 * operation in C++.
 *
 * For example construct_init_copy() can't just construct() and then
 * init_copy() because the C++ class may not be default-constructible. Instead
 * it has to construct by copy-constructor.
 */
