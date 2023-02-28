// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream std::istringstream

#include "../Sources/Integer.hpp"

#include "my_tools.hpp"

using namespace mdspp;

// constructor destructor digits()
TEST(Integer, basics)
{
    // Integer()
    Integer int1;
    ASSERT_EQ(int1.digits(), 0);

    // Integer(const String& str)
    Integer int2 = "123456789000";
    ASSERT_EQ(int2.digits(), 12);

    // Integer(long long integer)
    Integer int3 = 123456789000LL;
    ASSERT_EQ(int3.digits(), 12);

    // Integer(const Integer &that)
    Integer int4(int3);
    ASSERT_EQ(int4.digits(), 12);

    // Integer(Integer &&that)
    Integer int5(std::move(int4));
    ASSERT_EQ(int5.digits(), 12);
    ASSERT_EQ(int4.digits(), 0);

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

// operator+()
TEST(Integer, plus)
{
    // +pos
    ASSERT_EQ(+Integer("18446744073709551616"), Integer("18446744073709551616"));

    // pos + pos (2^64 + 2^64)
    ASSERT_EQ(Integer("18446744073709551616") + Integer("18446744073709551616"), Integer("36893488147419103232"));

    // pos + zero
    ASSERT_EQ(Integer("18446744073709551616") + Integer("0"), Integer("18446744073709551616"));

    // pos + neg
    ASSERT_EQ(Integer("18446744073709551616") + Integer("-18446744073709551616"), Integer("0"));

    // neg + pos
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("18446744073709551616"), Integer("0"));

    // neg + zero
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("0"), Integer("-18446744073709551616"));

    // neg + neg
    ASSERT_EQ(Integer("-18446744073709551616") + Integer("-18446744073709551616"), Integer("-36893488147419103232"));

    // zero + pos
    ASSERT_EQ(Integer("0") + Integer("18446744073709551616"), Integer("18446744073709551616"));

    // zero + zero
    ASSERT_EQ(Integer("0") + Integer("0"), Integer("0"));

    // zero + neg
    ASSERT_EQ(Integer("0") + Integer("-18446744073709551616"), Integer("-18446744073709551616"));

    // more tests
    ASSERT_EQ(Integer("1") + Integer("18446744073709551616"), Integer("18446744073709551617"));
    ASSERT_EQ(Integer("99999999") + Integer("1"), Integer("100000000"));
}

// operator-()
TEST(Integer, minus)
{
    // -pos
    ASSERT_EQ(-Integer("18446744073709551616"), Integer("-18446744073709551616"));

    // pos - pos (2^64 - 2^64)
    ASSERT_EQ(Integer("18446744073709551616") - Integer("18446744073709551616"), Integer("0"));

    // pos - zero
    ASSERT_EQ(Integer("18446744073709551616") - Integer("0"), Integer("18446744073709551616"));

    // pos - neg
    ASSERT_EQ(Integer("18446744073709551616") - Integer("-18446744073709551616"), Integer("36893488147419103232"));

    // neg - pos
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("18446744073709551616"), Integer("-36893488147419103232"));

    // neg - zero
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("0"), Integer("-18446744073709551616"));

    // neg - neg
    ASSERT_EQ(Integer("-18446744073709551616") - Integer("-18446744073709551616"), Integer("0"));
    ASSERT_EQ(Integer("-2") - Integer("-1"), Integer("-1"));
    ASSERT_EQ(Integer("-1") - Integer("-2"), Integer("1"));

    // zero - pos
    ASSERT_EQ(Integer("0") - Integer("18446744073709551616"), Integer("-18446744073709551616"));

    // zero - zero
    ASSERT_EQ(Integer("0") - Integer("0"), Integer("0"));

    // zero - neg
    ASSERT_EQ(Integer("0") - Integer("-18446744073709551616"), Integer("18446744073709551616"));

    // more tests
    ASSERT_EQ(Integer("1") - Integer("18446744073709551616"), Integer("-18446744073709551615"));
    ASSERT_EQ(Integer("100000000") - Integer("1"), Integer("99999999"));
}

