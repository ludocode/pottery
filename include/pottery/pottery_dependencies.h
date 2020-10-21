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

#ifndef POTTERY_DEPENDENCIES_H
#define POTTERY_DEPENDENCIES_H 1

/*
 * This file includes all dependencies needed by Pottery templates.
 *
 * It is included automatically when instantiating any Pottery template.
 * However, in C++, this will trigger a compiler error if a template is
 * instantiated outside of file scope (i.e. in a class, namespace or extern "C"
 * block.) To include a Pottery template in a non-file scope, you must first
 * include this dependencies header at file scope.
 */



#include "pottery/platform/pottery_platform_headers.h"

#include "pottery/platform/pottery_platform_features.h"
#include "pottery/platform/pottery_platform_attributes.h"
#include "pottery/platform/pottery_platform_functions.h"

#ifdef __cplusplus
#include "pottery/platform/pottery_platform_cxx.hxx"
#endif


#endif
