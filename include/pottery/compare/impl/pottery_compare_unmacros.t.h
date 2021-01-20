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

#ifndef POTTERY_COMPARE_INTERNAL
#error "This is an internal header. Do not include it."
#endif



/*
 * Configuration
 */

// user configuration

#undef POTTERY_COMPARE_PREFIX

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_COMPARE

    // types
    #undef POTTERY_COMPARE_VALUE_TYPE
    #undef POTTERY_COMPARE_REF_TYPE
    #undef POTTERY_COMPARE_ENTRY_TYPE
    #undef POTTERY_COMPARE_KEY_TYPE
    #undef POTTERY_COMPARE_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_COMPARE_ENTRY_REF
    #undef POTTERY_COMPARE_REF_KEY
    #undef POTTERY_COMPARE_REF_EQUAL
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:compare/unconfigure.m.h PREFIX:POTTERY_COMPARE
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) compare configuration on POTTERY_COMPARE

    #undef POTTERY_COMPARE_EQUAL
    #undef POTTERY_COMPARE_NOT_EQUAL
    #undef POTTERY_COMPARE_LESS
    #undef POTTERY_COMPARE_LESS_OR_EQUAL
    #undef POTTERY_COMPARE_GREATER
    #undef POTTERY_COMPARE_GREATER_OR_EQUAL
    #undef POTTERY_COMPARE_THREE_WAY
//!!! END_AUTOGENERATED

// internal stuff

#undef POTTERY_COMPARE_EXTERN
#undef POTTERY_COMPARE_CONTEXT_ARG
#undef POTTERY_COMPARE_CONTEXT_VAL
#undef POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED


/*
 * Renaming
 */

#undef POTTERY_COMPARE_CONTAINER_TYPES_PREFIX
//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_compare
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_compare

    #undef pottery_compare_value_t
    #undef pottery_compare_ref_t
    #undef pottery_compare_const_ref_t
    #undef pottery_compare_key_t
    #undef pottery_compare_context_t

    #undef pottery_compare_ref_key
    #undef pottery_compare_ref_equal
//!!! END_AUTOGENERATED

//!!! AUTOGENERATED:compare/unrename.m.h PREFIX:pottery_compare
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Un-renames compare identifiers with prefix "pottery_compare"

    #undef pottery_compare_three_way
    #undef pottery_compare_equal
    #undef pottery_compare_not_equal
    #undef pottery_compare_less
    #undef pottery_compare_less_or_equal
    #undef pottery_compare_greater
    #undef pottery_compare_greater_or_equal

    #undef pottery_compare_min
    #undef pottery_compare_max
    #undef pottery_compare_clamp
    #undef pottery_compare_median
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