// operator*()
TEST(Integer, times)
{
    // pos * pos (2^64 * 2)
    ASSERT_EQ(Integer("18446744073709551616") * Integer("2"), Integer("36893488147419103232"));

    // pos * zero
    ASSERT_EQ(Integer("18446744073709551616") * Integer("0"), Integer("0"));

    // pos * neg
    ASSERT_EQ(Integer("18446744073709551616") * Integer("-2"), Integer("-36893488147419103232"));

    // neg * pos
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("2"), Integer("-36893488147419103232"));

    // neg * zero
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("0"), Integer("0"));

    // neg * neg
    ASSERT_EQ(Integer("-18446744073709551616") * Integer("-2"), Integer("36893488147419103232"));

    // zero * pos
    ASSERT_EQ(Integer("0") * Integer("18446744073709551616"), Integer("0"));

    // zero * zero
    ASSERT_EQ(Integer("0") * Integer("0"), Integer("0"));

    // zero * neg
    ASSERT_EQ(Integer("0") * Integer("-18446744073709551616"), Integer("0"));

    // more tests
    ASSERT_EQ(Integer("1") * Integer("18446744073709551616"), Integer("18446744073709551616"));
    ASSERT_EQ(Integer("10000") * Integer("10000"), Integer("100000000"));
}

// operator/()
TEST(Integer, divide)
{
    // pos / pos (2^64 / 2^64)
    ASSERT_EQ(Integer("18446744073709551616") / Integer("18446744073709551616"), Integer("1"));
    ASSERT_EQ(Integer("36893488147419103232") / Integer("2"), Integer("18446744073709551616"));

    // pos / zero
    MY_ASSERT_THROWS_MESSAGE(Integer("18446744073709551616") / Integer("0"), std::runtime_error, "ERROR: Divide by zero.");

    // pos / neg
    ASSERT_EQ(Integer("18446744073709551616") / Integer("-18446744073709551616"), Integer("-1"));
    ASSERT_EQ(Integer("36893488147419103232") / Integer("-2"), Integer("-18446744073709551616"));

    // neg / pos
    ASSERT_EQ(Integer("-18446744073709551616") / Integer("18446744073709551616"), Integer("-1"));
    ASSERT_EQ(Integer("-36893488147419103232") / Integer("2"), Integer("-18446744073709551616"));

    // neg / zero
    MY_ASSERT_THROWS_MESSAGE(Integer("-18446744073709551616") / Integer("0"), std::runtime_error, "ERROR: Divide by zero.");

    // neg / neg
    ASSERT_EQ(Integer("-18446744073709551616") / Integer("-18446744073709551616"), Integer("1"));
    ASSERT_EQ(Integer("-36893488147419103232") / Integer("-2"), Integer("18446744073709551616"));

    // zero / pos
    ASSERT_EQ(Integer("0") / Integer("18446744073709551616"), Integer("0"));

    // zero / zero
    MY_ASSERT_THROWS_MESSAGE(Integer("0") / Integer("0"), std::runtime_error, "ERROR: Divide by zero.");

    // zero / neg
    ASSERT_EQ(Integer("0") / Integer("-18446744073709551616"), Integer("0"));

    // more tests
    ASSERT_EQ(Integer("10000") / Integer("2000"), Integer("5"));
    ASSERT_EQ(Integer("2000") / Integer("10000"), Integer("0"));
    ASSERT_EQ(Integer("12345") / Integer("11"), Integer("1122"));
}

// operator%()
TEST(Integer, mod)
{
    ASSERT_EQ(Integer("3") % Integer("2"), Integer("1"));
    ASSERT_EQ(Integer("3") % Integer("1"), Integer("0"));
    ASSERT_EQ(Integer("2") % Integer("3"), Integer("2"));
    ASSERT_EQ(Integer("100") % Integer("3"), Integer("1"));
}

// pow()
TEST(Integer, pow)
{
    ASSERT_EQ(Integer("3").pow(Integer("2")), Integer("9"));
    ASSERT_EQ(Integer("3").pow(Integer("3")), Integer("27"));
    ASSERT_EQ(Integer("2").pow(Integer("3")), Integer("8"));
    ASSERT_EQ(Integer("1").pow(Integer("1")), Integer("1"));
    ASSERT_EQ(Integer("1").pow(Integer("0")), Integer("1"));
    ASSERT_EQ(Integer("0").pow(Integer("1")), Integer("0"));
}

// factorial()
TEST(Integer, factorial)
{
    ASSERT_EQ(Integer("0").factorial(), Integer("1"));
    ASSERT_EQ(Integer("1").factorial(), Integer("1"));
    ASSERT_EQ(Integer("2").factorial(), Integer("2"));
    ASSERT_EQ(Integer("3").factorial(), Integer("6"));
    ASSERT_EQ(Integer("100").factorial(), Integer("93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000"));
}

// operator<<()
TEST(Integer, print)
{
    std::ostringstream oss;

    Integer zero;
    oss << zero;
    ASSERT_EQ(oss.str(), "0"); // string == char*, use eq
    oss.str("");

    Integer pos = 123456789000LL;
    oss << pos;
    ASSERT_EQ(oss.str(), "123456789000");
    oss.str("");

    Integer neg = -987654321000LL;
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
    std::istringstream("123456789000") >> pos;
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
