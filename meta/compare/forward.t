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
// Forwards compare configuration from {SRC} to {DEST}

// Note that ref/value types are not forwarded because templates may define
// either REF_TYPE or VALUE_TYPE, and dynamic containers usually define
// VALUE_TYPE as their ELEMENT_TYPE.

#ifdef {SRC}_CONTEXT_TYPE
    #define {DEST}_CONTEXT_TYPE {SRC}_CONTEXT_TYPE
#endif
#ifdef {SRC}_BY_VALUE
    #define {DEST}_BY_VALUE {SRC}_BY_VALUE
#endif

#ifdef {SRC}_EQUAL
    #define {DEST}_EQUAL {SRC}_EQUAL
#endif
#ifdef {SRC}_NOT_EQUAL
    #define {DEST}_NOT_EQUAL {SRC}_NOT_EQUAL
#endif
#ifdef {SRC}_LESS
    #define {DEST}_LESS {SRC}_LESS
#endif
#ifdef {SRC}_LESS_OR_EQUAL
    #define {DEST}_LESS_OR_EQUAL {SRC}_LESS_OR_EQUAL
#endif
#ifdef {SRC}_GREATER
    #define {DEST}_GREATER {SRC}_GREATER
#endif
#ifdef {SRC}_GREATER_OR_EQUAL
    #define {DEST}_GREATER_OR_EQUAL {SRC}_GREATER_OR_EQUAL
#endif
#ifdef {SRC}_THREE_WAY
    #define {DEST}_THREE_WAY {SRC}_THREE_WAY
#endif
