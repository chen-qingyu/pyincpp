#include "../sources/Integer.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor
TEST(Integer, basics)
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
TEST(Integer, compare)
{
    Integer pos_integer("12345");
    Integer neg_integer("-12345");
    Integer zero("0");

    // operator==
    Integer eq_pos_integer("12345");
    Integer eq_neg_integer("-12345");
    ASSERT_TRUE(eq_pos_integer == pos_integer);
    ASSERT_TRUE(eq_neg_integer == neg_integer);

    // operator!=
    Integer ne_pos_integer("54321");
    ASSERT_TRUE(ne_pos_integer != pos_integer);
    ASSERT_TRUE(ne_pos_integer != zero);
    ASSERT_TRUE(ne_pos_integer != neg_integer);

    // operator<
    Integer lt_pos_integer("12344");
    ASSERT_TRUE(lt_pos_integer < pos_integer);
    ASSERT_TRUE(neg_integer < pos_integer);
    ASSERT_TRUE(zero < pos_integer);

    // operator<=
    ASSERT_TRUE(lt_pos_integer <= pos_integer);
    ASSERT_TRUE(eq_pos_integer <= pos_integer);
    ASSERT_TRUE(neg_integer <= pos_integer);
    ASSERT_TRUE(zero <= pos_integer);

    // operator>
    Integer gt_pos_integer("12346");
    ASSERT_TRUE(gt_pos_integer > pos_integer);
    ASSERT_TRUE(pos_integer > neg_integer);
    ASSERT_TRUE(pos_integer > zero);

    ASSERT_TRUE(zero > neg_integer);

    // operator>=
    ASSERT_TRUE(eq_pos_integer >= pos_integer);
    ASSERT_TRUE(gt_pos_integer >= pos_integer);
    ASSERT_TRUE(pos_integer >= neg_integer);
    ASSERT_TRUE(pos_integer >= zero);
}

// operator=()
TEST(Integer, copy_assignment)
{
    Integer integer1("12345");
    Integer integer2("54321");

    integer1 = integer2;
    ASSERT_EQ(integer1, Integer("54321"));
    ASSERT_EQ(integer2, Integer("54321"));
}

// operator=()
TEST(Integer, move_assignment)
{
    Integer integer1("12345");
    Integer integer2("54321");

    integer1 = std::move(integer2);
    ASSERT_EQ(integer1, Integer("54321"));
    ASSERT_EQ(integer2, Integer());
}

// digits() is_zero() is_positive() is_negative() is_even() is_odd()
TEST(Integer, examination)
{
    // digits()
    ASSERT_EQ(Integer("0").digits(), 0);
    ASSERT_EQ(Integer("1").digits(), 1);
    ASSERT_EQ(Integer("-1").digits(), 1);
    ASSERT_EQ(Integer("123456789").digits(), 9);
    ASSERT_EQ(Integer("-123456789").digits(), 9);

    // is_zero()
    ASSERT_TRUE(Integer("0").is_zero());
    ASSERT_FALSE(Integer("1").is_zero());
    ASSERT_FALSE(Integer("-1").is_zero());
    ASSERT_FALSE(Integer("123456789").is_zero());
    ASSERT_FALSE(Integer("-123456789").is_zero());

    // is_positive()
    ASSERT_FALSE(Integer("0").is_positive());
    ASSERT_TRUE(Integer("1").is_positive());
    ASSERT_FALSE(Integer("-1").is_positive());
    ASSERT_TRUE(Integer("123456789").is_positive());
    ASSERT_FALSE(Integer("-123456789").is_positive());

    // is_negative()
    ASSERT_FALSE(Integer("0").is_negative());
    ASSERT_FALSE(Integer("1").is_negative());
    ASSERT_TRUE(Integer("-1").is_negative());
    ASSERT_FALSE(Integer("123456789").is_negative());
    ASSERT_TRUE(Integer("-123456789").is_negative());

    // is_even()
    ASSERT_TRUE(Integer("0").is_even());
    ASSERT_FALSE(Integer("1").is_even());
    ASSERT_FALSE(Integer("-1").is_even());
    ASSERT_FALSE(Integer("123456789").is_even());
    ASSERT_FALSE(Integer("-123456789").is_even());

    // is_odd()
    ASSERT_FALSE(Integer("0").is_odd());
    ASSERT_TRUE(Integer("1").is_odd());
    ASSERT_TRUE(Integer("-1").is_odd());
    ASSERT_TRUE(Integer("123456789").is_odd());
    ASSERT_TRUE(Integer("-123456789").is_odd());
}

