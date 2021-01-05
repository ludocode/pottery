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

// Forwards open_hash_table configuration from {SRC} to {DEST}
// Note that we don't forward types. These are forwarded (and changed)
// separately by wrapping containers.

#ifdef {SRC}_KEY_TYPE
    #define {DEST}_KEY_TYPE {SRC}_KEY_TYPE
#endif

#ifdef {SRC}_KEY_HASH
    #define {DEST}_KEY_HASH {SRC}_KEY_HASH
#endif
#ifdef {SRC}_REF_KEY
    #define {DEST}_REF_KEY {SRC}_REF_KEY
#endif
#ifdef {SRC}_KEY_EQUAL
    #define {DEST}_KEY_EQUAL {SRC}_KEY_EQUAL
#endif

#ifdef {SRC}_IS_EMPTY
    #define {DEST}_IS_EMPTY {SRC}_IS_EMPTY
#endif
#ifdef {SRC}_SET_EMPTY
    #define {DEST}_SET_EMPTY {SRC}_SET_EMPTY
#endif

#ifdef {SRC}_IS_TOMBSTONE
    #define {DEST}_IS_TOMBSTONE {SRC}_IS_TOMBSTONE
#endif
#ifdef {SRC}_SET_TOMBSTONE
    #define {DEST}_SET_TOMBSTONE {SRC}_SET_TOMBSTONE
#endif

#ifdef {SRC}_IS_VALUE
    #define {DEST}_IS_VALUE {SRC}_IS_ELEMENT
#endif

#ifdef {SRC}_LINEAR_PROBING
    #define {DEST}_LINEAR_PROBING {SRC}_LINEAR_PROBING
#endif
#ifdef {SRC}_QUADRATIC_PROBING
    #define {DEST}_QUADRATIC_PROBING {SRC}_QUADRATIC_PROBING
#endif
#ifdef {SRC}_DOUBLE_HASHING
    #define {DEST}_DOUBLE_HASHING {SRC}_DOUBLE_HASHING
#endif
