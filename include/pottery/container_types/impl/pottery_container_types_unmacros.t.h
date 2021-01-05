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

#ifndef POTTERY_CONTAINER_TYPES_INTERNAL
#error "This is an internal header. Do not include it."
#endif



/*
 * Configuration
 */

#undef POTTERY_CONTAINER_TYPES_PREFIX


/*
 * Common Function Arguments
 */

//!!! AUTOGENERATED:container_types/unconfigure.m.h CONFIG:POTTERY_CONTAINER_TYPES
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Undefines (forwardable/forwarded) container_types configuration on POTTERY_CONTAINER_TYPES

    // types
    #undef POTTERY_CONTAINER_TYPES_VALUE_TYPE
    #undef POTTERY_CONTAINER_TYPES_REF_TYPE
    #undef POTTERY_CONTAINER_TYPES_ENTRY_TYPE
    #undef POTTERY_CONTAINER_TYPES_KEY_TYPE
    #undef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE

    // Conversion and comparison expressions
    #undef POTTERY_CONTAINER_TYPES_ENTRY_REF
    #undef POTTERY_CONTAINER_TYPES_REF_KEY
    #undef POTTERY_CONTAINER_TYPES_REF_EQUAL
//!!! END_AUTOGENERATED



/*
 * Renaming
 */

//!!! AUTOGENERATED:container_types/unrename.m.h PREFIX:pottery_container_types
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Unrenames container_types template identifiers from pottery_container_types

    #undef pottery_container_types_value_t
    #undef pottery_container_types_ref_t
    #undef pottery_container_types_const_ref_t
    #undef pottery_container_types_entry_t
    #undef pottery_container_types_key_t
    #undef pottery_container_types_context_t

    #undef pottery_container_types_entry_ref
    #undef pottery_container_types_ref_key
    #undef pottery_container_types_ref_equal
//!!! END_AUTOGENERATED



#include "pottery/platform/pottery_platform_pragma_pop.t.h"
