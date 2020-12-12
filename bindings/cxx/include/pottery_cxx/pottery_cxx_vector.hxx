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

#ifndef POTTERY_CXX_ALGORITHMS_VECTOR_HXX
#define POTTERY_CXX_ALGORITHMS_VECTOR_HXX 1

/*
 * Wraps Pottery's vector in a C++ template.
 *
 * Use pottery::vector as a mostly drop-in replacement for std::vector
 * with some caveats:
 *
 * - Custom allocators, comparison operators, and some newer functions are not
 *   supported (yet).
 *
 * - Throwing move is not supported. If a move constructor throws, the program
 *   aborts. It is strongly recommended that your move constructor be marked
 *   noexcept.
 */

#include "pottery_cxx/pottery_cxx_dependencies.hxx"

namespace pottery {

namespace detail {

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 0

template <typename T>
struct vector_wrapper {
    #define POTTERY_VECTOR_PREFIX cvector
    #define POTTERY_VECTOR_VALUE_TYPE T

    // C++ std::vector requires that erase() and pop_back() don't invalidate
    // iterators to elements before the element being removed. In order to be
    // compatible with it we can't automatically shrink.
    #define POTTERY_VECTOR_AUTO_SHRINK 0

    // Note that we use `BY_VALUE` even if the type isn't copyable or
    // default-constructible. C++ doesn't instantiate functions that aren't
    // used so it doesn't matter if they don't compile.
    #define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1

    // We want to enable construct_*(), insert() overloads, etc.
    #define POTTERY_VECTOR_CXX 1

    #include "pottery/vector/pottery_vector_static.t.h"
};

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 1

} // namespace detail

template <typename T>
class vector {
public:
    typedef detail::vector_wrapper<T> wrapper;
    typedef T* iterator;
    typedef const T* const_iterator;

    // rule of 5

    ~vector() {
        wrapper::cvector_destroy(&cvector);
    }

    vector(const vector& other) {
        error_to_exception(wrapper::cvector_init_copy(&cvector, &other.cvector));
    }

    vector(vector&& other) {
        wrapper::cvector_init_steal(&cvector, &other.cvector);
    }

    vector& operator=(const vector& other) {
        error_to_exception(wrapper::cvector_copy(&cvector, &other.cvector));
        return *this;
    }

    vector& operator=(vector&& other) {
        error_to_exception(wrapper::cvector_steal(&cvector, &other.cvector));
        return *this;
    }

    // other constructors

    vector() {
        wrapper::cvector_init(&cvector);
    }

    // methods

    void swap(vector& other) {
        wrapper::cvector_swap(&cvector, &other.cvector);
    }

    void clear() {
        wrapper::cvector_clear(&cvector);
    }

    size_t size() const {
        return wrapper::cvector_count(&cvector);
    }

    const T& operator[](size_t index) const {
        return *wrapper::cvector_at(&cvector, index);
    }

    T& operator[](size_t index) {
        return *wrapper::cvector_at(&cvector, index);
    }

    T& at(size_t index) {
        // at() is supposed to be bounds checked.
        if (index >= size()) {
            #if POTTERY_CXX_EXCEPTIONS
            throw std::out_of_range("");
            #else
            terminate();
            #endif
        }
        return *wrapper::cvector_at(&cvector, index);
    }

    const T& front() const {
        return *wrapper::cvector_first(pottery_const_cast(typename wrapper::cvector_t*, &cvector));
    }

    T& front() {
        return *wrapper::cvector_first(&cvector);
    }

    const T& back() const {
        return *wrapper::cvector_last(pottery_const_cast(typename wrapper::cvector_t*, &cvector));
    }

    T& back() {
        return *wrapper::cvector_last(&cvector);
    }

    const T* data() const noexcept {
        return wrapper::cvector_begin(pottery_const_cast(typename wrapper::cvector_t*, &cvector));
    }

    T* data() noexcept {
        return wrapper::cvector_begin(&cvector);
    }

    pottery_nodiscard
    bool empty() const noexcept {
        return wrapper::cvector_is_empty(&cvector);
    }

    void reserve(size_t total_capacity) {
        wrapper::cvector_reserve(&cvector, total_capacity);
    }

    size_t capacity() const noexcept {
        return wrapper::cvector_capacity(&cvector);
    }

    void shrink_to_fit() const noexcept {
        wrapper::cvector_shrink(&cvector);
    }

    iterator insert(const_iterator pos, const T& value) {
        size_t index = pottery_cast(size_t, pos - data());
        iterator ref;
        error_to_exception(wrapper::cvector_construct_at(&cvector, index, &ref, value));
        return ref;
    }

    iterator insert(const_iterator pos, T&& value) {
        size_t index = pottery_cast(size_t, pos - data());
        iterator ref;
        error_to_exception(wrapper::cvector_construct_at(&cvector, index, &ref, std::move(value)));
        return ref;
    }

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_t index = pottery_cast(size_t, pos - data());
        iterator ref;
        error_to_exception(wrapper::cvector_construct_at(&cvector, index, &ref, std::forward<Args>(args)...));
        return ref;
    }

    template<typename... Args>
    iterator emplace_back(Args&&... args) {
        iterator ref;
        error_to_exception(wrapper::cvector_construct_last(&cvector, &ref, std::forward<Args>(args)...));
        return ref;
    }

    iterator erase(const_iterator pos) {
        size_t index = pottery_cast(size_t, pos - data());
        wrapper::cvector_remove_at(&cvector, index);
        return data() + index;
    }

    iterator erase(const_iterator first, const_iterator last) {
        size_t index = pottery_cast(size_t, first - data());
        wrapper::cvector_remove_bulk(&cvector, index, last - first);
        return data() + index;
    }

    void push_back(const T& value) {
        iterator ref;
        error_to_exception(wrapper::cvector_construct_last(&cvector, &ref, value));
    }

    void push_back(T&& value) {
        iterator ref;
        error_to_exception(wrapper::cvector_construct_last(&cvector, &ref, std::move(value)));
    }

    void pop_back() {
        wrapper::cvector_remove_at(&cvector, size() - 1);
    }

    iterator begin() noexcept {
        return data();
    }

    const_iterator begin() const noexcept {
        return data();
    }

    const_iterator cbegin() const noexcept {
        return data();
    }

    iterator end() noexcept {
        return data() + size();
    }

    const_iterator end() const noexcept {
        return data() + size();
    }

    const_iterator cend() const noexcept {
        return data() + size();
    }

private:
    typename wrapper::cvector_t cvector;
};

// ADL swap function
template <class T>
void swap(pottery::vector<T>& left, pottery::vector<T>& right) {
    left.swap(right);
}

} // namespace pottery

#endif
