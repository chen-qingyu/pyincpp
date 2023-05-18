/**
 * @file Map.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Map template class, implemented by Set of Pair.
 * @version 1.0
 * @date 2023.01.18
 *
 * @copyright Copyright (c) 2023
 */

#ifndef MAP_HPP
#define MAP_HPP

#include "Set.hpp"

namespace mdspp
{

/**
 * @brief Map template class, implemented by Set of Pair.
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
        friend class Map<K, V>;

        template <typename T>
        friend class Set;

    private:
        // Pair key.
        K key_;

        // Pair value.
        V value_;

        // Default constructor.
        Pair()
            : key_(K())
            , value_(V())
        {
        }

        // Operator equal.
        bool operator==(const Pair& that) const
        {
            return key_ == that.key_;
        }

        // Operator not equal.
        bool operator!=(const Pair& that) const
        {
            return key_ != that.key_;
        }

        // Operator less than.
        bool operator<(const Pair& that) const
        {
            return key_ < that.key_;
        }

        // Operator less than or equal.
        bool operator<=(const Pair& that) const
        {
            return key_ <= that.key_;
        }

        // Operator greater than.
        bool operator>(const Pair& that) const
        {
            return key_ > that.key_;
        }

        // Operator greater than or equal.
        bool operator>=(const Pair& that) const
        {
            return key_ >= that.key_;
        }

    public:
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
        K key() const
        {
            return key_;
        }

        /**
         * @brief Get the value.
         *
         * @return value of the pair
         */
        V value() const
        {
            return value_;
        }
    };

    /**
     * @brief Map iterator class.
     *
     * Walk the map in ascending order. This means that begin() is the smallest pair.
     *
     * Because the internal pairs of the map have a fixed order,
     * thus the iterator of the map only supports access and does not support modification.
     */
    class Iterator
    {
        friend class Map<K, V>;

    private:
        // Current node pointer.
        typename Set<Pair>::Iterator it_;

        // Constructor.
        Iterator(typename Set<Pair>::Node* current)
            : it_(current)
        {
        }

    public:
        /**
         * @brief Dereference.
         *
         * @return reference of the data
         */
        const Pair& operator*() const
        {
            return *it_;
        }

        /**
         * @brief Get current pointer.
         *
         * @return current pointer
         */
        const Pair* operator->() const
        {
            return it_.operator->();
        }

        /**
         * @brief Check if two iterators are same.
         *
         * @param that another iterator
         * @return ture if two iterators are same, false otherwise.
         */
        bool operator==(const Iterator& that) const
        {
            return it_ == that.it_;
        }

        /**
         * @brief Check if two iterators are different.
         *
         * @param that another iterator
         * @return ture if two iterators are different, false otherwise.
         */
        bool operator!=(const Iterator& that) const
        {
            return !(*this == that);
        }

        /**
         * @brief Increment the iterator: ++it.
         *
         * @return reference of this iterator that point to next data
         */
        Iterator& operator++()
        {
            ++it_;
            return *this;
        }

        /**
         * @brief Increment the iterator: it++.
         *
         * @return const reference of this iterator that point to current data
         */
        Iterator operator++(int)
        {
            auto tmp = *this;
            ++it_;
            return tmp;
        }

        /**
         * @brief Decrement the iterator: --it.
         *
         * @return reference of this iterator that point to previous data
         */
        Iterator& operator--()
        {
            --it_;
            return *this;
        }

        /**
         * @brief Decrement the iterator: it--.
         *
         * @return const reference of this iterator that point to current data
         */
        Iterator operator--(int)
        {
            auto tmp = *this;
            --it_;
            return tmp;
        }
    };

