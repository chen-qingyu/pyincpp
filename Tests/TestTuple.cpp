// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/Tuple.hpp"

#include "my_tools.hpp"

using namespace mdspp;

// constructor destructor size()
TEST(Tuple, basics)
{
    // Tuple()
    Tuple<> tuple1;
    ASSERT_EQ(tuple1.size(), 0);

    // Tuple(int)
    Tuple<int> tuple2(1);
    ASSERT_EQ(tuple2.size(), 1);

    // Tuple(int, double)
    Tuple<int, double> tuple3(1, 2.5);
    ASSERT_EQ(tuple3.size(), 2);

    // Tuple(int, double, char)
    Tuple<int, double, char> tuple4(1, 2.5, 'A');
    ASSERT_EQ(tuple4.size(), 3);

    // ~Tuple()
}

// operator==() operator!=()
TEST(Tuple, compare)
{
    // empty
    Tuple<> empty;
    ASSERT_TRUE(empty == Tuple<>());
    ASSERT_FALSE(empty != Tuple<int>(1));

    // one
    Tuple<int> one(1);
    ASSERT_TRUE(one == Tuple<int>(1));
    ASSERT_TRUE(one != Tuple<int>(2));
    ASSERT_TRUE(one != (Tuple<int, int>(2, 3)));

    // many
    Tuple<int, int, int> many(1, 2, 3);
    ASSERT_TRUE(many == (Tuple<int, int, int>(1, 2, 3)));
    ASSERT_TRUE(many != (Tuple<int, int, int>(3, 2, 1)));
    ASSERT_TRUE(many != (Tuple<int, int, int, int>(1, 2, 3, 4)));
}

// rest()
TEST(Tuple, rest)
{
    Tuple<int, double, char> tuple(1, 2.5, 'A');
    ASSERT_EQ(tuple.rest(), (Tuple<double, char>(2.5, 'A')));
    ASSERT_EQ(tuple.rest().rest(), Tuple<char>('A'));
    ASSERT_EQ(tuple.rest().rest().rest(), Tuple<>());
}

// get<>()
TEST(Tuple, get)
{
    Tuple<int, double, char> tuple(1, 2.5, 'A');

    ASSERT_EQ(tuple.get<0>(), 1);
    ASSERT_EQ(tuple.get<1>(), 2.5);
    ASSERT_EQ(tuple.get<2>(), 'A');
}

// operator<<()
TEST(Tuple, print)
{
    std::ostringstream oss;

    Tuple<> empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "()"); // string == char*, use eq
    oss.str("");

    Tuple<int> one(1);
    oss << one;
    ASSERT_EQ(oss.str(), "(1)");
    oss.str("");

    Tuple<int, double, char> many(1, 2.5, 'A');
    oss << many;
    ASSERT_EQ(oss.str(), "(1, 2.5, A)");
    oss.str("");
}
