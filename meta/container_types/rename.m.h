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

//!!! END_LICENSE
// Renames container_types template identifiers from {SRC} to {DEST}

#define {SRC}_value_t POTTERY_CONCAT({DEST}, _value_t)
#define {SRC}_ref_t POTTERY_CONCAT({DEST}, _ref_t)
#define {SRC}_const_ref_t POTTERY_CONCAT({DEST}, _const_ref_t)
#define {SRC}_entry_t POTTERY_CONCAT({DEST}, _entry_t)
#define {SRC}_key_t POTTERY_CONCAT({DEST}, _key_t)
#define {SRC}_context_t POTTERY_CONCAT({DEST}, _context_t)

#define {SRC}_entry_ref POTTERY_CONCAT({DEST}, _entry_ref)
#define {SRC}_ref_key POTTERY_CONCAT({DEST}, _ref_key)
#define {SRC}_ref_equal POTTERY_CONCAT({DEST}, _ref_equal)
