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
// Forwards array_access configuration from {SRC} to {DEST}

// absolute addressing configs
#ifdef {SRC}_BEGIN
    #define {DEST}_BEGIN {SRC}_BEGIN
#endif
#ifdef {SRC}_END
    #define {DEST}_END {SRC}_END
#endif
#ifdef {SRC}_COUNT
    #define {DEST}_COUNT {SRC}_COUNT
#endif

// required configs for non-standard array access
#ifdef {SRC}_SELECT
    #define {DEST}_SELECT {SRC}_SELECT
#endif
#ifdef {SRC}_INDEX
    #define {DEST}_INDEX {SRC}_INDEX
#endif

// required config for non-trivial entries
#ifdef {SRC}_EQUAL
    #define {DEST}_EQUAL {SRC}_EQUAL
#endif

// optional configs
#ifdef {SRC}_SHIFT
    #define {DEST}_SHIFT {SRC}_SHIFT
#endif
#ifdef {SRC}_OFFSET
    #define {DEST}_OFFSET {SRC}_OFFSET
#endif
#ifdef {SRC}_NEXT
    #define {DEST}_NEXT {SRC}_NEXT
#endif
#ifdef {SRC}_PREVIOUS
    #define {DEST}_PREVIOUS {SRC}_PREVIOUS
#endif
#ifdef {SRC}_EXISTS
    #define {DEST}_EXISTS {SRC}_EXISTS
#endif
