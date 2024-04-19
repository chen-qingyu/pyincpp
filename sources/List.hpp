/**
 * @file List.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief List template class, implemented by dynamic array.
 * @date 2023.01.05
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

#ifndef LIST_HPP
#define LIST_HPP

#include "utility.hpp"

namespace pyincpp
{

/**
 * @brief List template class, implemented by dynamic array.
 *
 * @tparam T the type of elements in the list
 */
template <typename T>
class List
{
    friend class String;
    friend class Integer;

public:
    /**
     * @brief List iterator class.
     */
    class Iterator
    {
        friend class List;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = value_type*;
        using reference = value_type&;

    private:
        // Current data pointer.
        T* current_;

        // Create an iterator that point to the current data of list.
        Iterator(T* current)
            : current_(current)
        {
        }

    public:
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
        bool operator==(const Iterator& that) const
        {
            return current_ == that.current_;
        }

        /**
         * @brief Check if two iterators are different.
         *
         * @param that another iterator
         * @return ture if two iterators are different, false otherwise
         */
        bool operator!=(const Iterator& that) const
        {
            return !(current_ == that.current_);
        }

        /**
         * @brief Increment the iterator: ++it.
         *
         * @return reference of this iterator that point to next data
         */
        Iterator& operator++()
        {
            ++current_;
            return *this;
        }

        /**
         * @brief Increment the iterator: it++.
         *
         * @return const reference of this iterator that point to current data
         */
        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++current_;
            return tmp;
        }

        /**
         * @brief Decrement the iterator: --it.
         *
         * @return reference of this iterator that point to previous data
         */
        Iterator& operator--()
        {
            --current_;
            return *this;
        }

        /**
         * @brief Decrement the iterator: it--.
         *
         * @return const reference of this iterator that point to current data
         */
        Iterator operator--(int)
        {
            Iterator tmp = *this;
            --current_;
            return tmp;
        }
    };

private:
    // Number of elements.
    int size_;

    // Available capacity.
    int capacity_;

    // Pointer to the data.
    T* data_;

    // Compare two lists.
    int compare(const List& that) const
    {
        for (int i = 0; i < size_ && i < that.size_; i++)
        {
            if (data_[i] > that.data_[i])
            {
                return 1;
            }
            else if (data_[i] < that.data_[i])
            {
                return -1;
            }
        }

        if (size_ > that.size_)
        {
            return 1;
        }
        else if (size_ < that.size_)
        {
            return -1;
        }
        else // size_ == that.size_
        {
            return 0;
        }
    }

