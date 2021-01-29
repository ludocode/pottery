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
// Renames array identifiers from {SRC} to {DEST}

// types
#define {SRC}_t POTTERY_CONCAT({DEST}, _t)
#define {SRC}_entry_t POTTERY_CONCAT({DEST}, _entry_t)

// lifecycle operations
#define {SRC}_copy POTTERY_CONCAT({DEST}, _copy)
#define {SRC}_destroy POTTERY_CONCAT({DEST}, _destroy)
#define {SRC}_init POTTERY_CONCAT({DEST}, _init)
#define {SRC}_init_copy POTTERY_CONCAT({DEST}, _init_copy)
#define {SRC}_init_steal POTTERY_CONCAT({DEST}, _init_steal)
#define {SRC}_move POTTERY_CONCAT({DEST}, _move)
#define {SRC}_steal POTTERY_CONCAT({DEST}, _steal)
#define {SRC}_swap POTTERY_CONCAT({DEST}, _swap)

// entry operations
#define {SRC}_entry_exists POTTERY_CONCAT({DEST}, _entry_exists)
#define {SRC}_entry_equal POTTERY_CONCAT({DEST}, _entry_equal)
#define {SRC}_entry_ref POTTERY_CONCAT({DEST}, _entry_ref)

// lookup
#define {SRC}_count POTTERY_CONCAT({DEST}, _count)
#define {SRC}_is_empty POTTERY_CONCAT({DEST}, _is_empty)
#define {SRC}_at POTTERY_CONCAT({DEST}, _at)
#define {SRC}_find POTTERY_CONCAT({DEST}, _find)
#define {SRC}_contains_key POTTERY_CONCAT({DEST}, _contains_key)
#define {SRC}_first POTTERY_CONCAT({DEST}, _first)
#define {SRC}_last POTTERY_CONCAT({DEST}, _last)
#define {SRC}_any POTTERY_CONCAT({DEST}, _any)

// array access
#define {SRC}_select POTTERY_CONCAT({DEST}, _select)
#define {SRC}_index POTTERY_CONCAT({DEST}, _index)
#define {SRC}_shift POTTERY_CONCAT({DEST}, _shift)
#define {SRC}_offset POTTERY_CONCAT({DEST}, _offset)
#define {SRC}_begin POTTERY_CONCAT({DEST}, _begin)
#define {SRC}_end POTTERY_CONCAT({DEST}, _end)
#define {SRC}_next POTTERY_CONCAT({DEST}, _next)
#define {SRC}_previous POTTERY_CONCAT({DEST}, _previous)

// misc
#define {SRC}_capacity POTTERY_CONCAT({DEST}, _capacity)
#define {SRC}_reserve POTTERY_CONCAT({DEST}, _reserve)
#define {SRC}_shrink POTTERY_CONCAT({DEST}, _shrink)

// insert
#define {SRC}_insert POTTERY_CONCAT({DEST}, _insert)
#define {SRC}_insert_at POTTERY_CONCAT({DEST}, _insert_at)
#define {SRC}_insert_first POTTERY_CONCAT({DEST}, _insert_first)
#define {SRC}_insert_last POTTERY_CONCAT({DEST}, _insert_last)
#define {SRC}_insert_before POTTERY_CONCAT({DEST}, _insert_before)
#define {SRC}_insert_after POTTERY_CONCAT({DEST}, _insert_after)
#define {SRC}_insert_bulk POTTERY_CONCAT({DEST}, _insert_bulk)
#define {SRC}_insert_at_bulk POTTERY_CONCAT({DEST}, _insert_at_bulk)
#define {SRC}_insert_first_bulk POTTERY_CONCAT({DEST}, _insert_first_bulk)
#define {SRC}_insert_last_bulk POTTERY_CONCAT({DEST}, _insert_last_bulk)
#define {SRC}_insert_before_bulk POTTERY_CONCAT({DEST}, _insert_before_bulk)
#define {SRC}_insert_after_bulk POTTERY_CONCAT({DEST}, _insert_after_bulk)

// emplace
#define {SRC}_emplace POTTERY_CONCAT({DEST}, _emplace)
#define {SRC}_emplace_at POTTERY_CONCAT({DEST}, _emplace_at)
#define {SRC}_emplace_key POTTERY_CONCAT({DEST}, _emplace_key)
#define {SRC}_emplace_first POTTERY_CONCAT({DEST}, _emplace_first)
#define {SRC}_emplace_last POTTERY_CONCAT({DEST}, _emplace_last)
#define {SRC}_emplace_before POTTERY_CONCAT({DEST}, _emplace_before)
#define {SRC}_emplace_after POTTERY_CONCAT({DEST}, _emplace_after)
#define {SRC}_emplace_bulk POTTERY_CONCAT({DEST}, _emplace_bulk)
#define {SRC}_emplace_at_bulk POTTERY_CONCAT({DEST}, _emplace_at_bulk)
#define {SRC}_emplace_first_bulk POTTERY_CONCAT({DEST}, _emplace_first_bulk)
#define {SRC}_emplace_last_bulk POTTERY_CONCAT({DEST}, _emplace_last_bulk)
#define {SRC}_emplace_before_bulk POTTERY_CONCAT({DEST}, _emplace_before_bulk)
#define {SRC}_emplace_after_bulk POTTERY_CONCAT({DEST}, _emplace_after_bulk)

