/**
 * @file String.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief String class, implemented by List of char.
 * @date 2023.01.08
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

#ifndef STRING_HPP
#define STRING_HPP

#include "utility.hpp"

#include "List.hpp"

namespace pyincpp
{

/**
 * @brief String class, implemented by List of char.
 */
class String
{
    friend class Integer;

private:
    // List of characters.
    List<char> list_;

    // Used for FSM.
    enum state
    {
        S_BEGIN_BLANK = 1 << 0,        // begin blank character
        S_SIGN = 1 << 1,               // positive or negative sign
        S_INT_PART = 1 << 2,           // integer part
        S_DEC_POINT_HAS_LEFT = 1 << 3, // decimal point has left digit
        S_DEC_POINT_NOT_LEFT = 1 << 4, // decimal point doesn't have left digit
        S_DEC_PART = 1 << 5,           // decimal part
        S_EXP = 1 << 6,                // scientific notation identifier
        S_EXP_SIGN = 1 << 7,           // positive or negative sign of exponent part
        S_EXP_PART = 1 << 8,           // exponent part
        S_END_BLANK = 1 << 9,          // end blank character
        S_OTHER = 1 << 10,             // other
    };

    // Used for FSM.
    enum event
    {
        E_BLANK = 1 << 11,     // blank character: '\n', '\r', '\t', ' '
        E_SIGN = 1 << 12,      // positive or negative sign: '+', '-'
        E_NUMBER = 1 << 13,    // number: '[0-9a-zA-Z]'
        E_DEC_POINT = 1 << 14, // decimal point: '.'
        E_EXP = 1 << 15,       // scientific notation identifier: 'e', 'E'
        E_OTHER = 1 << 16,     // other
    };

    // Use the KMP algorithm to find the position of the pattern.
    static int kmp(const char* this_str, const char* patt_str, int n, int m)
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

    // Try to transform a character to an event.
    static event get_event(const char ch, const int base)
    {
        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
        {
            return E_BLANK;
        }
        else if (char_to_integer(ch, base) != -1)
        {
            return E_NUMBER;
        }
        else if (ch == '-' || ch == '+')
        {
            return E_SIGN;
        }
        else if (ch == '.')
        {
            return E_DEC_POINT;
        }
        else if (ch == 'e' || ch == 'E')
        {
            return E_EXP;
        }
        return E_OTHER;
    }

    // Try to transform a character to an integer based on 2-36 base.
    static int char_to_integer(char digit, int base) // 2 <= base <= 36
    {
        static const char* upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const char* lower_digits = "0123456789abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < base; ++i)
        {
            if (digit == upper_digits[i] || digit == lower_digits[i])
            {
                return i;
            }
        }
        return -1; // not an integer
    }

