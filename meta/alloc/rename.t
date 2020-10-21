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

//!!! END_LICENSE
// Renames alloc identifiers with prefix "{PREFIX}"
// using macro {MACRO}()

#define {PREFIX}_free {MACRO}(_free)
#define {PREFIX}_impl_free_ea {MACRO}(_impl_free_ea)
#define {PREFIX}_impl_free_fa {MACRO}(_impl_free_fa)

#define {PREFIX}_malloc {MACRO}(_malloc)
#define {PREFIX}_impl_malloc_ea {MACRO}(_impl_malloc_ea)
#define {PREFIX}_impl_malloc_fa {MACRO}(_impl_malloc_fa)

#define {PREFIX}_malloc_zero {MACRO}(_malloc_zero)
#define {PREFIX}_impl_malloc_zero_ea {MACRO}(_impl_malloc_zero_ea)
#define {PREFIX}_impl_malloc_zero_fa {MACRO}(_impl_malloc_zero_fa)
#define {PREFIX}_impl_malloc_zero_fa_wrap {MACRO}(_impl_malloc_zero_fa_wrap)

#define {PREFIX}_malloc_array_at_least {MACRO}(_malloc_array_at_least)
#define {PREFIX}_impl_malloc_array_at_least_ea {MACRO}(_impl_malloc_array_at_least_ea)
#define {PREFIX}_impl_malloc_array_at_least_fa {MACRO}(_impl_malloc_array_at_least_fa)

#define {PREFIX}_malloc_array {MACRO}(_malloc_array)
#define {PREFIX}_malloc_array_zero {MACRO}(_malloc_array_zero)