// construct
#define {SRC}_construct POTTERY_CONCAT({DEST}, _construct)
#define {SRC}_construct_at POTTERY_CONCAT({DEST}, _construct_at)
#define {SRC}_construct_key POTTERY_CONCAT({DEST}, _construct_key)
#define {SRC}_construct_first POTTERY_CONCAT({DEST}, _construct_first)
#define {SRC}_construct_last POTTERY_CONCAT({DEST}, _construct_last)
#define {SRC}_construct_before POTTERY_CONCAT({DEST}, _construct_before)
#define {SRC}_construct_after POTTERY_CONCAT({DEST}, _construct_after)

// extract
#define {SRC}_extract POTTERY_CONCAT({DEST}, _extract)
#define {SRC}_extract_at POTTERY_CONCAT({DEST}, _extract_at)
#define {SRC}_extract_key POTTERY_CONCAT({DEST}, _extract_key)
#define {SRC}_extract_first POTTERY_CONCAT({DEST}, _extract_first)
#define {SRC}_extract_first_bulk POTTERY_CONCAT({DEST}, _extract_first_bulk)
#define {SRC}_extract_last POTTERY_CONCAT({DEST}, _extract_last)
#define {SRC}_extract_last_bulk POTTERY_CONCAT({DEST}, _extract_last_bulk)
#define {SRC}_extract_and_next POTTERY_CONCAT({DEST}, _extract_and_next)
#define {SRC}_extract_and_previous POTTERY_CONCAT({DEST}, _extract_and_previous)

// remove
#define {SRC}_remove POTTERY_CONCAT({DEST}, _remove)
#define {SRC}_remove_at POTTERY_CONCAT({DEST}, _remove_at)
#define {SRC}_remove_key POTTERY_CONCAT({DEST}, _remove_key)
#define {SRC}_remove_first POTTERY_CONCAT({DEST}, _remove_first)
#define {SRC}_remove_last POTTERY_CONCAT({DEST}, _remove_last)
#define {SRC}_remove_and_next POTTERY_CONCAT({DEST}, _remove_and_next)
#define {SRC}_remove_and_previous POTTERY_CONCAT({DEST}, _remove_and_previous)
#define {SRC}_remove_all POTTERY_CONCAT({DEST}, _remove_all)
#define {SRC}_remove_bulk POTTERY_CONCAT({DEST}, _remove_bulk)
#define {SRC}_remove_at_bulk POTTERY_CONCAT({DEST}, _remove_at_bulk)
#define {SRC}_remove_first_bulk POTTERY_CONCAT({DEST}, _remove_first_bulk)
#define {SRC}_remove_last_bulk POTTERY_CONCAT({DEST}, _remove_last_bulk)
#define {SRC}_remove_at_bulk POTTERY_CONCAT({DEST}, _remove_at_bulk)
#define {SRC}_remove_and_next_bulk POTTERY_CONCAT({DEST}, _remove_and_next_bulk)
#define {SRC}_remove_and_previous_bulk POTTERY_CONCAT({DEST}, _remove_and_previous_bulk)

// displace
#define {SRC}_displace POTTERY_CONCAT({DEST}, _displace)
#define {SRC}_displace_at POTTERY_CONCAT({DEST}, _displace_at)
#define {SRC}_displace_key POTTERY_CONCAT({DEST}, _displace_key)
#define {SRC}_displace_first POTTERY_CONCAT({DEST}, _displace_first)
#define {SRC}_displace_last POTTERY_CONCAT({DEST}, _displace_last)
#define {SRC}_displace_and_next POTTERY_CONCAT({DEST}, _displace_and_next)
#define {SRC}_displace_and_previous POTTERY_CONCAT({DEST}, _displace_and_previous)
#define {SRC}_displace_all POTTERY_CONCAT({DEST}, _displace_all)
#define {SRC}_displace_bulk POTTERY_CONCAT({DEST}, _displace_bulk)
#define {SRC}_displace_at_bulk POTTERY_CONCAT({DEST}, _displace_at_bulk)
#define {SRC}_displace_first_bulk POTTERY_CONCAT({DEST}, _displace_first_bulk)
#define {SRC}_displace_last_bulk POTTERY_CONCAT({DEST}, _displace_last_bulk)
#define {SRC}_displace_and_next_bulk POTTERY_CONCAT({DEST}, _displace_and_next_bulk)
#define {SRC}_displace_and_previous_bulk POTTERY_CONCAT({DEST}, _displace_and_previous_bulk)

// link
#define {SRC}_link POTTERY_CONCAT({DEST}, _link)
#define {SRC}_link_first POTTERY_CONCAT({DEST}, _link_first)
#define {SRC}_link_last POTTERY_CONCAT({DEST}, _link_last)
#define {SRC}_link_before POTTERY_CONCAT({DEST}, _link_before)
#define {SRC}_link_after POTTERY_CONCAT({DEST}, _link_after)

// unlink
#define {SRC}_unlink POTTERY_CONCAT({DEST}, _unlink)
#define {SRC}_unlink_at POTTERY_CONCAT({DEST}, _unlink_at)
#define {SRC}_unlink_key POTTERY_CONCAT({DEST}, _unlink_key)
#define {SRC}_unlink_first POTTERY_CONCAT({DEST}, _unlink_first)
#define {SRC}_unlink_last POTTERY_CONCAT({DEST}, _unlink_last)
#define {SRC}_unlink_and_next POTTERY_CONCAT({DEST}, _unlink_and_next)
#define {SRC}_unlink_and_previous POTTERY_CONCAT({DEST}, _unlink_and_previous)
#define {SRC}_unlink_all POTTERY_CONCAT({DEST}, _unlink_all)
