//! @file str.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Str class.
//! @date 2023.01.08

#ifndef STR_HPP
#define STR_HPP

#include "detail.hpp"

#include "int.hpp"
#include "list.hpp"

namespace pyincpp
{

/// Str is immutable sequence of characters.
class Str
{
private:
    // String.
    const std::string str_;

    // Used for FSM.
    enum state
    {
        S_START = 1 << 0,    // start with blank character
        S_SIGN = 1 << 1,     // positive or negative sign
        S_INT = 1 << 2,      // integer part
        S_POINT = 1 << 3,    // decimal point that doesn't have left digit
        S_DEC = 1 << 4,      // decimal part
        S_EXP = 1 << 5,      // scientific notation identifier
        S_EXP_SIGN = 1 << 6, // positive or negative sign of exponent part
        S_EXP_NUM = 1 << 7,  // exponent part number
        S_END = 1 << 8,      // end with blank character
        S_OTHER = 1 << 9,    // other
    };

    // Used for FSM.
    enum event
    {
        E_BLANK = 1 << 10, // blank character: ' ', '\n', '\t', '\r'
        E_SIGN = 1 << 11,  // positive or negative sign: '+', '-'
        E_DIGIT = 1 << 12, // 36-based digit: '[0-9a-zA-Z]'
        E_POINT = 1 << 13, // decimal point: '.'
        E_EXP = 1 << 14,   // scientific notation identifier: 'e', 'E'
        E_OTHER = 1 << 15, // other
    };