    // Format helper, see https://codereview.stackexchange.com/questions/269425/implementing-stdformat
    template <typename T>
    static void format_helper(std::ostringstream& oss, std::string_view& str, const T& value)
    {
        std::size_t open_bracket = str.find('{');
        std::size_t close_bracket = str.find('}', open_bracket + 1);
        if (open_bracket == std::string::npos || close_bracket == std::string::npos)
        {
            return;
        }
        oss << str.substr(0, open_bracket) << value;
        str = str.substr(close_bracket + 1);
    }

public:
    /*
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
     * @brief Create a string from null-terminated characters.
     *
     * @param chars a pointer to the null-terminated characters
     */
    String(const char* chars)
        : list_()
    {
        const int size = int(std::strlen(chars));
        list_.adjust_capacity(std::max(size, list_.INIT_CAPACITY));
        list_.size_ = size;
        std::memcpy(list_.data_, chars, size);
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
        : list_(std::move(that.list_))
    {
    }

    /**
     * @brief Destroy the string object.
     */
    ~String()
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two strings are equal.
     *
     * @param that another string
     * @return true if two strings are equal
     */
    bool operator==(const String& that) const
    {
        return list_ == that.list_;
    }

    /**
     * @brief Check whether two strings are not equal.
     *
     * @param that another string
     * @return true if two strings are not equal
     */
    bool operator!=(const String& that) const
    {
        return list_ != that.list_;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this < that
     */
    bool operator<(const String& that) const
    {
        return list_ < that.list_;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this <= that
     */
    bool operator<=(const String& that) const
    {
        return list_ <= that.list_;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this > that
     */
    bool operator>(const String& that) const
    {
        return list_ > that.list_;
    }

    /**
     * @brief Compare two strings lexicographically.
     *
     * @param that another string
     * @return true if this >= that
     */
    bool operator>=(const String& that) const
    {
        return list_ >= that.list_;
    }

    /*
     * Assignment
     */

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

    /*
     * Access
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

    /*
     * Getter / Setter
     */

    /**
     * @brief Get the copy of the contents of the string.
     *
     * @return a pointer to null-terminated characters
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
     * @brief Set the contents of the string using null-terminated characters.
     *
     * @param chars a pointer to null-terminated characters
     */
    void set(const char* chars)
    {
        list_.clear();
        for (int i = 0; chars[i] != '\0'; i++)
        {
            list_ += chars[i];
        }
    }

    /*
     * Examination
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
     * @brief Return the index of the first occurrence of the specified pattern in the string (at or after index start and before index stop).
     *
     * Or -1 if the string does not contain the pattern (in the specified range).
     *
     * @param pattern the pattern string
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return the index of the first occurrence of the specified pattern in the string, or -1 if the string does not contain the pattern
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
     * @brief Return true if the string contains the specified pattern.
     *
     * @param pattern pattern whose presence in the string is to be tested
     * @param start at or after index start (default 0)
     * @param stop before index stop (default size())
     * @return true if the string contains the specified pattern
     */
    bool contains(const String& pattern, int start = 0, int stop = INT_MAX) const
    {
        return find(pattern, start, stop) != -1;
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
     * @brief Convert the string to a double-precision floating-point decimal number.
     *
     * If the string is too big to be representable will return `HUGE_VAL`.
     *
     * If the string represents NaN will return `NAN`.
     *
     * If the string represents Infinity will return `(+-)INFINITY`.
     *
     * Example:
     * ```
     * String("233.33").to_decimal(); // 233.33
     * String("123.456e-3").to_decimal(); // 0.123456
     * String("1e+600").to_decimal(); // HUGE_VAL
     * String("nan").to_decimal(); // NAN
     * String("inf").to_decimal(); // INFINITY
     * ```
     *
     * @return a number that can represent the string or `HUGE_VAL` or `NAN` or `(+-)INFINITY`.
     */
    double to_decimal() const
    {
        // check infinity or nan
        static const char* pos_infs[12] = {"inf", "INF", "Inf", "+inf", "+INF", "+Inf", "infinity", "INFINITY", "Infinity", "+infinity", "+INFINITY", "+Infinity"};
        static const char* neg_infs[6] = {"-inf", "-INF", "-Inf", "-infinity", "-INFINITY", "-Infinity"};
        static const char* nans[9] = {"nan", "NaN", "NAN", "+nan", "+NaN", "+NAN", "-nan", "-NaN", "-NAN"};

        for (int i = 0; i < 12; ++i)
        {
            if (*this == pos_infs[i])
            {
                return INFINITY;
            }
        }
        for (int i = 0; i < 6; ++i)
        {
            if (*this == neg_infs[i])
            {
                return -INFINITY;
            }
        }
        for (int i = 0; i < 9; ++i)
        {
            if (*this == nans[i])
            {
                return NAN;
            }
        }

        // not infinity or nan

        double sign = 1; // default '+'
        double decimal_part = 0;
        int decimal_cnt = 0;
        double exp_sign = 1; // default '+'
        int exp_part = 0;

        // FSM
        state st = S_BEGIN_BLANK;
        for (int i = 0; i < list_.size_; ++i)
        {
            event ev = get_event(list_.data_[i], 10);
            switch (st | ev)
            {
                case S_BEGIN_BLANK | E_BLANK:
                    st = S_BEGIN_BLANK;
                    break;

                case S_BEGIN_BLANK | E_SIGN:
                    sign = (list_.data_[i] == '+') ? 1 : -1;
                    st = S_SIGN;
                    break;

                case S_BEGIN_BLANK | E_DEC_POINT:
                case S_SIGN | E_DEC_POINT:
                    st = S_DEC_POINT_NOT_LEFT;
                    break;

                case S_BEGIN_BLANK | E_NUMBER:
                case S_SIGN | E_NUMBER:
                case S_INT_PART | E_NUMBER:
                    decimal_part = decimal_part * 10 + char_to_integer(list_.data_[i], 10);
                    st = S_INT_PART;
                    break;

                case S_INT_PART | E_DEC_POINT:
                    st = S_DEC_POINT_HAS_LEFT;
                    break;

                case S_DEC_POINT_NOT_LEFT | E_NUMBER:
                case S_DEC_PART | E_NUMBER:
                case S_DEC_POINT_HAS_LEFT | E_NUMBER:
                    decimal_part = decimal_part * 10 + char_to_integer(list_.data_[i], 10);
                    decimal_cnt++;
                    st = S_DEC_PART;
                    break;

                case S_INT_PART | E_EXP:
                case S_DEC_POINT_HAS_LEFT | E_EXP:
                case S_DEC_PART | E_EXP:
                    st = S_EXP;
                    break;

                case S_EXP | E_SIGN:
                    exp_sign = (list_.data_[i] == '+') ? 1 : -1;
                    st = S_EXP_SIGN;
                    break;

                case S_EXP | E_NUMBER:
                case S_EXP_SIGN | E_NUMBER:
                case S_EXP_PART | E_NUMBER:
                    exp_part = exp_part * 10 + char_to_integer(list_.data_[i], 10);
                    st = S_EXP_PART;
                    break;

                case S_INT_PART | E_BLANK:
                case S_DEC_POINT_HAS_LEFT | E_BLANK:
                case S_DEC_PART | E_BLANK:
                case S_EXP_PART | E_BLANK:
                case S_END_BLANK | E_BLANK:
                    st = S_END_BLANK;
                    break;

                default:
                    st = S_OTHER;
                    i = list_.size_; // exit loop
                    break;
            }
        }
        if (st != S_INT_PART && st != S_DEC_POINT_HAS_LEFT && st != S_DEC_PART && st != S_EXP_PART && st != S_END_BLANK)
        {
            throw std::runtime_error("Error: Invalid literal for to_decimal().");
        }

        return sign * ((decimal_part / std::pow(10, decimal_cnt)) * std::pow(10, exp_sign * exp_part));
    }

    /**
     * @brief Convert the string to an integer number based on 2-36 base.
     *
     * Numeric character in 36 base: 0, 1, ..., 9, A(10), ..., F(15), G(16), ..., Y(34), Z(35).
     *
     * Example:
     * ```
     * String("233").to_integer(); // 233
     * String("cafebabe").to_integer(16); // 3405691582
     * String("z").to_integer(36); // 35
     * ```
     *
     * @param base the base of an integer (2 <= base <= 36, default 10)
     * @return an integer number that can represent the string
     */
    long long to_integer(int base = 10) const
    {
        // check base
        if (base < 2 || base > 36)
        {
            throw std::runtime_error("Error: Invalid base for to_integer().");
        }

        long long sign = 1; // default '+'
        long long integer_part = 0;

        // FSM
        state st = S_BEGIN_BLANK;
        for (int i = 0; i < list_.size_; ++i)
        {
            event ev = get_event(list_.data_[i], base);
            switch (st | ev)
            {
                case S_BEGIN_BLANK | E_BLANK:
                    st = S_BEGIN_BLANK;
                    break;

                case S_BEGIN_BLANK | E_SIGN:
                    sign = (list_.data_[i] == '+') ? 1 : -1;
                    st = S_SIGN;
                    break;

                case S_BEGIN_BLANK | E_NUMBER:
                case S_SIGN | E_NUMBER:
                case S_INT_PART | E_NUMBER:
                    integer_part = integer_part * base + char_to_integer(list_.data_[i], base);
                    st = S_INT_PART;
                    break;

                case S_INT_PART | E_BLANK:
                case S_END_BLANK | E_BLANK:
                    st = S_END_BLANK;
                    break;

                default:
                    st = S_OTHER;
                    i = list_.size_; // exit loop
                    break;
            }
        }
        if (st != S_INT_PART && st != S_END_BLANK)
        {
            throw std::runtime_error("Error: Invalid literal for to_integer().");
        }

        return sign * integer_part;
    }

    /**
     * @brief Return true if the string begins with the specified string, otherwise return false.
     *
     * @param str the specified string
     * @return true if the string begins with the specified string
     */
    bool begin_with(const String& str) const
    {
        if (size() < str.size())
        {
            return false;
        }

        for (int i = 0; i < str.size(); i++)
        {
            if (str.list_[i] != list_[i])
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Return true if the string ends with the specified string, otherwise return false.
     *
     * @param str the specified string
     * @return true if the string ends with the specified string
     */
    bool end_with(const String& str) const
    {
        if (size() < str.size())
        {
            return false;
        }

        for (int i = 0; i < str.size(); i++)
        {
            if (str.list_[str.size() - 1 - i] != list_[size() - 1 - i])
            {
                return false;
            }
        }

        return true;
    }

    /*
     * Manipulation
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
     * @brief Remove the first occurrence of the specified element from the string.
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
     *
     * @return self reference
     */
    String& clear()
    {
        list_.clear();

        return *this;
    }

    /**
     * @brief Perform the given action for each element of the string.
     *
     * @tparam F function
     * @param action a function that to be performed for each element
     * @return self reference
     */
    template <typename F>
    String& map(const F& action)
    {
        list_.map(action);

        return *this;
    }

    /**
     * @brief Filter the elements in the string so that the elements that meet the predicate are retained.
     *
     * @tparam F function
     * @param predicate a predicate function that to be performed for each element
     * @return self reference
     */
    template <typename F>
    String& filter(const F& predicate)
    {
        list_.filter(predicate);

        return *this;
    }

    /**
     * @brief Reverse the string in place.
     *
     * @return self reference
     */
    String& reverse()
    {
        list_.reverse();

        return *this;
    }

    /**
     * @brief Eliminate duplicate elements of the string.
     *
     * Will not change the original relative order of elements.
     *
     * @return self reference
     */
    String& uniquify()
    {
        list_.uniquify();

        return *this;
    }

    /**
     * @brief Sort the string according to the order induced by the specified comparator.
     *
     * The sort is stable: the method must not reorder equal elements.
     *
     * @param comparator a function that performs compare
     * @return self reference
     */
    String& sort(bool (*comparator)(const char& e1, const char& e2) = [](const char& e1, const char& e2)
                 { return e1 < e2; })
    {
        list_.sort(comparator);

        return *this;
    }

    /**
     * @brief Swap the contents of two strings.
     *
     * @param that second string
     * @return self reference
     */
    String& swap(String& that)
    {
        list_.swap(that.list_);

        return *this;
    }

    /**
     * @brief Convert the string to lowercase.
     *
     * @return self reference
     */
    String& lower()
    {
        for (int i = 0; i < list_.size_; ++i)
        {
            list_.data_[i] = (list_.data_[i] >= 'A' && list_.data_[i] <= 'Z' ? list_.data_[i] + ('a' - 'A') : list_.data_[i]);
        }

        return *this;
    }

    /**
     * @brief Convert the string to uppercase.
     *
     * @return self reference
     */
    String& upper()
    {
        for (int i = 0; i < list_.size_; ++i)
        {
            list_.data_[i] = (list_.data_[i] >= 'a' && list_.data_[i] <= 'z' ? list_.data_[i] - ('a' - 'A') : list_.data_[i]);
        }

        return *this;
    }

    /**
     * @brief Erase the contents of a range of string.
     *
     * @param start start range subscript (included)
     * @param stop stop range subscript (excluded)
     * @return self reference
     */
    String& erase(int start, int stop)
    {
        list_.erase(start, stop);

        return *this;
    }

    /**
     * @brief Replace the string.
     *
     * @param old_str old substring
     * @param new_str new substring
     * @return self reference
     */
    String& replace(const String& old_str, const String& new_str)
    {
        String buffer;

        int this_start = 0;
        for (int patt_start = 0; (patt_start = find(old_str, this_start)) != -1; this_start = patt_start + old_str.size())
        {
            buffer += slice(this_start, patt_start);
            buffer += new_str;
        }
        if (this_start != size())
        {
            buffer += slice(this_start, size());
        }

        return *this = std::move(buffer);
    }

    /**
     * @brief Remove leading and trailing characters of the string.
     *
     * @param ch specified character to remove (default is blank character)
     * @return self reference
     */
    String& strip(const signed char& ch = -1)
    {
        int i = 0;
        while (i < list_.size_ && (ch == -1 ? list_.data_[i] <= 0x20 : list_.data_[i] == ch))
        {
            ++i;
        }
        erase(0, i);

        i = list_.size_ - 1;
        while (i >= 0 && (ch == -1 ? list_.data_[i] <= 0x20 : list_.data_[i] == ch))
        {
            --i;
        }
        erase(i + 1, list_.size_);

        return *this;
    }

    /**
     * @brief Shift the string to right n characters.
     *
     * @param n length of shifted characters
     * @return self reference
     */
    String& operator>>=(int n)
    {
        list_ >>= n;
        return *this;
    }

    /**
     * @brief Shift the string to left n characters.
     *
     * @param n length of shifted characters
     * @return self reference
     */
    String& operator<<=(int n)
    {
        list_ <<= n;
        return *this;
    }

    /*
     * Production (will produce new object)
     */

    /**
     * @brief Return slice of the string from start to stop with certain step.
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
        string.list_ = list_.slice(start, stop, step);
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
     * @brief Generate a new string and remove the first occurrence of the specified element from the string.
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

    /**
     * @brief Split string with separator.
     *
     * Example:
     * ```
     * String("one, two, three").split(", "); // ["one", "two", "three"]
     * ```
     *
     * @param sep separator string (default = " ")
     * @return a list of split strings
     */
    List<String> split(const String& sep = " ") const
    {
        if (sep.size() == 0)
        {
            throw std::runtime_error("Error: Empty separator.");
        }

        List<String> str_list;
        int this_start = 0;
        for (int patt_start = 0; (patt_start = find(sep, this_start)) != -1; this_start = patt_start + sep.size())
        {
            str_list += slice(this_start, patt_start);
        }
        if (this_start != size())
        {
            str_list += slice(this_start, size());
        }

        return str_list;
    }

    /**
     * @brief Return a string which is the concatenation of the strings in str_list.
     *
     * Example:
     * ```
     * String(".").join({"192", "168", "0", "1"}) // "192.168.0.1"
     * ```
     *
     * @param str_list a list of strings
     * @return a string which is the concatenation of the strings in str_list
     */
    String join(const List<String>& str_list) const
    {
        if (str_list.size_ == 0)
        {
            return String();
        }

        if (str_list.size_ == 1)
        {
            return str_list.data_[0];
        }

        String str;
        for (int i = 0; i < str_list.size_ - 1; i++)
        {
            str += str_list.data_[i];
            str += *this;
        }
        str += str_list.data_[str_list.size_ - 1];
        return str;
    }

    /**
     * @brief Format args according to the format string, and return the result as a string.
     *
     * @tparam Args types of arguments
     * @param args arguments to be formatted
     * @return formatted string
     */
    template <typename... Args>
    String format(const Args&... args) const
    {
        std::ostringstream oss;
        const char* s = get();
        std::string_view str(s);
        (format_helper(oss, str, args), ...);
        oss << str;
        delete[] s;
        return oss.str().c_str();
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Output string to the specified output stream.
 *
 * @param os an output stream
 * @param string the string to be printed to the output stream
 * @return self reference of the output stream
 */
inline std::ostream& operator<<(std::ostream& os, const String& string)
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
 * @brief Get a line of string from the specified input stream.
 *
 * @param is an input stream
 * @param string the string
 * @return self reference of the input stream
 */
inline std::istream& operator>>(std::istream& is, String& string)
{
    string.clear();
    char ch;
    while (is.get(ch) && ch != '\n') // "is >> ch" can't read '\n'
    {
        string += ch;
    }

    return is;
}

} // namespace pyincpp

#endif // STRING_HPP
