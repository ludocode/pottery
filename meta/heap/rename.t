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
// Renames insertion_sort identifiers with prefix "{PREFIX}"
// using macro {MACRO}()
#define {PREFIX}_ref_t {MACRO}(_ref_t)
#define {PREFIX}_value_t {MACRO}(_value_t)
#define {PREFIX}_context_t {MACRO}(_context_t)
#define {PREFIX}_state_t {MACRO}(_state_t)

#define {PREFIX} {MACRO}()
#define {PREFIX}_access {MACRO}(_access)
#define {PREFIX}_before {MACRO}(_before)
#define {PREFIX}_set_index {MACRO}(_set_index)
#define {PREFIX}_move {MACRO}(_move)
#define {PREFIX}_swap {MACRO}(_swap)

#define {PREFIX}_parent {MACRO}(_parent)
#define {PREFIX}_child_left {MACRO}(_child_left)
#define {PREFIX}_child_right {MACRO}(_child_right)
#define {PREFIX}_sift_down {MACRO}(_sift_down)
#define {PREFIX}_sift_up {MACRO}(_sift_up)
#define {PREFIX}_build_impl {MACRO}(_build_impl)
#define {PREFIX}_push_impl {MACRO}(_push_impl)
#define {PREFIX}_pop_impl {MACRO}(_pop_impl)
#define {PREFIX}_remove_impl {MACRO}(_remove_impl)
#define {PREFIX}_valid_impl {MACRO}(_valid_impl)
#define {PREFIX}_valid_count_impl {MACRO}(_valid_count_impl)
#define {PREFIX}_build {MACRO}(_build)
#define {PREFIX}_push {MACRO}(_push)
#define {PREFIX}_pop {MACRO}(_pop)
#define {PREFIX}_remove {MACRO}(_remove)
#define {PREFIX}_valid {MACRO}(_valid)
#define {PREFIX}_valid_count {MACRO}(_valid_count)
