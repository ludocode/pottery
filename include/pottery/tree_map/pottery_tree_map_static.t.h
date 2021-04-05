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

#define POTTERY_TREE_MAP_IMPL

#ifndef POTTERY_TREE_MAP_EXTERN
    #define POTTERY_TREE_MAP_EXTERN pottery_maybe_unused static
#endif

#include "pottery/tree_map/impl/pottery_tree_map_config_types.t.h"
#include "pottery/container_types/pottery_container_types_static.t.h"

#include "pottery/tree_map/impl/pottery_tree_map_config_lifecycle.t.h"
#include "pottery/lifecycle/pottery_lifecycle_static.t.h"

#include "pottery/tree_map/impl/pottery_tree_map_macros.t.h"
#include "pottery/tree_map/impl/pottery_tree_map_forward.t.h"

// We need to clean up before defining the tree because it defines its own
// container_types.
#include "pottery/lifecycle/pottery_lifecycle_cleanup.t.h"
#include "pottery/container_types/pottery_container_types_cleanup.t.h"

#include "pottery/tree_map/impl/pottery_tree_map_config_tree.t.h"
#include "pottery/red_black_tree/pottery_red_black_tree_static.t.h"

#include "pottery/tree_map/impl/pottery_tree_map_config_alloc.t.h"
#include "pottery/alloc/pottery_alloc_static.t.h"

#include "pottery/tree_map/impl/pottery_tree_map_declarations.t.h"
#include "pottery/tree_map/impl/pottery_tree_map_definitions.t.h"
#include "pottery/tree_map/impl/pottery_tree_map_unmacros.t.h"

#undef POTTERY_TREE_MAP_IMPL
