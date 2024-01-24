#include "../sources/Fraction.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor
TEST_CASE("Fraction: basics")
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
TEST_CASE("Fraction: compare")
{
    REQUIRE(Fraction(0) == Fraction(0));
    REQUIRE(Fraction(9, 6) == Fraction(3, 2));

    REQUIRE(Fraction(1) > Fraction(1, 2));
    REQUIRE(Fraction(0) < Fraction(1, 2));

    REQUIRE(Fraction(2, 3) >= Fraction(1, 3));
    REQUIRE(Fraction(1, 3) >= Fraction(1, 3));

    REQUIRE(Fraction(-1, 3) <= Fraction(1, 3));
    REQUIRE(Fraction(1, 3) <= Fraction(1, 3));
}

// operator=()
TEST_CASE("Fraction: copy_assignment")
{
    Fraction fraction1(1, 2);
    Fraction fraction2(2, 3);

    fraction1 = fraction2;
    REQUIRE(fraction1 == Fraction(2, 3));
    REQUIRE(fraction2 == Fraction(2, 3));
}

// operator=()
TEST_CASE("Fraction: move_assignment")
{
    Fraction fraction1(1, 2);
    Fraction fraction2(2, 3);

    fraction1 = std::move(fraction2);
    REQUIRE(fraction1 == Fraction(2, 3));
    REQUIRE(fraction2 == Fraction());
}

// operator double()
TEST_CASE("Fraction: examination")
{
    REQUIRE(double(Fraction(0, 2)) == Catch::Approx(0.0));
    REQUIRE(double(Fraction(1, 2)) == Catch::Approx(0.5));
    REQUIRE(double(Fraction(2, 3)) == Catch::Approx(2.0 / 3.0));
    REQUIRE(double(Fraction(1, -2)) == Catch::Approx(-0.5));
}

// operator++() operator--()
TEST_CASE("Fraction: inc_dec")
{
    // operator++()
    REQUIRE(++Fraction(-1) == Fraction(0));
    REQUIRE(++Fraction(0) == Fraction(1));
    REQUIRE(++Fraction(1) == Fraction(2));
    REQUIRE(++Fraction(99999) == Fraction(100000));

    // operator--()
    REQUIRE(--Fraction(-1) == Fraction(-2));
    REQUIRE(--Fraction(0) == Fraction(-1));
    REQUIRE(--Fraction(1) == Fraction(0));
    REQUIRE(--Fraction(100000) == Fraction(99999));
}

// operator+() operator-() abs()
TEST_CASE("Fraction: unary")
{
    Fraction a(1, 2);
    REQUIRE(+a == Fraction(1, 2));
    REQUIRE(-a == Fraction(-1, 2));
    REQUIRE(a.abs() == Fraction(1, 2));

    Fraction b(-1, 3);
    REQUIRE(+b == Fraction(-1, 3));
    REQUIRE(-b == Fraction(1, 3));
    REQUIRE(b.abs() == Fraction(1, 3));
}

// operator+()
TEST_CASE("Fraction: plus")
{
    // pos + pos
    REQUIRE(Fraction(1, 2) + Fraction(1, 2) == Fraction(1));

    // pos + zero
    REQUIRE(Fraction(1, 2) + Fraction(0) == Fraction(1, 2));

    // pos + neg
    REQUIRE(Fraction(1, 2) + Fraction(1, -2) == Fraction(0));

    // neg + pos
    REQUIRE(Fraction(-1, 2) + Fraction(1, 2) == Fraction(0));

    // neg + zero
    REQUIRE(Fraction(-1, 2) + Fraction(0) == Fraction(-1, 2));

    // neg + neg
    REQUIRE(Fraction(-1, 2) + Fraction(-1, 2) == Fraction(-1));

    // zero + pos
    REQUIRE(Fraction(0) + Fraction(1, 2) == Fraction(1, 2));

    // zero + zero
    REQUIRE(Fraction(0) + Fraction(0) == Fraction(0));

    // zero + neg
    REQUIRE(Fraction(0) + Fraction(-1, 2) == Fraction(-1, 2));
}

