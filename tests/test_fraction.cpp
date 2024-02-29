#include "../sources/Fraction.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Fraction")
{
    // constructor destructor
    SECTION("basics")
    {
        // Fraction(int numerator = 0, int denominator = 1)
        Fraction f1;
        Fraction f2(2);
        Fraction f3(2, 3);
        REQUIRE_THROWS_MATCHES(Fraction(1, 0), std::runtime_error, Message("Error: Zero denominator."));

        // Fraction(const Fraction &that)
        Fraction f4(f3);

        // Fraction(Fraction &&that)
        Fraction f5(std::move(f4));

        // ~Fraction()
    }

    Fraction zero;
    Fraction positive(1, 2);
    Fraction negative(-1, 2);

    // operator==() operator!=() operator<() operator<=() operator>() operator>=()
    SECTION("compare")
    {
        REQUIRE(zero == zero);
        REQUIRE(Fraction(9, 6) == Fraction(3, 2));

        REQUIRE(zero != positive);
        REQUIRE(positive != negative);

        REQUIRE(zero > negative);
        REQUIRE(Fraction(1, 2) > Fraction(1, 3));

        REQUIRE(zero < positive);
        REQUIRE(Fraction(1, 4) < Fraction(1, 3));

        REQUIRE(zero >= zero);
        REQUIRE(zero >= negative);

        REQUIRE(zero <= zero);
        REQUIRE(zero <= positive);
    }

    // operator=()
    SECTION("copy_assignment")
    {
        positive = negative;
        REQUIRE(positive == Fraction(-1, 2));
        REQUIRE(negative == Fraction(-1, 2));
    }

    // operator=()
    SECTION("move_assignment")
    {
        positive = std::move(negative);
        REQUIRE(positive == Fraction(-1, 2));
        REQUIRE(negative == Fraction());
    }

    // operator double()
    SECTION("examination")
    {
        REQUIRE(double(Fraction(0, 2)) == Approx(0.0));
        REQUIRE(double(Fraction(1, 2)) == Approx(0.5));
        REQUIRE(double(Fraction(2, 3)) == Approx(2.0 / 3.0));
        REQUIRE(double(Fraction(1, -2)) == Approx(-0.5));
    }

    // operator++() operator--()
    SECTION("inc_dec")
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
    SECTION("unary")
    {
        REQUIRE(+positive == Fraction(1, 2));
        REQUIRE(-positive == Fraction(-1, 2));
        REQUIRE(positive.abs() == Fraction(1, 2));

        REQUIRE(+negative == Fraction(-1, 2));
        REQUIRE(-negative == Fraction(1, 2));
        REQUIRE(negative.abs() == Fraction(1, 2));
    }

    // operator+()
    SECTION("plus")
    {
        REQUIRE(positive + positive == Fraction(1));
        REQUIRE(positive + zero == Fraction(1, 2));
        REQUIRE(positive + negative == Fraction(0));

        REQUIRE(negative + positive == Fraction(0));
        REQUIRE(negative + zero == Fraction(-1, 2));
        REQUIRE(negative + negative == Fraction(-1));

        REQUIRE(zero + positive == Fraction(1, 2));
        REQUIRE(zero + zero == Fraction(0));
        REQUIRE(zero + negative == Fraction(-1, 2));
    }

    // operator-()
    SECTION("minus")
    {
        REQUIRE(positive - positive == Fraction(0));
        REQUIRE(positive - zero == Fraction(1, 2));
        REQUIRE(positive - negative == Fraction(1));

        REQUIRE(negative - positive == Fraction(-1));
        REQUIRE(negative - zero == Fraction(-1, 2));
        REQUIRE(negative - negative == Fraction(0));

        REQUIRE(zero - positive == Fraction(-1, 2));
        REQUIRE(zero - zero == Fraction(0));
        REQUIRE(zero - negative == Fraction(1, 2));
    }

    // operator*()
    SECTION("times")
    {
        REQUIRE(positive * positive == Fraction(1, 4));
        REQUIRE(positive * zero == Fraction(0));
        REQUIRE(positive * negative == Fraction(-1, 4));

        REQUIRE(negative * positive == Fraction(-1, 4));
        REQUIRE(negative * zero == Fraction(0));
        REQUIRE(negative * negative == Fraction(1, 4));

        REQUIRE(zero * positive == Fraction(0));
        REQUIRE(zero * zero == Fraction(0));
        REQUIRE(zero * negative == Fraction(0));
    }

    // operator/()
    SECTION("divide")
    {
        REQUIRE(positive / positive == Fraction(1));
        REQUIRE_THROWS_MATCHES(positive / zero, std::runtime_error, Message("Error: Zero denominator."));
        REQUIRE(positive / negative == Fraction(-1));

        REQUIRE(negative / positive == Fraction(-1));
        REQUIRE_THROWS_MATCHES(negative / zero, std::runtime_error, Message("Error: Zero denominator."));
        REQUIRE(negative / negative == Fraction(1));

        REQUIRE(zero / positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(zero / zero, std::runtime_error, Message("Error: Zero denominator."));
        REQUIRE(zero / negative == Fraction(0));
    }

    // operator%()
    SECTION("mod")
    {
        REQUIRE(positive % positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(positive % zero, std::runtime_error, Message("Error: Zero denominator."));
        REQUIRE(positive % negative == Fraction(0));

        REQUIRE(negative % positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(negative % zero, std::runtime_error, Message("Error: Zero denominator."));
        REQUIRE(negative % negative == Fraction(0));

        REQUIRE(zero % positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(zero % zero, std::runtime_error, Message("Error: Zero denominator."));
        REQUIRE(zero % negative == Fraction(0));
    }

    // to_string()
    SECTION("to_string")
    {
        REQUIRE(Fraction(0).to_string() == "0");
        REQUIRE(Fraction(3, 6).to_string() == "1/2");
        REQUIRE(Fraction(3, -6).to_string() == "-1/2");
    }

    // operator<<()
    SECTION("print")
    {
        std::ostringstream oss;

        oss << zero;
        REQUIRE(oss.str() == "0");
        oss.str("");

        oss << positive;
        REQUIRE(oss.str() == "1/2");
        oss.str("");

        oss << negative;
        REQUIRE(oss.str() == "-1/2");
        oss.str("");
    }
}
