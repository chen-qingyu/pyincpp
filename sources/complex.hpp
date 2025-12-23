//! @file complex.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Complex class.
//! @date 2024.05.22

#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "detail.hpp"

namespace pyincpp
{

/// Complex provides support for complex number arithmetic.
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

    /// Create a complex with value `real+imag*j`.
    Complex(double real = 0, double imag = 0)
        : real_(real)
        , imag_(imag)
    {
    }

    /// Create a complex with given string `real[imag(j)]`.
    Complex(const std::string& str)
    {
        std::regex regex(R"(([+-]?\d*\.?\d*)([+-]?\d*\.?\d*)j?)");
        std::smatch match;
        if (!std::regex_match(str, match, regex))
        {
            throw std::runtime_error("Error: Expect format `real[imag(j)]` but got: " + str);
        }

        real_ = match[1].str().empty() ? 0.0 : std::stod(match[1].str());
        imag_ = match[2].str().empty() ? 0.0 : std::stod(match[2].str());
        if (str.back() == 'j' && match[2].str().empty())
        {
            imag_ = real_;
            real_ = 0.0;
        }
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
        return std::hypot(real_, imag_);
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
        detail::check_zero(rhs);

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

    /// Convert to string.
    std::string to_string() const
    {
        return '(' + std::to_string(real_) + (imag_ < 0 ? "-" : "+") + std::to_string(std::abs(imag_)) + "j)";
    }

    /// Output the complex to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Complex& complex)
    {
        return os << complex.to_string();
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
        std::string input;
        is >> input;
        complex = Complex(input);
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
