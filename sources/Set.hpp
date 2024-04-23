/**
 * @file Set.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Set template class.
 * @date 2023.01.15
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

#ifndef SET_HPP
#define SET_HPP

#include "utility.hpp"

#include <set>

namespace pyincpp
{

/// Set is a collection of distinct objects.
/// Common uses include membership testing, removing duplicates from a sequence, and computing mathematical operations.
template <typename T>
class Set
{
private:
    // Set.
    std::set<T> set_;

public:
    /*
     * Constructor / Destructor
     */

    /// Default constructor. Construct an empty set.
    Set() = default;

    /// Construct a set with the contents of the initializer list `init`.
    Set(const std::initializer_list<T>& il)
        : set_(il)
    {
    }

    /// Construct a set with the contents of the range [`first`, `last`).
    template <typename InputIt>
    Set(InputIt first, InputIt last)
        : set_(first, last)
    {
    }

    /// Copy constructor.
    Set(const Set& that) = default;

    /// Move constructor.
    Set(Set&& that) = default;

    /*
     * Comparison
     */

    /// Test whether the set is equal to another set.
    bool operator==(const Set& that) const = default;

    /// Test whether the set is a proper subset of another set.
    bool operator<(const Set& that) const
    {
        return size() != that.size() && *this <= that;
    }

    /// Test whether every element in the set is in another set.
    bool operator<=(const Set& that) const
    {
        return std::includes(that.set_.cbegin(), that.set_.cend(), set_.cbegin(), set_.cend());
    }

    /// Test whether the set is a proper superset of another set.
    bool operator>(const Set& that) const
    {
        return that < *this;
    }

    /// Test whether every element in another set is in the set.
    bool operator>=(const Set& that) const
    {
        return that <= *this;
    }

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Set& operator=(const Set& that) = default;

    /// Move assignment operator.
    Set& operator=(Set&& that) = default;

    /*
     * Iterator
     */

    /// Return an iterator to the first element of the set.
    auto begin() const
    {
        return set_.begin();
    }

    /// Return an iterator to the element following the last element of the set.
    auto end() const
    {
        return set_.end();
    }

    /*
     * Examination
     */

    /// Return the number of elements in the set.
    int size() const
    {
        return set_.size();
    }

    /// Return `true` if the set contains no elements.
    bool is_empty() const
    {
        return set_.empty();
    }

    /// Return the iterator of the specified element in the set, or end() if the set does not contain the element.
    auto find(const T& element) const
    {
        return set_.find(element);
    }

    /// Return `true` if the set contains the specified element.
    bool contains(const T& element) const
    {
        return find(element) != end();
    }

    /// the smallest item of the set.
    T min() const
    {
        internal::check_empty(size());

        return *set_.cbegin();
    }

    /// Get the largest item of the set.
    T max() const
    {
        internal::check_empty(size());

        return *set_.crbegin();
    }

    /*
     * Manipulation
     */

    /// Add `element` to the set. Return `true` if the value was newly inserted.
    bool add(const T& element)
    {
        internal::check_full(size(), INT_MAX);

        return set_.insert(element).second;
    }

    /// Remove `element` from the set. Return `true` if such an element was present.
    bool remove(const T& element)
    {
        return set_.erase(element) == 1;
    }

    /// Intersection.
    /// Update the set, keeping only elements found in it and another set.
    Set& operator&=(const Set& that)
    {
        return *this = std::move(*this & that);
    }

    /// Union.
    /// Update the set, adding elements from another set.
    Set& operator|=(const Set& that)
    {
        return *this = std::move(*this | that);
    }

    /// Difference.
    /// Update the set, removing elements found in another set.
    Set& operator-=(const Set& that)
    {
        return *this = std::move(*this - that);
    }

    /// Symmetric difference.
    /// Update the set, keeping only elements found in either set, but not in both.
    Set& operator^=(const Set& that)
    {
        return *this = std::move(*this ^ that);
    }

    /// Remove all elements from the set.
    void clear()
    {
        set_.clear();
    }

    /*
     * Production
     */

    /// Intersection.
    /// Return a new set with elements common to the set and another set.
    Set operator&(const Set& that) const
    {
        Set new_set;
        std::set_intersection(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /// Union.
    /// Return a new set with elements from the set and another set.
    Set operator|(const Set& that) const
    {
        Set new_set;
        std::set_union(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /// Difference.
    /// Return a new set with elements in the set that are not in another set.
    Set operator-(const Set& that) const
    {
        Set new_set;
        std::set_difference(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /// Symmetric difference.
    /// Return a new set with elements in either the set or another set but not both.
    Set operator^(const Set& that) const
    {
        Set new_set;
        std::set_symmetric_difference(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /*
     * Print
     */

    /// Output the set to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Set& set)
    {
        return internal::print(os, set, '{', '}');
    }
};

} // namespace pyincpp

#endif // SET_HPP
