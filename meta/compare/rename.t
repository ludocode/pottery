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
// Renames compare identifiers with prefix "{PREFIX}"
// using macro {MACRO}()
#define {PREFIX}_ref_t {MACRO}(_ref_t)
#define {PREFIX}_value_t {MACRO}(_value_t)
#define {PREFIX}_context_t {MACRO}(_context_t)

#define {PREFIX}_three_way {MACRO}(_three_way)
#define {PREFIX}_equal {MACRO}(_equal)
#define {PREFIX}_not_equal {MACRO}(_not_equal)
#define {PREFIX}_less {MACRO}(_less)
#define {PREFIX}_less_or_equal {MACRO}(_less_or_equal)
#define {PREFIX}_greater {MACRO}(_greater)
#define {PREFIX}_greater_or_equal {MACRO}(_greater_or_equal)

#define {PREFIX}_min {MACRO}(_min)
#define {PREFIX}_max {MACRO}(_max)
#define {PREFIX}_clamp {MACRO}(_clamp)
#define {PREFIX}_median {MACRO}(_median)
