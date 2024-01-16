#include "../sources/Fraction.hpp"

#include "tool.hpp"

using namespace mds;

// constructor destructor
TEST(Fraction, basics)
{
    // Fraction(int numerator = 0, int denominator = 1)
    Fraction f1{}; // () -> function declare
    Fraction f2(0);
    Fraction f3(0, 1);
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 0), std::runtime_error, "Error: Zero denominator.");

    // Fraction(const Fraction &that)
    Fraction f4(f3);

    // Fraction(Fraction &&that)
    Fraction f5(std::move(f4));

    // ~Fraction()
}

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST(Fraction, compare)
{
    ASSERT_EQ(Fraction(0), 0);
    ASSERT_EQ(Fraction(9, 6), Fraction(3, 2));

    ASSERT_GT(Fraction(1), Fraction(1, 2));
    ASSERT_LT(Fraction(0), Fraction(1, 2));

    ASSERT_GE(Fraction(2, 3), Fraction(1, 3));
    ASSERT_GE(Fraction(1, 3), Fraction(1, 3));

    ASSERT_LE(Fraction(-1, 3), Fraction(1, 3));
    ASSERT_LE(Fraction(1, 3), Fraction(1, 3));
}

// operator=()
TEST(Fraction, copy_assignment)
{
    Fraction fraction1(1, 2);
    Fraction fraction2(2, 3);

    fraction1 = fraction2;
    ASSERT_EQ(fraction1, Fraction(2, 3));
    ASSERT_EQ(fraction2, Fraction(2, 3));
}

// operator=()
TEST(Fraction, move_assignment)
{
    Fraction fraction1(1, 2);
    Fraction fraction2(2, 3);

    fraction1 = std::move(fraction2);
    ASSERT_EQ(fraction1, Fraction(2, 3));
    ASSERT_EQ(fraction2, Fraction());
}

// operator int(), operator float(), operator double()
TEST(Fraction, examination)
{
    Fraction fraction1(1, 2);
    ASSERT_EQ(int(fraction1), 0);
    ASSERT_FLOAT_EQ(float(fraction1), 0.5f);
    ASSERT_DOUBLE_EQ(double(fraction1), 0.5);

    Fraction fraction2(2, 3);
    ASSERT_EQ(int(fraction2), 0);
    ASSERT_FLOAT_EQ(float(fraction2), 2.0f / 3.0f);
    ASSERT_DOUBLE_EQ(double(fraction2), 2.0 / 3.0);
}

// operator++() operator--()
TEST(Fraction, inc_dec)
{
    // operator++()
    ASSERT_EQ(++Fraction(-1), 0);
    ASSERT_EQ(++Fraction(0), 1);
    ASSERT_EQ(++Fraction(1), 2);
    ASSERT_EQ(++Fraction(99999), 100000);

    // operator--()
    ASSERT_EQ(--Fraction(-1), -2);
    ASSERT_EQ(--Fraction(0), -1);
    ASSERT_EQ(--Fraction(1), 0);
    ASSERT_EQ(--Fraction(100000), 99999);
}

// operator+() operator-() abs()
TEST(Fraction, unary)
{
    Fraction a(1, 2);
    ASSERT_EQ(+a, Fraction(1, 2));
    ASSERT_EQ(-a, Fraction(-1, 2));
    ASSERT_EQ(a.abs(), Fraction(1, 2));

    Fraction b(-1, 3);
    ASSERT_EQ(+b, Fraction(-1, 3));
    ASSERT_EQ(-b, Fraction(1, 3));
    ASSERT_EQ(b.abs(), Fraction(1, 3));
}

// operator+()
TEST(Fraction, plus)
{
    // pos + pos
    ASSERT_EQ(Fraction(1, 2) + Fraction(1, 2), 1);

    // pos + zero
    ASSERT_EQ(Fraction(1, 2) + Fraction(0), Fraction(1, 2));

    // pos + neg
    ASSERT_EQ(Fraction(1, 2) + Fraction(1, -2), 0);

    // neg + pos
    ASSERT_EQ(Fraction(-1, 2) + Fraction(1, 2), 0);

    // neg + zero
    ASSERT_EQ(Fraction(-1, 2) + Fraction(0), Fraction(-1, 2));

    // neg + neg
    ASSERT_EQ(Fraction(-1, 2) + Fraction(-1, 2), -1);

    // zero + pos
    ASSERT_EQ(Fraction(0) + Fraction(1, 2), Fraction(1, 2));

    // zero + zero
    ASSERT_EQ(Fraction(0) + Fraction(0), 0);

    // zero + neg
    ASSERT_EQ(Fraction(0) + Fraction(-1, 2), Fraction(-1, 2));
}

