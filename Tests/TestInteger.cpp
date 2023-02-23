// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/Integer.hpp"

using namespace mdspp;

// constructor destructor digits() is_zero()
TEST(Integer, basics)
{
    // Integer()
    Integer int1;
    ASSERT_EQ(int1.digits(), 0);
    ASSERT_TRUE(int1.is_zero());

    // Integer(const String& str)
    Integer int2 = "123456789000";
    ASSERT_EQ(int2.digits(), 12);
    ASSERT_FALSE(int2.is_zero());

    // Integer(long long integer)
    Integer int3 = 123456789000LL;
    ASSERT_EQ(int3.digits(), 12);
    ASSERT_FALSE(int3.is_zero());

    // Integer(const Integer &that)
    Integer int4(int3);
    ASSERT_EQ(int4.digits(), 12);
    ASSERT_FALSE(int4.is_zero());

    // Integer(Integer &&that)
    Integer int5(std::move(int4));
    ASSERT_EQ(int5.digits(), 12);
    ASSERT_FALSE(int5.is_zero());
    ASSERT_EQ(int4.digits(), 0);
    ASSERT_TRUE(int4.is_zero());

    // ~Integer()
}

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST(Integer, compare)
{
    Integer integer("12345");

    // operator==
    Integer eq_integer("12345");
    ASSERT_TRUE(eq_integer == integer);

    // operator!=
    Integer ne_integer("54321");
    ASSERT_TRUE(ne_integer != integer);

    // operator<
    Integer lt_integer("12344");
    ASSERT_TRUE(lt_integer < integer);

    // operator<=
    ASSERT_TRUE(lt_integer <= integer);
    ASSERT_TRUE(eq_integer <= integer);

    // operator>
    Integer gt_integer("12346");
    ASSERT_TRUE(gt_integer > integer);

    // operator>=
    ASSERT_TRUE(eq_integer >= integer);
    ASSERT_TRUE(gt_integer >= integer);
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

    // one more test
    ASSERT_EQ(Integer("1") + Integer("18446744073709551616"), Integer("18446744073709551617"));
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

    // zero - pos
    ASSERT_EQ(Integer("0") - Integer("18446744073709551616"), Integer("-18446744073709551616"));

    // zero - zero
    ASSERT_EQ(Integer("0") - Integer("0"), Integer("0"));

    // zero - neg
    ASSERT_EQ(Integer("0") - Integer("-18446744073709551616"), Integer("18446744073709551616"));

    // one more test
    ASSERT_EQ(Integer("1") - Integer("18446744073709551616"), Integer("-18446744073709551615"));
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

    // one more test
    ASSERT_EQ(Integer("1") * Integer("18446744073709551616"), Integer("18446744073709551616"));
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