    // Try to transform a character to an event.
    static event get_event(const char ch, const int base)
    {
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
        {
            return E_BLANK;
        }
        else if (ch == '+' || ch == '-')
        {
            return E_SIGN;
        }
        else if (char_to_integer(ch, base) != -1)
        {
            return E_DIGIT;
        }
        else if (ch == '.')
        {
            return E_POINT;
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
        std::string_view digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        auto pos = digits.find(std::toupper(digit));
        return (pos != std::string_view::npos && pos < base) ? pos : -1;
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
     * Constructor
     */

    /// Create an empty string.
    Str() = default;

    /// Create a string from null-terminated characters.
    Str(const char* chars)
        : str_(chars)
    {
    }

    /// Create a string from std::string.
    Str(const std::string& string)
        : str_(string)
    {
    }

    /// Copy constructor.
    Str(const Str& that) = default;

    /// Move constructor.
    Str(Str&& that)
        : str_(std::move(const_cast<std::string&>(that.str_)))
    {
    }

    /*
     * Comparison
     */

    /// Compare the string with another string.
    auto operator<=>(const Str& that) const = default;

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Str& operator=(const Str& that)
    {
        const_cast<std::string&>(str_) = that.str_;
        return *this;
    }

    /// Move assignment operator.
    Str& operator=(Str&& that)
    {
        const_cast<std::string&>(str_) = std::move(const_cast<std::string&>(that.str_));
        return *this;
    }

    /*
     * Iterator
     */

    /// Return an iterator to the first char of the string.
    auto begin() const
    {
        return str_.cbegin();
    }

    /// Return an iterator to the char following the last char of the string.
    auto end() const
    {
        return str_.cend();
    }

    /// Return a reverse iterator to the first char of the reversed string.
    auto rbegin() const
    {
        return str_.crbegin();
    }

    /// Return a reverse iterator to the char following the last char of the reversed string.
    auto rend() const
    {
        return str_.crend();
    }

    /*
     * Access
     */

    /// Return the const reference to element at the specified position in the string.
    /// Index can be negative, like Python's string: string[-1] gets the last element.
    const char& operator[](int index) const
    {
        detail::check_bounds(index, -size(), size());

        return str_[index >= 0 ? index : index + size()];
    }

    /*
     * Examination
     */

    /// Return the number of elements in the string.
    int size() const
    {
        return str_.size(); // no '\0'
    }

    /// Return true if the string contains no elements.
    bool is_empty() const
    {
        return str_.empty();
    }

    /// Return const pointer to contents. This is a pointer to internal data.
    /// It is undefined to modify the contents through the returned pointer.
    const char* data() const
    {
        return str_.data();
    }

    /// Return the index of the first occurrence of the specified pattern in the specified range [`start`, `stop`).
    /// Or -1 if the string does not contain the pattern (in the specified range).
    int find(const Str& pattern, int start = 0, int stop = INT_MAX) const
    {
        if (start > size())
        {
            return -1;
        }

        stop = stop > size() ? size() : stop;
        std::string_view view(str_.data() + start, stop - start);
        auto pos = view.find(pattern.str_);

        return pos == std::string::npos ? -1 : start + int(pos);
    }

    /// Return `true` if the string contains the specified `pattern` in the specified range [`start`, `stop`).
    bool contains(const Str& pattern, int start = 0, int stop = INT_MAX) const
    {
        return find(pattern, start, stop) != -1;
    }

    /// Count the total number of occurrences of the specified `pattern` in the string.
    int count(const Str& pattern) const
    {
        if (pattern.is_empty())
        {
            return size() + 1;
        }

        int cnt = 0;
        for (int start = 0; (start = find(pattern, start)) != -1; start += pattern.size())
        {
            ++cnt;
        }

        return cnt;
    }

    /// Convert the string to a double-precision floating-point decimal number.
    ///
    /// If the string is too big to be representable will return `HUGE_VAL`.
    /// If the string represents NaN will return `NAN`.
    /// If the string represents Infinity will return `(+-)INFINITY`.
    ///
    /// ### Example
    /// ```
    /// Str("233.33").to_decimal(); // 233.33
    /// Str("123.456e-3").to_decimal(); // 0.123456
    /// Str("1e+600").to_decimal(); // HUGE_VAL
    /// Str("nan").to_decimal(); // NAN
    /// Str("inf").to_decimal(); // INFINITY
    /// ```
    double to_decimal() const
    {
        // check infinity or nan
        static const std::vector pos_infs = {"inf", "+inf", "infinity", "+infinity"};
        static const std::vector neg_infs = {"-inf", "-infinity"};
        static const std::vector nans = {"nan", "+nan", "-nan"};

        auto tmp = lower().str_;
        if (std::find(pos_infs.begin(), pos_infs.end(), tmp) != pos_infs.end())
        {
            return INFINITY;
        }
        if (std::find(neg_infs.begin(), neg_infs.end(), tmp) != neg_infs.end())
        {
            return -INFINITY;
        }
        if (std::find(nans.begin(), nans.end(), tmp) != nans.end())
        {
            return NAN;
        }

        // not infinity or nan

        double sign = 1; // default '+'
        double decimal_part = 0;
        int decimal_cnt = 0;
        double exp_sign = 1; // default '+'
        int exp_part = 0;

        // FSM
        state st = S_START;
        for (int i = 0; i < size(); ++i)
        {
            event ev = get_event(str_[i], 10);
            switch (int(st) | int(ev))
            {
                case int(S_START) | int(E_BLANK):
                    st = S_START;
                    break;

                case int(S_START) | int(E_SIGN):
                    sign = (str_[i] == '+') ? 1 : -1;
                    st = S_SIGN;
                    break;

                case int(S_START) | int(E_POINT):
                case int(S_SIGN) | int(E_POINT):
                    st = S_POINT;
                    break;

                case int(S_START) | int(E_DIGIT):
                case int(S_SIGN) | int(E_DIGIT):
                case int(S_INT) | int(E_DIGIT):
                    decimal_part = decimal_part * 10 + char_to_integer(str_[i], 10);
                    st = S_INT;
                    break;

                case int(S_INT) | int(E_POINT):
                    st = S_DEC;
                    break;

                case int(S_POINT) | int(E_DIGIT):
                case int(S_DEC) | int(E_DIGIT):
                    decimal_part = decimal_part * 10 + char_to_integer(str_[i], 10);
                    decimal_cnt++;
                    st = S_DEC;
                    break;

                case int(S_INT) | int(E_EXP):
                case int(S_DEC) | int(E_EXP):
                    st = S_EXP;
                    break;

                case int(S_EXP) | int(E_SIGN):
                    exp_sign = (str_[i] == '+') ? 1 : -1;
                    st = S_EXP_SIGN;
                    break;

                case int(S_EXP) | int(E_DIGIT):
                case int(S_EXP_SIGN) | int(E_DIGIT):
                case int(S_EXP_NUM) | int(E_DIGIT):
                    exp_part = exp_part * 10 + char_to_integer(str_[i], 10);
                    st = S_EXP_NUM;
                    break;

                case int(S_INT) | int(E_BLANK):
                case int(S_DEC) | int(E_BLANK):
                case int(S_EXP_NUM) | int(E_BLANK):
                case int(S_END) | int(E_BLANK):
                    st = S_END;
                    break;

                default:
                    st = S_OTHER;
                    i = size(); // exit loop
                    break;
            }
        }
        if (st != S_INT && st != S_DEC && st != S_EXP_NUM && st != S_END)
        {
            throw std::runtime_error("Error: Invalid literal for to_decimal().");
        }

        return sign * ((decimal_part / std::pow(10, decimal_cnt)) * std::pow(10, exp_sign * exp_part));
    }

    /// Convert the string to an `Int` based on 2-36 `base`.
    ///
    /// Numeric character in 36 base: 0, 1, ..., 9, A(10), ..., F(15), G(16), ..., Y(34), Z(35).
    ///
    /// ### Example
    /// ```
    /// Str("233").to_integer(); // 233
    /// Str("cafebabe").to_integer(16); // 3405691582
    /// Str("z").to_integer(36); // 35
    /// Str("ffffffffffffffff").to_integer(16); // 18446744073709551615
    /// ```
    Int to_integer(int base = 10) const
    {
        // check base
        if (base < 2 || base > 36)
        {
            throw std::runtime_error("Error: Invalid base for to_integer().");
        }

        bool non_negative = true; // default '+'
        Int integer;

        // FSM
        state st = S_START;
        for (int i = 0; i < size(); ++i)
        {
            event ev = get_event(str_[i], base);
            switch (int(st) | int(ev))
            {
                case int(S_START) | int(E_BLANK):
                    st = S_START;
                    break;

                case int(S_START) | int(E_SIGN):
                    non_negative = (str_[i] == '+') ? true : false;
                    st = S_SIGN;
                    break;

                case int(S_START) | int(E_DIGIT):
                case int(S_SIGN) | int(E_DIGIT):
                case int(S_INT) | int(E_DIGIT):
                    integer = integer * base + char_to_integer(str_[i], base);
                    st = S_INT;
                    break;

                case int(S_INT) | int(E_BLANK):
                case int(S_END) | int(E_BLANK):
                    st = S_END;
                    break;

                default:
                    st = S_OTHER;
                    i = size(); // exit loop
                    break;
            }
        }
        if (st != S_INT && st != S_END)
        {
            throw std::runtime_error("Error: Invalid literal for to_integer().");
        }

        return non_negative ? integer : -integer;
    }

    /// Return `true` if the string begins with the specified string, otherwise return `false`.
    bool starts_with(const Str& str) const
    {
        return str_.starts_with(str.str_);
    }

    /// Return `true` if the string ends with the specified string, otherwise return `false`.
    bool ends_with(const Str& str) const
    {
        return str_.ends_with(str.str_);
    }

    /*
     * Production
     */

    /// Copy and rotate the string to right `n` characters.
    Str operator>>(int n) const
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this << -n;
        }

        return *this << size() - n;
    }

