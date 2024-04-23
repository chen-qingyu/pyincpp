/**
 * @file Integer.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Integer class, implemented by List of signed char.
 * @date 2023.01.21
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

#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "utility.hpp"

#include "List.hpp"

namespace pyincpp
{

/**
 * @brief Integer class, implemented by List of signed char.
 */
class Integer
{
private:
    // List of digits, represent absolute value of the integer.
    List<signed char> digits_; // base 10, little endian

    // Sign of integer, 1 is positive, -1 is negative, and 0 is zero.
    signed char sign_;

    // Compare two integers.
    int compare(const Integer& that) const
    {
        if (sign_ != that.sign_)
        {
            if (sign_ == 1) // this is +, that is - or 0
            {
                return 1; // gt
            }
            else if (sign_ == -1) // this is -, that is + or 0
            {
                return -1; // lt
            }
            else // this is 0, that is + or -
            {
                return that.sign_ == 1 ? -1 : 1;
            }
        }

        // the sign of two integers is the same

        if (digits_.size_ != that.digits_.size_)
        {
            if (sign_ == 1)
            {
                return digits_.size_ > that.digits_.size_ ? 1 : -1;
            }
            else
            {
                return digits_.size_ > that.digits_.size_ ? -1 : 1;
            }
        }

        for (int i = digits_.size_ - 1; i >= 0; i--)
        {
            if (digits_[i] != that.digits_[i])
            {
                if (sign_ == 1)
                {
                    return digits_[i] > that.digits_[i] ? 1 : -1;
                }
                else
                {
                    return digits_[i] > that.digits_[i] ? -1 : 1;
                }
            }
        }

        return 0; // eq
    }

    // Remove leading zeros.
    Integer& remove_leading_zeros()
    {
        while (digits_[-1] == 0 && digits_.size_ > 1)
        {
            digits_.remove(-1);
        }

        return *this;
    }

    // Add leading zeros.
    Integer& add_leading_zeros(int n)
    {
        digits_ += List<signed char>({0}) * n;

        return *this;
    }

    // Test whether the characters represent an integer.
    static bool is_integer(const char* chars, int len)
    {
        if (len == 0 || (len == 1 && (chars[0] == '+' || chars[0] == '-')))
        {
            return false;
        }

        for (int i = (chars[0] == '+' || chars[0] == '-'); i < len; i++)
        {
            if (chars[i] < '0' || chars[i] > '9')
            {
                return false;
            }
        }

        return true;
    }

    // Increment the absolute value by 1 quickly.
    void abs_inc()
    {
        digits_ += 0; // add a leading zero

        int i = 0;
        while (digits_[i] == 9)
        {
            ++i;
        }
        ++digits_[i];
        while (i != 0)
        {
            digits_[--i] = 0;
        }

        remove_leading_zeros();
    }

    // Decrement the absolute value by 1 quickly.
    void abs_dec()
    {
        int i = 0;
        while (digits_[i] == 0)
        {
            ++i;
        }
        --digits_[i];
        while (i != 0)
        {
            digits_[--i] = 9;
        }

        remove_leading_zeros();

        // if result is zero, set sign to 0
        sign_ = (digits_.size_ == 1 && digits_[0] == 0) ? 0 : sign_;
    }

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new zero integer object.
     */
    Integer()
        : digits_({0})
        , sign_(0)
    {
    }

    /**
     * @brief Construct a new integer object based on the given null-terminated characters.
     *
     * @param chars null-terminated characters
     */
    Integer(const char* chars)
        : digits_()
        , sign_()
    {
        int len = std::strlen(chars);
        if (!is_integer(chars, len))
        {
            throw std::runtime_error("Error: Wrong integer literal.");
        }

        sign_ = (chars[0] == '-' ? -1 : 1);
        int s = (chars[0] == '-' || chars[0] == '+'); // skip symbol
        for (int i = len - 1; i >= s; i--)
        {
            digits_ += chars[i] - '0';
        }

        remove_leading_zeros();

        if (digits_.size_ == 1 && digits_[0] == 0)
        {
            sign_ = 0;
        }
    }

