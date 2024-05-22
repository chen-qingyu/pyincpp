//! @file complex.hpp
//! @author Qingyu Chen (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
//! @brief Complex class.
//! @date 2024.05.22
//!
//! @copyright Copyright (C) 2024
//!
//! This program is free software: you can redistribute it and/or modify
//! it under the terms of the GNU General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//!
//! This program is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//! GNU General Public License for more details.
//!
//! You should have received a copy of the GNU General Public License
//! along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "utility.hpp"

namespace pyincpp
{

/// Complex number class.
class Complex
{
private:
    // Real part.
    double real_;

    // Imaginary part.
    double imag_;

public:
    /*
     * Constructor
     */

    /// Construct a new complex object with value `real+imag*j`.
    Complex(double real = 0, double imag = 0)
        : real_(real)
        , imag_(imag)
    {
    }

    /// Copy constructor.
    Complex(const Complex& that) = default;

    /// Move constructor.
    Complex(Complex&& that)
        : real_(std::move(that.real_))
        , imag_(std::move(that.imag_))
    {
        that.real_ = 0;
        that.imag_ = 0;
    }

    /*
     * Comparison
     */

    /// Compare the complex with another complex.
    bool operator==(const Complex& that) const
    {
        constexpr double epsilon = std::numeric_limits<double>::epsilon();
        return std::abs(real_ - that.real_) < epsilon && std::abs(imag_ - that.imag_) < epsilon;
    }

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Complex& operator=(const Complex& that) = default;

    /// Move assignment operator.
    Complex& operator=(Complex&& that)
    {
        real_ = std::move(that.real_);
        imag_ = std::move(that.imag_);

        that.real_ = 0;
        that.imag_ = 0;

        return *this;
    }

    /*
     * Examination
     */

    /// Return the real part.
    double real() const
    {
        return real_;
    }

    /// Return the imaginary part.
    double imag() const
    {
        return imag_;
    }

    /// Return the absolute value (distance from origin) of this.
    double abs() const
    {
        return std::sqrt(real_ * real_ + imag_ * imag_);
    }

    /// Return the phase angle (in radians) of this.
    double arg() const
    {
        return std::atan2(imag_, real_);
    }

    /*
     * Manipulation
     */

    /// Return this += `rhs`.
    Complex& operator+=(const Complex& rhs)
    {
        return *this = *this + rhs;
    }

    /// Return this -= `rhs`.
    Complex& operator-=(const Complex& rhs)
    {
        return *this = *this - rhs;
    }

    /// Return this *= `rhs`.
    Complex& operator*=(const Complex& rhs)
    {
        return *this = *this * rhs;
    }

    /// Return this /= `rhs` (not zero).
    Complex& operator/=(const Complex& rhs)
    {
        return *this = *this / rhs;
    }

    /*
     * Production
     */

    /// Return the copy of this.
    Complex operator+() const
    {
        return *this;
    }

    /// Return the opposite value of this.
    Complex operator-() const
    {
        return Complex(-real_, -imag_);
    }

    /// Return the conjugate value of this.
    Complex conjugate() const
    {
        return Complex(real_, -imag_);
    }

    /// Return this + `rhs`.
    Complex operator+(const Complex& rhs) const
    {
        return Complex(real_ + rhs.real_, imag_ + rhs.imag_);
    }

    /// Return this - `rhs`.
    Complex operator-(const Complex& rhs) const
    {
        return Complex(real_ - rhs.real_, imag_ - rhs.imag_);
    }

    /// Return this * `rhs`.
    Complex operator*(const Complex& rhs) const
    {
        return Complex(real_ * rhs.real_ - imag_ * rhs.imag_, real_ * rhs.imag_ + imag_ * rhs.real_);
    }

    /// Return this / `rhs` (not zero).
    Complex operator/(const Complex& rhs) const
    {
        internal::check_zero(rhs);

        double den = rhs.real_ * rhs.real_ + rhs.imag_ * rhs.imag_;
        return Complex((real_ * rhs.real_ + imag_ * rhs.imag_) / den, (imag_ * rhs.real_ - real_ * rhs.imag_) / den);
    }

    /*
     * Static
     */

    /// Return `base**exp`.
    static Complex pow(const Complex& base, const Complex& exp)
    {
        if (exp == 0)
        {
            return 1;
        }

        if (base == 0)
        {
            throw std::runtime_error("Error: Math domain error.");
        }

        double coef = std::pow(base.abs(), exp.real_) * std::exp(-base.arg() * exp.imag_);
        double theta = std::log(base.abs()) * exp.imag_ + base.arg() * exp.real_;

        return Complex(coef * std::cos(theta), coef * std::sin(theta));
    }

    /*
     * Print / Input
     */

    /// Output the complex to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Complex& complex)
    {
        return os << '(' << complex.real_ << (complex.imag_ < 0 ? '-' : '+') << std::abs(complex.imag_) << "j)";
    }

    /// Get a complex from the specified input stream.
    ///
    /// ### Example
    /// ```
    /// Complex c1, c2;
    /// std::istringstream("1-2j 233.33") >> c1 >> c2;
    /// // c1 == Complex(1, -2);
    /// // c2 == Complex(233.33);
    /// ```
    friend std::istream& operator>>(std::istream& is, Complex& complex)
    {
        while (is.peek() <= 0x20)
        {
            is.ignore();
        }
        if (!(std::isdigit(is.peek()) || is.peek() == '-' || is.peek() == '+' || is.peek() == '.')) // handle "z1+2j"
        {
            throw std::runtime_error("Error: Wrong complex literal.");
        }

        double real;
        is >> real;
        if (is.peek() <= 0x20) // next char is white space, ok
        {
            complex = real;
            return is;
        }

        double imag;
        char c;
        is >> imag >> c;
        if (c != 'j' || is.peek() > 0x20) // handle "1z+2j" or "1+z2j" or "1+2zj" or "1jj2j"
        {
            throw std::runtime_error("Error: Wrong complex literal.");
        }
        complex = Complex(real, imag);
        return is;
    }
};

} // namespace pyincpp

template <>
struct std::hash<pyincpp::Complex> // explicit specialization
{
    std::size_t operator()(const pyincpp::Complex& complex) const
    {
        return std::hash<double>{}(complex.real()) ^ (std::hash<double>{}(complex.imag()) << 1);
    }
};

#endif // COMPLEX_HPP
