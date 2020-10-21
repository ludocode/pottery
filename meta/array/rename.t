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

// Renames array identifiers with prefix "{PREFIX}" using macro {MACRO}()

// types
#define {PREFIX}_t {MACRO}(_t)
#define {PREFIX}_element_t {MACRO}(_element_t)
#define {PREFIX}_ref_t {MACRO}(_ref_t)
#define {PREFIX}_entry_t {MACRO}(_entry_t)

// lifecycle operations
#define {PREFIX}_copy {MACRO}(_copy)
#define {PREFIX}_destroy {MACRO}(_destroy)
#define {PREFIX}_init {MACRO}(_init)
#define {PREFIX}_init_copy {MACRO}(_init_copy)
#define {PREFIX}_init_steal {MACRO}(_init_steal)
#define {PREFIX}_move {MACRO}(_move)
#define {PREFIX}_steal {MACRO}(_steal)
#define {PREFIX}_swap {MACRO}(_swap)

// entry operations
#define {PREFIX}_entry_index {MACRO}(_entry_index)
#define {PREFIX}_entry_exists {MACRO}(_entry_exists)
#define {PREFIX}_entry_equal {MACRO}(_entry_equal)
#define {PREFIX}_entry_element {MACRO}(_entry_element)

// lookup
#define {PREFIX}_count {MACRO}(_count)
#define {PREFIX}_is_empty {MACRO}(_is_empty)
#define {PREFIX}_at {MACRO}(_at)
#define {PREFIX}_index {MACRO}(_index)
#define {PREFIX}_first {MACRO}(_first)
#define {PREFIX}_last {MACRO}(_last)
#define {PREFIX}_any {MACRO}(_any)

// iteration
#define {PREFIX}_begin {MACRO}(_begin)
#define {PREFIX}_end {MACRO}(_end)
#define {PREFIX}_next {MACRO}(_next)
#define {PREFIX}_previous {MACRO}(_previous)

// misc
#define {PREFIX}_reserve {MACRO}(_reserve)
#define {PREFIX}_shrink {MACRO}(_shrink)

// insert
#define {PREFIX}_insert_at {MACRO}(_insert_at)
#define {PREFIX}_insert_first {MACRO}(_insert_first)
#define {PREFIX}_insert_last {MACRO}(_insert_last)
#define {PREFIX}_insert_before {MACRO}(_insert_before)
#define {PREFIX}_insert_after {MACRO}(_insert_after)
#define {PREFIX}_insert_at_bulk {MACRO}(_insert_at_bulk)
#define {PREFIX}_insert_first_bulk {MACRO}(_insert_first_bulk)
#define {PREFIX}_insert_last_bulk {MACRO}(_insert_last_bulk)
#define {PREFIX}_insert_before_bulk {MACRO}(_insert_before_bulk)
#define {PREFIX}_insert_after_bulk {MACRO}(_insert_after_bulk)

// emplace
#define {PREFIX}_emplace_at {MACRO}(_emplace_at)
#define {PREFIX}_emplace_first {MACRO}(_emplace_first)
#define {PREFIX}_emplace_last {MACRO}(_emplace_last)
#define {PREFIX}_emplace_before {MACRO}(_emplace_before)
#define {PREFIX}_emplace_after {MACRO}(_emplace_after)
#define {PREFIX}_emplace_at_bulk {MACRO}(_emplace_at_bulk)
#define {PREFIX}_emplace_first_bulk {MACRO}(_emplace_first_bulk)
#define {PREFIX}_emplace_last_bulk {MACRO}(_emplace_last_bulk)
#define {PREFIX}_emplace_before_bulk {MACRO}(_emplace_before_bulk)
#define {PREFIX}_emplace_after_bulk {MACRO}(_emplace_after_bulk)

// construct
#define {PREFIX}_construct_at {MACRO}(_construct_at)
#define {PREFIX}_construct_first {MACRO}(_construct_first)
#define {PREFIX}_construct_last {MACRO}(_construct_last)
#define {PREFIX}_construct_before {MACRO}(_construct_before)
#define {PREFIX}_construct_after {MACRO}(_construct_after)

// extract
#define {PREFIX}_extract {MACRO}(_extract)
#define {PREFIX}_extract_at {MACRO}(_extract_at)
#define {PREFIX}_extract_first {MACRO}(_extract_first)
#define {PREFIX}_extract_last {MACRO}(_extract_last)
#define {PREFIX}_extract_and_next {MACRO}(_extract_and_next)
#define {PREFIX}_extract_and_previous {MACRO}(_extract_and_previous)

// remove
#define {PREFIX}_remove {MACRO}(_remove)
#define {PREFIX}_remove_at {MACRO}(_remove_at)
#define {PREFIX}_remove_first {MACRO}(_remove_first)
#define {PREFIX}_remove_last {MACRO}(_remove_last)
#define {PREFIX}_remove_and_next {MACRO}(_remove_and_next)
#define {PREFIX}_remove_and_previous {MACRO}(_remove_and_previous)
#define {PREFIX}_remove_all {MACRO}(_remove_all)
#define {PREFIX}_remove_at_bulk {MACRO}(_remove_at_bulk)
#define {PREFIX}_remove_first_bulk {MACRO}(_remove_first_bulk)
#define {PREFIX}_remove_last_bulk {MACRO}(_remove_last_bulk)
#define {PREFIX}_remove_at_bulk {MACRO}(_remove_at_bulk)

// displace
#define {PREFIX}_displace {MACRO}(_displace)
#define {PREFIX}_displace_at {MACRO}(_displace_at)
#define {PREFIX}_displace_first {MACRO}(_displace_first)
#define {PREFIX}_displace_last {MACRO}(_displace_last)
#define {PREFIX}_displace_and_next {MACRO}(_displace_and_next)
#define {PREFIX}_displace_and_previous {MACRO}(_displace_and_previous)
#define {PREFIX}_displace_all {MACRO}(_displace_all)
#define {PREFIX}_displace_at_bulk {MACRO}(_displace_at_bulk)
#define {PREFIX}_displace_first_bulk {MACRO}(_displace_first_bulk)
#define {PREFIX}_displace_last_bulk {MACRO}(_displace_last_bulk)
