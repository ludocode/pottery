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

#ifndef POTTERY_ARRAY_ACCESS_INTERNAL
#error "This is an internal header. Do not include it."
#endif



/*
 * Configuration
 */

// user configuration

#undef POTTERY_ARRAY_ACCESS_PREFIX

//!!! AUTOGENERATED:array_access/unconfigure.m.h PREFIX:POTTERY_ARRAY_ACCESS
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) array_access configuration on POTTERY_ARRAY_ACCESS

    // absolute addressing configs
    #undef POTTERY_ARRAY_ACCESS_BEGIN
    #undef POTTERY_ARRAY_ACCESS_END
    #undef POTTERY_ARRAY_ACCESS_COUNT

    // required configs for non-standard array access
    #undef POTTERY_ARRAY_ACCESS_SELECT
    #undef POTTERY_ARRAY_ACCESS_INDEX

    // required config for non-trivial entries
    #undef POTTERY_ARRAY_ACCESS_EQUAL

    // optional configs
    #undef POTTERY_ARRAY_ACCESS_SHIFT
    #undef POTTERY_ARRAY_ACCESS_OFFSET
    #undef POTTERY_ARRAY_ACCESS_NEXT
    #undef POTTERY_ARRAY_ACCESS_PREVIOUS
    #undef POTTERY_ARRAY_ACCESS_EXISTS
//!!! END_AUTOGENERATED

// internal stuff

#undef POTTERY_ARRAY_ACCESS_EXTERN
#undef POTTERY_ARRAY_ACCESS_CONTEXT_ARG
#undef POTTERY_ARRAY_ACCESS_CONTEXT_VAL
#undef POTTERY_ARRAY_ACCESS_CONTEXT_MAYBE_UNUSED
#undef POTTERY_ARRAY_ACCESS_SOLE_ARGS
#undef POTTERY_ARRAY_ACCESS_SOLE_VALS
#undef POTTERY_ARRAY_ACCESS_ARGS
#undef POTTERY_ARRAY_ACCESS_VALS
#undef POTTERY_ARRAY_ACCESS_ARGS_UNUSED


/*
 * Renaming
 */

//!!! AUTOGENERATED:array_access/unrename.m.h PREFIX:pottery_array_access
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames array_access identifiers with prefix "pottery_array_access"

    #undef pottery_array_access_value_t
    #undef pottery_array_access_ref_t
    #undef pottery_array_access_entry_t
    #undef pottery_array_access_context_t

    #undef pottery_array_access_begin
    #undef pottery_array_access_end
    #undef pottery_array_access_count

    #undef pottery_array_access_select
    #undef pottery_array_access_index

    #undef pottery_array_access_shift
    #undef pottery_array_access_offset
    #undef pottery_array_access_next
    #undef pottery_array_access_previous

    #undef pottery_array_access_ref
    #undef pottery_array_access_equal
    #undef pottery_array_access_exists
//!!! END_AUTOGENERATED



// container types

#undef POTTERY_ARRAY_ACCESS_CONTAINER_TYPES_PREFIX
//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_array_access
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_array_access

    #undef pottery_array_access_value_t
    #undef pottery_array_access_ref_t
    #undef pottery_array_access_const_ref_t
    #undef pottery_array_access_entry_t
    #undef pottery_array_access_key_t
    #undef pottery_array_access_context_t

    #undef pottery_array_access_entry_ref
    #undef pottery_array_access_ref_key
    #undef pottery_array_access_ref_equal
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_ARRAY_ACCESS
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_ARRAY_ACCESS

    // types
    #undef POTTERY_ARRAY_ACCESS_VALUE_TYPE
    #undef POTTERY_ARRAY_ACCESS_REF_TYPE
    #undef POTTERY_ARRAY_ACCESS_ENTRY_TYPE
    #undef POTTERY_ARRAY_ACCESS_KEY_TYPE
    #undef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_ARRAY_ACCESS_ENTRY_REF
    #undef POTTERY_ARRAY_ACCESS_REF_KEY
    #undef POTTERY_ARRAY_ACCESS_REF_EQUAL
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