// operator++() operator--()
TEST(Integer, inc_dec)
{
    // operator++()
    ASSERT_EQ(++Integer("-1"), "0");
    ASSERT_EQ(++Integer("0"), "1");
    ASSERT_EQ(++Integer("1"), "2");
    ASSERT_EQ(++Integer("99999999999999"), "100000000000000");

    // operator--()
    ASSERT_EQ(--Integer("-1"), "-2");
    ASSERT_EQ(--Integer("0"), "-1");
    ASSERT_EQ(--Integer("1"), "0");
    ASSERT_EQ(--Integer("100000000000000"), "99999999999999");
}

// operator+()
TEST(Integer, plus)
{
    // pos + pos
    ASSERT_EQ(Integer("18446744073709551616") + Integer("18446744073709551616"), "36893488147419103232");
    ASSERT_EQ(Integer("18446744073709551616") + Integer("1"), "18446744073709551617");
    ASSERT_EQ(Integer("1") + Integer("18446744073709551616"), "18446744073709551617");

    // pos + zero
    ASSERT_EQ(Integer("18446744073709551616") + Integer("0"), "18446744073709551616");

    // pos + neg
    ASSERT_EQ(Integer("18446744073709551616") + Integer("-18446744073709551616"), "0");
    ASSERT_EQ(Integer("18446744073709551616") + Integer("-1"), "18446744073709551615");
    ASSERT_EQ(Integer("1") + Integer("-18446744073709551616"), "-18446744073709551615");

    // neg + pos
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("18446744073709551616"), "0");
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("1"), "-18446744073709551615");
    ASSERT_EQ(Integer("-1") + Integer("18446744073709551616"), "18446744073709551615");

    // neg + zero
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("0"), "-18446744073709551616");

    // neg + neg
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("-18446744073709551616"), "-36893488147419103232");
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("-1"), "-18446744073709551617");
    ASSERT_EQ(Integer("-1") + Integer("-18446744073709551616"), "-18446744073709551617");

    // zero + pos
    ASSERT_EQ(Integer("0") + Integer("18446744073709551616"), "18446744073709551616");

    // zero + zero
    ASSERT_EQ(Integer("0") + Integer("0"), "0");

    // zero + neg
    ASSERT_EQ(Integer("0") + Integer("-18446744073709551616"), "-18446744073709551616");
}

// operator-()
TEST(Integer, minus)
{
    // pos - pos
    ASSERT_EQ(Integer("18446744073709551616") - Integer("18446744073709551616"), "0");
    ASSERT_EQ(Integer("18446744073709551616") - Integer("1"), "18446744073709551615");
    ASSERT_EQ(Integer("1") - Integer("18446744073709551616"), "-18446744073709551615");

    // pos - zero
    ASSERT_EQ(Integer("18446744073709551616") - Integer("0"), "18446744073709551616");

    // pos - neg
    ASSERT_EQ(Integer("18446744073709551616") - Integer("-18446744073709551616"), "36893488147419103232");
    ASSERT_EQ(Integer("18446744073709551616") - Integer("-1"), "18446744073709551617");
    ASSERT_EQ(Integer("1") - Integer("-18446744073709551616"), "18446744073709551617");

    // neg - pos
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("18446744073709551616"), "-36893488147419103232");
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("1"), "-18446744073709551617");
    ASSERT_EQ(Integer("-1") - Integer("18446744073709551616"), "-18446744073709551617");

    // neg - zero
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("0"), "-18446744073709551616");

    // neg - neg
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("-18446744073709551616"), "0");
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("-1"), "-18446744073709551615");
    ASSERT_EQ(Integer("-1") - Integer("-18446744073709551616"), "18446744073709551615");

    // zero - pos
    ASSERT_EQ(Integer("0") - Integer("18446744073709551616"), "-18446744073709551616");

    // zero - zero
    ASSERT_EQ(Integer("0") - Integer("0"), "0");

    // zero - neg
    ASSERT_EQ(Integer("0") - Integer("-18446744073709551616"), "18446744073709551616");
}

// operator*()
TEST(Integer, times)
{
    // pos * pos
    ASSERT_EQ(Integer("18446744073709551616") * Integer("1"), "18446744073709551616");
    ASSERT_EQ(Integer("1") * Integer("18446744073709551616"), "18446744073709551616");
    ASSERT_EQ(Integer("18446744073709551616") * Integer("18446744073709551616"), "340282366920938463463374607431768211456");

    // pos * zero
    ASSERT_EQ(Integer("18446744073709551616") * Integer("0"), "0");

    // pos * neg
    ASSERT_EQ(Integer("18446744073709551616") * Integer("-1"), "-18446744073709551616");
    ASSERT_EQ(Integer("1") * Integer("-18446744073709551616"), "-18446744073709551616");
    ASSERT_EQ(Integer("18446744073709551616") * Integer("-18446744073709551616"), "-340282366920938463463374607431768211456");

    // neg * pos
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("1"), "-18446744073709551616");
    ASSERT_EQ(Integer("-1") * Integer("18446744073709551616"), "-18446744073709551616");
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("18446744073709551616"), "-340282366920938463463374607431768211456");

    // neg * zero
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("0"), "0");

    // neg * neg
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("-1"), "18446744073709551616");
    ASSERT_EQ(Integer("-1") * Integer("-18446744073709551616"), "18446744073709551616");
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("-18446744073709551616"), "340282366920938463463374607431768211456");

    // zero * pos
    ASSERT_EQ(Integer("0") * Integer("18446744073709551616"), "0");

    // zero * zero
    ASSERT_EQ(Integer("0") * Integer("0"), "0");

    // zero * neg
    ASSERT_EQ(Integer("0") * Integer("-18446744073709551616"), "0");
}

