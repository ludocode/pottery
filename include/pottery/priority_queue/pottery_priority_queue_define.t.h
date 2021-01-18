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

#define POTTERY_PRIORITY_QUEUE_IMPL

#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_CONTAINER_TYPES
    #include "pottery/priority_queue/impl/pottery_priority_queue_config_types.t.h"
    #include "pottery/container_types/pottery_container_types_define.t.h"
#endif

#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_LIFECYCLE
    #include "pottery/priority_queue/impl/pottery_priority_queue_config_lifecycle.t.h"
    #include "pottery/lifecycle/pottery_lifecycle_define.t.h"
#endif

#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_COMPARE
    #include "pottery/priority_queue/impl/pottery_priority_queue_config_compare.t.h"
    #include "pottery/compare/pottery_compare_define.t.h"
#endif

#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_ARRAY
    #include "pottery/priority_queue/impl/pottery_priority_queue_config_vector.t.h"
    #include "pottery/vector/pottery_vector_define.t.h"
#endif

#include "pottery/priority_queue/impl/pottery_priority_queue_macros.t.h"

// Cleanup before heap since heap will need its own helper templates that use
// our internal array as context
#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_COMPARE
    #include "pottery/compare/pottery_compare_cleanup.t.h"
#endif
#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_LIFECYCLE
    #include "pottery/lifecycle/pottery_lifecycle_cleanup.t.h"
#endif
#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_CONTAINER_TYPES
    #include "pottery/container_types/pottery_container_types_cleanup.t.h"
#endif

#ifndef POTTERY_PRIORITY_QUEUE_EXTERNAL_HEAP
    #include "pottery/priority_queue/impl/pottery_priority_queue_config_heap.t.h"
    #include "pottery/heap/pottery_heap_define.t.h"
#endif

#include "pottery/priority_queue/impl/pottery_priority_queue_definitions.t.h"
#include "pottery/priority_queue/impl/pottery_priority_queue_unmacros.t.h"

#undef POTTERY_PRIORITY_QUEUE_IMPL
