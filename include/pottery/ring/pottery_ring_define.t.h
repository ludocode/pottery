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

#define POTTERY_RING_IMPL

#ifndef POTTERY_RING_EXTERNAL_CONTAINER_TYPES
    #include "pottery/ring/impl/pottery_ring_config_types.t.h"
    #include "pottery/container_types/pottery_container_types_define.t.h"
#endif

#include "pottery/ring/impl/pottery_ring_macros.t.h"

#ifndef POTTERY_RING_EXTERNAL_ALLOC
    #include "pottery/ring/impl/pottery_ring_config_alloc.t.h"
    #include "pottery/alloc/pottery_alloc_define.t.h"
#endif

#ifndef POTTERY_RING_EXTERNAL_LIFECYCLE
    #include "pottery/ring/impl/pottery_ring_config_lifecycle.t.h"
    #include "pottery/lifecycle/pottery_lifecycle_define.t.h"
#endif

#include "pottery/ring/impl/pottery_ring_definitions.t.h"

#ifndef POTTERY_RING_EXTERNAL_LIFECYCLE
    #include "pottery/lifecycle/pottery_lifecycle_cleanup.t.h"
#endif

#ifndef POTTERY_RING_EXTERNAL_CONTAINER_TYPES
    #include "pottery/container_types/pottery_container_types_cleanup.t.h"
#endif

#include "pottery/ring/impl/pottery_ring_unmacros.t.h"

#undef POTTERY_RING_IMPL
