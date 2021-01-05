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
// Un-renames open_hash_table identifiers with prefix "{PREFIX}"

#undef {PREFIX}_value_t
#undef {PREFIX}_ref_t
#undef {PREFIX}_entry_t
#undef {PREFIX}_context_t

#undef {PREFIX}_key_t

#undef {PREFIX}_emplace
#undef {PREFIX}_insert
#undef {PREFIX}_find
#undef {PREFIX}_displace
#undef {PREFIX}_displace_all
#undef {PREFIX}_remove
#undef {PREFIX}_remove_key
#undef {PREFIX}_remove_all
#undef {PREFIX}_destroy_all
#undef {PREFIX}_contains_key

#undef {PREFIX}_begin
#undef {PREFIX}_end
#undef {PREFIX}_first
#undef {PREFIX}_last
#undef {PREFIX}_next
#undef {PREFIX}_previous

#undef {PREFIX}_key
#undef {PREFIX}_key_hash
#undef {PREFIX}_key_equal
#undef {PREFIX}_double_hash_interval
#undef {PREFIX}_entry_is_tombstone
#undef {PREFIX}_entry_set_tombstone
#undef {PREFIX}_entry_is_empty
#undef {PREFIX}_entry_set_empty
#undef {PREFIX}_entry_is_element
#undef {PREFIX}_entry_in_use
#undef {PREFIX}_entry_exists

#undef {PREFIX}_bucket_for_hash
#undef {PREFIX}_next_probe
#undef {PREFIX}_probe
