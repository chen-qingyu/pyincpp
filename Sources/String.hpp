/**
 * @file String.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief My C++ string library. Because the <string> is too crude to use, so I wrote one myself.
 * @version 1.0
 * @date 2023.01.08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef STRING_H
#define STRING_H

#include "List.hpp"

namespace mdspp
{

class String
{
private:
    // List of characters.
    List<char> list_;

    // Compare two strings lexicographically.
    int compare(const String& that) const
    {
        int diff = 0;
        for (int i = 0; i < list_.size_ && i < that.list_.size_ && diff == 0; i++)
        {
            diff = list_.data_[i] - that.list_.data_[i];
        }

        if (diff > 0)
        {
            return 1;
        }
        else if (diff < 0)
        {
            return -1;
        }
        else // diff == 0
        {
            if (list_.size_ > that.list_.size_)
            {
                return 1;
            }
            else if (list_.size_ < that.list_.size_)
            {
                return -1;
            }
            else // list_.size_ == that.list_.size_
            {
                return 0;
            }
        }
    }

    // Use the KMP algorithm to find the position of the pattern.
    int kmp(const char* this_str, const char* patt_str, int n, int m) const
    {
        if (n < m)
        {
            return -1;
        }

        if (m == 0) // "" is in any string at index 0.
        {
            return 0;
        }

        int* match = new int[m];
        match[0] = -1;

        for (int j = 1; j < m; j++)
        {
            int i = match[j - 1];
            while ((i >= 0) && (patt_str[i + 1] != patt_str[j]))
            {
                i = match[i];
            }
            match[j] = (patt_str[i + 1] == patt_str[j]) ? i + 1 : -1;
        }

        int s = 0;
        int p = 0;
        while (s < n && p < m)
        {
            if (this_str[s] == patt_str[p])
            {
                s++;
                p++;
            }
            else if (p > 0)
            {
                p = match[p - 1] + 1;
            }
            else
            {
                s++;
            }
        }

        delete[] match;

        return (p == m) ? (s - m) : -1;
    }

public:
    /**
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new string object.
     */
    String()
        : list_()
    {
    }

    /**
     * @brief Create a string from null-terminated byte string.
     *
     * @param chars a pointer to a null-terminated byte string
     */
    String(const char* chars)
        : list_()
    {
        for (int i = 0; chars[i] != '\0'; i++)
        {
            list_ += chars[i];
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another string
     */
    String(const String& that)
        : list_(that.list_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another string
     */
    String(String&& that)
        : list_(that.list_)
    {
    }

    /**
     * @brief Destroy the string object.
     */
    ~String()
    {
    }

    /**
     * Element access
     */

    /**
     * @brief Return the reference to the element at the specified position in the string.
     *
     * Index can be negative, like Python's string: string[-1] gets the last element.
     *
     * @param index index of the element to return (-size() <= index < size())
     * @return reference to the element at the specified position in the string
     */
    char& operator[](int index)
    {
        return list_[index];
    }

    /**
     * @brief Return the const reference to element at the specified position in the string.
     *
     * Index can be negative, like Python's string: string[-1] gets the last element.
     *
     * @param index index of the element to return (-size() <= index < size())
     * @return const reference to the element at the specified position in the string
     */
    const char& operator[](int index) const
    {
        return list_[index];
    }

    /**
     * Getter / Setter
     */

    /**
     * @brief Get the copy of the contents of the string.
     *
     * @return a pointer to a null-terminated byte string
     */
    char* get() const
    {
        char* chars = new char[list_.size_ + 1]; // add '\0'

        for (int i = 0; i < list_.size_; i++)
        {
            chars[i] = list_.data_[i];
        }
        chars[list_.size_] = '\0';

        return chars;
    }

    /**
     * @brief Set the contents of the string using null-terminated byte string.
     *
     * @param chars a pointer to a null-terminated byte string
     */
    void set(const char* chars)
    {
        list_.clear();
        for (int i = 0; chars[i] != '\0'; i++)
        {
            list_ += chars[i];
        }
    }

    /**
     * Examination (will not change itself)
     */

    /**
     * @brief Return the number of elements in the string.
     *
     * @return the number of elements in the string
     */
    int size() const
    {
        return list_.size_; // no '\0'
    }

    /**
     * @brief Return true if the string contains no elements.
     *
     * @return true if the string contains no elements
     */
    bool is_empty() const
    {
        return list_.size_ == 0;
    }

    /**
     * @brief Check whether two strings are equal.
     *
     * @param that another string
     * @return true if two strings are equal
     */
    bool operator==(const String& that) const
    {
        return compare(that) == 0;
    }

    /**
     * @brief Check whether two strings are not equal.
     *
     * @param that another string
     * @return true if two strings are not equal
     */
    bool operator!=(const String& that) const
    {
        return compare(that) != 0;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this < that
     */
    bool operator<(const String& that) const
    {
        return compare(that) < 0;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this <= that
     */
    bool operator<=(const String& that) const
    {
        return compare(that) <= 0;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this > that
     */
    bool operator>(const String& that) const
    {
        return compare(that) > 0;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this >= that
     */
    bool operator>=(const String& that) const
    {
        return compare(that) >= 0;
    }

    /**
     * @brief Return the index of the first occurrence of the specified element in the string (at or after index start and before index stop).
     *
     * Or -1 if the string does not contain the element (in the specified range).
     *
     * @param element element to search for
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return the index of the first occurrence of the specified element in the string, or -1 if the string does not contain the element
     */
    int index_of(const char& element, int start = 0, int stop = INT_MAX) const
    {
        return list_.index_of(element, start, stop);
    }

    /**
     * @brief Return true if the string contains the specified element.
     *
     * @param element element whose presence in the string is to be tested
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return true if the string contains the specified element
     */
    bool contains(const char& element, int start = 0, int stop = INT_MAX) const
    {
        return list_.contains(element, start, stop);
    }

    /**
     * @brief Get the smallest item of the string.
     *
     * @return the smallest item
     */
    char min() const
    {
        return list_.min();
    }

    /**
     * @brief Get the largest item of the string.
     *
     * @return the largest item
     */
    char max() const
    {
        return list_.max();
    }

    /**
     * @brief Count the total number of occurrences of the specified element in the string.
     *
     * @param element the specified element
     * @return the total number of occurrences of the specified element in the string
     */
    int count(const char& element) const
    {
        return list_.count(element);
    }

    /**
     * @brief Return the index of the first occurrence of the specified pattern in the string (at or after index start and before index stop).
     *
     * Or -1 if the string does not contain the pattern (in the specified range).
     *
     * Implemented by the KMP algorithm.
     *
     * @param pattern the pattern string
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return the index of the first occurrence of the specified pattern in the list, or -1 if the list does not contain the pattern
     */
    int find(const String& pattern, int start = 0, int stop = INT_MAX) const
    {
        stop = stop > list_.size_ ? list_.size_ : stop;

        char* this_str = list_.data_ + start;
        char* patt_str = pattern.list_.data_;
        int n = stop - start;
        int m = pattern.list_.size_;

        int pos = kmp(this_str, patt_str, n, m);

        return pos == -1 ? -1 : pos + start;
    }

    /**
     * Manipulation (will change itself)
     */

    /**
     * @brief Insert the specified element at the specified position in the string.
     *
     * @param index index at which the specified element is to be inserted (-size() <= index <= size())
     * @param element element to be inserted
     */
    void insert(int index, const char& element)
    {
        list_.insert(index, element);
    }

    /**
     * @brief Remove the element at the specified position in the string.
     *
     * @param index the index of the element to be removed (-size() <= index < size())
     * @return the removed element
     */
    char remove(int index)
    {
        return list_.remove(index);
    }

    /**
     * @brief Append the specified element to the end of the string.
     *
     * @param element element to be appended to the string
     * @return self reference
     */
    String& operator+=(const char& element)
    {
        list_ += element;

        return *this;
    }

    /**
     * @brief Append the specified string to the end of the string.
     *
     * @param string string to be appended to the string
     * @return self reference
     */
    String& operator+=(const String& string)
    {
        list_ += string.list_;

        return *this;
    }

    /**
     * @brief Remove the first occurrence of the specified element from the string, if it is present.
     *
     * If the string does not contain the element, it is unchanged.
     *
     * @param element element to be removed
     * @return self reference
     */
    String& operator-=(const char& element)
    {
        list_ -= element;

        return *this;
    }

    /**
     * @brief Add the string to itself a certain number of times.
     *
     * @param times times to repeat
     * @return self reference
     */
    String& operator*=(int times)
    {
        list_ *= times;

        return *this;
    }

    /**
     * @brief Remove all of the elements from the string.
     */
    void clear()
    {
        list_.clear();
    }

    /**
     * @brief Perform the given action for each element of the string.
     *
     * @tparam F function
     * @param action a function that to be performed for each element
     */
    template <typename F>
    void traverse(F action)
    {
        list_.traverse(action);
    }

    /**
     * @brief Reverse the string in place.
     */
    void reverse()
    {
        list_.reverse();
    }

    /**
     * @brief Eliminate duplicate elements of the string.
     */
    void uniquify()
    {
        list_.uniquify();
    }

    /**
     * @brief Sort the string according to the order induced by the specified comparator.
     *
     * The sort is stable: the method must not reorder equal elements.
     *
     * @param comparator a function that performs compare
     */
    void sort(bool (*comparator)(const char& e1, const char& e2) = [](const char& e1, const char& e2)
              { return e1 < e2; })
    {
        list_.sort(comparator);
    }

    /**
     * @brief Copy assignment operator.
     *
     * @param that another string
     * @return self reference
     */
    String& operator=(const String& that)
    {
        list_ = that.list_;

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another string
     * @return self reference
     */
    String& operator=(String&& that)
    {
        list_ = std::move(that.list_);

        return *this;
    }

    /**
     * Generate
     */

    /**
     * @brief Slice of the string from start to stop with certain step.
     *
     * Index and step length can be negative.
     *
     * @param start start index (included)
     * @param stop stop index (excluded)
     * @param step step length (default 1)
     * @return the slice of the string
     */
    String slice(int start, int stop, int step = 1) const
    {
        String string;
        string.list_ = this->list_.slice(start, stop, step);
        return string;
    }

    /**
     * @brief Generate a new string and append the specified element to the end of the string.
     *
     * @param element element to be appended to the string
     * @return the generated string
     */
    String operator+(const char& element) const
    {
        String new_string = *this;
        return new_string += element;
    }

    /**
     * @brief Generate a new string and append the specified string to the end of the string.
     *
     * @param string string to be appended to the string
     * @return the generated string
     */
    String operator+(const String& string) const
    {
        String new_string = *this;
        return new_string += string;
    }

    /**
     * @brief Generate a new string and remove the first occurrence of the specified element from the string, if it is present.
     *
     * @param element element to be removed
     * @return the generated string
     */
    String operator-(const char& element) const
    {
        String new_string = *this;
        return new_string -= element;
    }

    /**
     * @brief Generate a new string and add the string to itself a certain number of times.
     *
     * @param times times to repeat
     * @return the generated string
     */
    String operator*(int times) const
    {
        String new_string = *this;
        return new_string *= times;
    }
};

/**
 * Non-member functions
 */

/**
 * @brief Output string to the specified output stream.
 *
 * @param os an output stream
 * @param string the string to be printed to the output stream
 * @return self reference of the output stream
 */
std::ostream& operator<<(std::ostream& os, const String& string)
{
    os << "\"";
    for (int i = 0; i < string.size(); i++)
    {
        os << string[i];
    }
    os << "\"";

    return os;
}

/**
 * @brief Generate a new string and add the string to itself a certain number of times.
 *
 * @param times times to repeat
 * @param string the string
 * @return the generated string
 */
String operator*(int times, const String& string)
{
    return string * times;
}

} // namespace mdspp

#endif // STRING_H
