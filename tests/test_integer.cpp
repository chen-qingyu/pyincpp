#include "../sources/Integer.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor
TEST_CASE("Integer: basics")
{
    // Integer()
    Integer int1;

    // Integer(const char* chars)
    Integer int2("123456789000");
    MY_ASSERT_THROW_MESSAGE(Integer("hello"), std::runtime_error, "Error: Wrong integer literal.");

    // Integer(const String& str)
    Integer int3(String("123456789000"));
    MY_ASSERT_THROW_MESSAGE(Integer(String("hello")), std::runtime_error, "Error: Wrong integer literal.");

    // Integer(int integer)
    Integer int4 = 123456789;

    // Integer(const Integer &that)
    Integer int5(int4);

    // Integer(Integer &&that)
    Integer int6(std::move(int5));

    // ~Integer()
}

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST_CASE("Integer: compare")
{
    Integer pos_integer("12345");
    Integer neg_integer("-12345");
    Integer zero("0");

    // operator==
    Integer eq_pos_integer("12345");
    Integer eq_neg_integer("-12345");
    REQUIRE(eq_pos_integer == pos_integer);
    REQUIRE(eq_neg_integer == neg_integer);

    // operator!=
    Integer ne_pos_integer("54321");
    REQUIRE(ne_pos_integer != pos_integer);
    REQUIRE(ne_pos_integer != zero);
    REQUIRE(ne_pos_integer != neg_integer);

    // operator<
    Integer lt_pos_integer("12344");
    REQUIRE(lt_pos_integer < pos_integer);
    REQUIRE(neg_integer < pos_integer);
    REQUIRE(zero < pos_integer);

    // operator<=
    REQUIRE(lt_pos_integer <= pos_integer);
    REQUIRE(eq_pos_integer <= pos_integer);
    REQUIRE(neg_integer <= pos_integer);
    REQUIRE(zero <= pos_integer);

    // operator>
    Integer gt_pos_integer("12346");
    REQUIRE(gt_pos_integer > pos_integer);
    REQUIRE(pos_integer > neg_integer);
    REQUIRE(pos_integer > zero);
    REQUIRE(zero > neg_integer);

    // operator>=
    REQUIRE(eq_pos_integer >= pos_integer);
    REQUIRE(gt_pos_integer >= pos_integer);
    REQUIRE(pos_integer >= neg_integer);
    REQUIRE(pos_integer >= zero);
}

// operator=()
TEST_CASE("Integer: copy_assignment")
{
    Integer integer1("12345");
    Integer integer2("54321");

    integer1 = integer2;
    REQUIRE(integer1 == Integer("54321"));
    REQUIRE(integer2 == Integer("54321"));
}

// operator=()
TEST_CASE("Integer: move_assignment")
{
    Integer integer1("12345");
    Integer integer2("54321");

    integer1 = std::move(integer2);
    REQUIRE(integer1 == Integer("54321"));
    REQUIRE(integer2 == Integer());
}

// digits() is_zero() is_positive() is_negative() is_even() is_odd()
TEST_CASE("Integer: examination")
{
    // digits()
    REQUIRE(Integer("0").digits() == 0);
    REQUIRE(Integer("1").digits() == 1);
    REQUIRE(Integer("-1").digits() == 1);
    REQUIRE(Integer("123456789").digits() == 9);
    REQUIRE(Integer("-123456789").digits() == 9);

    // is_zero()
    REQUIRE(Integer("0").is_zero());
    REQUIRE(!Integer("1").is_zero());
    REQUIRE(!Integer("-1").is_zero());
    REQUIRE(!Integer("123456789").is_zero());
    REQUIRE(!Integer("-123456789").is_zero());

    // is_positive()
    REQUIRE(!Integer("0").is_positive());
    REQUIRE(Integer("1").is_positive());
    REQUIRE(!Integer("-1").is_positive());
    REQUIRE(Integer("123456789").is_positive());
    REQUIRE(!Integer("-123456789").is_positive());

    // is_negative()
    REQUIRE(!Integer("0").is_negative());
    REQUIRE(!Integer("1").is_negative());
    REQUIRE(Integer("-1").is_negative());
    REQUIRE(!Integer("123456789").is_negative());
    REQUIRE(Integer("-123456789").is_negative());

    // is_even()
    REQUIRE(Integer("0").is_even());
    REQUIRE(!Integer("1").is_even());
    REQUIRE(!Integer("-1").is_even());
    REQUIRE(!Integer("123456789").is_even());
    REQUIRE(!Integer("-123456789").is_even());

    // is_odd()
    REQUIRE(!Integer("0").is_odd());
    REQUIRE(Integer("1").is_odd());
    REQUIRE(Integer("-1").is_odd());
    REQUIRE(Integer("123456789").is_odd());
    REQUIRE(Integer("-123456789").is_odd());
}

