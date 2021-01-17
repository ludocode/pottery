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
// Un-renames heap identifiers with prefix "{PREFIX}"

// internal
#undef {PREFIX}_set_index
#undef {PREFIX}_update_index
#undef {PREFIX}_parent
#undef {PREFIX}_child_left
#undef {PREFIX}_child_right
#undef {PREFIX}_sift_down
#undef {PREFIX}_sift_up

// public sub-range
#undef {PREFIX}_build_range
#undef {PREFIX}_expand_bulk_range
#undef {PREFIX}_contract_bulk_range
#undef {PREFIX}_contract_at_range
#undef {PREFIX}_valid_count_range
#undef {PREFIX}_valid_range

// public full-range
#undef {PREFIX}_build
#undef {PREFIX}_expand_bulk
#undef {PREFIX}_contract_bulk
#undef {PREFIX}_contract_at
#undef {PREFIX}_valid
#undef {PREFIX}_valid_count
