/**
 * @file Map.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Map template class, implemented by Set of Pair.
 * @version 0.1
 * @date 2023.01.18
 *
 * @copyright Copyright (c) 2023
 *
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
        // Pair key
        K key_;

        // Pair value
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

        // Operator equal.
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
     * @brief Return true if the map contains the specified key.
     *
     * @param key key whose presence in the map is to be tested
     * @return true if the map contains the specified key
     */
    bool contains(const K& key) const
    {
        return set_.find(Pair(key, V())) != end();
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
};

} // namespace mdspp

#endif // MAP_HPP
