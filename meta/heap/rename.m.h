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
// Renames heap identifiers from {SRC} to {DEST}

#define {SRC}_ref_t POTTERY_CONCAT({DEST}, _ref_t)
#define {SRC}_value_t POTTERY_CONCAT({DEST}, _value_t)
#define {SRC}_context_t POTTERY_CONCAT({DEST}, _context_t)
#define {SRC}_state_t POTTERY_CONCAT({DEST}, _state_t)

#define {SRC}_access POTTERY_CONCAT({DEST}, _access)
#define {SRC}_set_index POTTERY_CONCAT({DEST}, _set_index)

#define {SRC}_parent POTTERY_CONCAT({DEST}, _parent)
#define {SRC}_child_left POTTERY_CONCAT({DEST}, _child_left)
#define {SRC}_child_right POTTERY_CONCAT({DEST}, _child_right)
#define {SRC}_sift_down POTTERY_CONCAT({DEST}, _sift_down)
#define {SRC}_sift_up POTTERY_CONCAT({DEST}, _sift_up)
#define {SRC}_build_impl POTTERY_CONCAT({DEST}, _build_impl)
#define {SRC}_expand_bulk_impl POTTERY_CONCAT({DEST}, _expand_bulk_impl)
#define {SRC}_contract_bulk_impl POTTERY_CONCAT({DEST}, _contract_bulk_impl)
#define {SRC}_contract_at_impl POTTERY_CONCAT({DEST}, _contract_at_impl)
#define {SRC}_valid_count_impl POTTERY_CONCAT({DEST}, _valid_count_impl)
#define {SRC}_build POTTERY_CONCAT({DEST}, _build)
#define {SRC}_expand_bulk POTTERY_CONCAT({DEST}, _expand_bulk)
#define {SRC}_contract_bulk POTTERY_CONCAT({DEST}, _contract_bulk)
#define {SRC}_contract_at POTTERY_CONCAT({DEST}, _contract_at)
#define {SRC}_valid POTTERY_CONCAT({DEST}, _valid)
#define {SRC}_valid_count POTTERY_CONCAT({DEST}, _valid_count)
