//! @file int.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Int class.
//! @date 2023.01.21

#ifndef INT_HPP
#define INT_HPP

#include "utility.hpp"

namespace pyincpp
{

namespace internal
{
class PrivateTester;
}

/// Int provides support for big integer arithmetic.
class Int
{
    friend class internal::PrivateTester;

private:
    // Base radix of digits.
    static constexpr int BASE = 10000;

    // Number of decimal digits per chunk.
    static constexpr int DIGITS_PER_CHUNK = 4; // std::ceil(std::log10(BASE));

    // Sign of integer, 1 is positive, -1 is negative, and 0 is zero.
    signed char sign_;

    // List of digits, represent absolute value of the integer, little endian.
    // Example: `123456789`
    // ```
    // chunk: 6789 2345 0001
    // index: 0    1    2
    // ```
    std::vector<int> chunks_;

    // Remove leading zeros elegantly and correct sign.
    Int& trim()
    {
        auto it = chunks_.rbegin();
        while (it != chunks_.rend() && *it == 0)
        {
            ++it;
        }
        chunks_.erase(it.base(), chunks_.end()); // note: rbegin().base() == end()

        if (chunks_.empty())
        {
            sign_ = 0;
        }

        return *this;
    }

    // Test whether the characters represent an integer.
    static bool is_integer(const char* chars, int len)
    {
        if (len == 0 || (len == 1 && (chars[0] == '+' || chars[0] == '-')))
        {
            return false;
        }

        for (int i = (chars[0] == '+' || chars[0] == '-'); i < len; ++i)
        {
            // surprisingly, this is faster than `!std::isdigit(chars[i])`
            // my guess is that the conversion of char to int takes time
            if (chars[i] < '0' || chars[i] > '9')
            {
                return false;
            }
        }

        return true;
    }

    // Increment the absolute value by 1 quickly.
    // Require this != 0
    void abs_inc()
    {
        // add a leading zero for carry
        chunks_.push_back(0);

        int i = 0;
        while (chunks_[i] == BASE - 1)
        {
            ++i;
        }
        ++chunks_[i];
        while (i != 0)
        {
            chunks_[--i] = 0;
        }

        trim();

        // keep sign unchanged
    }

    // Decrement the absolute value by 1 quickly.
    // Require this != 0
    void abs_dec()
    {
        int i = 0;
        while (chunks_[i] == 0)
        {
            ++i;
        }
        --chunks_[i];
        while (i != 0)
        {
            chunks_[--i] = BASE - 1;
        }

        trim();
    }

    // This is equal to Python's //
    // a == (a floor_div b) * b + a cycle_mod b
    static int floor_div(int a, int b)
    {
        return std::floor(double(a) / double(b));
    }

    // This is equal to Python's %
    // a == (a floor_div b) * b + a cycle_mod b
    static int cycle_mod(int a, int b)
    {
        return a - floor_div(a, b) * b;
    }

    // Compare absolute value.
    int abs_cmp(const Int& that) const
    {
        if (chunks_.size() != that.chunks_.size())
        {
            return chunks_.size() > that.chunks_.size() ? 1 : -1;
        }

        for (int i = chunks_.size() - 1; i >= 0; --i) // i = -1 if is zero, ok
        {
            if (chunks_[i] != that.chunks_[i])
            {
                return chunks_[i] > that.chunks_[i] ? 1 : -1;
            }
        }

        return 0;
    }

    // Helper constructor.
    Int(signed char sign, const std::vector<int>& chunks)
        : sign_(sign)
        , chunks_(chunks)
    {
    }

    // Multiply with small int, for div_mod, O(N)
    Int& small_mul(int n)
    {
        assert(is_positive());
        assert(n > 0 && n < BASE);

        int carry = 0;
        for (int i = 0; i < chunks_.size(); ++i)
        {
            int tmp = chunks_[i] * n + carry;
            chunks_[i] = tmp % BASE;
            carry = tmp / BASE;
        }
        chunks_.push_back(carry);

        return trim();
    }

