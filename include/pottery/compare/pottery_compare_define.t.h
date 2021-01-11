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

#define POTTERY_COMPARE_INTERNAL

#ifndef POTTERY_COMPARE_EXTERNAL_CONTAINER_TYPES
    #include "pottery/compare/impl/pottery_compare_config_types.t.h"
    #include "pottery/container_types/pottery_container_types_define.t.h"
#endif

#include "pottery/compare/impl/pottery_compare_macros.t.h"
// no definitions
#include "pottery/compare/impl/pottery_compare_unmacros.t.h"
#ifndef POTTERY_COMPARE_NO_CLEANUP
    #include "pottery/compare/pottery_compare_cleanup.t.h"
#endif

#ifndef POTTERY_COMPARE_EXTERNAL_CONTAINER_TYPES
    #include "pottery/container_types/pottery_container_types_cleanup.t.h"
#endif

#undef POTTERY_COMPARE_INTERNAL