    /// Copy and rotate the string to left `n` characters.
    Str operator<<(int n) const
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        n %= size();

        if (n < 0)
        {
            n += size();
        }

        std::string buffer(size(), 0);
        std::rotate_copy(begin(), begin() + n, end(), buffer.begin());

        return buffer;
    }

    /// Rerurn the reversed string.
    Str reverse() const
    {
        std::string buffer(size(), 0);
        std::reverse_copy(begin(), end(), buffer.begin());

        return buffer;
    }

    /// Return a copy of the string with all the characters converted to lowercase.
    Str lower() const
    {
        std::string buffer(str_);
        std::transform(buffer.begin(), buffer.end(), buffer.begin(), [](char c)
                       { return std::tolower(c); });
        return buffer;
    }

    /// Return a copy of the string with all the characters converted to uppercase.
    Str upper() const
    {
        std::string buffer(str_);
        std::transform(buffer.begin(), buffer.end(), buffer.begin(), [](char c)
                       { return std::toupper(c); });
        return buffer;
    }

    /// Return a copy of the string and erase the contents of the string in the range [`start`, `stop`).
    Str erase(int start, int stop) const
    {
        detail::check_bounds(start, 0, size() + 1);
        detail::check_bounds(stop, 0, size() + 1);

        std::string buffer = str_;
        buffer.erase(buffer.begin() + start, buffer.begin() + stop);

        return buffer;
    }

    /// Replace the string.
    ///
    /// ### Example
    /// ```
    /// Str("hahaha").replace("a", "ooow~").replace("ooow", "o"); // "ho~ho~ho~"
    /// Str("abcdefg").replace("", "-"); // "-a-b-c-d-e-f-g-"
    /// ```
    Str replace(const Str& old_str, const Str& new_str) const
    {
        if (old_str.is_empty())
        {
            std::ostringstream ss;
            std::copy(str_.begin(), str_.end(), std::ostream_iterator<char>(ss, new_str.data()));
            return new_str.str_ + ss.str();
        }

        std::string buffer;
        int this_start = 0;
        for (int patt_start; (patt_start = find(old_str, this_start)) != -1; this_start = patt_start + old_str.size())
        {
            buffer += str_.substr(this_start, patt_start - this_start) + new_str.str_;
        }

        return buffer += str_.substr(this_start);
    }