    // Divide with small int, for div_mod, O(N)
    Int& small_div(int n)
    {
        assert(is_positive());
        assert(n > 0 && n < BASE);

        int remainder = 0;
        for (int i = chunks_.size() - 1; i >= 0; --i)
        {
            int tmp = remainder * BASE + chunks_[i];
            chunks_[i] = tmp / n;
            remainder = tmp % n;
        }

        return trim();
    }

    // Simultaneously calculate the quotient and remainder. O(N^2)
    std::pair<Int, Int> div_mod(const Int& rhs) const
    {
        // if rhs is zero, throw an exception
        internal::check_zero(rhs.sign_);

        // if this.abs() < rhs.abs(), just return {0, *this}
        if (digits() < rhs.digits())
        {
            return {0, *this};
        }

        // dividend, divisor, temporary quotient, accumulated quotient
        Int a = abs(), b = rhs.abs(), tmp = 1, q = 0;

        // double ~ left shift, O(log(2^N))) * O(N) = O(N^2)
        while (a.abs_cmp(b) >= 0)
        {
            b.small_mul(2);
            tmp.small_mul(2);
        }

        // halve ~ right shift, O(log(2^N))) * O(N) = O(N^2)
        while (tmp.is_positive())
        {
            if (a.abs_cmp(b) >= 0)
            {
                a -= b;
                q += tmp;
            }
            b.small_div(2);
            tmp.small_div(2);
        }

        // now q is the quotient.abs and a is the remainder
        return {sign_ == rhs.sign_ ? q.trim() : -q.trim(), a.trim()};
    }

public:
    /*
     * Constructor
     */

    /// Create an integer based on the given `integer`.
    Int(int integer = 0)
    {
        sign_ = integer == 0 ? 0 : (integer > 0 ? 1 : -1);
        integer = std::abs(integer);
        while (integer > 0)
        {
            chunks_.push_back(integer % BASE);
            integer /= BASE;
        }
    }

    /// Create an integer based on the given null-terminated characters.
    Int(const char* chars)
    {
        const int len = std::strlen(chars);
        if (!is_integer(chars, len))
        {
            throw std::runtime_error("Error: Wrong integer literal.");
        }

        sign_ = (chars[0] == '-' ? -1 : 1);

        // skip symbol
        std::string_view digits(chars + (chars[0] == '-' || chars[0] == '+'), chars + len);

        const int chunks_len = std::ceil(double(digits.size()) / DIGITS_PER_CHUNK);
        chunks_.resize(chunks_len, 0);

        // every DIGITS_PER_CHUNK digits into a chunk (align right)
        int chunk = 0;
        int idx = chunks_len;
        for (int i = 0; i < digits.size(); ++i)
        {
            chunk = chunk * 10 + (digits[i] - '0');
            // I think maybe it's not the fastest, but it's the most elegant
            if ((i + 1) % DIGITS_PER_CHUNK == digits.size() % DIGITS_PER_CHUNK)
            {
                chunks_[--idx] = chunk;
                chunk = 0;
            }
        }

        trim();
    }

    /// Copy constructor.
    Int(const Int& that) = default;

    /// Move constructor.
    Int(Int&& that)
        : sign_(std::move(that.sign_))
        , chunks_(std::move(that.chunks_))
    {
        that.sign_ = 0;
    }

    /*
     * Comparison
     */

    /// Determine whether this integer is equal to another integer.
    bool operator==(const Int& that) const
    {
        return sign_ == that.sign_ && chunks_ == that.chunks_;
    }

    /// Compare the integer with another integer.
    auto operator<=>(const Int& that) const
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

        // now, the sign of two integers is the same

        return sign_ >= 0 ? abs_cmp(that) : -abs_cmp(that);
    }

    /*
     * Assignment
     */

    /// Copy assignment operator.
    Int& operator=(const Int& that) = default;

    /// Move assignment operator.
    Int& operator=(Int&& that)
    {
        sign_ = std::move(that.sign_);
        chunks_ = std::move(that.chunks_);

        that.sign_ = 0;

        return *this;
    }

    /*
     * Examination
     */

