//! @file int.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Int class.
//! @date 2023.01.21

#ifndef INT_HPP
#define INT_HPP

#include "utility.hpp"

namespace pyincpp
{

/// Int provides support for big integer arithmetic.
class Int
{
private:
    // Base radix of digits.
    static const int BASE = 10; // TODO: base-10000

    // List of digits, represent absolute value of the integer, little endian.
    // Example: `12345000`
    // ```
    // digit: 0 0 0 5 4 3 2 1
    // index: 0 1 2 3 4 5 6 7
    // ```
    std::vector<int> digits_;

    // Sign of integer, 1 is positive, -1 is negative, and 0 is zero.
    signed char sign_;

    // Remove leading zeros elegantly and correct sign.
    Int& trim()
    {
        auto it = digits_.rbegin();
        while (it != digits_.rend() && *it == 0)
        {
            ++it;
        }
        digits_.erase(it.base(), digits_.end()); // note: rbegin().base() == end()

        if (digits_.empty())
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
        digits_.push_back(0);

        int i = 0;
        while (digits_[i] == BASE - 1)
        {
            ++i;
        }
        ++digits_[i];
        while (i != 0)
        {
            digits_[--i] = 0;
        }

        trim();

        // keep sign unchanged
    }

    // Decrement the absolute value by 1 quickly.
    // Require this != 0
    void abs_dec()
    {
        int i = 0;
        while (digits_[i] == 0)
        {
            ++i;
        }
        --digits_[i];
        while (i != 0)
        {
            digits_[--i] = BASE - 1;
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
        if (digits_.size() != that.digits_.size())
        {
            return digits_.size() > that.digits_.size() ? 1 : -1;
        }

        for (int i = digits_.size() - 1; i >= 0; --i) // i = -1 if is zero, ok
        {
            if (digits_[i] != that.digits_[i])
            {
                return digits_[i] > that.digits_[i] ? 1 : -1;
            }
        }

        return 0;
    }

    // Helper constructor.
    Int(signed char sign, const std::vector<int>& digits)
        : digits_(digits)
        , sign_(sign)
    {
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
            digits_.push_back(integer % BASE);
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
        const int s = (chars[0] == '-' || chars[0] == '+'); // skip symbol
        const int digit_len = len - s;

        // this is faster than `reserve` and `push_back`, cause `push_back` is slower than `[]`
        digits_.resize(digit_len);
        for (int i = 0; i != digit_len; ++i)
        {
            digits_[i] = chars[len - 1 - i] - '0';
        }

        trim();
    }

    /// Copy constructor.
    Int(const Int& that) = default;

    /// Move constructor.
    Int(Int&& that)
        : digits_(std::move(that.digits_))
        , sign_(std::move(that.sign_))
    {
        that.sign_ = 0;
    }

    /*
     * Comparison
     */

    /// Determine whether this integer is equal to another integer.
    bool operator==(const Int& that) const
    {
        return sign_ == that.sign_ && digits_ == that.digits_;
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
        digits_ = std::move(that.digits_);
        sign_ = std::move(that.sign_);

        that.sign_ = 0;

        return *this;
    }

    /*
     * Examination
     */

    /// Return the number of digits in the integer (based 10).
    int digits() const
    {
        if (digits_.size() > 0)
        {
            return (digits_.size() - 1) * std::log10(BASE) + std::floor(std::log10(digits_.back())) + 1;
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
        return is_zero() ? true : (digits_[0] & 1) == 0;
    }

    /// Determine whether the integer is odd quickly.
    bool is_odd() const
    {
        return is_zero() ? false : (digits_[0] & 1) == 1;
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
        auto& a = digits_;
        const auto& b = rhs.digits_;
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
        std::vector<int> rhs_digits = rhs.digits_;
        if (abs_cmp(rhs) == -1) // let a.len >= b.len
        {
            sign_ = -sign_;
            digits_.swap(rhs_digits);
        }
        auto& a = digits_;
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
        const auto& a = digits_;
        const auto& b = rhs.digits_;
        Int result(sign_ == rhs.sign_ ? 1 : -1, std::vector<int>(a.size() + b.size())); // the sign of result is depends on the sign of operands
        auto& c = result.digits_;

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
        // if rhs is zero, throw an exception
        internal::check_zero(rhs.sign_);

        // if this.abs() < rhs.abs(), just return 0
        if (digits() < rhs.digits())
        {
            return *this = 0;
        }

        // now, the sign of two integers is not zero

        // prepare variables
        int size = digits_.size() - rhs.digits_.size() + 1;

        Int tmp;       // intermediate variable for rhs * 10^i
        tmp.sign_ = 1; // positive

        // tmp = rhs * 10^(size), not size-1, since the for loop will erase first, so tmp is rhs * 10^(size-1) at first
        tmp.digits_ = std::vector<int>(size, 0);
        tmp.digits_.insert(tmp.digits_.end(), rhs.digits_.begin(), rhs.digits_.end());

        Int result;
        result.sign_ = (sign_ == rhs.sign_ ? 1 : -1); // the sign is depends on the sign of operands
        result.digits_.resize(size);

        sign_ = 1;

        // calculation
        for (int i = size - 1; i >= 0; --i)
        {
            // tmp = rhs * 10^i in O(1), I'm a fxxking genius
            // after testing, found that use vector is very faster than use deque `tmp.digits_.pop_front();`
            // my guess is that deque's various operations take longer than vector's
            tmp.digits_.erase(tmp.digits_.begin());

            while (*this >= tmp) // <= 9 loops, so O(1)
            {
                result.digits_[i]++;
                *this -= tmp;
            }
        }

        return *this = result.trim();
    }

    /// Return this %= `rhs` (not zero).
    Int& operator%=(const Int& rhs)
    {
        // if rhs is zero, throw an exception
        internal::check_zero(rhs.sign_);

        // if this.abs() < rhs.abs(), just return this
        if (digits() < rhs.digits())
        {
            return *this;
        }

        // now, the sign of two integers is not zero

        // prepare variables
        int size = digits_.size() - rhs.digits_.size() + 1;

        sign_ = 1;

        Int tmp;       // intermediate variable for rhs * 10^i
        tmp.sign_ = 1; // positive

        // tmp = rhs * 10^(size), not size-1, since the for loop will erase first, so tmp is rhs * 10^(size-1) at first
        tmp.digits_ = std::vector<int>(size, 0);
        tmp.digits_.insert(tmp.digits_.end(), rhs.digits_.begin(), rhs.digits_.end());

        // calculation
        for (int i = 0; i < size; ++i)
        {
            // tmp = rhs * 10^i in O(1), I'm a fxxking genius
            // after testing, found that use vector is very faster than use deque `tmp.digits_.pop_front();`
            // my guess is that deque's various operations take longer than vector's
            tmp.digits_.erase(tmp.digits_.begin());

            while (*this >= tmp) // <= 9 loops, so O(1)
            {
                *this -= tmp;
            }
        }

        return trim();
    }

    /// Increment the value by 1 quickly.
    Int& operator++()
    {
        if (sign_ == 0)
        {
            sign_ = 1;
            digits_.push_back(1);
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
            digits_.push_back(1);
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
        for (int i = digits_.size() - 1; i >= 0; --i) // i = -1 if is zero, ok
        {
            result = result * BASE + digits_[i];
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
        else if (integer < 4)
        {
            return 1;
        }
        else if (integer < 9)
        {
            return 2;
        }
        else if (integer < 16) // can not be omitted
        {
            return 3;
        }

        // using Newton's method

        // as far as possible to reduce the number of iterations
        // cur_sqrt = 10^(digits/2 - 1) in O(1)
        Int cur_sqrt;
        cur_sqrt.digits_ = std::vector<int>(integer.digits() / 2 - 1, 0); // integer.digits() >= 2
        cur_sqrt.digits_.push_back(1);
        cur_sqrt.sign_ = 1;

        Int pre_sqrt = -1;

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
        if (base.digits() == 1 && base.digits_[0] == 1)
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

    /// Return a non-negative random integer (with a specific number of `digits`).
    ///
    /// Using hardware device to generate true random integer if possible.
    ///
    /// If `digits` is not specified, it will generate a random integer with digits in [0, 4300],
    /// since the default limit of Python's int is 4300 digits.
    static Int random(int digits = -1)
    {
        if (digits < -1)
        {
            throw std::runtime_error("Error: `digits` must be a non-negative integer or default = -1.");
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> digit(0, BASE - 1);

        // the default limit of Python's int is 4300 digits as provided in `sys.int_info.default_max_str_digits`
        // see: https://docs.python.org/3/library/stdtypes.html#integer-string-conversion-length-limitation
        std::uniform_int_distribution<int> digits_limit(0, 4300);

        Int randint;
        randint.digits_.resize(digits == -1 ? digits_limit(gen) : digits); // may be 0
        randint.sign_ = randint.digits_.empty() ? 0 : 1;

        for (auto& d : randint.digits_)
        {
            d = digit(gen);
        }

        // reset most significant digit if is 0
        if (!randint.digits_.empty() && randint.digits_.back() == 0)
        {
            std::uniform_int_distribution<int> most_digit(1, BASE - 1);
            randint.digits_.back() = most_digit(gen);
        }

        return randint;
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

        std::for_each(integer.digits_.rbegin(), integer.digits_.rend(), [&](const auto& d)
                      { os << char(d + '0'); });

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

        for (const auto& d : integer.digits_)
        {
            value ^= std::hash<int>{}(d) << 1;
        }

        return value;
    }
};

#endif // INT_HPP
