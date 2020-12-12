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

#define POTTERY_DEQUE_IMPL

#include "pottery/deque/impl/pottery_deque_macros.t.h"

#include "pottery/deque/impl/pottery_deque_config_ring.t.h"
#include "pottery/ring/pottery_ring_define.t.h"

#include "pottery/deque/impl/pottery_deque_config_alloc.t.h"
#include "pottery/alloc/pottery_alloc_define.t.h"

#include "pottery/deque/impl/pottery_deque_config_lifecycle.t.h"
#include "pottery/lifecycle/pottery_lifecycle_define.t.h"

#include "pottery/deque/impl/pottery_deque_definitions.t.h"
#include "pottery/deque/impl/pottery_deque_unmacros.t.h"

#include "pottery/lifecycle/pottery_lifecycle_cleanup.t.h"

#undef POTTERY_DEQUE_IMPL