// operator++() operator--()
TEST_CASE("Integer: inc_dec")
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
TEST_CASE("Integer: plus")
{
    // pos + pos
    REQUIRE(Integer("18446744073709551616") + Integer("18446744073709551616") == "36893488147419103232");
    REQUIRE(Integer("18446744073709551616") + Integer("1") == "18446744073709551617");
    REQUIRE(Integer("1") + Integer("18446744073709551616") == "18446744073709551617");

    // pos + zero
    REQUIRE(Integer("18446744073709551616") + Integer("0") == "18446744073709551616");

    // pos + neg
    REQUIRE(Integer("18446744073709551616") + Integer("-18446744073709551616") == "0");
    REQUIRE(Integer("18446744073709551616") + Integer("-1") == "18446744073709551615");
    REQUIRE(Integer("1") + Integer("-18446744073709551616") == "-18446744073709551615");

    // neg + pos
    REQUIRE(Integer("-18446744073709551616") + Integer("18446744073709551616") == "0");
    REQUIRE(Integer("-18446744073709551616") + Integer("1") == "-18446744073709551615");
    REQUIRE(Integer("-1") + Integer("18446744073709551616") == "18446744073709551615");

    // neg + zero
    REQUIRE(Integer("-18446744073709551616") + Integer("0") == "-18446744073709551616");

    // neg + neg
    REQUIRE(Integer("-18446744073709551616") + Integer("-18446744073709551616") == "-36893488147419103232");
    REQUIRE(Integer("-18446744073709551616") + Integer("-1") == "-18446744073709551617");
    REQUIRE(Integer("-1") + Integer("-18446744073709551616") == "-18446744073709551617");

    // zero + pos
    REQUIRE(Integer("0") + Integer("18446744073709551616") == "18446744073709551616");

    // zero + zero
    REQUIRE(Integer("0") + Integer("0") == "0");

    // zero + neg
    REQUIRE(Integer("0") + Integer("-18446744073709551616") == "-18446744073709551616");
}

// operator-()
TEST_CASE("Integer: minus")
{
    // pos - pos
    REQUIRE(Integer("18446744073709551616") - Integer("18446744073709551616") == "0");
    REQUIRE(Integer("18446744073709551616") - Integer("1") == "18446744073709551615");
    REQUIRE(Integer("1") - Integer("18446744073709551616") == "-18446744073709551615");

    // pos - zero
    REQUIRE(Integer("18446744073709551616") - Integer("0") == "18446744073709551616");

    // pos - neg
    REQUIRE(Integer("18446744073709551616") - Integer("-18446744073709551616") == "36893488147419103232");
    REQUIRE(Integer("18446744073709551616") - Integer("-1") == "18446744073709551617");
    REQUIRE(Integer("1") - Integer("-18446744073709551616") == "18446744073709551617");

    // neg - pos
    REQUIRE(Integer("-18446744073709551616") - Integer("18446744073709551616") == "-36893488147419103232");
    REQUIRE(Integer("-18446744073709551616") - Integer("1") == "-18446744073709551617");
    REQUIRE(Integer("-1") - Integer("18446744073709551616") == "-18446744073709551617");

    // neg - zero
    REQUIRE(Integer("-18446744073709551616") - Integer("0") == "-18446744073709551616");

    // neg - neg
    REQUIRE(Integer("-18446744073709551616") - Integer("-18446744073709551616") == "0");
    REQUIRE(Integer("-18446744073709551616") - Integer("-1") == "-18446744073709551615");
    REQUIRE(Integer("-1") - Integer("-18446744073709551616") == "18446744073709551615");

    // zero - pos
    REQUIRE(Integer("0") - Integer("18446744073709551616") == "-18446744073709551616");

    // zero - zero
    REQUIRE(Integer("0") - Integer("0") == "0");

    // zero - neg
    REQUIRE(Integer("0") - Integer("-18446744073709551616") == "18446744073709551616");
}