// operator/()
TEST(Integer, divide)
{
    // pos / pos
    ASSERT_EQ(Integer("18446744073709551616") / Integer("18446744073709551616"), "1");
    ASSERT_EQ(Integer("36893488147419103232") / Integer("2"), "18446744073709551616");
    ASSERT_EQ(Integer("2") / Integer("36893488147419103232"), "0");

    // pos / zero
    MY_ASSERT_THROW_MESSAGE(Integer("18446744073709551616") / Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // pos / neg
    ASSERT_EQ(Integer("18446744073709551616") / Integer("-18446744073709551616"), "-1");
    ASSERT_EQ(Integer("36893488147419103232") / Integer("-2"), "-18446744073709551616");
    ASSERT_EQ(Integer("2") / Integer("-36893488147419103232"), "0");

    // neg / pos
    ASSERT_EQ(Integer("-18446744073709551616") / Integer("18446744073709551616"), "-1");
    ASSERT_EQ(Integer("-36893488147419103232") / Integer("2"), "-18446744073709551616");
    ASSERT_EQ(Integer("-2") / Integer("36893488147419103232"), "0");

    // neg / zero
    MY_ASSERT_THROW_MESSAGE(Integer("-18446744073709551616") / Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // neg / neg
    ASSERT_EQ(Integer("-18446744073709551616") / Integer("-18446744073709551616"), "1");
    ASSERT_EQ(Integer("-36893488147419103232") / Integer("-2"), "18446744073709551616");
    ASSERT_EQ(Integer("-2") / Integer("-36893488147419103232"), "0");

    // zero / pos
    ASSERT_EQ(Integer("0") / Integer("18446744073709551616"), "0");

    // zero / zero
    MY_ASSERT_THROW_MESSAGE(Integer("0") / Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // zero / neg
    ASSERT_EQ(Integer("0") / Integer("-18446744073709551616"), "0");
}

// operator%()
TEST(Integer, mod)
{
    // pos % pos
    ASSERT_EQ(Integer("18446744073709551616") % Integer("18446744073709551616"), "0");
    ASSERT_EQ(Integer("36893488147419103232") % Integer("2"), "0");
    ASSERT_EQ(Integer("2") % Integer("36893488147419103232"), "2");

    // pos % zero
    MY_ASSERT_THROW_MESSAGE(Integer("18446744073709551616") % Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // pos % neg
    ASSERT_EQ(Integer("18446744073709551616") % Integer("-18446744073709551616"), "0");
    ASSERT_EQ(Integer("36893488147419103232") % Integer("-2"), "0");
    ASSERT_EQ(Integer("2") % Integer("-36893488147419103232"), "2");

    // neg % pos
    ASSERT_EQ(Integer("-18446744073709551616") % Integer("18446744073709551616"), "0");
    ASSERT_EQ(Integer("-36893488147419103232") % Integer("2"), "0");
    ASSERT_EQ(Integer("-2") % Integer("36893488147419103232"), "-2");

    // neg % zero
    MY_ASSERT_THROW_MESSAGE(Integer("-18446744073709551616") % Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // neg % neg
    ASSERT_EQ(Integer("-18446744073709551616") % Integer("-18446744073709551616"), "0");
    ASSERT_EQ(Integer("-36893488147419103232") % Integer("-2"), "0");
    ASSERT_EQ(Integer("-2") % Integer("-36893488147419103232"), "-2");

    // zero % pos
    ASSERT_EQ(Integer("0") % Integer("18446744073709551616"), "0");

    // zero % zero
    MY_ASSERT_THROW_MESSAGE(Integer("0") % Integer("0"), std::runtime_error, "Error: Divide by zero.");

    // zero % neg
    ASSERT_EQ(Integer("0") % Integer("-18446744073709551616"), "0");
}

// pow()
TEST(Integer, pow)
{
    // 0^0 == 1
    ASSERT_EQ(Integer("0").pow("0"), "1");

    // 0^1 == 0
    ASSERT_EQ(Integer("0").pow("1"), "0");

    // 1^0 == 1
    ASSERT_EQ(Integer("1").pow("0"), "1");

    // 1^1 == 1
    ASSERT_EQ(Integer("1").pow("1"), "1");

    // 2^3 == 8
    ASSERT_EQ(Integer("2").pow("3"), "8");

    // 2^100 == 1267650600228229401496703205376
    ASSERT_EQ(Integer("2").pow("100"), "1267650600228229401496703205376");

    // (9^9)^9 == 196627050475552913618075908526912116283103450944214766927315415537966391196809
    ASSERT_EQ(Integer("9").pow("9").pow("9"), "196627050475552913618075908526912116283103450944214766927315415537966391196809");

    // 1024^1024 % 100 == 76
    ASSERT_EQ(Integer("1024").pow("1024", "100"), "76");

    // 9999^1001 % 100 == 99
    ASSERT_EQ(Integer("9999").pow("1001", "100"), "99");
}

// factorial()
TEST(Integer, factorial)
{
    // (negative)! throws exception
    MY_ASSERT_THROW_MESSAGE(Integer("-1").factorial(), std::runtime_error, "Error: Negative integer have no factorial.");

    // 0! == 1
    ASSERT_EQ(Integer("0").factorial(), "1");

    // 1! == 1
    ASSERT_EQ(Integer("1").factorial(), "1");

    // 2! == 2
    ASSERT_EQ(Integer("2").factorial(), "2");

    // 3! == 6
    ASSERT_EQ(Integer("3").factorial(), "6");

    // 100! == 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
    ASSERT_EQ(Integer("100").factorial(), "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000");

    // (5!)! == 6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000
    ASSERT_EQ(Integer("5").factorial().factorial(), "6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000");
}

// gcd() lcm()
TEST(Integer, gcd_lcm)
{
    // gcd()
    ASSERT_EQ(gcd("0", "1"), "1");
    ASSERT_EQ(gcd("6", "12"), "6");
    ASSERT_EQ(gcd("6", "11"), "1");
    ASSERT_EQ(gcd("12345", "54321"), "3");

    // lcm()
    ASSERT_EQ(lcm("0", "1"), "0");
    ASSERT_EQ(lcm("6", "12"), "12");
    ASSERT_EQ(lcm("6", "11"), "66");
    ASSERT_EQ(lcm("12345", "54321"), "223530915");
}

// sqrt()
TEST(Integer, sqrt)
{
    MY_ASSERT_THROW_MESSAGE(Integer("-1").sqrt(), std::runtime_error, "Error: Cannot compute square root of a negative integer.");

    ASSERT_EQ(Integer("0").sqrt(), "0");
    ASSERT_EQ(Integer("1").sqrt(), "1");
    ASSERT_EQ(Integer("2").sqrt(), "1");
    ASSERT_EQ(Integer("3").sqrt(), "1");
    ASSERT_EQ(Integer("4").sqrt(), "2");
    ASSERT_EQ(Integer("5").sqrt(), "2");
    ASSERT_EQ(Integer("9").sqrt(), "3");
    ASSERT_EQ(Integer("9801").sqrt(), "99");
}

// to_string()
TEST(Integer, to_string)
{
    ASSERT_EQ(Integer("0").to_string(), "0");
    ASSERT_EQ(Integer("2333").to_string(), "2333");
}

// operator<<()
TEST(Integer, print)
{
    std::ostringstream oss;

    Integer zero;
    oss << zero;
    ASSERT_EQ(oss.str(), "0"); // string == char*, use eq
    oss.str("");

    Integer pos = "123456789000";
    oss << pos;
    ASSERT_EQ(oss.str(), "123456789000");
    oss.str("");

    Integer neg = "-987654321000";
    oss << neg;
    ASSERT_EQ(oss.str(), "-987654321000");
    oss.str("");
}

// operator>>()
TEST(Integer, input)
{
    Integer zero;
    std::istringstream("0") >> zero;
    ASSERT_EQ(zero, Integer("0"));

    Integer one;
    std::istringstream("1") >> one;
    ASSERT_EQ(one, Integer("1"));

    Integer pos;
    std::istringstream("+123456789000") >> pos;
    ASSERT_EQ(pos, Integer("123456789000"));

    Integer neg;
    std::istringstream("-123456789000") >> neg;
    ASSERT_EQ(neg, Integer("-123456789000"));

    Integer int1, int2, int3;
    std::istringstream("+123\n-456\t789") >> int1 >> int2 >> int3;
    ASSERT_EQ(int1, Integer("123"));
    ASSERT_EQ(int2, Integer("-456"));
    ASSERT_EQ(int3, Integer("789"));
}