    /// Return the number of digits in the integer (based 10).
    int digits() const
    {
        if (chunks_.size() > 0)
        {
            return (chunks_.size() - 1) * DIGITS_PER_CHUNK + std::floor(std::log10(chunks_.back())) + 1;
        }
        else
        {
            return 0;
        }
    }

    /// Determine whether the integer is zero quickly.
    bool is_zero() const
    {
        return sign_ == 0;
    }

    /// Determine whether the integer is positive quickly.
    bool is_positive() const
    {
        return sign_ == 1;
    }

    /// Determine whether the integer is negative quickly.
    bool is_negative() const
    {
        return sign_ == -1;
    }

    /// Determine whether the integer is even quickly.
    bool is_even() const
    {
        return is_zero() ? true : (chunks_[0] & 1) == 0;
    }

    /// Determine whether the integer is odd quickly.
    bool is_odd() const
    {
        return is_zero() ? false : (chunks_[0] & 1) == 1;
    }

    /// Determine whether the integer is prime number.
    bool is_prime() const
    {
        if (*this <= 1)
        {
            return false; // prime >= 2
        }

        for (Int i = 2; i * i <= *this; i.abs_inc())
        {
            if ((*this % i).is_zero())
            {
                return false;
            }
        }

        return true;
    }

    /*
     * Manipulation
     */

    /// Return this += `rhs`.
    Int& operator+=(const Int& rhs)
    {
        // if one of the operands is zero, just return another one
        if (sign_ == 0 || rhs.sign_ == 0)
        {
            return sign_ == 0 ? *this = rhs : *this;
        }

        // if the operands are of opposite signs, perform subtraction
        if (sign_ != rhs.sign_)
        {
            return *this -= -rhs;
        }

        // now, the sign of two integers is the same and not zero

        // normalize
        auto& a = chunks_;
        const auto& b = rhs.chunks_;
        a.resize(std::max(a.size(), b.size()) + 1); // a.len is max+1

        // calculate
        for (int i = 0; i < b.size(); ++i)
        {
            auto tmp = a[i] + b[i];
            a[i] = tmp % BASE;
            a[i + 1] += tmp / BASE;
        }
        for (int i = b.size(); i < a.size() && a[i] >= BASE; ++i) // carry
        {
            ++a[i + 1];
            a[i] = 0;
        }

        return trim();
    }

    /// Return this -= `rhs`.
    Int& operator-=(const Int& rhs)
    {
        // if one of the operands is zero
        if (sign_ == 0 || rhs.sign_ == 0)
        {
            return sign_ == 0 ? *this = -rhs : *this;
        }

        // if the operands are of opposite signs, perform addition
        if (sign_ != rhs.sign_)
        {
            return *this += -rhs;
        }

        // now, the sign of two integers is the same and not zero

        // normalize
        std::vector<int> rhs_digits = rhs.chunks_;
        if (abs_cmp(rhs) == -1) // let a.len >= b.len
        {
            sign_ = -sign_;
            chunks_.swap(rhs_digits);
        }
        auto& a = chunks_;
        const auto& b = rhs_digits;

        // calculate
        for (int i = 0; i < b.size(); ++i)
        {
            auto tmp = a[i] - b[i];
            a[i] = cycle_mod(tmp, BASE);
            a[i + 1] += floor_div(tmp, BASE);
        }
        for (int i = b.size(); i < a.size() && a[i] < 0; ++i) // carry
        {
            --a[i + 1];
            a[i] = BASE - 1;
        }

        return trim();
    }

    /// Return this *= `rhs`.
    Int& operator*=(const Int& rhs)
    {
        // if one of the operands is zero, just return zero
        if (sign_ == 0 || rhs.sign_ == 0)
        {
            return *this = 0;
        }

        // now, the sign of two integers is not zero

        // normalize
        const auto& a = chunks_;
        const auto& b = rhs.chunks_;
        Int result{sign_ == rhs.sign_ ? 1 : -1, std::vector<int>(a.size() + b.size())};
        auto& c = result.chunks_;

        // calculate
        for (int i = 0; i < a.size(); ++i)
        {
            for (int j = 0; j < b.size(); ++j)
            {
                c[i + j] += a[i] * b[j];
                c[i + j + 1] += c[i + j] / BASE;
                c[i + j] %= BASE;
            }
        }

        return *this = result.trim();
    }

