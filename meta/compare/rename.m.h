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
// Renames compare identifiers from {SRC} to {DEST}

#define {SRC}_value_t POTTERY_CONCAT({DEST}, _value_t)
#define {SRC}_ref_t POTTERY_CONCAT({DEST}, _ref_t)
#define {SRC}_entry_t POTTERY_CONCAT({DEST}, _entry_t)
#define {SRC}_context_t POTTERY_CONCAT({DEST}, _context_t)

#define {SRC}_value POTTERY_CONCAT({DEST}, _value)

#define {SRC}_three_way POTTERY_CONCAT({DEST}, _three_way)
#define {SRC}_equal POTTERY_CONCAT({DEST}, _equal)
#define {SRC}_not_equal POTTERY_CONCAT({DEST}, _not_equal)
#define {SRC}_less POTTERY_CONCAT({DEST}, _less)
#define {SRC}_less_or_equal POTTERY_CONCAT({DEST}, _less_or_equal)
#define {SRC}_greater POTTERY_CONCAT({DEST}, _greater)
#define {SRC}_greater_or_equal POTTERY_CONCAT({DEST}, _greater_or_equal)

#define {SRC}_min POTTERY_CONCAT({DEST}, _min)
#define {SRC}_max POTTERY_CONCAT({DEST}, _max)
#define {SRC}_clamp POTTERY_CONCAT({DEST}, _clamp)
#define {SRC}_median POTTERY_CONCAT({DEST}, _median)