    /// Remove leading and trailing characters (default is blank character) of the string.
    Str strip(const signed char& ch = -1) const
    {
        std::string buffer = str_;

        int i = 0;
        while (i < int(buffer.size()) && (ch == -1 ? buffer[i] <= 0x20 : buffer[i] == ch))
        {
            ++i;
        }
        buffer.erase(buffer.begin(), buffer.begin() + i);

        i = buffer.size() - 1;
        while (i >= 0 && (ch == -1 ? buffer[i] <= 0x20 : buffer[i] == ch))
        {
            --i;
        }
        buffer.erase(buffer.begin() + i + 1, buffer.end());

        return buffer;
    }

    /// Return slice of the string from `start` to `stop` with certain `step`.
    /// Index and step length can be negative.
    Str slice(int start, int stop, int step = 1) const
    {
        if (step == 0)
        {
            throw std::runtime_error("Error: Require step != 0 for slice(start, stop, step).");
        }

        detail::check_bounds(start, -size(), size());
        detail::check_bounds(stop, -size() - 1, size() + 1);

        // convert
        start = start < 0 ? start + size() : start;
        stop = stop < 0 ? stop + size() : stop;

        // copy
        std::string buffer;
        for (int i = start; (step > 0) ? (i < stop) : (i > stop); i += step)
        {
            buffer += str_[i];
        }

        return buffer;
    }

    /// Generate a new string and append the specified `element` to the end of the string.
    Str operator+(const char& element) const
    {
        return str_ + element;
    }

    /// Generate a new string and append the specified `string` to the end of the string.
    Str operator+(const Str& string) const
    {
        return str_ + string.str_;
    }

    /// Generate a new string and add the string to itself a certain number of `times`.
    Str operator*(int times) const
    {
        if (times < 0)
        {
            throw std::runtime_error("Error: Require times >= 0 for repeat.");
        }

        std::string buffer(size() * times, 0);
        for (int part = 0; part < times; part++)
        {
            std::copy(begin(), end(), buffer.begin() + size() * part);
        }

        return buffer;
    }

    /// Split the string with separator (default = " ").
    /// If `keep_empty` is set (default = false), empty strings will be retained.
    ///
    /// ### Example
    /// ```
    /// Str("one, two, three").split(", "); // ["one", "two", "three"]
    /// Str("   1   2   3   ").split(); // ["1", "2", "3"]
    /// Str("aaa").split("a"); // []
    /// Str("aaa").split("a", true); // ["", "", "", ""]
    /// ```
    List<Str> split(const Str& sep = " ", bool keep_empty = false) const
    {
        if (sep.is_empty())
        {
            throw std::runtime_error("Error: Empty separator.");
        }

        List<Str> str_list;
        int this_start = 0;
        for (int patt_start; (patt_start = find(sep, this_start)) != -1; this_start = patt_start + sep.size())
        {
            if (keep_empty || patt_start != this_start)
            {
                str_list += str_.substr(this_start, patt_start - this_start);
            }
        }
        if (keep_empty || this_start != size())
        {
            str_list += str_.substr(this_start);
        }

        return str_list;
    }

    /// Return a string which is the concatenation of the strings in `str_list`.
    ///
    /// ### Example
    /// ```
    /// Str(".").join({"192", "168", "0", "1"}) // "192.168.0.1"
    /// ```
    Str join(const List<Str>& str_list) const
    {
        if (str_list.is_empty())
        {
            return Str();
        }

        std::string buffer = str_list[0].str_;
        for (int i = 1; i < str_list.size(); ++i)
        {
            buffer += str_ + str_list[i].str_;
        }
        return buffer;
    }

    /// Format `args` according to the format string, and return the result as a string.
    template <typename... Args>
    Str format(const Args&... args) const
    {
        std::ostringstream oss;
        std::string_view str(str_);
        (format_helper(oss, str, args), ...);
        oss << str;
        return oss.str();
    }

    /*
     * Print / Input
     */

    /// Output the string to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Str& string)
    {
        os << "\"";
        std::for_each(string.begin(), string.end(), [&](const char& c)
                      { os << c; });
        os << "\"";

        return os;
    }

    /// Get a line of string from the specified input stream.
    friend std::istream& operator>>(std::istream& is, Str& string)
    {
        return std::getline(is, const_cast<std::string&>(string.str_));
    }

    friend struct std::hash<pyincpp::Str>;
};

} // namespace pyincpp

template <>
struct std::hash<pyincpp::Str> // explicit specialization
{
    std::size_t operator()(const pyincpp::Str& string) const
    {
        return std::hash<std::string>{}(string.str_);
    }
};

#endif // STR_HPP