// operator*()
TEST_CASE("Integer: times")
{
    // pos * pos
    REQUIRE(Integer("18446744073709551616") * Integer("1") == "18446744073709551616");
    REQUIRE(Integer("1") * Integer("18446744073709551616") == "18446744073709551616");
    REQUIRE(Integer("18446744073709551616") * Integer("18446744073709551616") == "340282366920938463463374607431768211456");

    // pos * zero
    REQUIRE(Integer("18446744073709551616") * Integer("0") == "0");

    // pos * neg
    REQUIRE(Integer("18446744073709551616") * Integer("-1") == "-18446744073709551616");
    REQUIRE(Integer("1") * Integer("-18446744073709551616") == "-18446744073709551616");
    REQUIRE(Integer("18446744073709551616") * Integer("-18446744073709551616") == "-340282366920938463463374607431768211456");

    // neg * pos
    REQUIRE(Integer("-18446744073709551616") * Integer("1") == "-18446744073709551616");
    REQUIRE(Integer("-1") * Integer("18446744073709551616") == "-18446744073709551616");
    REQUIRE(Integer("-18446744073709551616") * Integer("18446744073709551616") == "-340282366920938463463374607431768211456");

    // neg * zero
    REQUIRE(Integer("-18446744073709551616") * Integer("0") == "0");

    // neg * neg
    REQUIRE(Integer("-18446744073709551616") * Integer("-1") == "18446744073709551616");
    REQUIRE(Integer("-1") * Integer("-18446744073709551616") == "18446744073709551616");
    REQUIRE(Integer("-18446744073709551616") * Integer("-18446744073709551616") == "340282366920938463463374607431768211456");

    // zero * pos
    REQUIRE(Integer("0") * Integer("18446744073709551616") == "0");

    // zero * zero
    REQUIRE(Integer("0") * Integer("0") == "0");

    // zero * neg
    REQUIRE(Integer("0") * Integer("-18446744073709551616") == "0");
}

