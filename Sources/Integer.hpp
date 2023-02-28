/**
 * @file Integer.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Big integer class.
 * @version 1.0
 * @date 2023.01.21
 *
 * @copyright Copyright (c) 2023
 */

#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "List.hpp"
#include "String.hpp"

namespace mdspp
{

/**
 * @brief Big integer class.
 */
class Integer
{
    friend std::ostream& operator<<(std::ostream& os, const Integer& integer);

private:
    // List of digits, represent absolute value of the integer.
    List<signed char> digits_; // base 10, little endian

    // Sign of integer, '+' is positive, '-' is negative, and '0' is zero.
    char sign_;

    // Compare two integers.
    int compare(const Integer& that) const
    {
        if (sign_ != that.sign_)
        {
            if (sign_ == '+') // this is +, that is - or 0
            {
                return 1; // gt
            }
            else if (sign_ == '-') // this is -, that is + or 0
            {
                return -1; // lt
            }
            else // this is 0, that is + or -
            {
                return that.sign_ == '+' ? -1 : 1;
            }
        }

        // the sign of two integers is the same

        if (digits_.size_ != that.digits_.size_)
        {
            if (sign_ == '+')
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
                if (sign_ == '+')
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
        while (n-- > 0)
        {
            digits_ += 0;
        }

        return *this;
    }

    // Test whether a string represents an integer.
    static bool is_integer(const String& str)
    {
        if (str.size() == 0 || (str.size() == 1 && (str[0] == '+' || str[0] == '-')))
        {
            return false;
        }

        for (int i = (str[0] == '+' || str[0] == '-'); i < str.size(); i++)
        {
            if (str[i] < '0' || str[i] > '9')
            {
                return false;
            }
        }

        return true;
    }

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new integer object.
     */
    Integer()
        : digits_({0})
        , sign_('0')
    {
    }

    /**
     * @brief Construct a new integer object based on the given string.
     *
     * @param str string
     */
    Integer(const String& str)
        : digits_()
        , sign_()
    {
        if (!is_integer(str))
        {
            throw std::runtime_error("ERROR: Wrong integer literal.");
        }

        if (str[0] == '-' || str[0] == '+')
        {
            sign_ = str[0];
            for (int i = str.size() - 1; i >= 1; i--)
            {
                digits_ += str[i] - '0';
            }
        }
        else
        {
            sign_ = '+';
            for (int i = str.size() - 1; i >= 0; i--)
            {
                digits_ += str[i] - '0';
            }
        }

        remove_leading_zeros();

        if (digits_.size_ == 1 && digits_[0] == 0)
        {
            sign_ = '0';
        }
    }

    /**
     * @brief Construct a new integer object based on the given long long int.
     *
     * @param integer long long int
     */
    Integer(long long integer)
        : digits_()
        , sign_()
    {
        if (integer < 0)
        {
            sign_ = '-';
        }
        else if (integer > 0)
        {
            sign_ = '+';
        }
        else
        {
            sign_ = '0';
        }

        if (integer == 0)
        {
            digits_ += 0;
        }

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
        that.sign_ = '0';
    }

    /**
     * @brief Destroy the integer object.
     */
    ~Integer()
    {
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
        that.sign_ = '0';

        return *this;
    }

    /*
     * Examination (will not change the object itself)
     */

    /**
     * @brief Count the number of digits in an integer (based 10).
     *
     * @return the number of digits
     */
    int digits() const
    {
        return sign_ == '0' ? 0 : digits_.size_;
    }

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
     * Manipulation (will change the object itself)
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
     * @param rhs right-hand-side value
     * @return self reference
     */
    Integer& operator/=(const Integer& rhs)
    {
        return *this = *this / rhs;
    }

    /**
     * @brief this %= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Integer& operator%=(const Integer& rhs)
    {
        return *this = *this % rhs;
    }

    /*
     * Production (will produce new object)
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
     * @brief Return the opposite value of the copy of this.
     *
     * @return the opposite value of the copy of this.
     */
    Integer operator-() const
    {
        Integer num = *this;
        if (num.sign_ != '0')
        {
            num.sign_ = num.sign_ == '+' ? '-' : '+';
        }
        return num;
    }

    /**
     * @brief Return the absolute value of the copy of this.
     *
     * @return the absolute value of the copy of this
     */
    Integer abs() const
    {
        return sign_ == '-' ? -*this : *this;
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
        if (sign_ == '0' || rhs.sign_ == '0')
        {
            return sign_ == '0' ? rhs : *this;
        }

        // if the operands are of opposite signs, perform subtraction
        if (sign_ == '+' && rhs.sign_ == '-')
        {
            return *this - (-rhs);
        }
        else if (sign_ == '-' && rhs.sign_ == '+')
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
        if (sign_ == '0' || rhs.sign_ == '0')
        {
            return sign_ == '0' ? -rhs : *this;
        }

        // if the operands are of opposite signs, perform addition
        if (sign_ == '+' && rhs.sign_ == '-')
        {
            return *this + (-rhs);
        }
        else if (sign_ == '-' && rhs.sign_ == '+')
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
        result.sign_ = sign_;                         // the signs are same
        if (sign_ == '+' ? num1 < num2 : num1 > num2) // let num1.abs() >= num2.abs()
        {
            common::swap(num1, num2);
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

        // if result is zero, set sign to '0'
        result.sign_ = (result.digits_.size_ == 1 && result.digits_[0] == 0) ? '0' : result.sign_;

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
        if (sign_ == '0' || rhs.sign_ == '0')
        {
            return Integer();
        }

        // the sign of two integers is not zero

        // prepare variables
        int size = digits_.size_ + rhs.digits_.size_;

        Integer result;
        result.sign_ = (sign_ == rhs.sign_ ? '+' : '-'); // the sign is depends on the sign of operands
        result.add_leading_zeros(size - 1);              // result initially has a 0

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
        if (rhs.sign_ == '0')
        {
            throw std::runtime_error("ERROR: Divide by zero.");
        }

        // if this is zero, just return zero
        if (sign_ == '0')
        {
            return Integer();
        }

        // the sign of two integers is not zero

        // prepare variables
        int size = digits_.size_ - rhs.digits_.size_ + 1;

        Integer num1 = (*this).abs();

        Integer tmp;     // intermediate variable for rhs * 10^i
        tmp.sign_ = '+'; // positive

        Integer result;
        result.sign_ = (sign_ == rhs.sign_ ? '+' : '-'); // the sign is depends on the sign of operands
        result.add_leading_zeros(size - 1);              // result initially has a 0

        // calculation
        const auto& b = rhs.digits_;
        auto& c = result.digits_;
        for (int i = size - 1; i >= 0; i--)
        {
            tmp.digits_ = List<signed char>({0}) * i + b; // tmp = rhs * 10^i in O(N)

            while (num1 >= tmp) // <= 9 loops
            {
                c[i]++;
                num1 -= tmp;
            }
        }

        // if result is zero, set sign to '0'
        result.sign_ = (result.digits_.size_ == 1 && result.digits_[0] == 0) ? '0' : result.sign_;

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
     * @brief Return (this**n) % mod.
     *
     * @param n the power of this
     * @param mod module (default 0 means does not perform module)
     * @return (this**n) % mod
     */
    Integer pow(const Integer& n, const Integer& mod = 0) const
    {
        if (mod.sign_ == '0')
        {
            if (n == 0)
            {
                return 1;
            }
            else if (n % 2 == 0) // n is even
            {
                Integer y = pow(n / 2);
                return y * y;
            }
            else // n is odd
            {
                Integer y = pow(n / 2); // integer divide
                return *this * y * y;
            }
        }
        else // fast power algorithm
        {
            Integer num = *this;
            Integer exp = n;
            Integer result = 1;

            num %= mod;

            while (exp != 0)
            {
                if (exp % 2 == 1)
                {
                    result = (result * num) % mod;
                }
                num = (num * num) % mod;
                exp /= 2;
            }
            return result;
        }
    }

    /**
     * @brief Return the logical not of this.
     *
     * @return true if this == 0, otherwise false
     */
    bool operator!() const
    {
        return sign_ == '0' ? true : false;
    }

    /**
     * @brief Calculate the factorial of this.
     *
     * @return the factorial of this
     */
    Integer factorial() const
    {
        if (sign_ == '-')
        {
            throw std::runtime_error("ERROR: Negative integer have no factorial.");
        }

        if (sign_ == '0') // 基例(0! == 1)
        {
            return 1;
        }
        else // 链条
        {
            return *this * (*this - 1).factorial();
        }
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Output integer to the specified output stream.
 *
 * @param os an output stream
 * @param integer the integer to be printed to the output stream
 * @return self reference of the output stream
 */
inline std::ostream& operator<<(std::ostream& os, const Integer& integer)
{
    if (integer.sign_ == '-')
    {
        os << '-';
    }

    for (int i = integer.digits_.size() - 1; i >= 0; i--)
    {
        os << (char)(integer.digits_[i] + '0');
    }
    return os;
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
    String str;
    char ch;
    while (is.get(ch) && !isspace(ch)) // isspace() wider than isblank()
    {
        str += ch;
    }
    integer = str;

    return is;
}

} // namespace mdspp

#endif // INTEGER_HPP
