//! @file Map.hpp
//! @author Qingyu Chen (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
//! @brief Map template class.
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

#ifndef MAP_HPP
#define MAP_HPP

#include "Set.hpp"

#include <map>

namespace pyincpp
{

/// Alias symbol `Pair<K, V>` = `std::pair<const K, V>`.
template <typename K, typename V>
using Pair = std::pair<const K, V>;

/// A Map object maps keys to arbitrary values.
template <typename K, typename V>
class Map
{
private:
    // Map of key-value pairs.
    std::map<const K, V> map_;

public:
    /*
     * Constructor / Destructor
     */

    /// Default constructor. Construct an empty map.
    Map() = default;

    /// Construct a map with the contents of the initializer list `init`.
    Map(const std::initializer_list<Pair<K, V>>& init)
        : map_(init)
    {
    }

    /// Construct a map with the contents of the range [`first`, `last`).
    template <typename InputIt>
    Map(const InputIt& first, const InputIt& last)
        : map_(first, last)
    {
    }

    /// Copy constructor.
    Map(const Map& that) = default;

    /// Move constructor.
    Map(Map&& that) = default;

    /*
     * Comparison
     */

    /// Determine whether this map is equal to another map.
    bool operator==(const Map& that) const = default;

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Map& operator=(const Map& that) = default;

    /// Move assignment operator.
    Map& operator=(Map&& that) = default;

    /*
     * Iterator
     */

    /// Return an iterator to the first element of the map.
    auto begin() const
    {
        return map_.begin();
    }

    /// Return an iterator to the element following the last element of the map.
    auto end() const
    {
        return map_.end();
    }

    /*
     * Access
     */

    /// Return the reference to the value of the `key` in the map.
    V& operator[](const K& key)
    {
        auto it = map_.find(key);
        if (it == map_.end())
        {
            throw std::runtime_error("Error: Key is not found in the map.");
        }

        return it->second;
    }

    /// Return the const reference to value of the `key` in the map.
    const V& operator[](const K& key) const
    {
        return const_cast<Map&>(*this)[key];
    }

    /// Return copy of the value for `key` if `key` is in the map, else `defaults` value.
    const V& get(const K& key, const V& defaults) const
    {
        return contains(key) ? (*this)[key] : defaults;
    }

    /*
     * Examination
     */

    /// Return the number of elements in the map.
    int size() const
    {
        return map_.size();
    }

    /// Return `true` if the map contains no elements.
    bool is_empty() const
    {
        return map_.empty();
    }

    /// Return the iterator of the specified key in the map, or end() if the map does not contain the key.
    auto find(const K& key) const
    {
        return map_.find(key);
    }

    /// Return `true` if the map contains the specified `key`.
    bool contains(const K& key) const
    {
        return map_.find(key) != map_.end();
    }

    /// Get the smallest key of the map.
    K min() const
    {
        return map_.cbegin()->first;
    }

    /// Get the largest key of the map.
    K max() const
    {
        return map_.crbegin()->first;
    }

    /// Return a new set of the map's keys.
    Set<K> keys() const
    {
        Set<K> keys;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { keys.add(pair.first); });

        return keys;
    }

    /// Return a new set of the map's values.
    Set<V> values() const
    {
        Set<V> values;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { values.add(pair.second); });

        return values;
    }

    /// Return a new set of the map's items.
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

    /// Add the specified `key` and `value` to the map. Return `true` if the `key` and `value` was newly inserted.
    bool add(const K& key, const V& value)
    {
        internal::check_full(size(), INT_MAX);

        return map_.insert({key, value}).second;
    }

    /// Remove `key` from the map. Return `true` if such an `key` was present.
    bool remove(const K& key)
    {
        return map_.erase(key) == 1;
    }

    /// Remove all of the elements from the map.
    void clear()
    {
        map_.clear();
    }

    /*
     * Print
     */

    /// Output the map to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Map& map)
    {
        return internal::print(os, map, '{', '}');
    }
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Pair<K, V>& pair)
{
    return os << pair.first << ": " << pair.second;
}

} // namespace pyincpp

#endif // MAP_HPP
