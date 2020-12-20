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
// Renames lifecycle identifiers from {SRC} to {DEST}

#define {SRC}_init_steal POTTERY_CONCAT({DEST}, _init_steal)
#define {SRC}_init_copy POTTERY_CONCAT({DEST}, _init_copy)
#define {SRC}_init POTTERY_CONCAT({DEST}, _init)
#define {SRC}_destroy POTTERY_CONCAT({DEST}, _destroy)

#define {SRC}_swap POTTERY_CONCAT({DEST}, _swap)
#define {SRC}_move POTTERY_CONCAT({DEST}, _move)
#define {SRC}_steal POTTERY_CONCAT({DEST}, _steal)
#define {SRC}_copy POTTERY_CONCAT({DEST}, _copy)
#define {SRC}_swap_restrict POTTERY_CONCAT({DEST}, _swap_restrict)
#define {SRC}_move_restrict POTTERY_CONCAT({DEST}, _move_restrict)
#define {SRC}_steal_restrict POTTERY_CONCAT({DEST}, _steal_restrict)
#define {SRC}_copy_restrict POTTERY_CONCAT({DEST}, _copy_restrict)

#define {SRC}_destroy_bulk POTTERY_CONCAT({DEST}, _destroy_bulk)
#define {SRC}_move_bulk POTTERY_CONCAT({DEST}, _move_bulk)
#define {SRC}_move_bulk_restrict POTTERY_CONCAT({DEST}, _move_bulk_restrict)
#define {SRC}_move_bulk_restrict_impl POTTERY_CONCAT({DEST}, _move_bulk_restrict_impl)
#define {SRC}_move_bulk_up POTTERY_CONCAT({DEST}, _move_bulk_up)
#define {SRC}_move_bulk_up_impl POTTERY_CONCAT({DEST}, _move_bulk_up_impl)
#define {SRC}_move_bulk_down POTTERY_CONCAT({DEST}, _move_bulk_down)
#define {SRC}_move_bulk_down_impl POTTERY_CONCAT({DEST}, _move_bulk_down_impl)
