/**
 * @file Integer.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Big integer class.
 * @version 0.1
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
    // List of digits.
    List<signed char> digits_;

    // '+' for positive, '-' for negative, and '0' for zero.
    char sign_;

    // Compare two integers.
    int compare(const Integer& that) const
    {
        if (sign_ != that.sign_)
        {
            if (sign_ == '+') // that is -
            {
                return 1; // gt
            }
            else // this is -, that is +
            {
                return -1; // lt
            }
        }

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

    Integer& remove_leading_zeros()
    {
        while (digits_[-1] == 0 && digits_.size_ > 1)
        {
            digits_.remove(-1);
        }

        return *this;
    }

    Integer& add_leading_zeros(int n)
    {
        while (n-- > 0)
        {
            digits_ += 0;
        }

        return *this;
    }

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
    Integer()
        : digits_({0})
        , sign_('0')
    {
    }

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

    Integer(const Integer& that)
        : digits_(that.digits_)
        , sign_(that.sign_)
    {
    }

    Integer(Integer&& that)
        : digits_(std::move(that.digits_))
        , sign_(std::move(that.sign_))
    {
        that.digits_ += 0;
        that.sign_ = '0';
    }

    ~Integer()
    {
    }

    Integer& operator=(const Integer& that)
    {
        digits_ = that.digits_;
        sign_ = that.sign_;

        return *this;
    }

    Integer& operator=(Integer&& that)
    {
        digits_ = std::move(that.digits_);
        sign_ = std::move(that.sign_);

        that.digits_ += 0;
        that.sign_ = '0';

        return *this;
    }

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

    Integer operator+() const
    {
        return *this;
    }

    Integer operator-() const
    {
        Integer num = *this;
        if (num.sign_ != '0')
        {
            num.sign_ = num.sign_ == '+' ? '-' : '+';
        }
        return num;
    }

    Integer abs() const
    {
        return sign_ == '-' ? -*this : *this;
    }

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
        Integer num1 = *this;
        Integer num2 = rhs;
        Integer result;
        result.sign_ = sign_; // the signs are same

        // add leading zeros
        int size = std::max(digits_.size_, rhs.digits_.size_) + 1;
        num1.add_leading_zeros(size - 1 - num1.digits_.size_);
        num2.add_leading_zeros(size - 1 - num2.digits_.size_);
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
        Integer num1 = *this;
        Integer num2 = rhs;
        Integer result;
        result.sign_ = sign_;                         // the signs are same
        if (sign_ == '+' ? num1 < num2 : num1 > num2) // let num1.abs() >= num2.abs()
        {
            common::swap(num1, num2);
            result = -result;
        }

        // add leading zeros
        int size = std::max(digits_.size_, rhs.digits_.size_);
        num1.add_leading_zeros(size - num1.digits_.size_);
        num2.add_leading_zeros(size - num2.digits_.size_);
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

    Integer operator*(const Integer& rhs) const
    {
        // if one of the operands is zero, just return zero
        if (sign_ == '0' || rhs.sign_ == '0')
        {
            return Integer();
        }

        // the sign of two integers is not zero

        // prepare variables
        Integer result;
        result.sign_ = (sign_ == rhs.sign_ ? '+' : '-'); // the sign is depends on the sign of operands

        // add leading zeros
        int size = digits_.size_ + rhs.digits_.size_;
        result.add_leading_zeros(size - 1); // result initially has a 0

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

    Integer operator/(const Integer& rhs) const
    {
    }

    Integer operator%(const Integer& rhs) const
    {
    }

    bool operator!() const
    {
        return sign_ == '0' ? true : false;
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

} // namespace mdspp

#endif // INTEGER_HPP
