/**
 * @file Set.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Set template class, implemented by red-black tree.
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

/**
 * @brief Set template class, implemented by red-black tree.
 *
 * @tparam T the type of elements in the set, require support operator<
 */
template <typename T>
class Set
{
    template <typename K, typename V>
    friend class Map;

private:
    // Set.
    std::set<T> set_;

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new empty set object.
     */
    Set()
        : set_()
    {
    }

    /**
     * @brief Create a set based on the given initializer list.
     *
     * @param il initializer list
     */
    Set(const std::initializer_list<T>& il)
        : set_(il)
    {
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another set
     */
    Set(const Set& that)
        : set_(that.set_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another set
     */
    Set(Set&& that)
        : set_(std::move(that.set_))
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two sets are equal.
     *
     * @param that another set
     * @return true if two sets are equal
     */
    bool operator==(const Set& that) const
    {
        return set_ == that.set_;
    }

    /**
     * @brief Check whether two sets are not equal.
     *
     * @param that another set
     * @return true if two sets are not equal
     */
    bool operator!=(const Set& that) const
    {
        return set_ != that.set_;
    }

    /**
     * @brief Test whether the set is a proper subset of another set.
     *
     * @param that another set
     * @return true if the set is a proper subset of another set
     */
    bool operator<(const Set& that) const
    {
        return (*this) <= that && size() != that.size();
    }

    /**
     * @brief Test whether every element in the set is in another set.
     *
     * @param that another set
     * @return true if every element in the set is in another set
     */
    bool operator<=(const Set& that) const
    {
        return std::includes(that.set_.cbegin(), that.set_.cend(), set_.cbegin(), set_.cend());
    }

    /**
     * @brief Test whether the set is a proper superset of another set.
     *
     * @param that another set
     * @return true if the set is a proper superset of another set
     */
    bool operator>(const Set& that) const
    {
        return that < *this;
    }

    /**
     * @brief Test whether every element in another set is in the set.
     *
     * @param that another set
     * @return true if every element in another set is in the set
     */
    bool operator>=(const Set& that) const
    {
        return that <= *this;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator=(const Set& that)
    {
        if (this != &that)
        {
            set_ = that.set_;
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator=(Set&& that)
    {
        if (this != &that)
        {
            set_ = std::move(that.set_);
        }

        return *this;
    }

    /*
     * Iterator
     */

    /**
     * @brief Return an iterator to the first element of the set.
     *
     * If the set is empty, the returned iterator will be equal to end().
     *
     * @return iterator to the first element
     */
    auto begin() const
    {
        return set_.begin();
    }

    /**
     * @brief Return an iterator to the element following the last element of the set.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    auto end() const
    {
        return set_.end();
    }

    /*
     * Examination
     */

    /**
     * @brief Return the number of elements in the set.
     *
     * @return the number of elements in the set
     */
    int size() const
    {
        return set_.size();
    }

    /**
     * @brief Return true if the set contains no elements.
     *
     * @return true if the set contains no elements
     */
    bool is_empty() const
    {
        return set_.empty();
    }

    /**
     * @brief Return the iterator of the specified element in the set.
     *
     * Or end() if the set does not contain the element.
     *
     * @param element element to search for
     * @return the iterator of the specified element in the set, or end() if the set does not contain the element
     */
    auto find(const T& element) const
    {
        return set_.find(element);
    }

    /**
     * @brief Return true if the set contains the specified element.
     *
     * @param element element whose presence in the set is to be tested
     * @return true if the set contains the specified element
     */
    bool contains(const T& element) const
    {
        return find(element) != end();
    }

    /**
     * @brief Get the smallest item of the set.
     *
     * @return the smallest item
     */
    T min() const
    {
        internal::check_empty(size());

        return *set_.cbegin();
    }

    /**
     * @brief Get the largest item of the set.
     *
     * @return the largest item
     */
    T max() const
    {
        internal::check_empty(size());

        return *set_.crbegin();
    }

    /*
     * Manipulation
     */

    /**
     * @brief Add the specified element to the set.
     *
     * @param element element to be added to the set
     * @return self reference
     */
    Set& operator+=(const T& element)
    {
        internal::check_full(size(), INT_MAX);

        set_.insert(element);

        return *this;
    }

    /**
     * @brief Remove the specified element from the set, if it is present.
     *
     * If the set does not contain the element, it is unchanged.
     *
     * @param element element to be removed
     * @return self reference
     */
    Set& operator-=(const T& element)
    {
        set_.erase(element);

        return *this;
    }

    /**
     * @brief Adds a value to the set. Returns whether the value was newly inserted.
     *
     * @param element element to be added to the set
     * @return true if the value was newly inserted
     */
    bool insert(const T& element)
    {
        return set_.insert(element).second;
    }

    /**
     * @brief Removes a value from the set. Returns whether such an element was present.
     *
     * @param element element to be removed
     * @return true if such an element was present
     */
    bool remove(const T& element)
    {
        return set_.erase(element) == 1;
    }

    /**
     * @brief Remove all of the elements from the set.
     *
     * @return self reference
     */
    Set& clear()
    {
        set_.clear();

        return *this;
    }

    /**
     * @brief Intersect with another set.
     *
     * Update the set, keeping only elements found in it and all others.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator&=(const Set& that)
    {
        return *this = std::move(*this & that);
    }

    /**
     * @brief Union with another set.
     *
     * Update the set, adding elements from all others.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator|=(const Set& that)
    {
        return *this = std::move(*this | that);
    }

    /**
     * @brief Difference with another set.
     *
     * Update the set, removing elements found in others.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator-=(const Set& that)
    {
        return *this = std::move(*this - that);
    }

    /**
     * @brief Symmetric difference with another set.
     *
     * Update the set, keeping only elements found in either set, but not in both.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator^=(const Set& that)
    {
        return *this = std::move(*this ^ that);
    }

    /*
     * Production (will produce new object)
     */

    /**
     * @brief Intersect with another set.
     *
     * @param that another set
     * @return a new set with elements common to the set and all others
     */
    Set operator&(const Set& that) const
    {
        Set new_set;
        std::set_intersection(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /**
     * @brief Union with another set.
     *
     * @param that another set
     * @return a new set with elements from the set and all others
     */
    Set operator|(const Set& that) const
    {
        Set new_set;
        std::set_union(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /**
     * @brief Difference with another set.
     *
     * @param that another set
     * @return a new set with elements in the set that are not in the others
     */
    Set operator-(const Set& that) const
    {
        Set new_set;
        std::set_difference(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /**
     * @brief Symmetric difference with another set.
     *
     * @param that another set
     * @return a new set with elements in either the set or other but not both
     */
    Set operator^(const Set& that) const
    {
        Set new_set;
        std::set_symmetric_difference(set_.cbegin(), set_.cend(), that.set_.cbegin(), that.set_.cend(), std::inserter(new_set.set_, new_set.set_.begin()));
        return new_set;
    }

    /*
     * Print
     */

    /**
     * @brief Output the set to the specified output stream.
     *
     * @param os an output stream
     * @param set the set to be printed to the output stream
     * @return self reference of the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Set& set)
    {
        if (set.is_empty())
        {
            return os << "{}";
        }

        os << "{";
        auto it = set.begin();
        while (true)
        {
            os << *it++;
            if (it == set.end())
            {
                return os << "}";
            }
            os << ", ";
        }
    }
};

} // namespace pyincpp

#endif // SET_HPP
