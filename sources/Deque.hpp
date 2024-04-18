/**
 * @file Deque.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Deque template class.
 * @date 2023.01.10
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

#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "utility.hpp"

#include <deque>

namespace pyincpp
{

/**
 * @brief Deque template class.
 *
 * @tparam T the type of elements in the deque
 */
template <typename T>
class Deque
{
private:
    // Deque.
    std::deque<T> deque_;

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new empty deque object.
     */
    Deque()
        : deque_()
    {
    }

    /**
     * @brief Create a deque based on the given initializer list.
     *
     * @param il initializer list
     */
    Deque(const std::initializer_list<T>& il)
        : deque_(il)
    {
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another deque
     */
    Deque(const Deque& that)
        : deque_(that.deque_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another deque
     */
    Deque(Deque&& that)
        : deque_(std::move(that.deque_))
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two deques are equal.
     *
     * @param that another deque
     * @return true if two deques are equal
     */
    bool operator==(const Deque& that) const
    {
        return deque_ == that.deque_;
    }

    /**
     * @brief Check whether two deques are not equal.
     *
     * @param that another deque
     * @return true if two deques are not equal
     */
    bool operator!=(const Deque& that) const
    {
        return deque_ != that.deque_;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this < that
     */
    bool operator<(const Deque& that) const
    {
        return deque_ < that.deque_;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this <= that
     */
    bool operator<=(const Deque& that) const
    {
        return deque_ <= that.deque_;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this > that
     */
    bool operator>(const Deque& that) const
    {
        return deque_ > that.deque_;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this >= that
     */
    bool operator>=(const Deque& that) const
    {
        return deque_ >= that.deque_;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another deque
     * @return self reference
     */
    Deque& operator=(const Deque& that)
    {
        if (this != &that)
        {
            deque_ = that.deque_;
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another deque
     * @return self reference
     */
    Deque& operator=(Deque&& that)
    {
        if (this != &that)
        {
            deque_ = std::move(that.deque_);
        }

        return *this;
    }

    /*
     * Access
     */

    /**
     * @brief Peek the back element.
     *
     * @return the back element
     */
    const T& back() const
    {
        internal::check_empty(size());

        return deque_.back();
    }

    /**
     * @brief Peek the front element.
     *
     * @return the front element
     */
    const T& front() const
    {
        internal::check_empty(size());

        return deque_.front();
    }

    /**
     * @brief Get the back element.
     *
     * @return the back element
     */
    T& back()
    {
        internal::check_empty(size());

        return deque_.back();
    }

    /**
     * @brief Get the front element.
     *
     * @return the front element
     */
    T& front()
    {
        internal::check_empty(size());

        return deque_.front();
    }

    /*
     * Examination
     */

    /**
     * @brief Return the number of elements in the deque.
     *
     * @return the number of elements in the deque
     */
    int size() const
    {
        return deque_.size();
    }

    /**
     * @brief Return true if the deque contains no elements.
     *
     * @return true if the deque contains no elements
     */
    bool is_empty() const
    {
        return deque_.empty();
    }

    /*
     * Manipulation
     */

    /**
     * @brief Push an element to the end of the deque.
     *
     * @param element element to be pushed
     */
    void push_back(const T& element)
    {
        deque_.push_back(element);
    }

    /**
     * @brief Push an element to the front of the deque.
     *
     * @param element element to be pushed
     */
    void push_front(const T& element)
    {
        deque_.push_front(element);
    }

    /**
     * @brief Pop an element of the end of the deque.
     *
     * @return poped element
     */
    T pop_back()
    {
        internal::check_empty(size());

        T data = std::move(deque_.back());
        deque_.pop_back();
        return data;
    }

    /**
     * @brief Pop an element of the front of the deque.
     *
     * @return poped element
     */
    T pop_front()
    {
        internal::check_empty(size());

        T data = std::move(deque_.front());
        deque_.pop_front();
        return data;
    }

    /**
     * @brief Clear data.
     *
     * @return self reference
     */
    Deque& clear()
    {
        deque_.clear();

        return *this;
    }

    /**
     * @brief Rotate n elements to the right.
     *
     * @param n number of rotate elements
     * @return self reference
     */
    Deque& operator>>=(int n)
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this <<= -n;
        }

        n %= size();

        std::rotate(deque_.begin(), deque_.begin() + size() - n, deque_.end());

        return *this;
    }

    /**
     * @brief Rotate n elements to the left.
     *
     * @param n number of rotate elements
     * @return self reference
     */
    Deque& operator<<=(int n)
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this >>= -n;
        }

        n %= size();

        std::rotate(deque_.begin(), deque_.begin() + n, deque_.end());

        return *this;
    }

    /**
     * @brief Reverse the deque in place.
     *
     * @return self reference
     */
    Deque& reverse()
    {
        std::reverse(deque_.begin(), deque_.end());

        return *this;
    }

    /*
     * Print
     */

    /**
     * @brief Output the deque to the specified output stream.
     *
     * @param os an output stream
     * @param deque the deque to be printed to the output stream
     * @return self reference of the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Deque& deque)
    {
        if (deque.is_empty())
        {
            return os << "<>";
        }

        os << "<";
        auto it = deque.deque_.begin();
        while (true)
        {
            os << *it++;
            if (it == deque.deque_.end())
            {
                return os << ">";
            }
            os << ", ";
        }
    }
};

} // namespace pyincpp

#endif // DEQUE_HPP
