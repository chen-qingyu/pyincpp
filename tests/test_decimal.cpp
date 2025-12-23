#include "../sources/decimal.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Decimal")
{
    Decimal zero;
    Decimal positive("0.3");
    Decimal negative("-0.3");

    SECTION("unary")
    {
        REQUIRE(-zero == Decimal("0"));
        REQUIRE(-positive == Decimal("-0.3"));
        REQUIRE(-negative == Decimal("0.3"));

        REQUIRE(zero.abs() == Decimal("0"));
        REQUIRE(positive.abs() == Decimal("0.3"));
        REQUIRE(negative.abs() == Decimal("0.3"));
    }

    SECTION("plus")
    {
        REQUIRE(positive + positive == Decimal("0.6"));
        REQUIRE(positive + zero == Decimal("0.3"));
        REQUIRE(positive + negative == Decimal("0"));

        REQUIRE(negative + positive == Decimal("0"));
        REQUIRE(negative + zero == Decimal("-0.3"));
        REQUIRE(negative + negative == Decimal("-0.6"));

        REQUIRE(zero + positive == Decimal("0.3"));
        REQUIRE(zero + zero == Decimal("0"));
        REQUIRE(zero + negative == Decimal("-0.3"));
    }

    SECTION("minus")
    {
        REQUIRE(positive - positive == Decimal("0"));
        REQUIRE(positive - zero == Decimal("0.3"));
        REQUIRE(positive - negative == Decimal("0.6"));

        REQUIRE(negative - positive == Decimal("-0.6"));
        REQUIRE(negative - zero == Decimal("-0.3"));
        REQUIRE(negative - negative == Decimal("0"));

        REQUIRE(zero - positive == Decimal("-0.3"));
        REQUIRE(zero - zero == Decimal("0"));
        REQUIRE(zero - negative == Decimal("0.3"));
    }

    SECTION("times")
    {
        REQUIRE(positive * positive == Decimal("0.09"));
        REQUIRE(positive * zero == Decimal("0"));
        REQUIRE(positive * negative == Decimal("-0.09"));

        REQUIRE(negative * positive == Decimal("-0.09"));
        REQUIRE(negative * zero == Decimal("0"));
        REQUIRE(negative * negative == Decimal("0.09"));

        REQUIRE(zero * positive == Decimal("0"));
        REQUIRE(zero * zero == Decimal("0"));
        REQUIRE(zero * negative == Decimal("0"));
    }

    SECTION("divide")
    {
        REQUIRE(positive / positive == Decimal("1"));
        REQUIRE(positive / negative == Decimal("-1"));
        REQUIRE(negative / positive == Decimal("-1"));
        REQUIRE(negative / negative == Decimal("1"));
        REQUIRE(zero / positive == Decimal("0"));
        REQUIRE(zero / negative == Decimal("0"));
    }

    SECTION("mod")
    {
        REQUIRE(positive % positive == Decimal("0"));
        REQUIRE(positive % negative == Decimal("0"));
        REQUIRE(negative % positive == Decimal("0"));
        REQUIRE(negative % negative == Decimal("0"));
        REQUIRE(zero % positive == Decimal("0"));
        REQUIRE(zero % negative == Decimal("0"));
    }

    SECTION("from_string")
    {
        REQUIRE(Decimal("0").as_fraction() == Fraction("0"));
        REQUIRE(Decimal("1.1").as_fraction() == Fraction("11/10"));
        REQUIRE(Decimal("1234.56789").as_fraction() == Fraction("123456789/100000"));
        REQUIRE(Decimal("0.75").as_fraction() == Fraction("3/4"));
        REQUIRE(Decimal("22.33").as_fraction() == Fraction("2233/100"));
        REQUIRE(Decimal("-1.2").as_fraction() == Fraction("-6/5"));

        REQUIRE(Decimal("0.5").as_fraction() == Fraction("1/2"));
        REQUIRE(Decimal("0.~3").as_fraction() == Fraction("1/3"));
        REQUIRE(Decimal("0.0~3").as_fraction() == Fraction("1/30"));
        REQUIRE(Decimal("0.8~3").as_fraction() == Fraction("5/6"));
        REQUIRE(Decimal("0.~83").as_fraction() == Fraction("83/99"));
        REQUIRE(Decimal("0.123").as_fraction() == Fraction("123/1000"));
        REQUIRE(Decimal("0.~123").as_fraction() == Fraction("41/333"));
        REQUIRE(Decimal("123").as_fraction() == Fraction("123"));
        REQUIRE(Decimal("0.1~123").as_fraction() == Fraction("187/1665"));
        REQUIRE(Decimal("12.34~56").as_fraction() == Fraction("61111/4950"));
        REQUIRE(Decimal("0.24~9").as_fraction() == Fraction("1/4"));
        REQUIRE(Decimal("0.~375").as_fraction() == Fraction("125/333"));
        REQUIRE(Decimal("4.~518").as_fraction() == Fraction("122/27"));
        REQUIRE(Decimal("0.~9").as_fraction() == Fraction("1"));

        REQUIRE(Decimal("-1").as_fraction() == Fraction("-1"));
        REQUIRE(Decimal("-0.~1").as_fraction() == Fraction("-1/9"));
        REQUIRE(Decimal("-1.9").as_fraction() == Fraction("-19/10"));
        REQUIRE(Decimal("-1.~9").as_fraction() == Fraction("-2"));
        REQUIRE(Decimal("-1.1~9").as_fraction() == Fraction("-6/5"));

        REQUIRE(Decimal("11").as_fraction() == Fraction("11"));
        REQUIRE(Decimal("11#10").as_fraction() == Fraction("11"));
        REQUIRE(Decimal("11#2").as_fraction() == Fraction("3"));
        REQUIRE(Decimal("11#16").as_fraction() == Fraction("17"));
        REQUIRE(Decimal("0.1#2").as_fraction() == Fraction("1/2"));
        REQUIRE(Decimal("0.0~0011#2").as_fraction() == Fraction("1/10"));
        REQUIRE(Decimal("-0.0~0011#2").as_fraction() == Fraction("-1/10"));
        REQUIRE(Decimal("0.~1#2").as_fraction() == Fraction("1"));
    }

    SECTION("to_string")
    {
        REQUIRE(Decimal("1.000").to_string() == std::string("1"));
        REQUIRE(Decimal("0.~3").to_string() == std::string("0.333..."));
        REQUIRE(Decimal("0.0~3").to_string() == std::string("0.0333..."));
        REQUIRE(Decimal("0.83~3").to_string() == std::string("0.8333..."));
        REQUIRE(Decimal("0.123").to_string() == std::string("0.123"));
        REQUIRE(Decimal("-0.~3").to_string() == std::string("-0.333..."));
        REQUIRE(Decimal("-0.~1").to_string() == std::string("-0.111..."));

        REQUIRE(Decimal("-0.0~0011#2").to_string() == std::string("-0.1"));
        REQUIRE(Decimal("0.~1#2").to_string() == std::string("1"));
    }

    SECTION("input")
    {
        Decimal d1, d2, d3;
        std::istringstream("  000\n\n  0.3\t\n\t-0.3\n\n") >> d1 >> d2 >> d3;

        REQUIRE(d1 == zero);
        REQUIRE(d2 == positive);
        REQUIRE(d3 == negative);

        Decimal err;
        REQUIRE_THROWS_MATCHES(std::istringstream("z0.3") >> err, std::runtime_error, Message("Error: Expect format `integer[.decimal][~cyclic][#radix]` but got: z0.3"));
        REQUIRE_THROWS_MATCHES(std::istringstream("0z.3") >> err, std::runtime_error, Message("Error: Expect format `integer[.decimal][~cyclic][#radix]` but got: 0z.3"));
        REQUIRE_THROWS_MATCHES(std::istringstream("0.z3") >> err, std::runtime_error, Message("Error: Expect format `integer[.decimal][~cyclic][#radix]` but got: 0.z3"));
        REQUIRE_THROWS_MATCHES(std::istringstream("0.3z") >> err, std::runtime_error, Message("Error: Expect format `integer[.decimal][~cyclic][#radix]` but got: 0.3z"));
        REQUIRE_THROWS_MATCHES(std::istringstream("0|3") >> err, std::runtime_error, Message("Error: Expect format `integer[.decimal][~cyclic][#radix]` but got: 0|3"));
    }
}
