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
// Forwards alloc configuration from {SRC} to {DEST}
#ifdef {SRC}_CONTEXT_TYPE
    #define {DEST}_CONTEXT_TYPE {SRC}_CONTEXT_TYPE
#endif

#ifdef {SRC}_FREE
    #define {DEST}_FREE {SRC}_FREE
#endif
#ifdef {SRC}_MALLOC
    #define {DEST}_MALLOC {SRC}_MALLOC
#endif
#ifdef {SRC}_ZALLOC
    #define {DEST}_ZALLOC {SRC}_ZALLOC
#endif
#ifdef {SRC}_REALLOC
    #define {DEST}_REALLOC {SRC}_REALLOC
#endif
#ifdef {SRC}_MALLOC_GOOD_SIZE
    #define {DEST}_GOOD_SIZE {SRC}_MALLOC_USABLE_SIZE
#endif

#ifdef {SRC}_ALIGNED_FREE
    #define {DEST}_ALIGNED_FREE {SRC}_ALIGNED_FREE
#endif
#ifdef {SRC}_ALIGNED_MALLOC
    #define {DEST}_ALIGNED_MALLOC {SRC}_ALIGNED_MALLOC
#endif
#ifdef {SRC}_ALIGNED_ZALLOC
    #define {DEST}_ALIGNED_ZALLOC {SRC}_ALIGNED_ZALLOC
#endif
#ifdef {SRC}_ALIGNED_REALLOC
    #define {DEST}_ALIGNED_REALLOC {SRC}_ALIGNED_REALLOC
#endif
#ifdef {SRC}_ALIGNED_MALLOC_GOOD_SIZE
    #define {DEST}_ALIGNED_GOOD_SIZE {SRC}_ALIGNED_MALLOC_USABLE_SIZE
#endif
