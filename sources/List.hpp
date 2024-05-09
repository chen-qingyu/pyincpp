//! @file List.hpp
//! @author Qingyu Chen (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
//! @brief List template class, implemented by dynamic array.
//! @date 2023.01.05
//!
//! @copyright Copyright (C) 2023 - 2024
//!
//! This program is free software: you can redistribute it and/or modify
//! it under the terms of the GNU General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//!
//! This program is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//! GNU General Public License for more details.
//!
//! You should have received a copy of the GNU General Public License
//! along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef LIST_HPP
#define LIST_HPP

#include "utility.hpp"

#include <vector>

namespace pyincpp
{

/// Lists are mutable sequences, typically used to store collections of homogeneous items.
template <typename T>
class List
{
private:
    // Vector.
    std::vector<T> vector_;

    // Create a list from std::vector.
    List(std::vector<T>&& vector)
        : vector_(vector)
    {
    }

public:
    /*
     * Constructor / Destructor
     */

    /// Default constructor. Construct an empty list.
    List() = default;

    /// Construct a list with the contents of the initializer list `init`.
    List(const std::initializer_list<T>& init)
        : vector_(init)
    {
    }

    /// Construct a list with the contents of the range [`first`, `last`).
    List(std::input_iterator auto first, std::input_iterator auto last)
        : vector_(first, last)
    {
    }

    /// Copy constructor.
    List(const List& that) = default;

    /// Move constructor.
    List(List&& that) = default;

    /*
     * Comparison
     */

    /// Compare the list with another list.
    constexpr auto operator<=>(const List& that) const = default;

    /*
     * Assignment
     */

    /// Copy assignment operator.
    List& operator=(const List& that) = default;

    /// Move assignment operator.
    List& operator=(List&& that) = default;

    /*
     * Iterator
     */

    /// Return an iterator to the first element of the list.
    constexpr auto begin() const
    {
        return vector_.begin();
    }

    /// Return an iterator to the element following the last element of the list.
    constexpr auto end() const
    {
        return vector_.end();
    }

    /// Return a reverse iterator to the first element of the reversed list.
    constexpr auto rbegin() const
    {
        return vector_.rbegin();
    }

    /// Return a reverse iterator to the element following the last element of the reversed list.
    constexpr auto rend() const
    {
        return vector_.rend();
    }

    /*
     * Access
     */

    /// Return the reference to the element at the specified position in the list.
    /// Index can be negative, like Python's list: list[-1] gets the last element.
    T& operator[](int index)
    {
        internal::check_bounds(index, -size(), size());

        return vector_[index >= 0 ? index : index + size()];
    }

    /// Return the const reference to element at the specified position in the list.
    /// Index can be negative, like Python's list: list[-1] gets the last element.
    constexpr const T& operator[](int index) const
    {
        return const_cast<List&>(*this)[index];
    }

    /*
     * Examination
     */

    /// Return the number of elements in the list.
    constexpr int size() const
    {
        return vector_.size();
    }

    /// Return `true` if the list contains no elements.
    constexpr bool is_empty() const
    {
        return vector_.empty();
    }

    /// Return the iterator of the specified element in the list, or end() if the list does not contain the element.
    constexpr auto find(const T& element) const
    {
        return std::find(begin(), end(), element);
    }

    /// Return the index of the first occurrence of the specified `element`, or -1 if the list does not contain the element in the specified range [`start`, `stop`].
    constexpr int index(const T& element, int start = 0, int stop = INT_MAX) const
    {
        stop = stop > size() ? size() : stop;
        auto it = std::find(begin() + start, begin() + stop, element);
        return it == begin() + stop ? -1 : it - begin();
    }

    /// Return `true` if the list contains the specified `element` in the specified range [`start`, `stop`].
    constexpr bool contains(const T& element, int start = 0, int stop = INT_MAX) const
    {
        return index(element, start, stop) != -1;
    }

    /// Count the total number of occurrences of the specified `element` in the list.
    constexpr int count(const T& element) const
    {
        return std::count_if(begin(), end(), [&](const T& e)
                             { return e == element; });
    }

    /*
     * Manipulation
     */

    /// Insert the specified `element` at the specified `index` in the list.
    /// Index can be negative.
    void insert(int index, const T& element)
    {
        internal::check_full(size(), INT_MAX);
        internal::check_bounds(index, -size(), size() + 1);

        index = index >= 0 ? index : index + size();
        vector_.insert(begin() + index, element);
    }

    /// Remove the `element` at the specified `index` in the list.
    /// Index can be negative.
    T remove(int index)
    {
        internal::check_empty(size());
        internal::check_bounds(index, -size(), size());

        index = index >= 0 ? index : index + size();
        T element = std::move(vector_[index]);
        vector_.erase(begin() + index);

        return element;
    }

    /// Append the specified `element` to the end of the list.
    List& operator+=(const T& element)
    {
        internal::check_full(size(), INT_MAX);

        vector_.insert(end(), element);

        return *this;
    }

