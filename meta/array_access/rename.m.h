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
// Renames array_access identifiers from {SRC} to {DEST}

#define {SRC}_value_t POTTERY_CONCAT({DEST}, _value_t)
#define {SRC}_ref_t POTTERY_CONCAT({DEST}, _ref_t)
#define {SRC}_entry_t POTTERY_CONCAT({DEST}, _entry_t)
#define {SRC}_context_t POTTERY_CONCAT({DEST}, _context_t)

#define {SRC}_begin POTTERY_CONCAT({DEST}, _begin)
#define {SRC}_end POTTERY_CONCAT({DEST}, _end)
#define {SRC}_count POTTERY_CONCAT({DEST}, _count)

#define {SRC}_select POTTERY_CONCAT({DEST}, _select)
#define {SRC}_index POTTERY_CONCAT({DEST}, _index)

#define {SRC}_shift POTTERY_CONCAT({DEST}, _shift)
#define {SRC}_offset POTTERY_CONCAT({DEST}, _offset)
#define {SRC}_next POTTERY_CONCAT({DEST}, _next)
#define {SRC}_previous POTTERY_CONCAT({DEST}, _previous)

#define {SRC}_ref POTTERY_CONCAT({DEST}, _ref)
#define {SRC}_equal POTTERY_CONCAT({DEST}, _equal)
#define {SRC}_exists POTTERY_CONCAT({DEST}, _exists)
