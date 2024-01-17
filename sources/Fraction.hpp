/**
 * @file Fraction.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Fraction class.
 * @date 2024.01.15
 *
 * @copyright Copyright (C) 2024
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

#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <cmath> // std::abs

#include "utility.hpp"

#include "Integer.hpp"
#include "String.hpp"

namespace mds
{

/**
 * @brief Fraction class.
 */
class Fraction
{
    friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction);

private:
    // Numerator.
    int numerator_;

    // Denominator.
    int denominator_;

    // Simplify the fraction.
    void simplify()
    {
        // make sure the denominator is a positive number
        if (denominator_ < 0)
        {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }

        // Euclid's algorithm
        int a = std::abs(numerator_);
        int b = std::abs(denominator_);
        while (b > 0)
        {
            int t = a % b;
            a = b;
            b = t;
        }
        numerator_ /= a;
        denominator_ /= a;
    }

    // Compare two fractions.
    int compare(const Fraction& that) const
    {
        // this = a/b; that = c/d;
        // so, this - that = a/b - c/d = (ad - bc)/(bd)
        // since bd is always positive, compute (ad-bc) only
        int a = this->numerator_;
        int b = this->denominator_;
        int c = that.numerator_;
        int d = that.denominator_;

        return a * d - b * c;
    }

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new fraction object.
     */
    Fraction(int numerator = 0, int denominator = 1)
    {
        if (denominator == 0)
        {
            throw std::runtime_error("Error: Zero denominator.");
        }

        numerator_ = numerator;
        denominator_ = denominator;

        simplify();
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another fraction
     */
    Fraction(const Fraction& that)
        : numerator_(that.numerator_)
        , denominator_(that.denominator_)
    {
    }

    /**
     * @brief Move constructor.
     *
     * @param that another fraction
     */
    Fraction(Fraction&& that)
        : numerator_(std::move(that.numerator_))
        , denominator_(std::move(that.denominator_))
    {
        that.numerator_ = 0;
        that.denominator_ = 1;
    }

    /**
     * @brief Destroy the fraction object.
     */
    ~Fraction()
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Compare two fractions.
     *
     * @param that another fraction
     * @return true if this == that
     */
    bool operator==(const Fraction& that) const
    {
        return compare(that) == 0;
    }

    /**
     * @brief Compare two fractions.
     *
     * @param that another fraction
     * @return true if this != that
     */
    bool operator!=(const Fraction& that) const
    {
        return compare(that) != 0;
    }

    /**
     * @brief Compare two fractions.
     *
     * @param that another fraction
     * @return true if this < that
     */
    bool operator<(const Fraction& that) const
    {
        return compare(that) < 0;
    }

    /**
     * @brief Compare two fractions.
     *
     * @param that another fraction
     * @return true if this <= that
     */
    bool operator<=(const Fraction& that) const
    {
        return compare(that) <= 0;
    }

    /**
     * @brief Compare two fractions.
     *
     * @param that another fraction
     * @return true if this > that
     */
    bool operator>(const Fraction& that) const
    {
        return compare(that) > 0;
    }

    /**
     * @brief Compare two fractions.
     *
     * @param that another fraction
     * @return true if this >= that
     */
    bool operator>=(const Fraction& that) const
    {
        return compare(that) >= 0;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another fraction
     * @return self reference
     */
    Fraction& operator=(const Fraction& that)
    {
        numerator_ = that.numerator_;
        denominator_ = that.denominator_;

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another fraction
     * @return self reference
     */
    Fraction& operator=(Fraction&& that)
    {
        numerator_ = std::move(that.numerator_);
        denominator_ = std::move(that.denominator_);

        that.numerator_ = 0;
        that.denominator_ = 0;

        return *this;
    }

    /*
     * Examination (will not change the object itself)
     */

    /**
     * @brief Convert this fraction to int type.
     *
     * @return a int represents the fraction
     */
    operator int()
    {
        return numerator_ / denominator_;
    }

    /**
     * @brief Convert this fraction to double type.
     *
     * @return a double represents the fraction
     */
    operator double()
    {
        return (double)numerator_ / denominator_;
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
    Fraction& operator+=(const Fraction& rhs)
    {
        return *this = *this + rhs;
    }

    /**
     * @brief this -= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Fraction& operator-=(const Fraction& rhs)
    {
        return *this = *this - rhs;
    }

    /**
     * @brief this *= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Fraction& operator*=(const Fraction& rhs)
    {
        return *this = *this * rhs;
    }

    /**
     * @brief this /= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Fraction& operator/=(const Fraction& rhs)
    {
        return *this = *this / rhs;
    }

    /**
     * @brief this %= rhs
     *
     * @param rhs right-hand-side value
     * @return self reference
     */
    Fraction& operator%=(const Fraction& rhs)
    {
        return *this = *this % rhs;
    }

    /**
     * @brief Increment the value by 1.
     *
     * @return this += 1
     */
    Fraction& operator++()
    {
        return *this += 1;
    }

    /**
     * @brief Decrement the value by 1.
     *
     * @return this -= 1
     */
    Fraction& operator--()
    {
        return *this -= 1;
    }

    /*
     * Production (will produce new object)
     */

    /**
     * @brief Return the copy of this.
     *
     * @return the copy of this.
     */
    Fraction operator+() const
    {
        return *this;
    }

    /**
     * @brief Return the opposite value of the copy of this.
     *
     * @return the opposite value of the copy of this.
     */
    Fraction operator-() const
    {
        Fraction fraction = *this;
        fraction.numerator_ = -fraction.numerator_;
        return fraction;
    }

    /**
     * @brief Return the absolute value of the copy of this.
     *
     * @return the absolute value of the copy of this
     */
    Fraction abs() const
    {
        return numerator_ >= 0 ? *this : -(*this);
    }

    /**
     * @brief Return this + rhs.
     *
     * @param rhs right-hand-side value
     * @return this + rhs
     */
    Fraction operator+(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.denominator_ + denominator_ * rhs.numerator_, denominator_ * rhs.denominator_);
    }

    /**
     * @brief Return this - rhs.
     *
     * @param rhs right-hand-side value
     * @return this - rhs
     */
    Fraction operator-(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.denominator_ - denominator_ * rhs.numerator_, denominator_ * rhs.denominator_);
    }

    /**
     * @brief Return this * rhs.
     *
     * @param rhs right-hand-side value
     * @return this * rhs
     */
    Fraction operator*(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.numerator_, denominator_ * rhs.denominator_);
    }

    /**
     * @brief Return this / rhs.
     *
     * @param rhs right-hand-side value (not zero)
     * @return this / rhs
     */
    Fraction operator/(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.denominator_, denominator_ * rhs.numerator_);
    }

    /**
     * @brief Return this % rhs.
     *
     * @param rhs right-hand-side value (not zero)
     * @return this % rhs
     */
    Fraction operator%(const Fraction& rhs) const
    {
        if (rhs == 0)
        {
            throw std::runtime_error("Error: Zero denominator.");
        }

        return Fraction((numerator_ * rhs.denominator_) % (rhs.numerator_ * denominator_), denominator_ * rhs.denominator_);
    }

    /**
     * @brief Generate a string that represents the fraction.
     *
     * @return a string that represents the fraction
     */
    String to_string() const
    {
        String str;

        if (denominator_ == 1)
        {
            str += Integer(numerator_).to_string();
        }
        else
        {
            str += Integer(numerator_).to_string();
            str += "/";
            str += Integer(denominator_).to_string();
        }

        return str;
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Output fraction to the specified output stream.
 *
 * @param os an output stream
 * @param fraction the fraction to be printed to the output stream
 * @return self reference of the output stream
 */
inline std::ostream& operator<<(std::ostream& os, const Fraction& fraction)
{
    if (fraction.denominator_ == 1)
    {
        return os << fraction.numerator_;
    }
    else
    {
        return os << fraction.numerator_ << "/" << fraction.denominator_;
    }
}

} // namespace mds

#endif // FRACTION_HPP
