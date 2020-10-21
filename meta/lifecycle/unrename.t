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

// Un-renames lifecycle identifiers with prefix "{PREFIX}"

// types
#undef {PREFIX}_ref_t
#undef {PREFIX}_const_ref_t
#undef {PREFIX}_value_t
#undef {PREFIX}_context_t

// fundamental operations
#undef {PREFIX}_init_steal
#undef {PREFIX}_init_copy
#undef {PREFIX}_init
#undef {PREFIX}_destroy
#undef {PREFIX}_swap
#undef {PREFIX}_move
#undef {PREFIX}_steal
#undef {PREFIX}_copy

// bulk operations
#undef {PREFIX}_move_bulk
#undef {PREFIX}_move_bulk_restrict
#undef {PREFIX}_move_bulk_restrict_impl
#undef {PREFIX}_move_bulk_up
#undef {PREFIX}_move_bulk_up_impl
#undef {PREFIX}_move_bulk_down
#undef {PREFIX}_move_bulk_down_impl

// C++
#undef {PREFIX}_construct
#undef {PREFIX}_assign
