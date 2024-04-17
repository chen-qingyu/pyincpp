/**
 * @file Map.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Map template class.
 * @date 2023.01.18
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

#ifndef MAP_HPP
#define MAP_HPP

#include "Set.hpp"

#include <map>

namespace pyincpp
{

/**
 * @brief Map template class.
 *
 * @tparam K the type of keys in the map, require support operator<
 * @tparam V the type of values in the map
 */
template <typename K, typename V>
class Map
{
private:
    // Set of pairs.
    std::map<K, V> map_;

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new empty map object.
     */
    Map()
        : map_()
    {
    }

    /**
     * @brief Create a map based on the given initializer list.
     *
     * @param il initializer list
     */
    Map(const std::initializer_list<std::pair<const K, V>>& il)
        : map_(il)
    {
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another map
     */
    Map(const Map& that)
        : map_(that.map_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another map
     */
    Map(Map&& that)
        : map_(std::move(that.map_))
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two maps are equal.
     *
     * @param that another map
     * @return true if two maps are equal
     */
    bool operator==(const Map& that) const
    {
        return map_ == that.map_;
    }

    /**
     * @brief Check whether two maps are not equal.
     *
     * @param that another map
     * @return true if two maps are not equal
     */
    bool operator!=(const Map& that) const
    {
        return map_ != that.map_;
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this < that
     */
    bool operator<(const Map& that) const
    {
        return (*this) <= that && size() != that.size();
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this <= that
     */
    bool operator<=(const Map& that) const
    {
        return std::includes(that.map_.cbegin(), that.map_.cend(), map_.cbegin(), map_.cend());
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this > that
     */
    bool operator>(const Map& that) const
    {
        return that < *this;
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this >= that
     */
    bool operator>=(const Map& that) const
    {
        return that <= *this;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another map
     * @return self reference
     */
    Map& operator=(const Map& that)
    {
        map_ = that.map_;

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another map
     * @return self reference
     */
    Map& operator=(Map&& that)
    {
        map_ = std::move(that.map_);

        return *this;
    }

    /*
     * Access
     */

    /**
     * @brief Return the reference to the value of the key in the map.
     *
     * @param key key of the key-value pair
     * @return reference to the value of the key in the map
     */
    V& operator[](const K& key)
    {
        auto it = map_.find(key);
        if (it == map_.end())
        {
            throw std::runtime_error("Error: Key is not found in the map.");
        }

        return it->second;
    }

    /**
     * @brief Return the const reference to value of the key in the map.
     *
     * @param key key of the key-value pair
     * @return const reference to the value of the key in the map
     */
    const V& operator[](const K& key) const
    {
        return const_cast<Map&>(*this)[key];
    }

    /**
     * @brief Return copy of the value for key if key is in the map, else default value.
     *
     * If default value is not given, it defaults to the default value of type V.
     *
     * @param key key of the key-value pair
     * @param defaults default value
     * @return copy of the value for key if key is in the map, else default value
     */
    V get(const K& key, const V& defaults = V()) const
    {
        return contains(key) ? map_.at(key) : defaults;
    }

    /*
     * Iterator
     */

    /**
     * @brief Return an iterator to the first element of the map.
     *
     * If the map is empty, the returned iterator will be equal to end().
     *
     * @return iterator to the first element
     */
    auto begin() const
    {
        return map_.begin();
    }

    /**
     * @brief Return an iterator to the element following the last element of the map.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    auto end() const
    {
        return map_.end();
    }

    /*
     * Examination
     */

    /**
     * @brief Return the number of elements in the map.
     *
     * @return the number of elements in the map
     */
    int size() const
    {
        return map_.size();
    }

    /**
     * @brief Return true if the map contains no elements.
     *
     * @return true if the map contains no elements
     */
    bool is_empty() const
    {
        return map_.empty();
    }

    /**
     * @brief Return the iterator of the specified key in the map.
     *
     * Or end() if the map does not contain the key.
     *
     * @param key key to search for
     * @return the iterator of the specified key in the map, or end() if the map does not contain the key
     */
    auto find(const K& key) const
    {
        return map_.find(key);
    }

    /**
     * @brief Return true if the map contains the specified key.
     *
     * @param key key whose presence in the map is to be tested
     * @return true if the map contains the specified key
     */
    bool contains(const K& key) const
    {
        return map_.find(key) != map_.end();
    }

    /**
     * @brief Get the smallest key of the map.
     *
     * @return the smallest key
     */
    K min() const
    {
        return map_.cbegin()->first;
    }

    /**
     * @brief Get the largest key of the map.
     *
     * @return the largest key
     */
    K max() const
    {
        return map_.crbegin()->first;
    }

    /**
     * @brief Return a new set of the map's keys.
     *
     * @return a new set of the map's keys
     */
    Set<K> keys() const
    {
        Set<K> keys;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { keys += pair.first; });

        return keys;
    }

    /**
     * @brief Return a new set of the map's values.
     *
     * @return a new set of the map's values
     */
    Set<V> values() const
    {
        Set<V> values;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { values += pair.second; });

        return values;
    }

    /**
     * @brief Return a new set of the map's items.
     *
     * @return a new set of the map's items
     */
    Set<std::pair<K, V>> items() const
    {
        Set<std::pair<K, V>> items;
        std::for_each(map_.cbegin(), map_.cend(), [&](const auto& pair)
                      { items += pair; });

        return items;
    }

    /*
     * Manipulation
     */

    /**
     * @brief Add the specified pair to the map.
     *
     * @param pair pair to be added to the map
     * @return self reference
     */
    Map& operator+=(const std::pair<K, V>& pair)
    {
        map_.insert(pair);

        return *this;
    }

    /**
     * @brief Remove the specified key-value pair from the map, if it is present.
     *
     * If the map does not contain the key-value pair, it is unchanged.
     *
     * @param key key of the key-value pair to be removed
     * @return self reference
     */
    Map& operator-=(const K& key)
    {
        map_.erase(key);

        return *this;
    }

    /**
     * @brief Remove all of the elements from the map.
     *
     * @return self reference
     */
    Map& clear()
    {
        map_.clear();

        return *this;
    }

    /*
     * Print
     */

    /**
     * @brief Output the map to the specified output stream.
     *
     * @param os an output stream
     * @param map the map to be printed to the output stream
     * @return self reference of the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Map& map)
    {
        if (map.is_empty())
        {
            return os << "{}";
        }

        os << "{";
        auto it = map.begin();
        while (true)
        {
            os << *it++;
            if (it == map.end())
            {
                return os << "}";
            }
            os << ", ";
        }
    }
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& pair)
{
    return os << pair.first << ": " << pair.second;
}

} // namespace pyincpp

#endif // MAP_HPP
