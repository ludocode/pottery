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

#ifndef POTTERY_CONTAINER_TYPES_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#include "pottery/pottery_dependencies.h"
#include "pottery/platform/pottery_platform_pragma_push.t.h"

#ifdef POTTERY_CONTAINER_TYPES_HAS_CONTEXT
#error "A previous instantiation of the container_types template was not cleaned up."
#endif



/*
 * Configuration
 */

// must have exactly one of value or ref type
#if !defined(POTTERY_CONTAINER_TYPES_VALUE_TYPE) && \
    !defined(POTTERY_CONTAINER_TYPES_REF_TYPE)
#error "One of VALUE_TYPE or REF_TYPE must be defined."
#endif
#if defined(POTTERY_CONTAINER_TYPES_VALUE_TYPE) && \
    defined(POTTERY_CONTAINER_TYPES_REF_TYPE)
#error "Only one of VALUE_TYPE or REF_TYPE must be defined, not both."
#endif

// has value
#if defined(POTTERY_CONTAINER_TYPES_VALUE_TYPE)
    #define POTTERY_CONTAINER_TYPES_HAS_VALUE 1
#else
    #define POTTERY_CONTAINER_TYPES_HAS_VALUE 0
#endif

// has key
#if defined(POTTERY_CONTAINER_TYPES_KEY_TYPE)
    #define POTTERY_CONTAINER_TYPES_HAS_KEY 1
#else
    #define POTTERY_CONTAINER_TYPES_HAS_KEY 0
#endif

// has context
#if defined(POTTERY_CONTAINER_TYPES_CONTEXT_TYPE)
    #define POTTERY_CONTAINER_TYPES_HAS_CONTEXT 1
#else
    #define POTTERY_CONTAINER_TYPES_HAS_CONTEXT 0
#endif



/*
 * Renaming
 */

//!!! AUTOGENERATED:container_types/rename.m.h SRC:pottery_container_types DEST:POTTERY_CONTAINER_TYPES_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames container_types template identifiers from pottery_container_types to POTTERY_CONTAINER_TYPES_PREFIX

    #define pottery_container_types_value_t POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _value_t)
    #define pottery_container_types_ref_t POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _ref_t)
    #define pottery_container_types_const_ref_t POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _const_ref_t)
    #define pottery_container_types_entry_t POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _entry_t)
    #define pottery_container_types_key_t POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _key_t)
    #define pottery_container_types_context_t POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _context_t)

    #define pottery_container_types_entry_ref POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _entry_ref)
    #define pottery_container_types_ref_key POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _ref_key)
    #define pottery_container_types_ref_equal POTTERY_CONCAT(POTTERY_CONTAINER_TYPES_PREFIX, _ref_equal)
//!!! END_AUTOGENERATED