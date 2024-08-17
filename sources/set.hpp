//! @file set.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Set template class.
//! @date 2023.01.15

#ifndef SET_HPP
#define SET_HPP

#include "utility.hpp"

#include <set>

namespace pyincpp
{

/// Set is collection of distinct objects.
/// Common uses include membership testing, removing duplicates from a sequence, and computing mathematical operations.
template <typename T>
class Set
{
private:
    // Set.
    std::set<T> set_;

public:
    /*
     * Constructor
     */

    /// Construct an empty set.
    Set() = default;

    /// Construct a set with the contents of the initializer list `init`.
    Set(const std::initializer_list<T>& init)
        : set_(init)
    {
    }

    /// Construct a set with the contents of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    Set(const InputIt& first, const InputIt& last)
        : set_(first, last)
    {
    }

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

    /// Return a reverse iterator to the first element of the reversed set.
    auto rbegin() const
    {
        return set_.rbegin();
    }

    /// Return a reverse iterator to the element following the last element of the reversed set.
    auto rend() const
    {
        return set_.rend();
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

    /// Get the smallest item of the set.
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

    /// Add `element` to the set. Return `true` if the `element` was newly inserted.
    bool add(const T& element)
    {
        internal::check_full(size(), INT_MAX);

        return set_.insert(element).second;
    }

    /// Remove `element` from the set. Return `true` if such an `element` was present.
    bool remove(const T& element)
    {
        return set_.erase(element) == 1;
    }

    /// Remove and return an arbitrary element from the set.
    T pop()
    {
        internal::check_empty(size());

        return set_.extract(set_.begin()).value();
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

    /// Extend the set by adding elements of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    void extend(const InputIt& first, const InputIt& last)
    {
        set_.insert(first, last);
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
        return internal::print(os, set.begin(), set.end(), '{', '}');
    }
};

} // namespace pyincpp

#endif // SET_HPP
