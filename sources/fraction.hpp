//! @file fraction.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Fraction class.
//! @date 2024.01.15

#ifndef FRACTION_HPP
#define FRACTION_HPP

#include "detail.hpp"

namespace pyincpp
{

/// Fraction provides support for rational number arithmetic.
class Fraction
{
private:
    // Numerator.
    int numerator_;

    // Denominator.
    int denominator_;

public:
    /*
     * Constructor
     */

    /// Create a fraction with value `numerator/denominator`.
    Fraction(int numerator = 0, int denominator = 1)
        : numerator_(numerator)
        , denominator_(denominator)
    {
        // make sure the denominator is not zero
        detail::check_zero(denominator_);

        // make sure the denominator is a positive number
        if (denominator_ < 0)
        {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }

        // simplify
        int gcd = std::gcd(numerator_, denominator_);
        numerator_ /= gcd;
        denominator_ /= gcd;
    }

    /// Copy constructor.
    Fraction(const Fraction& that) = default;

    /// Move constructor.
    Fraction(Fraction&& that)
        : numerator_(std::move(that.numerator_))
        , denominator_(std::move(that.denominator_))
    {
        that.numerator_ = 0;
        that.denominator_ = 1;
    }

    /*
     * Comparison
     */

    /// Compare the fraction with another fraction.
    auto operator<=>(const Fraction& that) const
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

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Fraction& operator=(const Fraction& that) = default;

    /// Move assignment operator.
    Fraction& operator=(Fraction&& that)
    {
        numerator_ = std::move(that.numerator_);
        denominator_ = std::move(that.denominator_);

        that.numerator_ = 0;
        that.denominator_ = 1;

        return *this;
    }

    /*
     * Examination
     */

    /// Convert the fraction to double type.
    operator double() const
    {
        return double(numerator_) / double(denominator_);
    }

    /// Get the numerator of this.
    int numerator() const
    {
        return numerator_;
    }

    /// Get the denominator of this.
    int denominator() const
    {
        return denominator_;
    }

    /*
     * Manipulation
     */

    /// Return this += `rhs`.
    Fraction& operator+=(const Fraction& rhs)
    {
        return *this = *this + rhs;
    }

    /// Return this -= `rhs`.
    Fraction& operator-=(const Fraction& rhs)
    {
        return *this = *this - rhs;
    }

    /// Return this *= `rhs`.
    Fraction& operator*=(const Fraction& rhs)
    {
        return *this = *this * rhs;
    }

    /// Return this /= `rhs` (not zero).
    Fraction& operator/=(const Fraction& rhs)
    {
        return *this = *this / rhs;
    }

    /// Return this %= `rhs` (not zero).
    Fraction& operator%=(const Fraction& rhs)
    {
        return *this = *this % rhs;
    }

    /// Increment the value by 1.
    Fraction& operator++()
    {
        return *this += 1;
    }

    /// Decrement the value by 1.
    Fraction& operator--()
    {
        return *this -= 1;
    }

    /*
     * Production
     */

    /// Return the copy of this.
    Fraction operator+() const
    {
        return *this;
    }

    /// Return the opposite value of this.
    Fraction operator-() const
    {
        return Fraction(-numerator_, denominator_);
    }

    /// Return the absolute value of this.
    Fraction abs() const
    {
        return numerator_ >= 0 ? *this : -(*this);
    }

    /// Return this + `rhs`.
    Fraction operator+(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.denominator_ + denominator_ * rhs.numerator_, denominator_ * rhs.denominator_);
    }

    /// Return this - `rhs`.
    Fraction operator-(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.denominator_ - denominator_ * rhs.numerator_, denominator_ * rhs.denominator_);
    }

    /// Return this * `rhs`.
    Fraction operator*(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.numerator_, denominator_ * rhs.denominator_);
    }

    /// Return this / `rhs` (not zero).
    Fraction operator/(const Fraction& rhs) const
    {
        return Fraction(numerator_ * rhs.denominator_, denominator_ * rhs.numerator_);
    }

    /// Return this % `rhs` (not zero).
    Fraction operator%(const Fraction& rhs) const
    {
        detail::check_zero(rhs);

        return Fraction((numerator_ * rhs.denominator_) % (rhs.numerator_ * denominator_), denominator_ * rhs.denominator_);
    }

    /// Calculate the greatest common divisor of two fractions.
    static Fraction gcd(const Fraction& a, const Fraction& b)
    {
        return detail::gcd(a, b);
    }

    /// Calculate the least common multiple of two fractions.
    static Fraction lcm(const Fraction& a, const Fraction& b)
    {
        if (a == 0 || b == 0)
        {
            return 0;
        }

        return (a * b).abs() / gcd(a, b); // LCM = |a * b| / GCD
    }

    /*
     * Print / Input
     */

    /// Output the fraction to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction)
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

    /// Get a fraction from the specified input stream.
    ///
    /// ### Example
    /// ```
    /// Fraction f1, f2;
    /// std::istringstream("+1/-2 233") >> f1 >> f2;
    /// // f1 == Fraction(-1, 2);
    /// // f2 == Fraction(233);
    /// ```
    friend std::istream& operator>>(std::istream& is, Fraction& fraction)
    {
        while (is.peek() <= 0x20)
        {
            is.ignore();
        }
        if (!(std::isdigit(is.peek()) || is.peek() == '-' || is.peek() == '+')) // handle "z1/2"
        {
            throw std::runtime_error("Error: Wrong fraction literal.");
        }

        int num;
        is >> num;
        if (is.peek() <= 0x20) // next char is white space, ok
        {
            fraction = num;
            return is;
        }

        char c;
        int den;
        is >> c;
        if (!(std::isdigit(is.peek()) || is.peek() == '-' || is.peek() == '+')) // handle "1z/2" or "1/z2"
        {
            throw std::runtime_error("Error: Wrong fraction literal.");
        }
        is >> den;
        if (c != '/' || is.peek() > 0x20) // handle "1|2" or "1/2z"
        {
            throw std::runtime_error("Error: Wrong fraction literal.");
        }
        fraction = Fraction(num, den);
        return is;
    }
};

} // namespace pyincpp

template <>
struct std::hash<pyincpp::Fraction> // explicit specialization
{
    std::size_t operator()(const pyincpp::Fraction& fraction) const
    {
        return std::hash<int>{}(fraction.numerator()) ^ (std::hash<int>{}(fraction.denominator()) << 1);
    }
};

#endif // FRACTION_HPP
