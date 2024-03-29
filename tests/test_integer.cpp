#include "../sources/Integer.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Integer")
{
    // constructor destructor
    SECTION("basics")
    {
        // Integer()
        Integer int1;

        // Integer(const char* chars)
        Integer int2("123456789000");
        REQUIRE_THROWS_MATCHES(Integer("hello"), std::runtime_error, Message("Error: Wrong integer literal."));

        // Integer(const String& str)
        Integer int3(String("123456789000"));
        REQUIRE_THROWS_MATCHES(Integer(String("hello")), std::runtime_error, Message("Error: Wrong integer literal."));

        // Integer(int integer)
        Integer int4 = 123456789;

        // Integer(const Integer &that)
        Integer int5(int4);

        // Integer(Integer &&that)
        Integer int6(std::move(int5));

        // ~Integer()
    }

    Integer zero;
    Integer positive = "18446744073709551617";  // 2^64+1
    Integer negative = "-18446744073709551617"; // -(2^64+1)

    // operator==() operator!=() operator<() operator<=() operator>() operator>=()
    SECTION("compare")
    {
        // operator==
        REQUIRE(zero == zero);
        REQUIRE(positive == positive);
        REQUIRE(negative == negative);

        // operator!=
        REQUIRE(zero != positive);
        REQUIRE(zero != negative);

        // operator<
        REQUIRE(negative < zero);
        REQUIRE(negative < positive);

        // operator<=
        REQUIRE(negative <= zero);
        REQUIRE(negative <= positive);
        REQUIRE(negative <= negative);

        // operator>
        REQUIRE(positive > zero);
        REQUIRE(positive > negative);

        // operator>=
        REQUIRE(positive >= zero);
        REQUIRE(positive >= negative);
        REQUIRE(positive >= positive);
    }

    // operator=()
    SECTION("copy_assignment")
    {
        positive = negative;
        REQUIRE(positive == Integer("-18446744073709551617"));
        REQUIRE(negative == Integer("-18446744073709551617"));
    }

    // operator=()
    SECTION("move_assignment")
    {
        positive = std::move(negative);
        REQUIRE(positive == Integer("-18446744073709551617"));
        REQUIRE(negative == Integer());
    }

    // digits() is_zero() is_positive() is_negative() is_even() is_odd()
    SECTION("examination")
    {
        // digits()
        REQUIRE(zero.digits() == 0);
        REQUIRE(positive.digits() == 20);
        REQUIRE(negative.digits() == 20);

        // is_zero()
        REQUIRE(zero.is_zero());
        REQUIRE(!positive.is_zero());
        REQUIRE(!negative.is_zero());

        // is_positive()
        REQUIRE(!zero.is_positive());
        REQUIRE(positive.is_positive());
        REQUIRE(!negative.is_positive());

        // is_negative()
        REQUIRE(!zero.is_negative());
        REQUIRE(!positive.is_negative());
        REQUIRE(negative.is_negative());

        // is_even()
        REQUIRE(zero.is_even());
        REQUIRE(!positive.is_even());
        REQUIRE(!negative.is_even());

        // is_odd()
        REQUIRE(!zero.is_odd());
        REQUIRE(positive.is_odd());
        REQUIRE(negative.is_odd());
    }

    // operator++() operator--()
    SECTION("inc_dec")
    {
        // operator++()
        REQUIRE(++Integer("-1") == "0");
        REQUIRE(++Integer("0") == "1");
        REQUIRE(++Integer("1") == "2");
        REQUIRE(++Integer("99999999999999") == "100000000000000");

        // operator--()
        REQUIRE(--Integer("-1") == "-2");
        REQUIRE(--Integer("0") == "-1");
        REQUIRE(--Integer("1") == "0");
        REQUIRE(--Integer("100000000000000") == "99999999999999");
    }

    // operator+()
    SECTION("plus")
    {
        REQUIRE(positive + positive == "36893488147419103234");
        REQUIRE(positive + zero == "18446744073709551617");
        REQUIRE(positive + negative == "0");

        REQUIRE(negative + positive == "0");
        REQUIRE(negative + zero == "-18446744073709551617");
        REQUIRE(negative + negative == "-36893488147419103234");

        REQUIRE(zero + positive == "18446744073709551617");
        REQUIRE(zero + zero == "0");
        REQUIRE(zero + negative == "-18446744073709551617");
    }

    // operator-()
    SECTION("minus")
    {
        REQUIRE(positive - positive == "0");
        REQUIRE(positive - zero == "18446744073709551617");
        REQUIRE(positive - negative == "36893488147419103234");

        REQUIRE(negative - positive == "-36893488147419103234");
        REQUIRE(negative - zero == "-18446744073709551617");
        REQUIRE(negative - negative == "0");

        REQUIRE(zero - positive == "-18446744073709551617");
        REQUIRE(zero - zero == "0");
        REQUIRE(zero - negative == "18446744073709551617");
    }

    // operator*()
    SECTION("times")
    {
        REQUIRE(positive * positive == "340282366920938463500268095579187314689");
        REQUIRE(positive * zero == "0");
        REQUIRE(positive * negative == "-340282366920938463500268095579187314689");

        REQUIRE(negative * positive == "-340282366920938463500268095579187314689");
        REQUIRE(negative * zero == "0");
        REQUIRE(negative * negative == "340282366920938463500268095579187314689");

        REQUIRE(zero * positive == "0");
        REQUIRE(zero * zero == "0");
        REQUIRE(zero * negative == "0");
    }

    // operator/()
    SECTION("divide")
    {
        REQUIRE(positive / positive == "1");
        REQUIRE_THROWS_MATCHES(positive / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(positive / negative == "-1");

        REQUIRE(negative / positive == "-1");
        REQUIRE_THROWS_MATCHES(negative / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(negative / negative == "1");

        REQUIRE(zero / positive == "0");
        REQUIRE_THROWS_MATCHES(zero / zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(zero / negative == "0");
    }

    // operator%()
    SECTION("mod")
    {
        REQUIRE(positive % positive == "0");
        REQUIRE_THROWS_MATCHES(positive % zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(positive % negative == "0");

        REQUIRE(negative % positive == "0");
        REQUIRE_THROWS_MATCHES(negative % zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(negative % negative == "0");

        REQUIRE(zero % positive == "0");
        REQUIRE_THROWS_MATCHES(zero % zero, std::runtime_error, Message("Error: Divide by zero."));
        REQUIRE(zero % negative == "0");
    }

    // pow()
    SECTION("pow")
    {
        // 0^0 == 1
        REQUIRE(Integer("0").pow("0") == "1");

        // 0^1 == 0
        REQUIRE(Integer("0").pow("1") == "0");

        // 1^0 == 1
        REQUIRE(Integer("1").pow("0") == "1");

        // 1^1 == 1
        REQUIRE(Integer("1").pow("1") == "1");

        // 2^3 == 8
        REQUIRE(Integer("2").pow("3") == "8");

        // 2^100 == 1267650600228229401496703205376
        REQUIRE(Integer("2").pow("100") == "1267650600228229401496703205376");

        // (9^9)^9 == 196627050475552913618075908526912116283103450944214766927315415537966391196809
        REQUIRE(Integer("9").pow("9").pow("9") == "196627050475552913618075908526912116283103450944214766927315415537966391196809");

        // 1024^1024 % 100 == 76
        REQUIRE(Integer("1024").pow("1024", "100") == "76");

        // 9999^1001 % 100 == 99
        REQUIRE(Integer("9999").pow("1001", "100") == "99");
    }

    // factorial()
    SECTION("factorial")
    {
        // (negative)! throws exception
        REQUIRE_THROWS_MATCHES(Integer("-1").factorial(), std::runtime_error, Message("Error: Negative integer have no factorial."));

        // 0! == 1
        REQUIRE(Integer("0").factorial() == "1");

        // 1! == 1
        REQUIRE(Integer("1").factorial() == "1");

        // 2! == 2
        REQUIRE(Integer("2").factorial() == "2");

        // 3! == 6
        REQUIRE(Integer("3").factorial() == "6");

        // 100! == 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
        REQUIRE(Integer("100").factorial() == "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000");

        // (5!)! == 6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000
        REQUIRE(Integer("5").factorial().factorial() == "6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000");
    }

    // gcd() lcm()
    SECTION("gcd_lcm")
    {
        // gcd()
        REQUIRE(gcd("0", "1") == "1");
        REQUIRE(gcd("6", "12") == "6");
        REQUIRE(gcd("6", "11") == "1");
        REQUIRE(gcd("12345", "54321") == "3");

        // lcm()
        REQUIRE(lcm("0", "1") == "0");
        REQUIRE(lcm("6", "12") == "12");
        REQUIRE(lcm("6", "11") == "66");
        REQUIRE(lcm("12345", "54321") == "223530915");
    }

    // sqrt()
    SECTION("sqrt")
    {
        REQUIRE_THROWS_MATCHES(Integer("-1").sqrt(), std::runtime_error, Message("Error: Cannot compute square root of a negative integer."));

        REQUIRE(Integer("0").sqrt() == "0");
        REQUIRE(Integer("1").sqrt() == "1");
        REQUIRE(Integer("2").sqrt() == "1");
        REQUIRE(Integer("3").sqrt() == "1");
        REQUIRE(Integer("4").sqrt() == "2");
        REQUIRE(Integer("5").sqrt() == "2");
        REQUIRE(Integer("9").sqrt() == "3");
        REQUIRE(Integer("9801").sqrt() == "99");
    }

    // to_string()
    SECTION("to_string")
    {
        REQUIRE(zero.to_string() == "0");
        REQUIRE(positive.to_string() == "18446744073709551617");
        REQUIRE(negative.to_string() == "-18446744073709551617");
    }

    // operator<<()
    SECTION("print")
    {
        std::ostringstream oss;

        oss << zero;
        REQUIRE(oss.str() == "0");
        oss.str("");

        oss << positive;
        REQUIRE(oss.str() == "18446744073709551617");
        oss.str("");

        oss << negative;
        REQUIRE(oss.str() == "-18446744073709551617");
        oss.str("");
    }

    // operator>>()
    SECTION("input")
    {
        Integer int1, int2, int3, int4;
        std::istringstream("+123\n-456\t789 0") >> int1 >> int2 >> int3 >> int4;

        REQUIRE(int1 == Integer("123"));
        REQUIRE(int2 == Integer("-456"));
        REQUIRE(int3 == Integer("789"));
        REQUIRE(int4 == Integer("0"));
    }
}
