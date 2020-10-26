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
// Renames insertion_sort identifiers from {SRC} to {DEST}

#define {SRC}_ref_t POTTERY_CONCAT({DEST}, _ref_t)
#define {SRC}_value_t POTTERY_CONCAT({DEST}, _value_t)
#define {SRC}_context_t POTTERY_CONCAT({DEST}, _context_t)
#define {SRC}_state_t POTTERY_CONCAT({DEST}, _state_t)

#define {SRC} POTTERY_CONCAT({DEST}, )
#define {SRC}_access POTTERY_CONCAT({DEST}, _access)
#define {SRC}_impl POTTERY_CONCAT({DEST}, _impl)