    /**
     * @brief Construct a new integer object based on the given int.
     *
     * @param integer int
     */
    Integer(int integer)
        : digits_()
        , sign_()
    {
        if (integer == 0)
        {
            digits_ += 0;
            sign_ = 0;
            return;
        }

        // integer != 0
        sign_ = (integer > 0 ? 1 : -1);
        integer = std::abs(integer);
        while (integer > 0)
        {
            digits_ += integer % 10;
            integer /= 10;
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another integer
     */
    Integer(const Integer& that)
        : digits_(that.digits_)
        , sign_(that.sign_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another integer
     */
    Integer(Integer&& that)
        : digits_(std::move(that.digits_))
        , sign_(std::move(that.sign_))
    {
        that.digits_ += 0;
        that.sign_ = 0;
    }

    /**
     * @brief Destroy the integer object.
     */
    ~Integer()
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Compare two integers.
     *
     * @param that another integer
     * @return true if this == that
     */
    bool operator==(const Integer& that) const
    {
        return compare(that) == 0;
    }

    /**
     * @brief Compare two integers.
     *
     * @param that another integer
     * @return true if this != that
     */
    bool operator!=(const Integer& that) const
    {
        return compare(that) != 0;
    }

    /**
     * @brief Compare two integers.
     *
     * @param that another integer
     * @return true if this < that
     */
    bool operator<(const Integer& that) const
    {
        return compare(that) < 0;
    }

    /**
     * @brief Compare two integers.
     *
     * @param that another integer
     * @return true if this <= that
     */
    bool operator<=(const Integer& that) const
    {
        return compare(that) <= 0;
    }

    /**
     * @brief Compare two integers.
     *
     * @param that another integer
     * @return true if this > that
     */
    bool operator>(const Integer& that) const
    {
        return compare(that) > 0;
    }

    /**
     * @brief Compare two integers.
     *
     * @param that another integer
     * @return true if this >= that
     */
    bool operator>=(const Integer& that) const
    {
        return compare(that) >= 0;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another integer
     * @return self reference
     */
    Integer& operator=(const Integer& that)
    {
        digits_ = that.digits_;
        sign_ = that.sign_;

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another integer
     * @return self reference
     */
    Integer& operator=(Integer&& that)
    {
        digits_ = std::move(that.digits_);
        sign_ = std::move(that.sign_);

        that.digits_ += 0;
        that.sign_ = 0;

        return *this;
    }

    /*
     * Examination
     */

    /**
     * @brief Count the number of digits in the integer (based 10).
     *
     * @return the number of digits
     */
    int digits() const
    {
        return sign_ == 0 ? 0 : digits_.size_;
    }

    /**
     * @brief Determine whether the integer is zero quickly.
     *
     * @return true if this == 0
     */
    bool is_zero() const
    {
        return sign_ == 0;
    }

    /**
     * @brief Determine whether the integer is positive quickly.
     *
     * @return true if this > 0
     */
    bool is_positive() const
    {
        return sign_ == 1;
    }

    /**
     * @brief Determine whether the integer is negative quickly.
     *
     * @return true if this < 0
     */
    bool is_negative() const
    {
        return sign_ == -1;
    }

    /**
     * @brief Determine whether the integer is even quickly.
     *
     * @return true if this % 2 == 0
     */
    bool is_even() const
    {
        return digits_.data_[0] % 2 == 0;
    }

    /**
     * @brief Determine whether the integer is odd quickly.
     *
     * @return true if this % 2 == 1
     */
    bool is_odd() const
    {
        return digits_.data_[0] % 2 == 1;
    }

    /*
     * Manipulation
     */

    /**
     * @brief this += rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Integer& operator+=(const Integer& rhs)
    {
        return *this = *this + rhs;
    }

    /**
     * @brief this -= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Integer& operator-=(const Integer& rhs)
    {
        return *this = *this - rhs;
    }

    /**
     * @brief this *= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Integer& operator*=(const Integer& rhs)
    {
        return *this = *this * rhs;
    }

    /**
     * @brief this /= rhs
     *
     * @param rhs right-hand-side value (not zero)
     * @return self reference
     */
    Integer& operator/=(const Integer& rhs)
    {
        return *this = *this / rhs;
    }

    /**
     * @brief this %= rhs
     *
     * @param rhs right-hand-side value (not zero)
     * @return self reference
     */
    Integer& operator%=(const Integer& rhs)
    {
        return *this = *this % rhs;
    }

    /**
     * @brief Increment the value by 1 quickly.
     *
     * @return this += 1 faster
     */
    Integer& operator++()
    {
        if (sign_ == 1)
        {
            abs_inc();
        }
        else if (sign_ == -1)
        {
            abs_dec();
        }
        else // sign_ == 0
        {
            sign_ = 1;
            digits_[0] = 1;
        }

        return *this;
    }

    /**
     * @brief Decrement the value by 1 quickly.
     *
     * @return this -= 1 faster
     */
    Integer& operator--()
    {
        if (sign_ == 1)
        {
            abs_dec();
        }
        else if (sign_ == -1)
        {
            abs_inc();
        }
        else // sign_ == 0
        {
            sign_ = -1;
            digits_[0] = 1;
        }

        return *this;
    }

    /*
     * Production
     */

    /**
     * @brief Return the copy of this.
     *
     * @return the copy of this.
     */
    Integer operator+() const
    {
        return *this;
    }

    /**
     * @brief Return the opposite value of this.
     *
     * @return the opposite value of this.
     */
    Integer operator-() const
    {
        Integer num = *this;
        num.sign_ = -num.sign_;
        return num;
    }

    /**
     * @brief Return the absolute value of this.
     *
     * @return the absolute value of this
     */
    Integer abs() const
    {
        return sign_ == -1 ? -*this : *this;
    }

    /**
     * @brief Return this + rhs.
     *
     * @param rhs right-hand-side value
     * @return this + rhs
     */
    Integer operator+(const Integer& rhs) const
    {
        // if one of the operands is zero, just return another one
        if (sign_ == 0 || rhs.sign_ == 0)
        {
            return sign_ == 0 ? rhs : *this;
        }

        // if the operands are of opposite signs, perform subtraction
        if (sign_ == 1 && rhs.sign_ == -1)
        {
            return *this - (-rhs);
        }
        else if (sign_ == -1 && rhs.sign_ == 1)
        {
            return rhs - (-*this);
        }

        // the sign of two integers is the same and not zero

        // prepare variables
        int size = std::max(digits_.size_, rhs.digits_.size_) + 1;

        Integer num1 = *this;
        num1.add_leading_zeros(size - 1 - num1.digits_.size_);

        Integer num2 = rhs;
        num2.add_leading_zeros(size - 1 - num2.digits_.size_);

        Integer result;
        result.sign_ = sign_;               // the signs are same
        result.add_leading_zeros(size - 1); // result initially has a 0

        // simulate the vertical calculation
        auto& a = num1.digits_;
        auto& b = num2.digits_;
        auto& c = result.digits_;
        for (int i = 0; i < size - 1; i++)
        {
            c[i] += a[i] + b[i];
            c[i + 1] = c[i] / 10;
            c[i] %= 10;
        }

        // remove leading zeros and return result
        return result.remove_leading_zeros();
    }

    /**
     * @brief Return this - rhs.
     *
     * @param rhs right-hand-side value
     * @return this - rhs
     */
    Integer operator-(const Integer& rhs) const
    {
        // if one of the operands is zero
        if (sign_ == 0 || rhs.sign_ == 0)
        {
            return sign_ == 0 ? -rhs : *this;
        }

        // if the operands are of opposite signs, perform addition
        if (sign_ != rhs.sign_)
        {
            return *this + (-rhs);
        }

        // the sign of two integers is the same and not zero

        // prepare variables
        int size = std::max(digits_.size_, rhs.digits_.size_);

        Integer num1 = *this;
        num1.add_leading_zeros(size - num1.digits_.size_);

        Integer num2 = rhs;
        num2.add_leading_zeros(size - num2.digits_.size_);

        Integer result;
        result.sign_ = sign_;                       // the signs are same
        if (sign_ == 1 ? num1 < num2 : num1 > num2) // let num1.abs() >= num2.abs()
        {
            std::swap(num1, num2);
            result = -result;
        }
        result.add_leading_zeros(size - 1); // result initially has a 0

        // simulate the vertical calculation, assert a >= b
        auto& a = num1.digits_;
        auto& b = num2.digits_;
        auto& c = result.digits_;
        for (int i = 0; i < size; i++)
        {
            if (a[i] < b[i]) // carry
            {
                a[i + 1]--;
                a[i] += 10;
            }
            c[i] = a[i] - b[i];
        }

        // remove leading zeros
        result.remove_leading_zeros();

        // if result is zero, set sign to 0
        result.sign_ = ((result.digits_.size_ == 1 && result.digits_[0] == 0) ? 0 : result.sign_);

        // return result
        return result;
    }

    /**
     * @brief Return this * rhs.
     *
     * @param rhs right-hand-side value
     * @return this * rhs
     */
    Integer operator*(const Integer& rhs) const
    {
        // if one of the operands is zero, just return zero
        if (sign_ == 0 || rhs.sign_ == 0)
        {
            return 0;
        }

        // the sign of two integers is not zero

        // prepare variables
        int size = digits_.size_ + rhs.digits_.size_;

        Integer result;
        result.sign_ = (sign_ == rhs.sign_ ? 1 : -1); // the sign is depends on the sign of operands
        result.add_leading_zeros(size - 1);           // result initially has a 0

        // simulate the vertical calculation
        const auto& a = digits_;
        const auto& b = rhs.digits_;
        auto& c = result.digits_;
        for (int i = 0; i < a.size_; i++)
        {
            for (int j = 0; j < b.size_; j++)
            {
                c[i + j] += a[i] * b[j];
                c[i + j + 1] += c[i + j] / 10;
                c[i + j] %= 10;
            }
        }

        // remove leading zeros and return
        return result.remove_leading_zeros();
    }

    /**
     * @brief Return this / rhs.
     *
     * @param rhs right-hand-side value (not zero)
     * @return this / rhs
     */
    Integer operator/(const Integer& rhs) const
    {
        // if rhs is zero, throw an exception
        if (rhs.sign_ == 0)
        {
            throw std::runtime_error("Error: Divide by zero.");
        }

        // if this is zero or this.abs() < rhs.abs(), just return zero
        if (sign_ == 0 || digits_.size_ < rhs.digits_.size_)
        {
            return 0;
        }

        // the sign of two integers is not zero

        // prepare variables
        int size = digits_.size_ - rhs.digits_.size_ + 1;

        Integer num1 = (*this).abs();

        Integer tmp;   // intermediate variable for rhs * 10^i
        tmp.sign_ = 1; // positive

        Integer result;
        result.sign_ = (sign_ == rhs.sign_ ? 1 : -1); // the sign is depends on the sign of operands
        result.add_leading_zeros(size - 1);           // result initially has a 0

        // calculation
        const auto& b = rhs.digits_;
        auto& c = result.digits_;
        for (int i = size - 1; i >= 0; i--)
        {
            tmp.digits_ = List<signed char>({0}) * i + b; // tmp = rhs * 10^i in O(N)

            while (num1 >= tmp) // <= 9 loops, so O(1)
            {
                c[i]++;
                num1 -= tmp;
            }
        }

        // if result is zero, set sign to 0
        result.sign_ = ((result.digits_.size_ == 1 && result.digits_[0] == 0) ? 0 : result.sign_);

        // remove leading zeros and return
        return result.remove_leading_zeros();
    }

    /**
     * @brief Return this % rhs.
     *
     * @param rhs right-hand-side value (not zero)
     * @return this % rhs
     */
    Integer operator%(const Integer& rhs) const
    {
        return *this - (*this / rhs) * rhs;
    }

    /**
     * @brief Return (this**exp) % mod (mod default = 0 means does not perform module).
     *
     * @param exp the power of this
     * @param mod module (default = 0 means does not perform module)
     * @return (this**exp) % mod
     */
    Integer pow(const Integer& exp, const Integer& mod = 0) const
    {
        if (exp.is_negative())
        {
            return 0;
        }

        // fast power algorithm

        Integer num = *this;
        Integer n = exp;
        Integer result = 1; // this**0 == 1

        while (!n.is_zero())
        {
            if (n.is_odd())
            {
                result = (mod.is_zero() ? result * num : (result * num) % mod);
            }
            num = (mod.is_zero() ? num * num : (num * num) % mod);
            n /= 2; // integer divide
        }
        return result;
    }

    /**
     * @brief Return the factorial of this.
     *
     * @return the factorial of this
     */
    Integer factorial() const
    {
        if (sign_ == -1)
        {
            throw std::runtime_error("Error: Negative integer have no factorial.");
        }

        Integer result = 1;                           // 0! == 1
        for (Integer i = *this; i.is_positive(); --i) // fast judgement, fast decrement
        {
            result *= i;
        }
        return result;
    }

    /**
     * @brief Return the square root of this using Newton's method.
     *
     * @return the square root of this
     */
    Integer sqrt() const
    {
        if (sign_ == -1)
        {
            throw std::runtime_error("Error: Cannot compute square root of a negative integer.");
        }

        if (is_zero())
        {
            return 0;
        }
        else if (*this < 4) // can not be omitted, otherwise will enter an infinite loop due to precision problem
        {
            return 1;
        }

        // as far as possible to reduce the number of iterations
        Integer cur_sqrt = *this / 2;
        Integer pre_sqrt = 2;

        while (cur_sqrt != pre_sqrt)
        {
            pre_sqrt = cur_sqrt;
            cur_sqrt = (cur_sqrt + *this / cur_sqrt) / 2;
        }

        return cur_sqrt;
    }

    /**
     * @brief Convert the integer object to some integer of type T.
     *
     * @tparam T integer type: int, long, and any custom type that support basic arithmetic operations
     * @return an integer of type T represents the integer object
     */
    template <typename T>
    T to_integer() const
    {
        T result = 0;
        for (int i = digits_.size_ - 1; i >= 0; i--)
        {
            result = result * 10 + digits_.data_[i];
        }
        return result * sign_;
    }

    /*
     * Print
     */

    /**
     * @brief Output the integer to the specified output stream.
     *
     * @param os an output stream
     * @param integer the integer to be printed to the output stream
     * @return self reference of the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Integer& integer)
    {
        if (integer.sign_ == -1)
        {
            os << '-';
        }

        for (int i = integer.digits_.size() - 1; i >= 0; i--)
        {
            os << (char)(integer.digits_[i] + '0');
        }

        return os;
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Calculate the greatest common divisor of two integers using Euclidean algorithm.
 *
 * @param int1 integer 1
 * @param int2 integer 2
 * @return the greatest common divisor of two integers
 */
inline Integer gcd(const Integer& int1, const Integer& int2)
{
    Integer a = int1;
    Integer b = int2;

    while (!b.is_zero()) // a, b = b, a % b until b == 0
    {
        auto t = b;
        b = a % b;
        a = t;
    }

    return a; // a is GCD
}

/**
 * @brief Calculate the least common multiple of two integers.
 *
 * @param int1 integer 1
 * @param int2 integer 2
 * @return the least common multiple of two integers
 */
inline Integer lcm(const Integer& int1, const Integer& int2)
{
    if (int1.is_zero() || int2.is_zero())
    {
        return 0;
    }

    return (int1 * int2) / gcd(int1, int2); // LCM = (int1 * int2) / GCD
}

/**
 * @brief Get an integer from the specified input stream.
 *
 * @param is an input stream
 * @param integer the integer
 * @return self reference of the input stream
 */
inline std::istream& operator>>(std::istream& is, Integer& integer)
{
    std::string str;
    is >> str;
    integer = str.c_str();

    return is;
}

} // namespace pyincpp

#endif // INTEGER_HPP