    /// Extend the specified `list` to the end of the list.
    List& operator+=(const List& list)
    {
        internal::check_full(size() / 2 + list.size() / 2, INT_MAX / 2);

        vector_.insert(end(), list.begin(), list.end());

        return *this;
    }

    /// Remove the first occurrence of the specified element from the list.
    List& operator-=(const T& element)
    {
        if (auto it = std::find(begin(), end(), element); it != end())
        {
            vector_.erase(it);
        }

        return *this;
    }

    /// Add the list to itself a certain number of `times`.
    List& operator*=(int times)
    {
        return *this = std::move(*this * times);
    }

    /// Remove all the specified `element`s from the list.
    List& operator/=(const T& element)
    {
        auto it = std::remove(vector_.begin(), vector_.end(), element);
        vector_.erase(it, vector_.end());
        return *this;
    }

    /// Rotate the list to right `n` elements.
    List& operator>>=(int n)
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

    /// Rotate the list to left `n` elements.
    List& operator<<=(int n)
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

        std::rotate(vector_.begin(), vector_.begin() + n, vector_.end());

        return *this;
    }

    /// Reverse the list in place.
    List& reverse()
    {
        std::reverse(vector_.begin(), vector_.end());

        return *this;
    }

    /// Eliminate duplicate elements of the list.
    /// Will not change the original relative order of elements.
    List& uniquify()
    {
        std::vector<T> buffer;
        for (auto&& e : vector_)
        {
            if (std::find(buffer.begin(), buffer.end(), e) == buffer.end())
            {
                buffer.push_back(e);
            }
        }
        vector_ = std::move(buffer);

        return *this;
    }

    /// Sort the list according to the order induced by the specified comparator.
    /// The sort is stable: the method will not reorder equal elements.
    List& sort(bool (*comparator)(const T& e1, const T& e2) = [](const T& e1, const T& e2)
               { return e1 < e2; })
    {
        std::stable_sort(vector_.begin(), vector_.end(), comparator);

        return *this;
    }

    /// Erase the contents of the range [`start`, `stop`) of the list.
    List& erase(int start, int stop)
    {
        internal::check_bounds(start, 0, size() + 1);
        internal::check_bounds(stop, 0, size() + 1);

        vector_.erase(vector_.begin() + start, vector_.begin() + stop);

        return *this;
    }

    /// Perform the given `action` for each element of the list.
    template <typename F>
    List& map(const F& action)
    {
        std::for_each(vector_.begin(), vector_.end(), action);

        return *this;
    }

    /// Filter the elements in the list so that the elements that meet the `predicate` are retained.
    template <typename F>
    List& filter(const F& predicate)
    {
        auto it = std::copy_if(vector_.begin(), vector_.end(), vector_.begin(), predicate);
        vector_.erase(it, vector_.end());

        return *this;
    }

    /// Extend the list by appending elements of the range [`first`, `last`).
    void extend(std::input_iterator auto first, std::input_iterator auto last)
    {
        vector_.insert(vector_.end(), first, last);
    }

    /// Remove all of the elements from the list.
    void clear()
    {
        vector_.clear();
    }

    /*
     * Production
     */

    /// Return slice of the list from `start` (included) to `stop` (excluded) with certain `step` (default 1).
    /// Index and step length can be negative.
    List slice(int start, int stop, int step = 1) const
    {
        if (step == 0)
        {
            throw std::runtime_error("Error: Slice step can not be zero.");
        }

        internal::check_bounds(start, -size(), size());
        internal::check_bounds(stop, -size() - 1, size() + 1);

        // convert
        start = start < 0 ? start + size() : start;
        stop = stop < 0 ? stop + size() : stop;

        // copy
        std::vector<T> buffer;
        for (int i = start; (step > 0) ? (i < stop) : (i > stop); i += step)
        {
            buffer.push_back(vector_[i]);
        }

        return buffer;
    }

    /// Generate a new list and append the specified `element` to the end of the list.
    List operator+(const T& element) const
    {
        return List(*this) += element;
    }

    /// Generate a new list and extend the specified `list` to the end of the list.
    List operator+(const List& list) const
    {
        return List(*this) += list;
    }

    /// Generate a new list and remove the first occurrence of the specified `element` from the list.
    List operator-(const T& element) const
    {
        return List(*this) -= element;
    }

    /// Generate a new list and add the list to itself a certain number of `times`.
    List operator*(int times) const
    {
        if (times < 0)
        {
            throw std::runtime_error("Error: Times to repeat can not be less than zero.");
        }

        internal::check_full(size() * times, INT_MAX);

        std::vector<T> buffer(size() * times);
        for (int part = 0; part < times; part++)
        {
            std::copy(begin(), end(), buffer.begin() + size() * part);
        }

        return buffer;
    }

    /// Generate a new list and remove all the specified `elements` from the list.
    List operator/(const T& element) const
    {
        return List(*this) /= element;
    }

    /*
     * Print
     */

    /// Output the list to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const List& list)
    {
        return internal::print(os, list.begin(), list.end(), '[', ']');
    }
};

} // namespace pyincpp

#endif // LIST_HPP
