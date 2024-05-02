//! @file Dict.hpp
//! @author Qingyu Chen (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
//! @brief Dict template class.
//! @date 2023.01.18
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

#ifndef DICT_HPP
#define DICT_HPP

#include "Set.hpp"

#include <map>

namespace pyincpp
{

/// Alias symbol `Pair<K, V>` = `std::pair<const K, V>`.
template <typename K, typename V>
using Pair = std::pair<const K, V>;

/// A Dict object maps keys to arbitrary values.
template <typename K, typename V>
class Dict
{
private:
    // Map of key-value pairs.
    std::map<const K, V> map_;

public:
    /*
     * Constructor / Destructor
     */

    /// Default constructor. Construct an empty dictionary.
    Dict() = default;

    /// Construct a dictionary with the contents of the initializer list `init`.
    Dict(const std::initializer_list<Pair<K, V>>& init)
        : map_(init)
    {
    }

    /// Construct a dictionary with the contents of the range [`first`, `last`).
    template <typename InputIt>
    Dict(const InputIt& first, const InputIt& last)
        : map_(first, last)
    {
    }

    /// Copy constructor.
    Dict(const Dict& that) = default;

    /// Move constructor.
    Dict(Dict&& that) = default;

    /*
     * Comparison
     */

    /// Determine whether this dictionary is equal to another dictionary.
    bool operator==(const Dict& that) const = default;

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Dict& operator=(const Dict& that) = default;

    /// Move assignment operator.
    Dict& operator=(Dict&& that) = default;

    /*
     * Iterator
     */

    /// Return an iterator to the first element of the dictionary.
    auto begin() const
    {
        return map_.begin();
    }

    /// Return an iterator to the element following the last element of the dictionary.
    auto end() const
    {
        return map_.end();
    }

    /// Return a reverse iterator to the first element of the reversed dictionary.
    auto rbegin() const
    {
        return map_.rbegin();
    }

    /// Return a reverse iterator to the element following the last element of the reversed dictionary.
    auto rend() const
    {
        return map_.rend();
    }

    /*
     * Access
     */

    /// Return the reference to the value of the `key` in the dictionary.
    V& operator[](const K& key)
    {
        auto it = map_.find(key);
        if (it == map_.end())
        {
            throw std::runtime_error("Error: Key is not found in the dictionary.");
        }

        return it->second;
    }

    /// Return the const reference to value of the `key` in the dictionary.
    const V& operator[](const K& key) const
    {
        return const_cast<Dict&>(*this)[key];
    }

    /// Return copy of the value for `key` if `key` is in the dictionary, else `defaults` value.
    const V& get(const K& key, const V& defaults) const
    {
        return contains(key) ? (*this)[key] : defaults;
    }

    /*
     * Examination
     */

    /// Return the number of elements in the dictionary.
    int size() const
    {
        return map_.size();
    }

    /// Return `true` if the dictionary contains no elements.
    bool is_empty() const
    {
        return map_.empty();
    }

    /// Return the iterator of the specified key or end() if the dictionary does not contain the key.
    auto find(const K& key) const
    {
        return map_.find(key);
    }

    /// Return `true` if the dictionary contains the specified `key`.
    bool contains(const K& key) const
    {
        return map_.find(key) != map_.end();
    }

    /// Get the smallest key of the dictionary.
    K min() const
    {
        return map_.cbegin()->first;
    }

    /// Get the largest key of the dictionary.
    K max() const
    {
        return map_.crbegin()->first;
    }

    /// Return a new set of the dictionary's keys.
    Set<K> keys() const
    {
        Set<K> keys;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { keys.add(pair.first); });

        return keys;
    }

    /// Return a new set of the dictionary's values.
    Set<V> values() const
    {
        Set<V> values;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { values.add(pair.second); });

        return values;
    }

    /// Return a new set of the dictionary's items.
    Set<Pair<K, V>> items() const
    {
        Set<Pair<K, V>> items;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { items.add(pair); });

        return items;
    }

    /*
     * Manipulation
     */

    /// Add the specified `key` and `value` to the dictionary. Return `true` if the `key` and `value` was newly inserted.
    bool add(const K& key, const V& value)
    {
        internal::check_full(size(), INT_MAX);

        return map_.insert({key, value}).second;
    }

    /// Remove `key` from the dictionary. Return `true` if such an `key` was present.
    bool remove(const K& key)
    {
        return map_.erase(key) == 1;
    }

    /// Remove and return an arbitrary key-value pair from the dictionary.
    Pair<K, V> pop()
    {
        internal::check_empty(size());

        auto node = map_.extract(begin());
        return Pair{node.key(), node.mapped()};
    }

    /// Remove all of the elements from the dictionary.
    void clear()
    {
        map_.clear();
    }

    /*
     * Print
     */

    /// Output the dictionary to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Dict& dictionary)
    {
        return internal::print(os, dictionary, '{', '}');
    }
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Pair<K, V>& pair)
{
    return os << pair.first << ": " << pair.second;
}

} // namespace pyincpp

#endif // DICT_HPP
