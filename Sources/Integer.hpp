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

public:
    Integer()
        : digits_()
        , sign_('0')
    {
    }

    Integer(const String& str)
        : digits_()
        , sign_()
    {
        // TODO 去掉开头的零，验证合法性

        String s = str;
        s.strip();

        if (s == "" || s == "0")
        {
            digits_ += 0;
            sign_ = '0';
        }
        else if (s[0] == '-' || s[0] == '+')
        {
            sign_ = s[0];
            for (int i = s.size() - 1; i >= 1; i--)
            {
                digits_ += s[i] - '0';
            }
        }
        else
        {
            sign_ = '+';
            for (int i = s.size() - 1; i >= 0; i--)
            {
                digits_ += s[i] - '0';
            }
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
        that.sign_ = '0';
        return *this;
    }

    int digits() const
    {
        return digits_.size_;
    }

    bool is_zero() const
    {
        return sign_ == '0';
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

    Integer operator+(const Integer& rhs) const
    {
        // if one of the operands is zero, just return another one
        if (sign_ == '0')
        {
            return rhs;
        }
        else if (rhs.sign_ == '0')
        {
            return *this;
        }

        // if the operands are of opposite signs, perform subtraction
        if (sign_ == '+' && rhs.sign_ == '-')
        {
            Integer num = rhs;
            num.sign_ = '+';
            return *this - num;
        }
        else if (sign_ == '-' && rhs.sign_ == '+')
        {
            Integer num = *this;
            num.sign_ = '+';
            return rhs - num;
        }

        // the signs are same
        Integer result;
        result.sign_ = sign_;

        // reserve space
        int size = std::max(digits_.size_, rhs.digits_.size_) + 1;
        for (int i = 0; i < size; i++)
        {
            result.digits_ += 0;
        }

        // add leading zeros
        Integer a = *this;
        Integer b = rhs;
        while (a.digits_.size_ < size - 1)
        {
            a.digits_ += 0;
        }
        while (b.digits_.size_ < size - 1)
        {
            b.digits_ += 0;
        }

        // simulate the vertical calculation
        for (int i = 0; i < size - 1; i++)
        {
            result.digits_[i] += a.digits_[i] + b.digits_[i];
            result.digits_[i + 1] = result.digits_[i] / 10;
            result.digits_[i] %= 10;
        }

        // eliminate leading zero
        if (result.digits_[size - 1] == 0)
        {
            result.digits_.remove(size - 1);
        }

        return result;
    }

    Integer operator-(const Integer& rhs) const
    {
        // if one of the operands is zero
        if (sign_ == '0')
        {
            return -rhs;
        }
        else if (rhs.sign_ == '0')
        {
            return *this;
        }

        // if the operands are of opposite signs, perform add
        if (sign_ == '+' && rhs.sign_ == '-')
        {
            Integer num = rhs;
            num.sign_ = '+';
            return *this + num;
        }
        else if (sign_ == '-' && rhs.sign_ == '+')
        {
            Integer num = rhs;
            num.sign_ = '-';
            return *this + num;
        }

        // the signs are same
        Integer result;
        result.sign_ = sign_;

        // reserve space
        int size = std::max(digits_.size_, rhs.digits_.size_);
        for (int i = 0; i < size; i++)
        {
            result.digits_ += 0;
        }

        // add leading zeros, let a >= b
        Integer a = *this;
        Integer b = rhs;
        if (a < b)
        {
            common::swap(a, b);
            result = -result;
        }
        while (b.digits_.size_ < size)
        {
            b.digits_ += 0;
        }

        // simulate the vertical calculation, assert a >= b
        for (int i = 0; i < size; i++)
        {
            if (a.digits_[i] < b.digits_[i]) // carry
            {
                a.digits_[i + 1]--;
                a.digits_[i] += 10;
            }
            result.digits_[i] = a.digits_[i] - b.digits_[i];
        }

        // eliminate leading zeros
        while (result.digits_[size - 1] == 0 && size > 1)
        {
            result.digits_.remove(size - 1);
            size--;
        }

        // if result is zero, set sign_ to '0'
        result.sign_ = (result.digits_.size_ == 1 && result.digits_[0] == 0) ? '0' : result.sign_;

        return result;
    }

    Integer operator*(const Integer& rhs) const
    {
        // if one of the operands is zero, just return zero
        if (sign_ == '0' || rhs.sign_ == '0')
        {
            return Integer(0);
        }

        // the sign is depends on the sign of operands
        Integer result;
        result.sign_ = (sign_ == rhs.sign_ ? '+' : '-');

        // reserve space
        int size = digits_.size_ + rhs.digits_.size_;
        for (int i = 0; i < size; i++)
        {
            result.digits_ += 0;
        }

        // simulate the vertical calculation
        for (int i = 0; i < digits_.size_; i++)
        {
            for (int j = 0; j < rhs.digits_.size_; j++)
            {
                result.digits_[i + j] += digits_[i] * rhs.digits_[j];
                result.digits_[i + j + 1] += result.digits_[i + j] / 10;
                result.digits_[i + j] %= 10;
            }
        }

        // eliminate leading zero
        if (result.digits_[size - 1] == 0)
        {
            result.digits_.remove(size - 1);
        }

        return result;
    }

    Integer operator/(const Integer& rhs) const
    {
    }

    Integer operator%(const Integer& rhs) const
    {
    }

    bool operator!() const
    {
        return is_zero() ? true : false;
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
    if (integer.sign_ == '0')
    {
        return os << '0';
    }

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