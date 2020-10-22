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

// Forwards lifecycle configuration from {SRC} to {DEST}

#ifdef {SRC}_BY_VALUE
    #define {DEST}_BY_VALUE {SRC}_BY_VALUE
#endif

#ifdef {SRC}_DESTROY
    #define {DEST}_DESTROY {SRC}_DESTROY
#endif
#ifdef {SRC}_DESTROY_BY_VALUE
    #define {DEST}_DESTROY_BY_VALUE {SRC}_DESTROY_BY_VALUE
#endif

#ifdef {SRC}_INIT
    #define {DEST}_INIT {SRC}_INIT
#endif
#ifdef {SRC}_INIT_BY_VALUE
    #define {DEST}_INIT_BY_VALUE {SRC}_INIT_BY_VALUE
#endif

#ifdef {SRC}_INIT_COPY
    #define {DEST}_INIT_COPY {SRC}_INIT_COPY
#endif
#ifdef {SRC}_INIT_COPY_BY_VALUE
    #define {DEST}_INIT_COPY_BY_VALUE {SRC}_INIT_COPY_BY_VALUE
#endif

#ifdef {SRC}_INIT_STEAL
    #define {DEST}_INIT_STEAL {SRC}_INIT_STEAL
#endif
#ifdef {SRC}_INIT_STEAL_BY_VALUE
    #define {DEST}_INIT_STEAL_BY_VALUE {SRC}_INIT_STEAL_BY_VALUE
#endif

#ifdef {SRC}_MOVE
    #define {DEST}_MOVE {SRC}_MOVE
#endif
#ifdef {SRC}_MOVE_BY_VALUE
    #define {DEST}_MOVE_BY_VALUE {SRC}_MOVE_BY_VALUE
#endif

#ifdef {SRC}_COPY
    #define {DEST}_COPY {SRC}_COPY
#endif
#ifdef {SRC}_COPY_BY_VALUE
    #define {DEST}_COPY_BY_VALUE {SRC}_COPY_BY_VALUE
#endif

#ifdef {SRC}_STEAL
    #define {DEST}_STEAL {SRC}_STEAL
#endif
#ifdef {SRC}_STEAL_BY_VALUE
    #define {DEST}_STEAL_BY_VALUE {SRC}_STEAL_BY_VALUE
#endif

#ifdef {SRC}_SWAP
    #define {DEST}_SWAP {SRC}_SWAP
#endif
#ifdef {SRC}_SWAP_BY_VALUE
    #define {DEST}_SWAP_BY_VALUE {SRC}_SWAP_BY_VALUE
#endif
