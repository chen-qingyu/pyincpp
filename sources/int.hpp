//! @file int.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Int class.
//! @date 2023.01.21

#ifndef INT_HPP
#define INT_HPP

#include "detail.hpp"

namespace pyincpp
{

/// Int provides support for big integer arithmetic.
class Int
{
private:
    // Base radix of digits.
    static constexpr int BASE = 1'000'000'000; // 10^(floor(log10(INT_MAX)))

    // Number of decimal digits per chunk.
    static constexpr int DIGITS_PER_CHUNK = 9; // ceil(log10(base));

    // Sign of integer, 1 is positive, -1 is negative, and 0 is zero.
    signed char sign_;

    // List of digits, represent absolute value of the integer, little endian.
    // Example: `123456789000`
    // ```
    // chunk: 456789000 123
    // index: 0         1
    // ```
    std::vector<int> chunks_;

    // Remove leading zeros and correct sign.
    Int& trim()
    {
        while (!chunks_.empty() && chunks_.back() == 0)
        {
            chunks_.pop_back();
        }

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

    // Increase the absolute value by 1 quickly.
    void abs_inc()
    {
        assert(sign_ != 0);

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

        trim(); // sign unchanged
    }

    // Decrease the absolute value by 1 quickly.
    void abs_dec()
    {
        assert(sign_ != 0);

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

        trim(); // sign may change to zero
    }

    // This is equal to Python's "//".
    // `a == (a floor_div b) * b + a cycle_mod b`
    static int floor_div(int a, int b)
    {
        int q = a / b;
        return q * b == a ? q : q - ((a < 0) ^ (b < 0)); // if not modulo and signs different then - 1
    }

    // This is equal to Python's "%".
    // `a == (a floor_div b) * b + a cycle_mod b`
    static int cycle_mod(int a, int b)
    {
        return a - floor_div(a, b) * b;
    }

    // Compare absolute value.
    int abs_cmp(const std::vector<int>& that_chunks) const
    {
        if (chunks_.size() != that_chunks.size())
        {
            return chunks_.size() > that_chunks.size() ? 1 : -1;
        }

        for (int i = chunks_.size() - 1; i >= 0; --i) // i = -1 if is zero, ok
        {
            if (chunks_[i] != that_chunks[i])
            {
                return chunks_[i] > that_chunks[i] ? 1 : -1;
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

    // Multiply with small int. O(N)
    void small_mul(int n)
    {
        assert(is_positive());
        assert(n > 0 && n < BASE);

        int carry = 0;
        for (auto& chunk : chunks_)
        {
            long long tmp = 1ll * chunk * n + carry;
            chunk = tmp % BASE; // t%b < b
            carry = tmp / BASE; // t/b <= ((b-1)*(b-1) + (b-1))/b = b - 1 < b
        }
        chunks_.push_back(carry);

        trim();
    }

    // Divide with small int. O(N)
    // Return the remainder.
    int small_div(int n)
    {
        assert(is_positive());
        assert(n > 0 && n < BASE);

        long long r = 0;
        for (auto& chunk : chunks_ | std::views::reverse)
        {
            r = r * BASE + chunk;
            chunk = r / n; // r/n <= ((n-1)*b+(b-1))/n = (n*b - 1)/n < b
            r %= n;        // r%n < r%b < b
        }

        trim();
        return int(r);
    }

public:
    /*
     * Constructor
     */

    /// Create an integer based on the given integer `n` (default = 0).
    /// @tparam T a primitive integer type: int (default), long, etc.
    template <std::integral T = int>
    Int(T n = 0)
    {
        sign_ = n == 0 ? 0 : (n > 0 ? 1 : -1);
        if constexpr (std::is_signed_v<T>)
        {
            n = std::abs(n);
        }
        while (n > 0)
        {
            chunks_.push_back(n % BASE);
            n /= BASE;
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
            chunk = chunk * 10 + (digits[i] - '0'); // faster than (digits[i] ^ 0x30) in -O2
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
            return sign_ - that.sign_;
        }

        return sign_ >= 0 ? abs_cmp(that.chunks_) : -abs_cmp(that.chunks_);
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
        if (chunks_.empty())
        {
            return 0;
        }

        return (chunks_.size() - 1) * DIGITS_PER_CHUNK + std::floor(std::log10(chunks_.back())) + 1;
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

        if (*this == 2 || *this == 3)
        {
            return true;
        }

        if (is_even())
        {
            return false;
        }

        Int s = sqrt(*this);
        for (Int n = 3; n <= s; n += 2)
        {
            if ((*this % n).is_zero())
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
            int tmp = a[i] + b[i]; // t <= (b-1) + (b-1) < 2*b = 2'000'000'000 < INT_MAX
            a[i] = tmp % BASE;
            a[i + 1] += tmp / BASE; // 1 or 0
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
        if (abs_cmp(rhs.chunks_) == -1) // let a.len >= b.len
        {
            sign_ = -sign_;
            chunks_.swap(rhs_digits);
        }
        auto& a = chunks_;
        const auto& b = rhs_digits;
        a.push_back(0);

        // calculate
        for (int i = 0; i < b.size(); ++i)
        {
            int tmp = a[i] - b[i];
            a[i] = cycle_mod(tmp, BASE);
            a[i + 1] += floor_div(tmp, BASE); // -1 or 0
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
        Int result(sign_ == rhs.sign_ ? 1 : -1, std::vector<int>(a.size() + b.size()));
        auto& c = result.chunks_;

        // calculate
        for (int i = 0; i < a.size(); ++i)
        {
            for (int j = 0; j < b.size(); ++j)
            {
                long long tmp = 1ll * a[i] * b[j] + c[i + j];
                c[i + j] = tmp % BASE;      // t%b < b
                c[i + j + 1] += tmp / BASE; // be modulo by the previous line in the next loop, or finally c + t/b <= 0 + ((b-1)^2 + (b-1))/b = b - 1 < b
            }
        }

        return *this = result.trim();
    }

    /// Return this /= `rhs`.
    /// Divide by zero will throw a `runtime_error` exception.
    Int& operator/=(const Int& rhs)
    {
        return *this = divmod(rhs).first;
    }

    /// Return this %= `rhs`.
    /// Divide by zero will throw a `runtime_error` exception.
    Int& operator%=(const Int& rhs)
    {
        return *this = divmod(rhs).second;
    }

    /// Return the quotient and remainder simultaneously.
    /// `this == (this / rhs) * rhs + this % rhs`
    /// Divide by zero will throw a `runtime_error` exception.
    std::pair<Int, Int> divmod(const Int& rhs) const
    {
        // if rhs is zero, throw an exception
        detail::check_zero(rhs.sign_);

        // if this.abs < rhs.abs, just return {0, this}
        if (digits() < rhs.digits())
        {
            return {0, *this};
        }

        // now, the sign of two integers is not zero

        // if rhs < base, then use small_div in O(N)
        if (rhs.chunks_.size() == 1)
        {
            Int a = abs();                                   // can't be chained cause q is ref
            int r = a.small_div(rhs.chunks_[0]);             // this.abs divmod rhs.abs
            return {sign_ == rhs.sign_ ? a : -a, sign_ * r}; // r.sign = this.sign
        }

        // dividend, divisor, temporary quotient, accumulated quotient
        Int a = abs(), b = rhs.abs(), t = 1, q = 0;

        // double ~ left shift, O(log(2^N))) * O(N) = O(N^2)
        while (a.abs_cmp(b.chunks_) >= 0)
        {
            b.small_mul(2);
            t.small_mul(2);
        }

        // halve ~ right shift, O(log(2^N))) * O(N) = O(N^2)
        while (t.is_positive())
        {
            if (a.abs_cmp(b.chunks_) >= 0)
            {
                a -= b;
                q += t;
            }
            b.small_div(2);
            t.small_div(2);
        }

        // now q is the quotient.abs, a is the remainder.abs
        return {sign_ == rhs.sign_ ? q : -q, sign_ == 1 ? a : -a};
    }

    /// Increase the value by 1 quickly.
    Int& operator++()
    {
        if (sign_ == 1)
        {
            abs_inc();
        }
        else if (sign_ == -1)
        {
            abs_dec();
        }
        else
        {
            sign_ = 1;
            chunks_.push_back(1);
        }

        return *this;
    }

    /// Decrease the value by 1 quickly.
    Int& operator--()
    {
        if (sign_ == 1)
        {
            abs_dec();
        }
        else if (sign_ == -1)
        {
            abs_inc();
        }
        else
        {
            sign_ = -1;
            chunks_.push_back(1);
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
        return Int(-sign_, chunks_);
    }

    /// Return the absolute value of this.
    Int abs() const
    {
        return Int(std::abs(sign_), chunks_);
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

    /// Return this / `rhs`.
    /// Divide by zero will throw a `runtime_error` exception.
    Int operator/(const Int& rhs) const
    {
        return Int(*this) /= rhs;
    }

    /// Return this % `rhs`.
    /// Divide by zero will throw a `runtime_error` exception.
    Int operator%(const Int& rhs) const
    {
        return Int(*this) %= rhs;
    }

    /// Return the factorial of this.
    Int factorial() const
    {
        if (sign_ == -1)
        {
            throw std::runtime_error("Error: Require this >= 0 for factorial().");
        }

        if (chunks_.size() > 1)
        {
            throw std::runtime_error("Error: This integer is too large to calculate for factorial().");
        }

        Int result = 1; // 0! == 1
        for (int i = to_number(); i != 0; --i)
        {
            result.small_mul(i);
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
            prime += 2;

            if (prime.is_prime())
            {
                break;
            }
        }

        return prime;
    }

    /// Attempt to convert this integer to a number of the specified type `T`.
    /// @tparam T a numeric type: int (default), long, double, etc. or any custom numeric type.
    template <typename T = int>
    T to_number() const
    {
        T result = 0;
        for (const auto& chunk : chunks_ | std::views::reverse)
        {
            result = result * BASE + chunk;
        }
        return result * sign_;
    }

    /*
     * Static
     */

    /// Return the square root of integer `n`.
    static Int sqrt(const Int& n)
    {
        if (n.sign_ == -1)
        {
            throw std::runtime_error("Error: Require n >= 0 for sqrt(n).");
        }

        // binary search
        Int lo = 0, hi = n, res;
        while (lo <= hi)
        {
            Int mid = lo + (hi - lo) / 2;

            if (mid * mid <= n) // if mid^2 <= n, update the result and search in upper half
            {
                res = mid;
                lo = mid + 1;
            }
            else // else mid^2 > n, search in the lower half
            {
                hi = mid - 1;
            }
        }

        return res;
    }

    /// Return `(base**exp) % mod` (`mod` default = 0 means does not perform module).
    static Int pow(const Int& base, const Int& exp, const Int& mod = 0)
    {
        // if base.abs is 1, only when base is negative and exp is odd return -1, otherwise return 1
        if (base.chunks_.size() == 1 && base.chunks_[0] == 1)
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
        Int num = base, n = exp, res = 1;
        while (!n.is_zero())
        {
            if (n.is_odd())
            {
                res = mod.is_zero() ? res * num : (res * num) % mod;
            }
            num = mod.is_zero() ? num * num : (num * num) % mod;
            n.small_div(2);
        }

        return res;
    }

    /// Return the logarithm of integer `n` based on integer `base`.
    static Int log(const Int& n, const Int& base)
    {
        if (n.sign_ <= 0 || base < 2)
        {
            throw std::runtime_error("Error: Math domain error.");
        }

        if (base == 10) // log10 == digits-1
        {
            return n.digits() - 1;
        }

        Int num = n / base, res;
        while (!num.is_zero())
        {
            ++res;
            num /= base;
        }

        return res;
    }

    /// Calculate the greatest common divisor of two integers.
    static Int gcd(const Int& a, const Int& b)
    {
        return detail::gcd(a, b);
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
        if (a > b)
        {
            throw std::runtime_error("Error: Require a <= b for random(a, b).");
        }

        std::mt19937 gen(std::random_device{}());
        Int range = b - a + 1;
        std::vector<int> chunks;
        Int current = range;
        while (!current.is_zero())
        {
            std::uniform_int_distribution<int> chunk_dis(0, BASE - 1);
            chunks.push_back(chunk_dis(gen) % current.to_number<int>());
            current /= BASE;
        }

        Int result;
        for (int i = chunks.size() - 1; i >= 0; --i)
        {
            result = result * BASE + chunks[i];
        }

        return result % range + a;
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
            throw std::runtime_error("Error: Require digits > 0 for random(digits).");
        }

        // random number generator
        std::mt19937 gen(std::random_device{}());

        // little chunks
        auto chunks = std::vector<int>((digits - 1) / DIGITS_PER_CHUNK);
        std::uniform_int_distribution<int> chunk(0, BASE - 1);
        std::for_each(chunks.begin(), chunks.end(), [&](auto& x)
                      { x = chunk(gen); });

        // most significant chunk
        int n = (digits - 1) % DIGITS_PER_CHUNK + 1;
        std::uniform_int_distribution<int> most_chunk(std::pow(10, n - 1), std::pow(10, n) - 1);
        chunks.push_back(most_chunk(gen));

        return Int(1, chunks);
    }

    /// Calculate the `n`th term of the Fibonacci sequence: 0 (n=0), 1, 1, 2, 3, 5, ...
    static Int fibonacci(const Int& n)
    {
        if (n.is_negative())
        {
            throw std::runtime_error("Error: Require n >= 0 for fibonacci(n).");
        }

        // ref: https://sicp-solutions.net/post/sicp-solution-exercise-1-19

        // T_pq(a, b) = (bq + aq + ap, bp + aq)
        // T_pq(T_pq(a, b)) = ((bp+aq)q + (bq+aq+ap)q + (bq+aq+ap)p, (bp+aq)p + (bq+aq+ap)q)
        //                  = (b(2pq+q^2) + a(p^2+q^2) + a(2pq+q^2), b(p^2+q^2) + a(2pq+q^2))
        //                  = T_p'q'(a, b)
        // => p' = p^2 + q^2, q' = 2pq + q^2

        Int a = 1, b = 0, p = 0, q = 1, cnt = n;
        while (!cnt.is_zero())
        {
            if (cnt.is_even())
            {
                Int p_ = p * p + q * q;
                Int q_ = p * q * 2 + q * q;
                p = p_;
                q = q_;
                cnt.small_div(2);
            }
            else
            {
                Int a_ = b * q + a * (p + q);
                Int b_ = b * p + a * q;
                a = a_;
                b = b_;
                cnt.abs_dec();
            }
        }
        return b;
    }

    /// The well-known Ackermann function (perhaps not so well-known) is a rapidly growing function.
    /// Please input parameters carefully.
    /// See: https://en.wikipedia.org/wiki/Ackermann_function
    static Int ackermann(const Int& m, const Int& n)
    {
        if (m.is_negative() || n.is_negative())
        {
            throw std::runtime_error("Error: Require m >= 0 and n >= 0 for ackermann(m, n).");
        }

        // ref: https://rosettacode.org/wiki/Ackermann_function
        switch (m.to_number())
        {
            case 0:
                return n + 1;
            case 1:
                return n + 2;
            case 2:
                return n * 2 + 3;
            case 3:
                return Int::pow(2, n + 3) - 3;
            default:
                return n.is_zero() ? ackermann(m - 1, 1) : ackermann(m - 1, ackermann(m, n - 1));
        }
    }

    /// The hyperoperation sequence is an infinite sequence of arithmetic operations.
    /// This sequence starts with unary successor (n = 0), continues with addition (n = 1), multiplication (n = 2), exponentiation (n = 3), etc.
    /// See: https://en.wikipedia.org/wiki/Hyperoperation
    static Int hyperoperation(const Int& n, const Int& a, const Int& b)
    {
        if (n.is_negative() || a.is_negative() || b.is_negative())
        {
            throw std::runtime_error("Error: Require n >= 0 and a >= 0 and b >= 0 for hyperoperation(n, a, b).");
        }

        // special cases
        if (n > 3)
        {
            if (a.is_zero() && b.is_even())
            {
                return 1;
            }
            else if (a.is_zero() && b.is_odd())
            {
                return 0;
            }
            else if (a == 1 || b.is_zero())
            {
                return 1;
            }
            else if (b == 1)
            {
                return a;
            }
            else if (a == 2 && b == 2)
            {
                return 4;
            }
        }

        switch (n.to_number())
        {
            case 0:
                return Int(1) + b;
            case 1:
                return a + b;
            case 2:
                return a * b;
            case 3:
                return Int::pow(a, b);
            default:
                return hyperoperation(n - 1, a, hyperoperation(n, a, b - 1));
        }
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
        for (auto it = integer.chunks_.rbegin() + 1; it != integer.chunks_.rend(); ++it)
        {
            os << std::setw(Int::DIGITS_PER_CHUNK) << std::setfill('0') << *it;
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
