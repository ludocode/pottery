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

#define POTTERY_ARRAY_MAP_IMPL
#define POTTERY_ARRAY_MAP_EXTERN /*nothing*/

#ifndef POTTERY_ARRAY_MAP_EXTERNAL_CONTAINER_TYPES
    #include "pottery/array_map/impl/pottery_array_map_config_types.t.h"
    #include "pottery/container_types/pottery_container_types_declare.t.h"
#endif

#ifndef POTTERY_ARRAY_MAP_EXTERNAL_LIFECYCLE
    #include "pottery/array_map/impl/pottery_array_map_config_lifecycle.t.h"
    #include "pottery/lifecycle/pottery_lifecycle_declare.t.h"
#endif

#ifndef POTTERY_ARRAY_MAP_EXTERNAL_ARRAY
    #include "pottery/array_map/impl/pottery_array_map_config_vector.t.h"
    #include "pottery/vector/pottery_vector_declare.t.h"
#endif

// Cleanup container_types before compare since compare uses its own
// container_types for our key type
#ifndef POTTERY_ARRAY_MAP_EXTERNAL_CONTAINER_TYPES
    #include "pottery/container_types/pottery_container_types_cleanup.t.h"
#endif

#include "pottery/array_map/impl/pottery_array_map_macros.t.h"

#ifndef POTTERY_ARRAY_MAP_EXTERNAL_COMPARE
    #include "pottery/array_map/impl/pottery_array_map_config_compare.t.h"
    #include "pottery/compare/pottery_compare_declare.t.h"
#endif

#include "pottery/array_map/impl/pottery_array_map_declarations.t.h"

#ifndef POTTERY_ARRAY_MAP_EXTERNAL_COMPARE
    #include "pottery/compare/pottery_compare_cleanup.t.h"
#endif

#ifndef POTTERY_ARRAY_MAP_EXTERNAL_LIFECYCLE
    #include "pottery/lifecycle/pottery_lifecycle_cleanup.t.h"
#endif

#include "pottery/array_map/impl/pottery_array_map_unmacros.t.h"

#undef POTTERY_ARRAY_MAP_IMPL