    /// Return this /= `rhs` (not zero).
    Int& operator/=(const Int& rhs)
    {
        return *this = div_mod(rhs).first;
    }

    /// Return this %= `rhs` (not zero).
    Int& operator%=(const Int& rhs)
    {
        return *this = div_mod(rhs).second;
    }

    /// Increment the value by 1 quickly.
    Int& operator++()
    {
        if (sign_ == 0)
        {
            sign_ = 1;
            chunks_.push_back(1);
        }
        else
        {
            (sign_ == 1) ? abs_inc() : abs_dec();
        }

        return *this;
    }

    /// Decrement the value by 1 quickly.
    Int& operator--()
    {
        if (sign_ == 0)
        {
            sign_ = -1;
            chunks_.push_back(1);
        }
        else
        {
            (sign_ == 1) ? abs_dec() : abs_inc();
        }

        return *this;
    }

    /*
     * Production
     */

    /// Return the copy of this.
    Int operator+() const
    {
        return *this;
    }

    /// Return the opposite value of this.
    Int operator-() const
    {
        Int num = *this;
        num.sign_ = -num.sign_;
        return num;
    }

    /// Return the absolute value of this.
    Int abs() const
    {
        return sign_ == -1 ? -*this : *this;
    }

    /// Return this + `rhs`.
    Int operator+(const Int& rhs) const
    {
        return Int(*this) += rhs;
    }

    /// Return this - `rhs`.
    Int operator-(const Int& rhs) const
    {
        return Int(*this) -= rhs;
    }

    /// Return this * `rhs`.
    Int operator*(const Int& rhs) const
    {
        return Int(*this) *= rhs;
    }

    /// Return this / `rhs` (not zero).
    Int operator/(const Int& rhs) const
    {
        return Int(*this) /= rhs;
    }

    /// Return this % `rhs` (not zero).
    Int operator%(const Int& rhs) const
    {
        return Int(*this) %= rhs;
    }

    /// Return the factorial of this.
    Int factorial() const
    {
        if (sign_ == -1)
        {
            throw std::runtime_error("Error: Negative integer have no factorial.");
        }

        Int result = 1; // 0! == 1

        for (Int i = *this; i.is_positive(); i.abs_dec()) // fast judgement, fast decrement
        {
            result *= i;
        }

        return result;
    }

    /// Calculate the next prime that greater than this.
    Int next_prime() const
    {
        if (*this < 2)
        {
            return 2;
        }

        Int prime = *this; // >= 2

        // if prime is even, let it odd and < this, because prime > 2 is odd and while prime += 2
        if (prime.is_even())
        {
            prime.abs_dec();
        }

        // prime >= 1
        while (true)
        {
            // faster than prime += 2
            prime.abs_inc();
            prime.abs_inc();

            if (prime.is_prime())
            {
                break;
            }
        }

        return prime;
    }

    /// Convert the integer to some integer of type T.
    /// @tparam T an integer type : int, long, and any custom type that support basic arithmetic operations.
    template <typename T>
    T to_integer() const
    {
        T result = 0;
        for (int i = chunks_.size() - 1; i >= 0; --i) // i = -1 if is zero, ok
        {
            result = result * BASE + chunks_[i];
        }
        return result * sign_;
    }

    /*
     * Static
     */

    /// Return the square root of `integer`.
    static Int sqrt(const Int& integer)
    {
        if (integer.sign_ == -1)
        {
            throw std::runtime_error("Error: Cannot compute square root of a negative integer.");
        }

        if (integer.is_zero())
        {
            return 0;
        }
        else if (integer < 4) // can not omit
        {
            return 1;
        }

        // using Newton's method

        // as far as possible to reduce the number of iterations
        // cur_sqrt is about 10^(digits/2) in O(1)
        Int cur_sqrt{1, std::vector<int>(integer.chunks_.size() / 2, 0)};
        cur_sqrt.chunks_.push_back(1);

        Int pre_sqrt;
        while (cur_sqrt != pre_sqrt)
        {
            pre_sqrt = cur_sqrt;
            cur_sqrt = (cur_sqrt + integer / cur_sqrt) / 2;
        }

        return cur_sqrt;
    }

