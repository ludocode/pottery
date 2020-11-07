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
// Renames open_hash_table identifiers from {SRC} to {DEST}

#define {SRC}_ref_t POTTERY_CONCAT({DEST}, _ref_t)
#define {SRC}_value_t POTTERY_CONCAT({DEST}, _value_t)
#define {SRC}_key_t POTTERY_CONCAT({DEST}, _key_t)
#define {SRC}_context_t POTTERY_CONCAT({DEST}, _context_t)

#define {SRC}_emplace POTTERY_CONCAT({DEST}, _emplace)
#define {SRC}_insert POTTERY_CONCAT({DEST}, _insert)
#define {SRC}_find POTTERY_CONCAT({DEST}, _find)
#define {SRC}_displace POTTERY_CONCAT({DEST}, _displace)
#define {SRC}_remove POTTERY_CONCAT({DEST}, _remove)
#define {SRC}_remove_key POTTERY_CONCAT({DEST}, _remove_key)
#define {SRC}_contains_key POTTERY_CONCAT({DEST}, _contains_key)

#define {SRC}_begin POTTERY_CONCAT({DEST}, _begin)
#define {SRC}_end POTTERY_CONCAT({DEST}, _end)
#define {SRC}_first POTTERY_CONCAT({DEST}, _first)
#define {SRC}_last POTTERY_CONCAT({DEST}, _last)
#define {SRC}_next POTTERY_CONCAT({DEST}, _next)
#define {SRC}_previous POTTERY_CONCAT({DEST}, _previous)

#define {SRC}_access_at POTTERY_CONCAT({DEST}, _access_at)
#define {SRC}_access_index POTTERY_CONCAT({DEST}, _access_index)
#define {SRC}_access_next POTTERY_CONCAT({DEST}, _access_next)
#define {SRC}_access_previous POTTERY_CONCAT({DEST}, _access_previous)

#define {SRC}_key POTTERY_CONCAT({DEST}, _key)
#define {SRC}_key_hash POTTERY_CONCAT({DEST}, _key_hash)
#define {SRC}_key_equal POTTERY_CONCAT({DEST}, _key_equal)
#define {SRC}_double_hash_interval POTTERY_CONCAT({DEST}, _double_hash_interval)
#define {SRC}_ref_is_tombstone POTTERY_CONCAT({DEST}, _ref_is_tombstone)
#define {SRC}_ref_set_tombstone POTTERY_CONCAT({DEST}, _ref_set_tombstone)
#define {SRC}_ref_is_empty POTTERY_CONCAT({DEST}, _ref_is_empty)
#define {SRC}_ref_set_empty POTTERY_CONCAT({DEST}, _ref_set_empty)
#define {SRC}_ref_is_element POTTERY_CONCAT({DEST}, _ref_is_element)
#define {SRC}_ref_in_use POTTERY_CONCAT({DEST}, _ref_in_use)
#define {SRC}_ref_exists POTTERY_CONCAT({DEST}, _ref_exists)

#define {SRC}_bucket_for_hash POTTERY_CONCAT({DEST}, _bucket_for_hash)
#define {SRC}_next_probe POTTERY_CONCAT({DEST}, _next_probe)
#define {SRC}_probe POTTERY_CONCAT({DEST}, _probe)
