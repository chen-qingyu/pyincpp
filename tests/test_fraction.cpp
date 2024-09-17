#include "../sources/fraction.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Fraction")
{
    SECTION("basics")
    {
        // Fraction(int numerator = 0, int denominator = 1)
        Fraction f1;
        Fraction f2(2);
        Fraction f3(2, 3);
        REQUIRE_THROWS_MATCHES(Fraction(1, 0), std::runtime_error, Message("Error: Divide by zero."));

        // Fraction(const Fraction& that)
        Fraction f4(f3);

        // Fraction(Fraction&& that)
        Fraction f5(std::move(f4));

        // ~Fraction()
    }

    Fraction zero;
    Fraction positive(1, 2);
    Fraction negative(-1, 2);

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

    SECTION("assignment")
    {
        positive = negative; // copy
        REQUIRE(positive == Fraction(-1, 2));
        REQUIRE(negative == Fraction(-1, 2));

        zero = std::move(negative); // move
        REQUIRE(zero == Fraction(-1, 2));
        REQUIRE(negative == Fraction());
    }

    SECTION("examination")
    {
        REQUIRE(double(Fraction(0, 2)) == Approx(0.0));
        REQUIRE(double(Fraction(1, 2)) == Approx(0.5));
        REQUIRE(double(Fraction(2, 3)) == Approx(2.0 / 3.0));
        REQUIRE(double(Fraction(1, -2)) == Approx(-0.5));

        REQUIRE(zero.numerator() == 0);
        REQUIRE(positive.numerator() == 1);
        REQUIRE(negative.numerator() == -1);

        REQUIRE(zero.denominator() == 1);
        REQUIRE(positive.denominator() == 2);
        REQUIRE(negative.denominator() == 2);
    }

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

    SECTION("unary")
    {
        REQUIRE(+zero == Fraction(0));
        REQUIRE(+positive == Fraction(1, 2));
        REQUIRE(+negative == Fraction(-1, 2));

        REQUIRE(-zero == Fraction(0));
        REQUIRE(-positive == Fraction(-1, 2));
        REQUIRE(-negative == Fraction(1, 2));

        REQUIRE(zero.abs() == Fraction(0));
        REQUIRE(positive.abs() == Fraction(1, 2));
        REQUIRE(negative.abs() == Fraction(1, 2));
    }

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

    SECTION("divide")
    {
        REQUIRE(positive / positive == Fraction(1));
        REQUIRE_THROWS_MATCHES(positive / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(positive / negative == Fraction(-1));

        REQUIRE(negative / positive == Fraction(-1));
        REQUIRE_THROWS_MATCHES(negative / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(negative / negative == Fraction(1));

        REQUIRE(zero / positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(zero / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(zero / negative == Fraction(0));
    }

    SECTION("mod")
    {
        REQUIRE(positive % positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(positive % zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(positive % negative == Fraction(0));

        REQUIRE(negative % positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(negative % zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(negative % negative == Fraction(0));

        REQUIRE(zero % positive == Fraction(0));
        REQUIRE_THROWS_MATCHES(zero % zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(zero % negative == Fraction(0));
    }

    SECTION("gcd_lcm")
    {
        // gcd()
        REQUIRE(Fraction::gcd(Fraction(0), Fraction(0)) == Fraction(0));
        REQUIRE(Fraction::gcd(Fraction(0), Fraction(1)) == Fraction(1));
        REQUIRE(Fraction::gcd(Fraction(1), Fraction(0)) == Fraction(1));
        REQUIRE(Fraction::gcd(Fraction(1), Fraction(1)) == Fraction(1));

        REQUIRE(Fraction::gcd(Fraction(1, 2), Fraction(3, 4)) == Fraction(1, 4));
        REQUIRE(Fraction::gcd(Fraction(3, 4), Fraction(1, 6)) == Fraction(1, 12));
        REQUIRE(Fraction::gcd(Fraction(233, 2333), Fraction(7, 77)) == Fraction(1, 25663));
        REQUIRE(Fraction::gcd(Fraction(-1, 2), Fraction(-3, 4)) == Fraction(-1, 4));

        // lcm()
        REQUIRE(Fraction::lcm(Fraction(0), Fraction(0)) == Fraction(0));
        REQUIRE(Fraction::lcm(Fraction(0), Fraction(1)) == Fraction(0));
        REQUIRE(Fraction::lcm(Fraction(1), Fraction(0)) == Fraction(0));
        REQUIRE(Fraction::lcm(Fraction(1), Fraction(1)) == Fraction(1));

        REQUIRE(Fraction::lcm(Fraction(1, 2), Fraction(3, 4)) == Fraction(3, 2));
        REQUIRE(Fraction::lcm(Fraction(3, 4), Fraction(1, 6)) == Fraction(3, 2));
        REQUIRE(Fraction::lcm(Fraction(233, 2333), Fraction(7, 77)) == Fraction(233));
        REQUIRE(Fraction::lcm(Fraction(-1, 2), Fraction(-3, 4)) == Fraction(-3, 2));
    }

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

    SECTION("input")
    {
        Fraction f1, f2, f3, f4;
        std::istringstream("  +1/-2  \n  233 \t 1234/4321  0") >> f1 >> f2 >> f3 >> f4;

        REQUIRE(f1 == Fraction(-1, 2));
        REQUIRE(f2 == Fraction(233));
        REQUIRE(f3 == Fraction(1234, 4321));
        REQUIRE(f4 == Fraction(0));

        Fraction err;
        REQUIRE_THROWS_MATCHES(std::istringstream("z1/2") >> err, std::runtime_error, Message("Error: Wrong fraction literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1z/2") >> err, std::runtime_error, Message("Error: Wrong fraction literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1/z2") >> err, std::runtime_error, Message("Error: Wrong fraction literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1/2z") >> err, std::runtime_error, Message("Error: Wrong fraction literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("1|2") >> err, std::runtime_error, Message("Error: Wrong fraction literal."));
        REQUIRE_THROWS_MATCHES(std::istringstream("0/0") >> err, std::runtime_error, Message("Error: Divide by zero."));
    }
}
