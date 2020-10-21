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

// Renames lifecycle identifiers with prefix "{PREFIX}" using macro {MACRO}()

// types
#define {PREFIX}_value_t {MACRO}(_value_t)
#define {PREFIX}_ref_t {MACRO}(_ref_t)
#define {PREFIX}_const_ref_t {MACRO}(_const_ref_t)
#define {PREFIX}_context_t {MACRO}(_context_t)

// fundamental operations
#define {PREFIX}_init_steal {MACRO}(_init_steal)
#define {PREFIX}_init_copy {MACRO}(_init_copy)
#define {PREFIX}_init {MACRO}(_init)
#define {PREFIX}_destroy {MACRO}(_destroy)
#define {PREFIX}_swap {MACRO}(_swap)
#define {PREFIX}_move {MACRO}(_move)
#define {PREFIX}_steal {MACRO}(_steal)
#define {PREFIX}_copy {MACRO}(_copy)

// bulk operations
#define {PREFIX}_destroy_bulk {MACRO}(_destroy_bulk)
#define {PREFIX}_move_bulk {MACRO}(_move_bulk)
#define {PREFIX}_move_bulk_restrict {MACRO}(_move_bulk_restrict)
#define {PREFIX}_move_bulk_restrict_impl {MACRO}(_move_bulk_restrict_impl)
#define {PREFIX}_move_bulk_up {MACRO}(_move_bulk_up)
#define {PREFIX}_move_bulk_up_impl {MACRO}(_move_bulk_up_impl)
#define {PREFIX}_move_bulk_down {MACRO}(_move_bulk_down)
#define {PREFIX}_move_bulk_down_impl {MACRO}(_move_bulk_down_impl)

// C++
#define {PREFIX}_construct {MACRO}(_construct)
#define {PREFIX}_assign {MACRO}(_assign)
