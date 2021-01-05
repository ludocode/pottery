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

// Configuration for Clayfish's vector of moves.

#define POTTERY_VECTOR_PREFIX moves
#define POTTERY_VECTOR_VALUE_TYPE move_t
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1

// Clayfish move lists are generally short lived and also sometimes re-used as
// scratch space. In either case we don't want them to bother shrinking; this
// would just slow things down.
#define POTTERY_VECTOR_AUTO_SHRINK 0

// This is a reasonable upper bound on the number of possible moves, but not a
// hard limit. The algorithm will slow down significantly in states where there
// are more available moves than this because it will have to malloc(). The
// average number of possible moves for reasonable gamestates is roughly 35. A
// move is only five bytes so the vector is still fairly small despite having
// space for pretty much all moves it will ever contain.
#define POTTERY_VECTOR_INTERNAL_CAPACITY 90
