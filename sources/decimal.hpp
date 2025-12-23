//! @file decimal.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Decimal class.
//! @date 2025.12.09

#ifndef DECIMAL_HPP
#define DECIMAL_HPP

#include "detail.hpp"

#include "fraction.hpp"

namespace pyincpp
{

/// Decimal provides decimal arithmetic with repeating support.
class Decimal
{
private:
    Fraction value_;

    explicit Decimal(const Fraction& f)
        : value_(f)
    {
    }

    // a is numerator, b is denominator, return (start_index, length)
    static std::pair<int, int> find_cyclic(int a, int b)
    {
        std::vector<int> remainders;
        int remainder = a % b;

        // check for repeating cycle
        while (remainder != 0)
        {
            auto it = std::find(remainders.begin(), remainders.end(), remainder);
            if (it != remainders.end()) // found a repeating cycle
            {
                int start = std::distance(remainders.begin(), it);
                int length = remainders.size() - start;
                return {start, length};
            }
            remainders.push_back(remainder);
            remainder = (remainder * 10) % b; // left shift and continue iteration
        }

        // no repeating cycle found
        return {-1, -1};
    }

public:
    /*
     * Constructor
     */

    Decimal() = default;

    Decimal(int integer)
        : value_(integer)
    {
    }

    /// Create a decimal with given string `integer[.decimal][~cyclic][#radix]`.
    Decimal(const std::string& str)
    {
        std::regex re(R"(^([-+])?(\d+)\.?(\d+)?~?(\d+)?#?(\d+)?$)");
        std::smatch m;
        if (!std::regex_match(str, m, re))
        {
            throw std::runtime_error("Error: Expect format `integer[.decimal][~cyclic][#radix]` but got: " + str);
        }

        std::string sign = m[1].matched ? m[1].str() : "+";
        int radix = m[5].matched ? std::stoi(m[5].str()) : 10;
        int integral = std::stoi(m[2].str(), nullptr, radix);
        int decimal = m[3].matched ? std::stoi(m[3].str(), nullptr, radix) : -1;
        int cyclic = m[4].matched ? std::stoi(m[4].str(), nullptr, radix) : -1;

        // x = c/((base^len(c)-1)*(base^len(d))) = c/(base^len(c+d)-base^len(d))
        int decimal_len = m[3].matched ? m[3].length() : 0;
        int cyclic_len = m[4].matched ? m[4].length() : 0;
        int scale = std::pow(radix, decimal_len);
        int repeat = std::pow(radix, decimal_len + cyclic_len) - scale;

        if (decimal == -1 && cyclic == -1) // integer only
        {
            value_ = Fraction(integral);
        }
        else if (decimal != -1 && cyclic == -1) // integer + decimal
        {
            value_ = Fraction(integral * scale + decimal, scale);
        }
        else if (decimal == -1 && cyclic != -1) // integer + cyclic
        {
            value_ = Fraction(integral) + Fraction(cyclic, repeat);
        }
        else // integer + decimal + cyclic
        {
            Fraction head(integral * scale + decimal, scale);
            Fraction tail(cyclic, repeat);
            value_ = head + tail;
        }

        if (sign == "-")
        {
            value_ = -value_;
        }
    }

    Decimal(const Decimal& that) = default;
    Decimal(Decimal&& that) = default;

    /*
     * Comparison
     */

    auto operator<=>(const Decimal& that) const
    {
        return value_ <=> that.value_;
    }

    /*
     * Assignment
     */

    Decimal& operator=(const Decimal& that) = default;
    Decimal& operator=(Decimal&& that) = default;

    /*
     * Examination
     */

    operator double() const
    {
        return static_cast<double>(value_);
    }

    explicit operator float() const
    {
        return static_cast<float>(value_);
    }

    /// Convert to fraction.
    Fraction as_fraction() const
    {
        return value_;
    }

    /// Return the absolute value.
    Decimal abs() const
    {
        return Decimal(value_.abs());
    }

    /*
     * Manipulation
     */

    Decimal& operator+=(const Decimal& rhs)
    {
        value_ += rhs.value_;
        return *this;
    }

    Decimal& operator-=(const Decimal& rhs)
    {
        value_ -= rhs.value_;
        return *this;
    }

    Decimal& operator*=(const Decimal& rhs)
    {
        value_ *= rhs.value_;
        return *this;
    }

    Decimal& operator/=(const Decimal& rhs)
    {
        value_ /= rhs.value_;
        return *this;
    }

    Decimal& operator%=(const Decimal& rhs)
    {
        value_ %= rhs.value_;
        return *this;
    }

    /*
     * Production
     */

    Decimal operator-() const
    {
        return Decimal(-value_);
    }

    Decimal operator+(const Decimal& rhs) const
    {
        return Decimal(value_ + rhs.value_);
    }

    Decimal operator-(const Decimal& rhs) const
    {
        return Decimal(value_ - rhs.value_);
    }

    Decimal operator*(const Decimal& rhs) const
    {
        return Decimal(value_ * rhs.value_);
    }

    Decimal operator/(const Decimal& rhs) const
    {
        return Decimal(value_ / rhs.value_);
    }

    Decimal operator%(const Decimal& rhs) const
    {
        return Decimal(value_ % rhs.value_);
    }

    /*
     * Print / Input
     */

    /// Convert to string.
    std::string to_string() const
    {
        auto [start, length] = find_cyclic(value_.numerator(), value_.denominator());
        if (start != -1)
        {
            std::string s = std::to_string(static_cast<double>(value_));
            auto pos = s.find('.');
            std::size_t end = pos + 1 + start + length * 3;
            if (end > s.size())
            {
                end = s.size();
            }
            return s.substr(0, end) + "...";
        }

        // use string stream to avoid trailing zeros
        std::ostringstream oss;
        oss << static_cast<double>(value_);
        return oss.str();
    }

    /// Output formatting: if cyclic detected, append "..." after a short substring.
    friend std::ostream& operator<<(std::ostream& os, const Decimal& d)
    {
        return os << d.to_string();
    }

    /// Get a decimal from the specified input stream.
    friend std::istream& operator>>(std::istream& is, Decimal& decimal)
    {
        std::string input;
        is >> input;
        decimal = Decimal(input);
        return is;
    }
};

} // namespace pyincpp

template <>
struct std::hash<pyincpp::Decimal>
{
    std::size_t operator()(const pyincpp::Decimal& decimal) const
    {
        return std::hash<pyincpp::Fraction>{}(decimal.as_fraction() + pyincpp::Fraction(42));
    }
};

#endif // DECIMAL_HPP