// operator-()
TEST_CASE("Fraction: minus")
{
    // pos - pos
    REQUIRE(Fraction(1, 2) - Fraction(1, 2) == Fraction(0));

    // pos - zero
    REQUIRE(Fraction(1, 2) - Fraction(0) == Fraction(1, 2));

    // pos - neg
    REQUIRE(Fraction(1, 2) - Fraction(-1, 2) == Fraction(1));

    // neg - pos
    REQUIRE(Fraction(-1, 2) - Fraction(1, 2) == Fraction(-1));

    // neg - zero
    REQUIRE(Fraction(-1, 2) - Fraction(0) == Fraction(-1, 2));

    // neg - neg
    REQUIRE(Fraction(-1, 2) - Fraction(-1, 2) == Fraction(0));

    // zero - pos
    REQUIRE(Fraction(0) - Fraction(1, 2) == Fraction(-1, 2));

    // zero - zero
    REQUIRE(Fraction(0) - Fraction(0) == Fraction(0));

    // zero - neg
    REQUIRE(Fraction(0) - Fraction(-1, 2) == Fraction(1, 2));
}

// operator*()
TEST_CASE("Fraction: times")
{
    // pos * pos
    REQUIRE(Fraction(1, 2) * Fraction(1, 2) == Fraction(1, 4));

    // pos * zero
    REQUIRE(Fraction(1, 2) * Fraction(0) == Fraction(0));

    // pos * neg
    REQUIRE(Fraction(1, 2) * Fraction(-1) == Fraction(-1, 2));

    // neg * pos
    REQUIRE(Fraction(-1, 2) * Fraction(1) == Fraction(-1, 2));

    // neg * zero
    REQUIRE(Fraction(-1, 2) * Fraction(0) == Fraction(0));

    // neg * neg
    REQUIRE(Fraction(-1, 2) * Fraction(-1) == Fraction(1, 2));

    // zero * pos
    REQUIRE(Fraction(0) * Fraction(1, 2) == Fraction(0));

    // zero * zero
    REQUIRE(Fraction(0) * Fraction(0) == Fraction(0));

    // zero * neg
    REQUIRE(Fraction(0) * Fraction(-1, 2) == Fraction(0));
}

// operator/()
TEST_CASE("Fraction: divide")
{
    // pos / pos
    REQUIRE(Fraction(1, 2) / Fraction(1, 2) == Fraction(1));

    // pos / zero
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 2) / Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // pos / neg
    REQUIRE(Fraction(1, 2) / Fraction(-1, 2) == Fraction(-1));

    // neg / pos
    REQUIRE(Fraction(-1, 2) / Fraction(1, 2) == Fraction(-1));

    // neg / zero
    MY_ASSERT_THROW_MESSAGE(Fraction(-1, 2) / Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // neg / neg
    REQUIRE(Fraction(-1, 2) / Fraction(-1, 2) == Fraction(1));

    // zero / pos
    REQUIRE(Fraction(0) / Fraction(1, 2) == Fraction(0));

    // zero / zero
    MY_ASSERT_THROW_MESSAGE(Fraction(0) / Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // zero / neg
    REQUIRE(Fraction(0) / Fraction(-1, 2) == Fraction(0));
}

// operator%()
TEST_CASE("Fraction: mod")
{
    // pos % pos
    REQUIRE(Fraction(1, 2) % Fraction(1, 3) == Fraction(1, 6));

    // pos % zero
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 2) % Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // pos % neg
    REQUIRE(Fraction(1, 2) % Fraction(-1, 3) == Fraction(1, 6));

    // neg % pos
    REQUIRE(Fraction(-1, 2) % Fraction(1, 3) == Fraction(-1, 6));

    // neg % zero
    MY_ASSERT_THROW_MESSAGE(Fraction(1, 2) % Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // neg % neg
    REQUIRE(Fraction(-1, 2) % Fraction(-1, 2) == Fraction(0));

    // zero % pos
    REQUIRE(Fraction(0) % Fraction(1, 2) == Fraction(0));

    // zero % zero
    MY_ASSERT_THROW_MESSAGE(Fraction(0) % Fraction(0), std::runtime_error, "Error: Zero denominator.");

    // zero % neg
    REQUIRE(Fraction(0) % Fraction(-1, 2) == Fraction(0));
}

// to_string()
TEST_CASE("Fraction: to_string")
{
    REQUIRE(Fraction(0).to_string() == "0");
    REQUIRE(Fraction(3, 6).to_string() == "1/2");
    REQUIRE(Fraction(3, -6).to_string() == "-1/2");
}

// operator<<()
TEST_CASE("Fraction: print")
{
    std::ostringstream oss;

    Fraction zero;
    oss << zero;
    REQUIRE(oss.str() == "0");
    oss.str("");

    Fraction pos(2, 4);
    oss << pos;
    REQUIRE(oss.str() == "1/2");
    oss.str("");

    Fraction neg(2, -4);
    oss << neg;
    REQUIRE(oss.str() == "-1/2");
    oss.str("");
}
