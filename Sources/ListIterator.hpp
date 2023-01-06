/**
 * @file ListIterator.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief List (implemented by array list) iterator template class.
 * @version 1.0
 * @date 2023.01.05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP

namespace mdspp
{

template <typename T>
class List;

template <typename T>
class ListIterator
{
    friend class List<T>;

private:
    // Current data pointer.
    T* current_;

    // Create an iterator that point to the current data of list.
    ListIterator(T* current)
        : current_(current)
    {
    }

public:
    ListIterator() = delete;

    /**
     * @brief Dereference.
     *
     * @return reference of the data
     */
    T& operator*() const
    {
        return *current_;
    }

    /**
     * @brief Get current pointer.
     *
     * @return current pointer
     */
    T* operator->() const
    {
        return current_;
    }

    /**
     * @brief Check if two iterators are same.
     *
     * @param that another iterator
     * @return ture if two iterators are same, false otherwise
     */
    bool operator==(const ListIterator& that) const
    {
        return current_ == that.current_;
    }

    /**
     * @brief Check if two iterators are different.
     *
     * @param that another iterator
     * @return ture if two iterators are different, false otherwise
     */
    bool operator!=(const ListIterator& that) const
    {
        return !(current_ == that.current_);
    }

    /**
     * @brief Increment the iterator: ++it.
     *
     * @return reference of this iterator that point to next data
     */
    ListIterator& operator++()
    {
        ++current_;
        return *this;
    }

    /**
     * @brief Increment the iterator: it++.
     *
     * @return const reference of this iterator that point to current data
     */
    ListIterator operator++(int)
    {
        ListIterator tmp = *this;
        ++current_;
        return tmp;
    }

    /**
     * @brief Decrement the iterator: --it.
     *
     * @return reference of this iterator that point to previous data
     */
    ListIterator& operator--()
    {
        --current_;
        return *this;
    }

    /**
     * @brief Decrement the iterator: it--.
     *
     * @return const reference of this iterator that point to current data
     */
    ListIterator operator--(int)
    {
        ListIterator tmp = *this;
        --current_;
        return tmp;
    }
};

} // namespace mdspp

#endif // LISTITERATOR_HPP
