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

#define POTTERY_ARRAY_ACCESS_INTERNAL

#ifndef POTTERY_ARRAY_ACCESS_EXTERNAL_CONTAINER_TYPES
    #include "pottery/array_access/impl/pottery_array_access_config_types.t.h"
    #include "pottery/container_types/pottery_container_types_static.t.h"
#endif

#include "pottery/array_access/impl/pottery_array_access_macros.t.h"
#include "pottery/array_access/impl/pottery_array_access_declarations.t.h"
// No definitions file! Everything is inline. We still need macros though for
// configuration purposes.
#include "pottery/array_access/impl/pottery_array_access_unmacros.t.h"
#ifndef POTTERY_ARRAY_ACCESS_NO_CLEANUP
    #include "pottery/array_access/pottery_array_access_cleanup.t.h"
#endif

#ifndef POTTERY_ARRAY_ACCESS_EXTERNAL_CONTAINER_TYPES
    #include "pottery/container_types/pottery_container_types_cleanup.t.h"
#endif

#undef POTTERY_ARRAY_ACCESS_INTERNAL
