/**
 * @file List.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief List template class, implemented by array list.
 * @version 1.0
 * @date 2023.01.05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef LIST_HPP
#define LIST_HPP

#include <climits>          // INT_MAX
#include <initializer_list> // std::initializer_list
#include <ostream>          // std::ostream

#include "ListIterator.hpp"

#include "common/check_bounds.hpp"
#include "common/check_empty.hpp"
#include "common/check_full.hpp"

namespace mdspp
{

template <typename T>
class List
{
private:
    // Number of elements.
    int count;

    // Available capacity.
    int capacity;

    // Pointer to the data.
    T* data;

    // The default initial capacity.
    static const int DEFAULT_CAPACITY = 8;

    // Expand capacity safely.
    void expand_capacity()
    {
        capacity = (capacity < INT_MAX / 2) ? capacity * 2 : INT_MAX; // double the capacity until INT_MAX
        T* tmp = new T[capacity];
        for (int i = 0; i < count; ++i)
        {
            tmp[i] = data[i];
        }
        delete[] data;
        data = tmp;
    }

public:
    /**
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new list object.
     */
    List()
        : count(0)
        , capacity(DEFAULT_CAPACITY)
        , data(new T[capacity])
    {
    }

    /**
     * @brief Create a list based on the given initializer list.
     *
     * @param il initializer list
     */
    List(const std::initializer_list<T>& il)
        : count((int)il.size())
        , capacity(count)
        , data(new T[capacity]) // capacity may be 0, but OK
    {
        for (int i = 0; i < count; ++i)
        {
            data[i] = *(il.begin() + i);
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another list
     */
    List(const List<T>& that)
        : count(that.count)
        , capacity(that.capacity)
        , data(new T[capacity])
    {
        for (int i = 0; i < count; ++i)
        {
            data[i] = that.data[i];
        }
    }

    /**
     * @brief Move constructor.
     *
     * @param that another list
     */
    List(List<T>&& that)
        : count(that.count)
        , capacity(that.capacity)
        , data(that.data)
    {
        that.count = 0;
        that.capacity = DEFAULT_CAPACITY;
        that.data = new T[that.capacity];
    }

    /**
     * @brief Destroy the list object.
     */
    ~List()
    {
        delete[] data; // the data pointer is guaranteed to be valid
    }

    /**
     * Capacity
     */

    /**
     * @brief Return the number of elements in the list.
     *
     * @return the number of elements in the list
     */
    int size() const
    {
        return count;
    }

    /**
     * @brief Return true if the list contains no elements.
     *
     * @return true if the list contains no elements
     */
    bool is_empty() const
    {
        return count == 0;
    }

    /**
     * Element access
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
        common::check_bounds(index, -count, count);

        return index >= 0 ? data[index] : data[count + index];
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
        common::check_bounds(index, -count, count);

        return index >= 0 ? data[index] : data[count + index];
    }

    /**
     * Iterators
     */

    /**
     * @brief Return an iterator to the first element of the list.
     *
     * If the list is empty, the returned iterator will be equal to end().
     *
     * @return iterator to the first element
     */
    ListIterator<T> begin() const
    {
        return ListIterator<T>(data);
    }

    /**
     * @brief Return an iterator to the element following the last element of the list.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    ListIterator<T> end() const
    {
        return ListIterator<T>(data + count); // not nullptr, because count <= capacity
    }

    /**
     * Examination (will not change itself)
     */

    /**
     * @brief Return the index of the first occurrence of the specified element in the list, or -1 if the list does not contain the element.
     *
     * @param element element to search for
     * @return the index of the first occurrence of the specified element in the list, or -1 if the list does not contain the element
     */
    int index_of(const T& element) const
    {
        for (int i = 0; i < count; ++i)
        {
            if (data[i] == element)
            {
                return i;
            }
        }

        return -1;
    }

    /**
     * @brief Find the first occurrence of the specified element in the list.
     *
     * @param element element to search for
     * @return the iterator to the first occurrence of the specified element in the list, or end() if the list does not contain the element
     */
    ListIterator<T> find(const T& element) const
    {
        for (int i = 0; i < count; ++i)
        {
            if (data[i] == element)
            {
                return ListIterator<T>(data + i);
            }
        }

        return end();
    }

    /**
     * @brief Return true if the list contains the specified element.
     *
     * @param element element whose presence in the list is to be tested
     * @return true if the list contains the specified element
     */
    bool contains(const T& element) const
    {
        for (int i = 0; i < count; ++i)
        {
            if (data[i] == element)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Check whether two lists are equal.
     *
     * @param that another list
     * @return true if two lists are equal
     */
    bool operator==(const List<T>& that) const
    {
        if (count != that.count)
        {
            return false;
        }

        for (int i = 0; i < count; ++i)
        {
            if (data[i] != that.data[i])
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Check whether two lists are not equal.
     *
     * @param that another list
     * @return true if two lists are not equal
     */
    bool operator!=(const List<T>& that) const
    {
        return !(*this == that);
    }

    /**
     * Manipulation (will change itself)
     */

    /**
     * @brief Insert the specified element at the specified position in the list.
     *
     * @param index index at which the specified element is to be inserted (-size() <= index < size())
     * @param element element to be inserted
     */
    void insert(int index, const T& element)
    {
        // check
        common::check_full(count, INT_MAX);

        common::check_bounds(index, -count, count + 1);

        // expand capacity
        if (count == capacity)
        {
            expand_capacity();
        }

        // index
        index = index >= 0 ? index : index + count;
        for (int i = count; i > index; --i)
        {
            data[i] = data[i - 1];
        }

        // insert
        data[index] = element; // copy assignment on T

        // resize
        ++count;
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
        common::check_empty(count);

        common::check_bounds(index, -count, count);

        // get element
        index = index >= 0 ? index : index + count;
        T element = data[index];

        // index and remove
        for (int i = index + 1; i < count; ++i)
        {
            data[i - 1] = data[i];
        }

        // resize
        --count;

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
        insert(count, element);

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
        for (int i = 0; i < list.count; i++)
        {
            *this += list[i];
        }

        return *this;
    }

    /**
     * @brief Remove the first occurrence of the specified element from the list, if it is present.
     *
     * If the list does not contain the element, it is unchanged.
     *
     * @param element element to be removed
     * @return self reference
     */
    List& operator-=(const T& element)
    {
        int index = index_of(element);
        if (index != -1)
        {
            remove(index);
        }

        return *this;
    }

    /**
     * @brief Remove all of the elements from the list.
     */
    void clear()
    {
        if (count != 0)
        {
            count = 0;
            capacity = DEFAULT_CAPACITY;
            delete[] data;
            data = new T[capacity];
        }
    }

    /**
     * @brief Perform the given action for each element of the list.
     *
     * @tparam F function
     * @param action a function that to be performed for each element
     */
    template <typename F>
    void traverse(F action)
    {
        for (int i = 0; i < count; ++i)
        {
            action(data[i]);
        }
    }

    /**
     * @brief Sort the list according to the order induced by the specified comparator.
     *
     * The sort is stable: the method must not reorder equal elements.
     * All elements in the list must be mutually comparable using the specified comparator.
     *
     * @param comparator a function that performs compare
     */
    // TODO
    // void sort(bool (*comparator)(const T& e1, const T& e2) = [](const T& e1, const T& e2)
    //           { return e1 < e2; })
    // {
    // }

    /**
     * @brief Reverse the list in place.
     */
    void reverse()
    {
        for (int i = 0, j = count - 1; i < j; ++i, --j)
        {
            auto tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
        }
    }

    /**
     * @brief Eliminate duplicate elements of the list.
     */
    void uniquify()
    {
        // TODO
    }

    /**
     * @brief Slice of the list from start to stop with certain step.
     *
     * @param start start index (included)
     * @param stop stop index (excluded)
     * @param step step length
     * @return the slice of the list
     */
    List slice(int start, int stop, int step = 1)
    {
        // TODO 负数情况

        List list;

        for (int i = start; i < stop; i += step)
        {
            list += (*this)[i];
        }

        return list;
    }

    /**
     * @brief Copy assignment operator.
     *
     * @param that another list
     * @return self reference
     */
    List& operator=(const List<T>& that)
    {
        if (this != &that)
        {
            count = that.count;
            capacity = that.capacity;

            delete[] data;
            data = new T[capacity];
            for (int i = 0; i < count; ++i)
            {
                data[i] = that.data[i];
            }
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another list
     * @return self reference
     */
    List& operator=(List<T>&& that)
    {
        if (this != &that)
        {
            delete[] data;

            count = that.count;
            capacity = that.capacity;
            data = that.data;

            that.count = 0;
            that.capacity = DEFAULT_CAPACITY;
            that.data = new T[that.capacity];
        }

        return *this;
    }
};

/**
 * Non-member functions
 */

/**
 * @brief Output list data to the specified output stream.
 *
 * @tparam T the type of elements in the list, must be printable.
 * @param os an output stream
 * @param list the list to be printed to the output stream
 * @return self reference of the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list)
{
    if (list.is_empty())
    {
        return os << "[]";
    }

    auto it = list.begin();
    os << "[";
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

} // namespace mdspp

#endif // LIST_HPP