    /// Return `(base**exp) % mod` (`mod` default = 0 means does not perform module).
    static Int pow(const Int& base, const Int& exp, const Int& mod = 0)
    {
        // check if base.abs() is 1
        // if base.abs() is 1, only when base is negative and exp is odd return -1, otherwise return 1
        if (base.digits() == 1 && base.chunks_[0] == 1)
        {
            return base.sign_ == -1 && exp.is_odd() ? -1 : 1;
        }

        // then, check if exp is negative
        if (exp.is_negative())
        {
            if (base.is_zero())
            {
                throw std::runtime_error("Error: Math domain error.");
            }

            return 0;
        }

        // fast power algorithm

        Int num = base;
        Int n = exp;
        Int result = 1; // this**0 == 1

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

    /// Return the logarithm of `integer` based on `base`.
    static Int log(const Int& integer, const Int& base)
    {
        if (integer.sign_ <= 0 || base < 2)
        {
            throw std::runtime_error("Error: Math domain error.");
        }

        if (base == 10) // log10 == digits-1
        {
            return integer.digits() - 1;
        }

        Int result;
        Int value = integer / base;
        while (!value.is_zero())
        {
            ++result;
            value /= base;
        }

        return result;
    }

    /// Calculate the greatest common divisor of two integers.
    static Int gcd(const Int& a, const Int& b)
    {
        return internal::gcd(a, b);
    }

    /// Calculate the least common multiple of two integers.
    static Int lcm(const Int& a, const Int& b)
    {
        if (a.is_zero() || b.is_zero())
        {
            return 0;
        }

        return (a * b).abs() / gcd(a, b); // LCM = |a * b| / GCD
    }

    /// Generate a random integer in [`a`, `b`].
    ///
    /// ### Example
    /// ```
    /// random(0, 9); // x in [0, 9]
    /// random(1, 6); // x in [1, 6]
    /// ```
    static Int random(const Int& a, const Int& b)
    {
        if (b < a)
        {
            throw std::runtime_error("Error: Invalid range.");
        }

        // TODO: TAOCP Chapter 3

        return a;
    }

    /// Generate a random integer of a specified number of `digits`.
    ///
    /// ### Example
    /// ```
    /// random(1); // x in [1, 9]
    /// random(3); // x in [100, 999]
    /// ```
    static Int random(int digits)
    {
        if (digits <= 0)
        {
            throw std::runtime_error("Error: `digits` must be a positive integer.");
        }

        return random(pow(10, digits - 1), --pow(10, digits));
    }

    /*
     * Print / Input
     */

    /// Output the integer to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const Int& integer)
    {
        if (integer.sign_ == 0)
        {
            return os << '0';
        }

        if (integer.sign_ == -1)
        {
            os << '-';
        }

        os << *integer.chunks_.rbegin();
        if (integer.chunks_.size() > 1)
        {
            std::for_each(integer.chunks_.rbegin() + 1, integer.chunks_.rend(), [&](const auto& c)
                          { os << std::setw(Int::DIGITS_PER_CHUNK) << std::setfill('0') << std::to_string(c); });
        }

        return os;
    }

    /// Get an integer from the specified input stream.
    friend std::istream& operator>>(std::istream& is, Int& integer)
    {
        std::string str;
        is >> str;
        integer = str.c_str();

        return is;
    }

    friend struct std::hash<pyincpp::Int>;
};

} // namespace pyincpp

template <>
struct std::hash<pyincpp::Int> // explicit specialization
{
    std::size_t operator()(const pyincpp::Int& integer) const
    {
        std::size_t value = std::hash<signed char>{}(integer.sign_);

        for (const auto& d : integer.chunks_)
        {
            value ^= std::hash<int>{}(d) << 1;
        }

        return value;
    }
};

#endif // INT_HPP
