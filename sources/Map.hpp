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
public:
    /**
     * @brief Map key-value pair class.
     */
    class Pair
    {
        friend class Map;

        friend std::ostream& operator<<(std::ostream& os, const Pair& pair)
        {
            return os << pair.key() << ": " << pair.value();
        }

    private:
        // Pair key.
        const K key_;

        // Pair value.
        V value_;

    public:
        bool operator==(const Pair& that) const
        {
            return key_ == that.key_;
        }

        bool operator!=(const Pair& that) const
        {
            return key_ != that.key_;
        }

        bool operator<(const Pair& that) const
        {
            return key_ < that.key_;
        }

        bool operator<=(const Pair& that) const
        {
            return key_ <= that.key_;
        }

        bool operator>(const Pair& that) const
        {
            return key_ > that.key_;
        }

        bool operator>=(const Pair& that) const
        {
            return key_ >= that.key_;
        }

        /**
         * @brief Construct a new pair object.
         *
         * @param key the key
         * @param value the value
         */
        Pair(const K& key, const V& value)
            : key_(key)
            , value_(value)
        {
        }

        /**
         * @brief Get the key.
         *
         * @return key of the pair
         */
        const K& key() const
        {
            return key_;
        }

        /**
         * @brief Get the value.
         *
         * @return value of the pair
         */
        const V& value() const
        {
            return value_;
        }

        /**
         * @brief Get the value.
         *
         * @return value of the pair
         */
        V& value()
        {
            return value_;
        }
    };

public:
    class Iterator
    {
        friend class Map;

    private:
        typename std::set<Pair>::iterator it_;

        Iterator(const typename std::set<Pair>::iterator& it)
            : it_(it)
        {
        }

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = Pair;
        using difference_type = int;
        using pointer = value_type*;
        using reference = value_type&;

        Pair& operator*()
        {
            return const_cast<Pair&>(*it_);
        }

        Pair* operator->()
        {
            return &(operator*());
        }

        Iterator& operator++()
        {
            ++it_;
            return *this;
        }

        Iterator& operator--()
        {
            --it_;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++it_;
            return tmp;
        }

        Iterator operator--(int)
        {
            Iterator tmp = *this;
            --it_;
            return tmp;
        }

        bool operator==(const Iterator& other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const Iterator& other) const
        {
            return it_ != other.it_;
        }
    };

public:
    class ConstIterator
    {
        friend class Map;

    private:
        typename std::set<Pair>::const_iterator it_;

        ConstIterator(const typename std::set<Pair>::const_iterator& it)
            : it_(it)
        {
        }

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = Pair;
        using difference_type = int;
        using pointer = value_type*;
        using reference = value_type&;

        const Pair& operator*() const
        {
            return *it_;
        }

        const Pair* operator->() const
        {
            return &(operator*());
        }

        ConstIterator& operator++()
        {
            ++it_;
            return *this;
        }

        ConstIterator& operator--()
        {
            --it_;
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator tmp = *this;
            ++it_;
            return tmp;
        }

        ConstIterator operator--(int)
        {
            ConstIterator tmp = *this;
            --it_;
            return tmp;
        }

        bool operator==(const ConstIterator& other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return it_ != other.it_;
        }
    };

private:
    // Set of pairs.
    std::set<Pair> map_;

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
    Map(const std::initializer_list<Pair>& il)
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
        return size() == that.size() && std::equal(begin(), end(), that.begin(), [](const Pair& p1, const Pair& p2)
                                                   { return p1.key_ == p2.key_ && p1.value_ == p2.value_; });
    }

    /**
     * @brief Check whether two maps are not equal.
     *
     * @param that another map
     * @return true if two maps are not equal
     */
    bool operator!=(const Map& that) const
    {
        return !(*this == that);
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
    Map&
    operator=(const Map& that)
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
        auto it = map_.find(Pair(key, V()));
        if (it == map_.end())
        {
            throw std::runtime_error("Error: Key is not found in the map.");
        }

        return const_cast<V&>(it->value());
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
    V& get(const K& key, const V& defaults) const
    {
        return contains(key) ? const_cast<Map&>(*this)[key] : defaults;
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
    Iterator begin()
    {
        return Iterator(map_.begin());
    }

    ConstIterator begin() const
    {
        return ConstIterator(map_.begin());
    }

    /**
     * @brief Return an iterator to the element following the last element of the map.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    Iterator end()
    {
        return Iterator(map_.end());
    }

    ConstIterator end() const
    {
        return ConstIterator(map_.end());
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
    Iterator find(const K& key) const
    {
        return Iterator(const_cast<std::set<Pair>&>(map_).find(Pair(key, V())));
    }

    /**
     * @brief Return true if the map contains the specified key.
     *
     * @param key key whose presence in the map is to be tested
     * @return true if the map contains the specified key
     */
    bool contains(const K& key) const
    {
        return map_.find(Pair(key, V())) != map_.end();
    }

    /**
     * @brief Get the smallest key of the map.
     *
     * @return the smallest key
     */
    K min() const
    {
        return map_.cbegin()->key();
    }

    /**
     * @brief Get the largest key of the map.
     *
     * @return the largest key
     */
    K max() const
    {
        return map_.crbegin()->key();
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
                      { keys += pair.key_; });

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
                      { values += pair.value_; });

        return values;
    }

    /**
     * @brief Return a new set of the map's items.
     *
     * @return a new set of the map's items
     */
    Set<Pair> items() const
    {
        Set<Pair> items;
        items.set_ = map_;

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
    Map& operator+=(const Pair& pair)
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
        map_.erase(Pair(key, V()));

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

/**
 * @brief Export symbol `Pair`: `Pair<K, V> = Map<K, V>::Pair`.
 */
template <typename K, typename V>
using Pair = typename Map<K, V>::Pair;

} // namespace pyincpp

#endif // MAP_HPP
