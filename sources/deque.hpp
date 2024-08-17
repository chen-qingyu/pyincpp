//! @file deque.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Deque template class.
//! @date 2023.01.10

#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "utility.hpp"

#include <deque>

namespace pyincpp
{

/// Deque is generalization of stack and queue, supports memory efficient pushes and pops from either side.
template <typename T>
class Deque
{
private:
    // Deque.
    std::deque<T> deque_;

public:
    /*
     * Constructor
     */

    /// Construct an empty deque.
    Deque() = default;

    /// Construct a deque with the contents of the initializer list `init`.
    Deque(const std::initializer_list<T>& init)
        : deque_(init)
    {
    }

    /// Construct a deque with the contents of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    Deque(const InputIt& first, const InputIt& last)
        : deque_(first, last)
    {
    }

    /*
     * Comparison
     */

    /// Compare the deque with another deque.
    auto operator<=>(const Deque& that) const = default;

    /*
     * Iterator
     */

    /// Return an iterator to the first element of the deque.
    auto begin() const
    {
        return deque_.begin();
    }

    /// Return an iterator to the element following the last element of the deque.
    auto end() const
    {
        return deque_.end();
    }

    /// Return a reverse iterator to the first element of the reversed deque.
    auto rbegin() const
    {
        return deque_.rbegin();
    }

    /// Return a reverse iterator to the element following the last element of the reversed deque.
    auto rend() const
    {
        return deque_.rend();
    }

    /*
     * Access
     */

    /// Get the last element.
    T& back()
    {
        internal::check_empty(size());

        return deque_.back();
    }

    /// Peek the last element.
    const T& back() const
    {
        return const_cast<Deque&>(*this).back();
    }

    /// Get the first element.
    T& front()
    {
        internal::check_empty(size());

        return deque_.front();
    }

    /// Peek the first element.
    const T& front() const
    {
        return const_cast<Deque&>(*this).front();
    }

    /// Return a reference to the element at specified `index`.
    T& operator[](int index)
    {
        internal::check_bounds(index, -size(), size());

        return index >= 0 ? deque_[index] : deque_[index + size()];
    }

    /// Return a const reference to the element at specified `index`.
    const T& operator[](int index) const
    {
        return const_cast<Deque&>(*this)[index];
    }

    /*
     * Examination
     */

    /// Return the number of elements in the deque.
    int size() const
    {
        return deque_.size();
    }

    /// Return `true` if the deque is empty.
    bool is_empty() const
    {
        return deque_.empty();
    }

    /*
     * Manipulation
     */

    /// Append the given `element` to the end of the deque.
    void push_back(const T& element)
    {
        internal::check_full(size(), INT_MAX);

        deque_.push_back(element);
    }

    /// Prepend the given `element` to the beginning of the deque.
    void push_front(const T& element)
    {
        internal::check_full(size(), INT_MAX);

        deque_.push_front(element);
    }

    /// Remove and return the last element of the deque.
    /// If no elements are present, throws an error.
    T pop_back()
    {
        internal::check_empty(size());

        T data = std::move(deque_.back());
        deque_.pop_back();
        return data;
    }

    /// Remove and return the first element of the deque.
    /// If no elements are present, throws an error.
    T pop_front()
    {
        internal::check_empty(size());

        T data = std::move(deque_.front());
        deque_.pop_front();
        return data;
    }

    /// Extend the right side of the deque by appending elements of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    void extend_back(const InputIt& first, const InputIt& last)
    {
        deque_.insert(deque_.end(), first, last);
    }

    /// Extend the left side of the deque by prepending elements of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    void extend_front(const InputIt& first, const InputIt& last)
    {
        deque_.insert(deque_.begin(), first, last);
    }

    /// Rotate `n` elements to the right.
    Deque& operator>>=(int n)
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this <<= -n;
        }

        return *this <<= size() - n;
    }

    /// Rotate `n` elements to the left.
    Deque& operator<<=(int n)
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        n %= size();

        if (n < 0)
        {
            return *this >>= -n;
        }

        std::rotate(deque_.begin(), deque_.begin() + n, deque_.end());

        return *this;
    }

    /// Reverse the deque in place.
    Deque& reverse()
    {
        std::reverse(deque_.begin(), deque_.end());

        return *this;
    }

    /// Remove all elements from the deque.
    void clear()
    {
        deque_.clear();
    }

    /*
     * Print
     */

    /// Output the deque to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Deque& deque)
    {
        return internal::print(os, deque.begin(), deque.end(), '<', '>');
    }
};

} // namespace pyincpp

#endif // DEQUE_HPP