private:
    // Set of pairs.
    Set<Pair> set_;

    // Access.
    V& access(const K& key) const
    {
        auto it = set_.find(Pair(key, V()));
        if (it == set_.end())
        {
            throw std::runtime_error("ERROR: Key is not found in the map.");
        }

        return it.current_->data_.value_;
    }

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new map object.
     */
    Map()
        : set_()
    {
    }

    /**
     * @brief Create a map based on the given initializer list.
     *
     * @param il initializer list
     */
    Map(const std::initializer_list<Pair>& il)
        : set_()
    {
        for (auto it = il.begin(); it != il.end(); ++it)
        {
            set_ += *it;
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another map
     */
    Map(const Map& that)
        : set_(that.set_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another map
     */
    Map(Map&& that)
        : set_(std::move(that.set_))
    {
    }

    /**
     * @brief Destroy the map object.
     */
    ~Map()
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
        return set_ == that.set_;
    }

    /**
     * @brief Check whether two maps are not equal.
     *
     * @param that another map
     * @return true if two maps are not equal
     */
    bool operator!=(const Map& that) const
    {
        return set_ != that.set_;
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this < that
     */
    bool operator<(const Map& that) const
    {
        return set_ < that.set_;
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this <= that
     */
    bool operator<=(const Map& that) const
    {
        return set_ <= that.set_;
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this > that
     */
    bool operator>(const Map& that) const
    {
        return set_ > that.set_;
    }

    /**
     * @brief Compare two maps.
     *
     * @param that another map
     * @return true if this >= that
     */
    bool operator>=(const Map& that) const
    {
        return set_ >= that.set_;
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
        set_ = that.set_;

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
        set_ = std::move(that.set_);

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
        return access(key);
    }

    /**
     * @brief Return the const reference to value of the key in the map.
     *
     * @param key key of the key-value pair
     * @return const reference to the value of the key in the map
     */
    const V& operator[](const K& key) const
    {
        return access(key);
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
        return contains(key) ? access(key) : defaults;
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
    Iterator begin() const
    {
        return Iterator(set_.min_);
    }

    /**
     * @brief Return an iterator to the element following the last element of the map.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    Iterator end() const
    {
        return Iterator(set_.end_);
    }

    /*
     * Examination (will not change the object itself)
     */

    /**
     * @brief Return the number of elements in the map.
     *
     * @return the number of elements in the map
     */
    int size() const
    {
        return set_.size_;
    }

    /**
     * @brief Return true if the map contains no elements.
     *
     * @return true if the map contains no elements
     */
    bool is_empty() const
    {
        return set_.size_ == 0;
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
        auto it = set_.find(Pair(key, V()));

        return it == set_.end() ? end() : Iterator(it.current_);
    }

    /**
     * @brief Return true if the map contains the specified key.
     *
     * @param key key whose presence in the map is to be tested
     * @return true if the map contains the specified key
     */
    bool contains(const K& key) const
    {
        return set_.contains(Pair(key, V()));
    }

    /**
     * @brief Get the smallest key of the map.
     *
     * @return the smallest key
     */
    K min() const
    {
        return set_.min().key_;
    }

    /**
     * @brief Get the largest key of the map.
     *
     * @return the largest key
     */
    K max() const
    {
        return set_.max().key_;
    }

    /**
     * @brief Return a new set of the map's keys.
     *
     * @return a new set of the map's keys
     */
    Set<K> keys() const
    {
        Set<K> keys;
        set_.level_action(set_.root_, [&](const Pair& pair)
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
        set_.level_action(set_.root_, [&](const Pair& pair)
                          { values += pair.value_; });

        return values;
    }

    /*
     * Manipulation (will change the object itself)
     */

    /**
     * @brief Add the specified pair to the map.
     *
     * @param pair pair to be added to the map
     * @return self reference
     */
    Map& operator+=(const Pair& pair)
    {
        set_ += pair;

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
        set_ -= Pair(key, V());

        return *this;
    }

    /**
     * @brief Remove all of the elements from the map.
     *
     * @return self reference
     */
    Map& clear()
    {
        set_.clear();

        return *this;
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Output map data to the specified output stream.
 *
 * @tparam K the key of pairs in the map, must be printable
 * @tparam V the value of pairs in the map, must be printable
 * @param os an output stream
 * @param map the map to be printed to the output stream
 * @return self reference of the output stream
 */
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Map<K, V>& map)
{
    if (map.is_empty())
    {
        return os << "{}";
    }

    auto it = map.begin();
    os << "{";
    while (true)
    {
        os << it->key() << ": " << it->value();
        ++it;
        if (it == map.end())
        {
            return os << "}";
        }
        os << ", ";
    }
}

} // namespace mdspp

#endif // MAP_HPP