// operator-()
TEST(Fraction, minus)
{
    // pos - pos
    ASSERT_EQ(Fraction(1, 2) - Fraction(1, 2), 0);

    // pos - zero
    ASSERT_EQ(Fraction(1, 2) - Fraction(0), Fraction(1, 2));

    // pos - neg
    ASSERT_EQ(Fraction(1, 2) - Fraction(-1, 2), 1);

    // neg - pos
    ASSERT_EQ(Fraction(-1, 2) - Fraction(1, 2), -1);

    // neg - zero
    ASSERT_EQ(Fraction(-1, 2) - Fraction(0), Fraction(-1, 2));

    // neg - neg
    ASSERT_EQ(Fraction(-1, 2) - Fraction(-1, 2), 0);

    // zero - pos
    ASSERT_EQ(Fraction(0) - Fraction(1, 2), Fraction(-1, 2));

    // zero - zero
    ASSERT_EQ(Fraction(0) - Fraction(0), 0);

    // zero - neg
    ASSERT_EQ(Fraction(0) - Fraction(-1, 2), Fraction(1, 2));
}

// operator*()
TEST(Fraction, times)
{
    // pos * pos
    ASSERT_EQ(Fraction(1, 2) * Fraction(1, 2), Fraction(1, 4));

    // pos * zero
    ASSERT_EQ(Fraction(1, 2) * Fraction(0), 0);

    // pos * neg
    ASSERT_EQ(Fraction(1, 2) * Fraction(-1), Fraction(-1, 2));

    // neg * pos
    ASSERT_EQ(Fraction(-1, 2) * Fraction(1), Fraction(-1, 2));

    // neg * zero
    ASSERT_EQ(Fraction(-1, 2) * Fraction(0), 0);

    // neg * neg
    ASSERT_EQ(Fraction(-1, 2) * Fraction(-1), Fraction(1, 2));

    // zero * pos
    ASSERT_EQ(Fraction(0) * Fraction(1, 2), 0);

    // zero * zero
    ASSERT_EQ(Fraction(0) * Fraction(0), 0);

    // zero * neg
    ASSERT_EQ(Fraction(0) * Fraction(-1, 2), 0);
}

// operator/()
TEST(Fraction, divide)
{
    // pos / pos
    ASSERT_EQ(Fraction(1, 2) / Fraction(1, 2), 1);

    // pos / zero
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 2) / Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // pos / neg
    ASSERT_EQ(Fraction(1, 2) / Fraction(-1, 2), -1);

    // neg / pos
    ASSERT_EQ(Fraction(-1, 2) / Fraction(1, 2), -1);

    // neg / zero
    MY_ASSERT_THROW_MESSAGE(Fraction(-1, 2) / Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // neg / neg
    ASSERT_EQ(Fraction(-1, 2) / Fraction(-1, 2), 1);

    // zero / pos
    ASSERT_EQ(Fraction(0) / Fraction(1, 2), 0);

    // zero / zero
    MY_ASSERT_THROW_MESSAGE(Fraction(0) / Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // zero / neg
    ASSERT_EQ(Fraction(0) / Fraction(-1, 2), 0);
}

// operator%()
TEST(Fraction, mod)
{
    // pos % pos
    ASSERT_EQ(Fraction(1, 2) % Fraction(1, 3), Fraction(1, 6));

    // pos % zero
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 2) % Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // pos % neg
    ASSERT_EQ(Fraction(1, 2) % Fraction(-1, 3), Fraction(1, 6));

    // neg % pos
    ASSERT_EQ(Fraction(-1, 2) % Fraction(1, 3), Fraction(-1, 6));

    // neg % zero
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 2) % Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // neg % neg
    ASSERT_EQ(Fraction(-1, 2) % Fraction(-1, 2), 0);

    // zero % pos
    ASSERT_EQ(Fraction(0) % Fraction(1, 2), 0);

    // zero % zero
    MY_ASSERT_THROW_MESSAGE(Fraction(0) % Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // zero % neg
    ASSERT_EQ(Fraction(0) % Fraction(-1, 2), 0);
}

// to_string()
TEST(Fraction, to_string)
{
    ASSERT_EQ(Fraction(0).to_string(), "0");
    ASSERT_EQ(Fraction(3, 6).to_string(), "1/2");
    ASSERT_EQ(Fraction(3, -6).to_string(), "-1/2");
}

// operator<<()
TEST(Fraction, print)
{
    std::ostringstream oss;

    Fraction zero;
    oss << zero;
    ASSERT_EQ(oss.str(), "0"); // string == char*, use eq
    oss.str("");

    Fraction pos(2, 4);
    oss << pos;
    ASSERT_EQ(oss.str(), "1/2");
    oss.str("");

    Fraction neg(2, -4);
    oss << neg;
    ASSERT_EQ(oss.str(), "-1/2");
    oss.str("");
}
