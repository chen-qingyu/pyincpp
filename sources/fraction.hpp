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
    int num_;

    // Denominator.
    int den_;

public:
    /*
     * Constructor
     */

    /// Create a fraction with value `numerator/denominator`.
    Fraction(int numerator = 0, int denominator = 1)
        : num_(numerator)
        , den_(denominator)
    {
        // make sure the denominator is not zero
        detail::check_zero(den_);

        // make sure the denominator is a positive number
        if (den_ < 0)
        {
            num_ = -num_;
            den_ = -den_;
        }

        // simplify
        int gcd = std::gcd(num_, den_);
        num_ /= gcd;
        den_ /= gcd;
    }

    /// Create a fraction with given double-precision floating-point `number`.
    Fraction(double number)
    {
        if (std::isnan(number) || std::isinf(number))
        {
            throw std::invalid_argument("Error: Invalid floating-point number.");
        }

        double int_part = std::floor(number);
        double dec_part = number - int_part;
        int precision = 1'000'000'000; // 10^floor(log10(INT_MAX))

        int gcd = std::gcd(int(std::round(dec_part * precision)), precision);
        num_ = std::round(dec_part * precision) / gcd;
        den_ = precision / gcd;
        num_ += int_part * den_;
    }

    /// Copy constructor.
    Fraction(const Fraction& that) = default;

    /// Move constructor.
    Fraction(Fraction&& that)
        : num_(std::move(that.num_))
        , den_(std::move(that.den_))
    {
        that.num_ = 0;
        that.den_ = 1;
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
        const int a = this->num_;
        const int b = this->den_;
        const int c = that.num_;
        const int d = that.den_;

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
        num_ = std::move(that.num_);
        den_ = std::move(that.den_);

        that.num_ = 0;
        that.den_ = 1;

        return *this;
    }

    /*
     * Examination
     */

    /// Convert the fraction to double type.
    operator double() const
    {
        return double(num_) / double(den_);
    }

    /// Get the numerator of this.
    int numerator() const
    {
        return num_;
    }

    /// Get the denominator of this.
    int denominator() const
    {
        return den_;
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
        num_ += den_;
        return *this;
    }

    /// Decrement the value by 1.
    Fraction& operator--()
    {
        num_ -= den_;
        return *this;
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
        return Fraction(-num_, den_);
    }

    /// Return the absolute value of this.
    Fraction abs() const
    {
        return Fraction(std::abs(num_), den_);
    }

    /// Return this + `rhs`.
    Fraction operator+(const Fraction& rhs) const
    {
        return Fraction(num_ * rhs.den_ + den_ * rhs.num_, den_ * rhs.den_);
    }

    /// Return this - `rhs`.
    Fraction operator-(const Fraction& rhs) const
    {
        return Fraction(num_ * rhs.den_ - den_ * rhs.num_, den_ * rhs.den_);
    }

    /// Return this * `rhs`.
    Fraction operator*(const Fraction& rhs) const
    {
        return Fraction(num_ * rhs.num_, den_ * rhs.den_);
    }

    /// Return this / `rhs` (not zero).
    Fraction operator/(const Fraction& rhs) const
    {
        return Fraction(num_ * rhs.den_, den_ * rhs.num_);
    }

    /// Return this % `rhs` (not zero).
    Fraction operator%(const Fraction& rhs) const
    {
        detail::check_zero(rhs);

        return Fraction((num_ * rhs.den_) % (rhs.num_ * den_), den_ * rhs.den_);
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
        if (fraction.den_ == 1)
        {
            return os << fraction.num_;
        }
        else
        {
            return os << fraction.num_ << "/" << fraction.den_;
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
