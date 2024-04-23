/**
 * @file Deque.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Deque template class.
 * @date 2023.01.10
 *
 * @copyright Copyright (C) 2023
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "utility.hpp"

#include <deque>

namespace pyincpp
{

/// Deques are a generalization of stacks and queues.
/// Deques support memory efficient pushes and pops from either side of the deque with approximately the same O(1) performance in either direction.
template <typename T>
class Deque
{
private:
    // Deque.
    std::deque<T> deque_;

public:
    /*
     * Constructor / Destructor
     */

    /// Default constructor. Construct an empty deque.
    Deque() = default;

    /// Construct the deque with the contents of the initializer list `init`.
    Deque(const std::initializer_list<T>& init)
        : deque_(init)
    {
    }

    /// Construct the deque with the contents of the range [`first`, `last`).
    template <typename InputIt>
    Deque(const InputIt& first, const InputIt& last)
        : deque_(first, last)
    {
    }

    /// Copy constructor.
    Deque(const Deque& that) = default;

    /// Move constructor.
    Deque(Deque&& that) = default;

    /*
     * Comparison
     */

    /// Compare two deques.
    auto operator<=>(const Deque& that) const = default;

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Deque& operator=(const Deque& that) = default;

    /// Move assignment operator.
    Deque& operator=(Deque&& that) = default;

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

    /// Return `true` if the deque contains no elements.
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
        deque_.push_back(element);
    }

    /// Prepend the given `element` to the beginning of the deque.
    void push_front(const T& element)
    {
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

        if (n < 0)
        {
            return *this >>= -n;
        }

        n %= size();

        std::rotate(deque_.begin(), deque_.begin() + n, deque_.end());

        return *this;
    }

    /// Reverse the deque in place.
    void reverse()
    {
        std::reverse(deque_.begin(), deque_.end());
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
        return internal::print(os, deque, '<', '>');
    }
};

} // namespace pyincpp

#endif // DEQUE_HPP
