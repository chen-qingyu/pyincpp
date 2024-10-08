#include "../sources/int.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Int")
{
    SECTION("basics")
    {
        // Int(int integer = 0)
        Int int1;
        REQUIRE(int1.digits() == 0);
        REQUIRE(int1.is_zero());
        Int int2(123456789);
        REQUIRE(int2.digits() == 9);
        REQUIRE(!int2.is_zero());

        // Int(const char* chars)
        Int int3("123456789000");
        REQUIRE(int3.digits() == 12);
        REQUIRE(!int3.is_zero());
        REQUIRE_THROWS_MATCHES(Int("hello"), std::runtime_error, Message("Error: Wrong integer literal."));

        // Int(const Int& that)
        Int int4(int3);
        REQUIRE(int4.digits() == 12);
        REQUIRE(!int4.is_zero());

        // Int(Int&& that)
        Int int5(std::move(int4));
        REQUIRE(int5.digits() == 12);
        REQUIRE(!int5.is_zero());
        REQUIRE(int4.digits() == 0);
        REQUIRE(int4.is_zero());

        // ~Int()
    }

    Int zero;
    Int positive = "18446744073709551617";  // 2^64+1
    Int negative = "-18446744073709551617"; // -(2^64+1)

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

    SECTION("assignment")
    {
        positive = negative; // copy
        REQUIRE(positive == Int("-18446744073709551617"));
        REQUIRE(negative == Int("-18446744073709551617"));

        zero = std::move(negative); // move
        REQUIRE(zero == Int("-18446744073709551617"));
        REQUIRE(negative == Int());
    }

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

    SECTION("is_prime")
    {
        REQUIRE(!Int("-1").is_prime());
        REQUIRE(!Int("0").is_prime());
        REQUIRE(!Int("1").is_prime());
        REQUIRE(Int("2").is_prime());
        REQUIRE(Int("3").is_prime());
        REQUIRE(!Int("4").is_prime());
        REQUIRE(Int("5").is_prime());
        REQUIRE(!Int("6").is_prime());
        REQUIRE(Int("7").is_prime());
        REQUIRE(!Int("8").is_prime());
        REQUIRE(!Int("9").is_prime());
        REQUIRE(!Int("10").is_prime());

        REQUIRE(Int("2147483629").is_prime()); // maximum prime number that < INT_MAX
        REQUIRE(Int("2147483647").is_prime()); // INT_MAX is a prime number
        REQUIRE(Int("2147483659").is_prime()); // minimum prime number that > INT_MAX
    }

    SECTION("inc_dec")
    {
        // operator++()
        REQUIRE(++Int("-1") == "0");
        REQUIRE(++Int("0") == "1");
        REQUIRE(++Int("1") == "2");
        REQUIRE(++Int("99999999999999") == "100000000000000");

        // operator--()
        REQUIRE(--Int("-1") == "-2");
        REQUIRE(--Int("0") == "-1");
        REQUIRE(--Int("1") == "0");
        REQUIRE(--Int("100000000000000") == "99999999999999");
    }

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

        REQUIRE(Int("99999") + Int("1") == "100000");
    }

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

        REQUIRE(Int("100000") - Int("1") == "99999");
    }

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

        REQUIRE(Int("100000") * Int("1") == "100000");
    }

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

        REQUIRE(Int("100000") / Int("1") == "100000");
    }

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

        REQUIRE(Int("100000") % Int("1") == "0");
    }

    SECTION("factorial")
    {
        // (negative)! throws exception
        REQUIRE_THROWS_MATCHES(Int("-1").factorial(), std::runtime_error, Message("Error: Negative integer have no factorial."));

        // 0! == 1
        REQUIRE(Int("0").factorial() == "1");

        // 1! == 1
        REQUIRE(Int("1").factorial() == "1");

        // 2! == 2
        REQUIRE(Int("2").factorial() == "2");

        // 3! == 6
        REQUIRE(Int("3").factorial() == "6");

        // 100! == 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
        REQUIRE(Int("100").factorial() == "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000");

        // (5!)! == 6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000
        REQUIRE(Int("5").factorial().factorial() == "6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000");
    }

    SECTION("next_prime")
    {
        Int number; // 0
        int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
        for (auto&& prime : primes)
        {
            number = number.next_prime();
            REQUIRE(number == prime);
        }

        REQUIRE(Int(104728).next_prime() == 104729); // the 10000th prime

        REQUIRE(Int("2147483628").next_prime() == "2147483629"); // maximum prime number that < INT_MAX
        REQUIRE(Int("2147483629").next_prime() == "2147483647"); // INT_MAX is a prime number
        REQUIRE(Int("2147483647").next_prime() == "2147483659"); // minimum prime number that > INT_MAX
    }

    SECTION("to_integer")
    {
        REQUIRE(zero.to_integer<signed char>() == 0);
        REQUIRE(!std::is_same<decltype(zero.to_integer<signed char>()), unsigned char>::value);
        REQUIRE(std::is_same<decltype(zero.to_integer<signed char>()), signed char>::value);

        REQUIRE(zero.to_integer<long long>() == 0);
        REQUIRE(!std::is_same<decltype(zero.to_integer<long long>()), long>::value);
        REQUIRE(std::is_same<decltype(zero.to_integer<long long>()), long long>::value);

        REQUIRE(Int("1024").to_integer<int>() == 1024);
        REQUIRE(Int("-1024").to_integer<int>() == -1024);
    }

    SECTION("sqrt")
    {
        REQUIRE_THROWS_MATCHES(Int::sqrt("-1"), std::runtime_error, Message("Error: Cannot compute square root of a negative integer."));

        REQUIRE(Int::sqrt("0") == "0");
        REQUIRE(Int::sqrt("1") == "1");
        REQUIRE(Int::sqrt("2") == "1");
        REQUIRE(Int::sqrt("3") == "1");
        REQUIRE(Int::sqrt("4") == "2");
        REQUIRE(Int::sqrt("5") == "2");
        REQUIRE(Int::sqrt("9") == "3");
        REQUIRE(Int::sqrt("10") == "3");
        REQUIRE(Int::sqrt("16") == "4");
        REQUIRE(Int::sqrt("100") == "10");
        REQUIRE(Int::sqrt("9801") == "99");
    }

    SECTION("pow")
    {
        // special situations
        REQUIRE(Int::pow("-1", "-1") == "-1");
        REQUIRE(Int::pow("-1", "0") == "1");
        REQUIRE(Int::pow("-1", "1") == "-1");
        REQUIRE_THROWS_MATCHES(Int::pow("0", "-1"), std::runtime_error, Message("Error: Math domain error."));
        REQUIRE(Int::pow("0", "0") == "1");
        REQUIRE(Int::pow("0", "1") == "0");
        REQUIRE(Int::pow("1", "-1") == "1");
        REQUIRE(Int::pow("1", "0") == "1");
        REQUIRE(Int::pow("1", "1") == "1");

        // 2^3 == 8
        REQUIRE(Int::pow("2", "3") == "8");

        // 2^100 == 1267650600228229401496703205376
        REQUIRE(Int::pow("2", "100") == "1267650600228229401496703205376");

        // (9^9)^9 == 196627050475552913618075908526912116283103450944214766927315415537966391196809
        REQUIRE(Int::pow(Int::pow("9", "9"), "9") == "196627050475552913618075908526912116283103450944214766927315415537966391196809");

        // 1024^1024 % 100 == 76
        REQUIRE(Int::pow("1024", "1024", "100") == "76");

        // 9999^1001 % 100 == 99
        REQUIRE(Int::pow("9999", "1001", "100") == "99");
    }

    SECTION("log")
    {
        REQUIRE_THROWS_MATCHES(Int::log(negative, 2), std::runtime_error, Message("Error: Math domain error."));
        REQUIRE_THROWS_MATCHES(Int::log(zero, 2), std::runtime_error, Message("Error: Math domain error."));
        REQUIRE_THROWS_MATCHES(Int::log(positive, 1), std::runtime_error, Message("Error: Math domain error."));

        REQUIRE(Int::log(1, 2) == 0);
        REQUIRE(Int::log(1, 3) == 0);
        REQUIRE(Int::log(1, 4) == 0);

        REQUIRE(Int::log(2, 2) == 1);
        REQUIRE(Int::log(4, 2) == 2);
        REQUIRE(Int::log(8, 2) == 3);

        REQUIRE(Int::log(10, 10) == 1);
        REQUIRE(Int::log(100, 10) == 2);
        REQUIRE(Int::log(1000, 10) == 3);

        REQUIRE(Int::log(123, 10) == 2);
        REQUIRE(Int::log(12345, 10) == 4);
        REQUIRE(Int::log(123456789, 10) == 8);

        REQUIRE(Int::log(positive, 2) == 64);         // integer: 2^64+1
        REQUIRE(Int::log(positive * 2 - 3, 2) == 64); // integer: 2^65-1
        REQUIRE(Int::log(positive * 2 - 2, 2) == 65); // integer: 2^65
        REQUIRE(Int::log(positive * 2, 2) == 65);     // integer: 2^65+2

        REQUIRE(Int::log("123456789000", 233) == 4); // 4.6851911360933745
    }

    SECTION("gcd_lcm")
    {
        // gcd()
        REQUIRE(Int::gcd("0", "0") == "0");
        REQUIRE(Int::gcd("0", "1") == "1");
        REQUIRE(Int::gcd("1", "0") == "1");
        REQUIRE(Int::gcd("1", "1") == "1");

        REQUIRE(Int::gcd("6", "8") == "2");
        REQUIRE(Int::gcd("24", "48") == "24");
        REQUIRE(Int::gcd("37", "48") == "1");
        REQUIRE(Int::gcd("12345", "54321") == "3");

        // lcm()
        REQUIRE(Int::lcm("0", "0") == "0");
        REQUIRE(Int::lcm("0", "1") == "0");
        REQUIRE(Int::lcm("1", "0") == "0");
        REQUIRE(Int::lcm("1", "1") == "1");

        REQUIRE(Int::lcm("6", "8") == "24");
        REQUIRE(Int::lcm("24", "48") == "48");
        REQUIRE(Int::lcm("37", "48") == "1776");
        REQUIRE(Int::lcm("12345", "54321") == "223530915");
    }

    SECTION("random")
    {
        REQUIRE_THROWS_MATCHES(Int::random(-2), std::runtime_error, Message("Error: `digits` must be a non-negative integer or default = -1."));

        REQUIRE(Int::random(0) == 0);

        REQUIRE(Int::random(1).digits() == 1);
        REQUIRE(Int::random(2).digits() == 2);
        REQUIRE(Int::random(3).digits() == 3);

        for (int i = 0; i < 100; ++i)
        {
            REQUIRE(Int::random().digits() <= 4300);
        }
    }

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

    SECTION("input")
    {
        Int int1, int2, int3, int4;
        std::istringstream("+123\n-456\t789 0") >> int1 >> int2 >> int3 >> int4;

        REQUIRE(int1 == Int("123"));
        REQUIRE(int2 == Int("-456"));
        REQUIRE(int3 == Int("789"));
        REQUIRE(int4 == Int("0"));
    }
}
