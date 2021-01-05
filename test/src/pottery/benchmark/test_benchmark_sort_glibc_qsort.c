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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>

#ifdef __GNUC__
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

typedef int (*__compar_d_fn_t)(const void* left, const void* right, void* context);

// Lots of stuff to work around in glibc's qsort...

#define __mempcpy(d,s,n) (memcpy(d,s,n),(d)+(n))
#define __alloca alloca
#define __sysconf sysconf
#define atomic_write_barrier() atomic_thread_fence(memory_order_seq_cst)
#define qsort glibc_qsort
#define __set_errno(x) (errno = (x))
#define libc_hidden_def(x) /*nothing*/
#define weak_alias(x,y) /*nothing*/

#include "glibc_qsort.c"
#include "glibc_msort.c"
