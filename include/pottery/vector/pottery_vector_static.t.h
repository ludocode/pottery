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

#define POTTERY_VECTOR_IMPL

#ifndef POTTERY_VECTOR_EXTERN
    #define POTTERY_VECTOR_EXTERN pottery_maybe_unused static
#endif

#ifndef POTTERY_VECTOR_EXTERNAL_CONTAINER_TYPES
    #include "pottery/vector/impl/pottery_vector_config_types.t.h"
    #include "pottery/container_types/pottery_container_types_static.t.h"
#endif

#include "pottery/vector/impl/pottery_vector_macros.t.h"
#include "pottery/vector/impl/pottery_vector_forward.t.h"

#include "pottery/vector/impl/pottery_vector_config_alloc.t.h"
#include "pottery/alloc/pottery_alloc_static.t.h"

#ifndef POTTERY_VECTOR_EXTERNAL_LIFECYCLE
    #include "pottery/vector/impl/pottery_vector_config_lifecycle.t.h"
    #include "pottery/lifecycle/pottery_lifecycle_static.t.h"
#endif

#include "pottery/vector/impl/pottery_vector_declarations.t.h"
#include "pottery/vector/impl/pottery_vector_definitions.t.h"

#ifndef POTTERY_VECTOR_EXTERNAL_LIFECYCLE
    #include "pottery/lifecycle/pottery_lifecycle_cleanup.t.h"
#endif

#ifndef POTTERY_VECTOR_EXTERNAL_CONTAINER_TYPES
    #include "pottery/container_types/pottery_container_types_cleanup.t.h"
#endif

#include "pottery/vector/impl/pottery_vector_unmacros.t.h"

#undef POTTERY_VECTOR_IMPL