// operator/()
TEST_CASE("Integer: divide")
{
    // pos / pos
    REQUIRE(Integer("18446744073709551616") / Integer("18446744073709551616") == "1");
    REQUIRE(Integer("36893488147419103232") / Integer("2") == "18446744073709551616");
    REQUIRE(Integer("2") / Integer("36893488147419103232") == "0");

    // pos / zero
    MY_ASSERT_THROW_MESSAGE(Integer("18446744073709551616") / Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // pos / neg
    REQUIRE(Integer("18446744073709551616") / Integer("-18446744073709551616") == "-1");
    REQUIRE(Integer("36893488147419103232") / Integer("-2") == "-18446744073709551616");
    REQUIRE(Integer("2") / Integer("-36893488147419103232") == "0");

    // neg / pos
    REQUIRE(Integer("-18446744073709551616") / Integer("18446744073709551616") == "-1");
    REQUIRE(Integer("-36893488147419103232") / Integer("2") == "-18446744073709551616");
    REQUIRE(Integer("-2") / Integer("36893488147419103232") == "0");

    // neg / zero
    MY_ASSERT_THROW_MESSAGE(Integer("-18446744073709551616") / Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // neg / neg
    REQUIRE(Integer("-18446744073709551616") / Integer("-18446744073709551616") == "1");
    REQUIRE(Integer("-36893488147419103232") / Integer("-2") == "18446744073709551616");
    REQUIRE(Integer("-2") / Integer("-36893488147419103232") == "0");

    // zero / pos
    REQUIRE(Integer("0") / Integer("18446744073709551616") == "0");

    // zero / zero
    MY_ASSERT_THROW_MESSAGE(Integer("0") / Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // zero / neg
    REQUIRE(Integer("0") / Integer("-18446744073709551616") == "0");
}

// operator%()
TEST_CASE("Integer: mod")
{
    // pos % pos
    REQUIRE(Integer("18446744073709551616") % Integer("18446744073709551616") == "0");
    REQUIRE(Integer("36893488147419103232") % Integer("2") == "0");
    REQUIRE(Integer("2") % Integer("36893488147419103232") == "2");

    // pos % zero
    MY_ASSERT_THROW_MESSAGE(Integer("18446744073709551616") % Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // pos % neg
    REQUIRE(Integer("18446744073709551616") % Integer("-18446744073709551616") == "0");
    REQUIRE(Integer("36893488147419103232") % Integer("-2") == "0");
    REQUIRE(Integer("2") % Integer("-36893488147419103232") == "2");

    // neg % pos
    REQUIRE(Integer("-18446744073709551616") % Integer("18446744073709551616") == "0");
    REQUIRE(Integer("-36893488147419103232") % Integer("2") == "0");
    REQUIRE(Integer("-2") % Integer("36893488147419103232") == "-2");

    // neg % zero
    MY_ASSERT_THROW_MESSAGE(Integer("-18446744073709551616") % Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // neg % neg
    REQUIRE(Integer("-18446744073709551616") % Integer("-18446744073709551616") == "0");
    REQUIRE(Integer("-36893488147419103232") % Integer("-2") == "0");
    REQUIRE(Integer("-2") % Integer("-36893488147419103232") == "-2");

    // zero % pos
    REQUIRE(Integer("0") % Integer("18446744073709551616") == "0");

    // zero % zero
    MY_ASSERT_THROW_MESSAGE(Integer("0") % Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // zero % neg
    REQUIRE(Integer("0") % Integer("-18446744073709551616") == "0");
}

// pow()
TEST_CASE("Integer: pow")
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
TEST_CASE("Integer: factorial")
{
    // (negative)! throws exception
    MY_ASSERT_THROW_MESSAGE(Integer("-1").factorial(), std::runtime_error, "Error: Negative integer have no factorial.");

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
TEST_CASE("Integer: gcd_lcm")
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
TEST_CASE("Integer: sqrt")
{
    MY_ASSERT_THROW_MESSAGE(Integer("-1").sqrt(), std::runtime_error, "Error: Cannot compute square root of a negative integer.");

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
TEST_CASE("Integer: to_string")
{
    REQUIRE(Integer("0").to_string() == "0");
    REQUIRE(Integer("2333").to_string() == "2333");
}

// operator<<()
TEST_CASE("Integer: print")
{
    std::ostringstream oss;

    Integer zero;
    oss << zero;
    REQUIRE(oss.str() == "0");
    oss.str("");

    Integer pos = "123456789000";
    oss << pos;
    REQUIRE(oss.str() == "123456789000");
    oss.str("");

    Integer neg = "-987654321000";
    oss << neg;
    REQUIRE(oss.str() == "-987654321000");
    oss.str("");
}

// operator>>()
TEST_CASE("Integer: input")
{
    Integer zero;
    std::istringstream("0") >> zero;
    REQUIRE(zero == Integer("0"));

    Integer one;
    std::istringstream("1") >> one;
    REQUIRE(one == Integer("1"));

    Integer pos;
    std::istringstream("+123456789000") >> pos;
    REQUIRE(pos == Integer("123456789000"));

    Integer neg;
    std::istringstream("-123456789000") >> neg;
    REQUIRE(neg == Integer("-123456789000"));

    Integer int1, int2, int3;
    std::istringstream("+123\n-456\t789") >> int1 >> int2 >> int3;
    REQUIRE(int1 == Integer("123"));
    REQUIRE(int2 == Integer("-456"));
    REQUIRE(int3 == Integer("789"));
}