public:
    /**
     * @brief Initial capacity.
     */
    static constexpr int INIT_CAPACITY = 4;

    /**
     * @brief Maximum capacity.
     */
    static constexpr int MAX_CAPACITY = INT_MAX - 1; // - 1 to prevent boundary subscript overflow

    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new empty list object.
     */
    List()
        : size_(0)
        , capacity_(INIT_CAPACITY)
        , data_(new T[capacity_])
    {
    }

    /**
     * @brief Create a list based on the given initializer list.
     *
     * @param il initializer list
     */
    List(const std::initializer_list<T>& il)
        : size_(int(il.size()))
        , capacity_(size_ > INIT_CAPACITY ? size_ : INIT_CAPACITY)
        , data_(new T[capacity_])
    {
        std::copy(il.begin(), il.end(), data_);
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another list
     */
    List(const List& that)
        : size_(that.size_)
        , capacity_(that.capacity_)
        , data_(new T[capacity_])
    {
        std::copy(that.data_, that.data_ + size_, data_);
    }

    /**
     * @brief Move constructor.
     *
     * @param that another list
     */
    List(List&& that)
        : size_(that.size_)
        , capacity_(that.capacity_)
        , data_(that.data_)
    {
        that.size_ = 0;
        that.capacity_ = INIT_CAPACITY;
        that.data_ = new T[that.capacity_];
    }

    /**
     * @brief Destroy the list object.
     */
    ~List()
    {
        delete[] data_; // the data pointer is guaranteed to be valid
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two lists are equal.
     *
     * @param that another list
     * @return true if two lists are equal
     */
    bool operator==(const List& that) const
    {
        return size_ == that.size_ && std::equal(data_, data_ + size_, that.data_);
    }

    /**
     * @brief Check whether two lists are not equal.
     *
     * @param that another list
     * @return true if two lists are not equal
     */
    bool operator!=(const List& that) const
    {
        return !(*this == that);
    }

    /**
     * @brief Compare two lists.
     *
     * @param that another list
     * @return true if this < that
     */
    bool operator<(const List& that) const
    {
        return compare(that) < 0;
    }

    /**
     * @brief Compare two lists.
     *
     * @param that another list
     * @return true if this <= that
     */
    bool operator<=(const List& that) const
    {
        return compare(that) <= 0;
    }

    /**
     * @brief Compare two lists.
     *
     * @param that another list
     * @return true if this > that
     */
    bool operator>(const List& that) const
    {
        return compare(that) > 0;
    }

    /**
     * @brief Compare two lists.
     *
     * @param that another list
     * @return true if this >= that
     */
    bool operator>=(const List& that) const
    {
        return compare(that) >= 0;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another list
     * @return self reference
     */
    List& operator=(const List& that)
    {
        if (this != &that)
        {
            size_ = that.size_;
            capacity_ = that.capacity_;

            delete[] data_;
            data_ = new T[capacity_];
            std::copy(that.data_, that.data_ + size_, data_);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another list
     * @return self reference
     */
    List& operator=(List&& that)
    {
        if (this != &that)
        {
            delete[] data_;

            size_ = that.size_;
            capacity_ = that.capacity_;
            data_ = that.data_;

            that.size_ = 0;
            that.capacity_ = INIT_CAPACITY;
            that.data_ = new T[that.capacity_];
        }

        return *this;
    }

    /*
     * Access
     */

    /**
     * @brief Return the reference to the element at the specified position in the list.
     *
     * Index can be negative, like Python's list: list[-1] gets the last element.
     *
     * @param index index of the element to return (-size() <= index < size())
     * @return reference to the element at the specified position in the list
     */
    T& operator[](int index)
    {
        internal::check_bounds(index, -size_, size_);

        return index >= 0 ? data_[index] : data_[size_ + index];
    }

    /**
     * @brief Return the const reference to element at the specified position in the list.
     *
     * Index can be negative, like Python's list: list[-1] gets the last element.
     *
     * @param index index of the element to return (-size() <= index < size())
     * @return const reference to the element at the specified position in the list
     */
    const T& operator[](int index) const
    {
        return const_cast<List&>(*this)[index];
    }

    /*
     * Iterator
     */

    /**
     * @brief Return an iterator to the first element of the list.
     *
     * If the list is empty, the returned iterator will be equal to end().
     *
     * @return iterator to the first element
     */
    Iterator begin() const
    {
        return Iterator(data_);
    }

    /**
     * @brief Return an iterator to the element following the last element of the list.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    Iterator end() const
    {
        return Iterator(data_ + size_); // not nullptr, because size_ <= capacity_
    }

    /*
     * Examination
     */

    /**
     * @brief Return the number of elements in the list.
     *
     * @return the number of elements in the list
     */
    int size() const
    {
        return size_;
    }

    /**
     * @brief Return true if the list contains no elements.
     *
     * @return true if the list contains no elements
     */
    bool is_empty() const
    {
        return size_ == 0;
    }

    /**
     * @brief Return the available capacity in the list.
     *
     * @return the available capacity in the list.
     */
    int capacity() const
    {
        return capacity_;
    }

    /**
     * @brief Return the iterator of the specified element in the list.
     *
     * Or end() if the list does not contain the element.
     *
     * @param element element to search for
     * @return the iterator of the specified element in the list, or end() if the list does not contain the element
     */
    Iterator find(const T& element) const
    {
        return std::find(begin(), end(), element);
    }

    /**
     * @brief Return the index of the first occurrence of the specified element in the list (at or after index start and before index stop).
     *
     * Or -1 if the list does not contain the element (in the specified range).
     *
     * @param element element to search for
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return the index of the first occurrence of the specified element in the list, or -1 if the list does not contain the element
     */
    int index(const T& element, int start = 0, int stop = MAX_CAPACITY + 1) const
    {
        stop = stop > size_ ? size_ : stop;
        auto it = std::find(data_ + start, data_ + stop, element);
        return it == data_ + stop ? -1 : it - data_;
    }

    /**
     * @brief Return true if the list contains the specified element.
     *
     * @param element element whose presence in the list is to be tested
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return true if the list contains the specified element
     */
    bool contains(const T& element, int start = 0, int stop = MAX_CAPACITY + 1) const
    {
        return index(element, start, stop) != -1;
    }

    /**
     * @brief Get the smallest item of the list.
     *
     * @return the smallest item
     */
    T min() const
    {
        internal::check_empty(size_);

        return *std::min_element(data_, data_ + size_);
    }

    /**
     * @brief Get the largest item of the list.
     *
     * @return the largest item
     */
    T max() const
    {
        internal::check_empty(size_);

        return *std::max_element(data_, data_ + size_);
    }

    /**
     * @brief Count the total number of occurrences of the specified element in the list.
     *
     * @param element the specified element
     * @return the total number of occurrences of the specified element in the list
     */
    int count(const T& element) const
    {
        return std::count_if(data_, data_ + size_, [&](const T& e)
                             { return e == element; });
    }

    /*
     * Manipulation
     */

    /**
     * @brief Insert the specified element at the specified position in the list.
     *
     * @param index index at which the specified element is to be inserted (-size() <= index <= size())
     * @param element element to be inserted
     */
    void insert(int index, const T& element)
    {
        // check
        internal::check_full(size_, MAX_CAPACITY);
        internal::check_bounds(index, -size_, size_ + 1);

        // if need, double capacity until MAX_CAPACITY
        if (size_ == capacity_)
        {
            adjust_capacity((capacity_ < MAX_CAPACITY / 2) ? capacity_ * 2 : MAX_CAPACITY);
        }

        // shift
        index = index >= 0 ? index : index + size_;
        std::copy_backward(data_ + index, data_ + size_, data_ + size_ + 1);

        // insert
        data_[index] = element; // copy assignment on T

        // resize
        ++size_;
    }

    /**
     * @brief Remove the element at the specified position in the list.
     *
     * @param index the index of the element to be removed (-size() <= index < size())
     * @return the removed element
     */
    T remove(int index)
    {
        // check
        internal::check_empty(size_);
        internal::check_bounds(index, -size_, size_);

        // move element
        index = index >= 0 ? index : index + size_;
        T element = std::move(data_[index]);

        // shift
        std::copy(data_ + index + 1, data_ + size_, data_ + index);

        // resize
        --size_;

        // return element
        return element;
    }

    /**
     * @brief Append the specified element to the end of the list.
     *
     * @param element element to be appended to the list
     * @return self reference
     */
    List& operator+=(const T& element)
    {
        insert(size_, element);

        return *this;
    }

    /**
     * @brief Append the specified list to the end of the list.
     *
     * @param list list to be appended to the list
     * @return self reference
     */
    List& operator+=(const List& list)
    {
        internal::check_full(size_ / 2 + list.size_ / 2, MAX_CAPACITY / 2);

        if (capacity_ < size_ + list.size_)
        {
            adjust_capacity(std::max(size_ + list.size_, INIT_CAPACITY));
        }

        std::copy(list.data_, list.data_ + list.size_, data_ + size_);
        size_ += list.size_;

        return *this;
    }

    /**
     * @brief Remove the first occurrence of the specified element from the list.
     *
     * @param element element to be removed
     * @return self reference
     */
    List& operator-=(const T& element)
    {
        int idx = index(element);
        if (idx != -1)
        {
            remove(idx);
        }

        return *this;
    }

    /**
     * @brief Add the list to itself a certain number of times.
     *
     * @param times times to repeat
     * @return self reference
     */
    List& operator*=(int times)
    {
        return *this = std::move(*this * times);
    }

    /**
     * @brief Remove all the specified elements from the list.
     *
     * @param element element to be removed
     * @return self reference
     */
    List& operator/=(const T& element)
    {
        T* it = std::remove(data_, data_ + size_, element);
        size_ -= (data_ + size_) - it;
        return *this;
    }

    /**
     * @brief Remove all of the elements from the list.
     *
     * @return self reference
     */
    List& clear()
    {
        // If the elements themselves are pointers, the pointed-to memory is not touched in any way.
        // Managing the pointer is the user's responsibility.
        size_ = 0;

        return *this;
    }

    /**
     * @brief Perform the given action for each element of the list.
     *
     * @tparam F function
     * @param action a function that to be performed for each element
     * @return self reference
     */
    template <typename F>
    List& map(const F& action)
    {
        std::for_each(begin(), end(), action);

        return *this;
    }

    /**
     * @brief Filter the elements in the list so that the elements that meet the predicate are retained.
     *
     * @tparam F funtion
     * @param predicate a predicate function that to be performed for each element
     * @return self reference
     */
    template <typename F>
    List& filter(const F& predicate)
    {
        auto it = std::copy_if(data_, data_ + size_, data_, predicate);
        size_ = it - data_;

        return *this;
    }

    /**
     * @brief Reverse the list in place.
     *
     * @return self reference
     */
    List& reverse()
    {
        std::reverse(data_, data_ + size_);

        return *this;
    }

    /**
     * @brief Eliminate duplicate elements of the list.
     *
     * Will not change the original relative order of elements.
     *
     * @return self reference
     */
    List& uniquify()
    {
        List buffer;
        buffer.adjust_capacity(std::max(size_, INIT_CAPACITY));
        for (int i = 0; i < size_; i++)
        {
            if (!buffer.contains(data_[i]))
            {
                buffer.data_[buffer.size_++] = data_[i];
            }
        }

        return *this = std::move(buffer);
    }

    /**
     * @brief Sort the list according to the order induced by the specified comparator.
     *
     * The sort is stable: the method will not reorder equal elements.
     *
     * @param comparator a function that performs compare
     * @return self reference
     */
    List& sort(bool (*comparator)(const T& e1, const T& e2) = [](const T& e1, const T& e2)
               { return e1 < e2; })
    {
        std::stable_sort(data_, data_ + size_, comparator);

        return *this;
    }

    /**
     * @brief Swap the contents of two lists.
     *
     * @param that the second list
     * @return self reference
     */
    List& swap(List& that)
    {
        std::swap(size_, that.size_);
        std::swap(capacity_, that.capacity_);
        std::swap(data_, that.data_);

        return *this;
    }

    /**
     * @brief Adjust capacity safely.
     *
     * @param new_capacity new capacity
     * @return self reference
     */
    List& adjust_capacity(int new_capacity)
    {
        if (new_capacity == 0)
        {
            throw std::runtime_error("Error: Capacity can not be zero.");
        }

        if (new_capacity < size_)
        {
            throw std::runtime_error("Error: Capacity can not be smaller than the size.");
        }

        if (new_capacity > MAX_CAPACITY)
        {
            throw std::runtime_error("Error: Capacity can not be larger than the maximum capacity.");
        }

        capacity_ = new_capacity;

        // move data
        T* tmp = new T[capacity_];
        std::copy(data_, data_ + size_, tmp);
        delete[] data_;
        data_ = tmp;

        return *this;
    }

    /**
     * @brief Shift the list to right n elements.
     *
     * @param n length of shifted elements
     * @return self reference
     */
    List& operator>>=(int n)
    {
        n %= size_;

        if (size_ <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this <<= -n;
        }

        std::rotate(data_, data_ + size_ - n, data_ + size_);

        return *this;
    }

    /**
     * @brief Shift the list to left n elements.
     *
     * @param n length of shifted elements
     * @return self reference
     */
    List& operator<<=(int n)
    {
        n %= size_;

        if (size_ <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this >>= -n;
        }

        std::rotate(data_, data_ + n, data_ + size_);

        return *this;
    }

    /**
     * @brief Erase the contents of a range of the list.
     *
     * @param start start range subscript (included)
     * @param stop stop range subscript (excluded)
     * @return self reference
     */
    List& erase(int start, int stop)
    {
        internal::check_bounds(start, 0, size_ + 1);
        internal::check_bounds(stop, 0, size_ + 1);

        std::copy(data_ + stop, data_ + size_, data_ + start);
        size_ -= (stop - start);

        return *this;
    }

    /*
     * Production (will produce new object)
     */

    /**
     * @brief Return slice of the list from start to stop with certain step.
     *
     * Index and step length can be negative.
     *
     * @param start start index (included)
     * @param stop stop index (excluded)
     * @param step step length (default 1)
     * @return the slice of the list
     */
    List slice(int start, int stop, int step = 1) const
    {
        // although I have implemented the Python-like slice function (index can exceed the size),
        // I finally decided to check the index for the simplicity of source code and the security of use, and consistent with the operator[]

        // check
        if (step == 0)
        {
            throw std::runtime_error("Error: Slice step can not be zero.");
        }

        internal::check_bounds(start, -size_, size_);
        internal::check_bounds(stop, -size_ - 1, size_ + 1);

        // convert
        start = start < 0 ? start + size_ : start;
        stop = stop < 0 ? stop + size_ : stop;

        // copy
        List list;
        for (int i = start; (step > 0) ? (i < stop) : (i > stop); i += step)
        {
            list += data_[i];
        }

        return list;
    }

    /**
     * @brief Generate a new list and append the specified element to the end of the list.
     *
     * @param element element to be appended to the list
     * @return the generated list
     */
    List operator+(const T& element) const
    {
        return List(*this) += element;
    }

    /**
     * @brief Generate a new list and append the specified list to the end of the list.
     *
     * @param list list to be appended to the list
     * @return the generated list
     */
    List operator+(const List& list) const
    {
        return List(*this) += list;
    }

    /**
     * @brief Generate a new list and remove the first occurrence of the specified element from the list.
     *
     * @param element element to be removed
     * @return the generated list
     */
    List operator-(const T& element) const
    {
        return List(*this) -= element;
    }

    /**
     * @brief Generate a new list and add the list to itself a certain number of times.
     *
     * @param times times to repeat
     * @return the generated list
     */
    List operator*(int times) const
    {
        if (times < 0)
        {
            throw std::runtime_error("Error: Times to repeat can not be less than zero.");
        }

        List buffer;
        buffer.adjust_capacity(std::max(size_ * times, INIT_CAPACITY));
        for (int part = 0; part < times; part++)
        {
            std::copy(data_, data_ + size_, buffer.data_ + part * size_);
        }
        buffer.size_ = size_ * times;

        return buffer;
    }

    /**
     * @brief Generate a new list and remove all the specified elements from the list.
     *
     * @param element element to be removed
     * @return the generated list
     */
    List operator/(const T& element) const
    {
        return List(*this) /= element;
    }

    /*
     * Print
     */

    /**
     * @brief Output the list to the specified output stream.
     *
     * @param os an output stream
     * @param list the list to be printed to the output stream
     * @return self reference of the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const List& list)
    {
        if (list.is_empty())
        {
            return os << "[]";
        }

        // This form looks complex, but there is only one judgment in the loop.
        // At the Assembly level (see https://godbolt.org/z/qT9n7GKf8), this is more efficient
        // than the usual short form of the generated machine code under O3-level optimization.
        // The inspiration comes from Java source code.
        os << "[";
        auto it = list.begin();
        while (true)
        {
            os << *it++;
            if (it == list.end())
            {
                return os << "]";
            }
            os << ", ";
        }
    }
};

} // namespace pyincpp

#endif // LIST_HPP
