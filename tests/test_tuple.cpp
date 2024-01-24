#include "../sources/Tuple.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor size()
TEST_CASE("Tuple: basics")
{
    // Tuple()
    Tuple<> tuple1;
    REQUIRE(tuple1.size() == 0);

    // Tuple(int)
    Tuple<int> tuple2(1);
    REQUIRE(tuple2.size() == 1);

    // Tuple(int, double)
    Tuple<int, double> tuple3(1, 2.5);
    REQUIRE(tuple3.size() == 2);

    // Tuple(int, double, char)
    Tuple<int, double, char> tuple4(1, 2.5, 'A');
    REQUIRE(tuple4.size() == 3);

    // ~Tuple()
}

// operator==() operator!=()
TEST_CASE("Tuple: compare")
{
    // empty
    Tuple<> empty;
    REQUIRE(empty == Tuple<>());
    REQUIRE(empty != Tuple<int>(1));
    REQUIRE(empty != (Tuple<int, int>(2, 3)));

    // one
    Tuple<int> one(1);
    REQUIRE(one == Tuple<int>(1));
    REQUIRE(one != Tuple<int>(2));
    REQUIRE(one != (Tuple<int, int>(2, 3)));

    // many
    Tuple<int, int, int> many(1, 2, 3);
    REQUIRE(many == (Tuple<int, int, int>(1, 2, 3)));
    REQUIRE(many != (Tuple<int, int, int>(3, 2, 1)));
    REQUIRE(many != (Tuple<int, int, int, int>(1, 2, 3, 4)));
}

// rest()
TEST_CASE("Tuple: rest")
{
    Tuple<int, double, char> tuple(1, 2.5, 'A');
    REQUIRE(tuple.rest() == (Tuple<double, char>(2.5, 'A')));
    REQUIRE(tuple.rest().rest() == Tuple<char>('A'));
    REQUIRE(tuple.rest().rest().rest() == Tuple<>());
}

// get<>()
TEST_CASE("Tuple: get")
{
    Tuple<int, double, char> tuple(1, 2.5, 'A');

    REQUIRE(tuple.get<0>() == 1);
    REQUIRE(tuple.get<1>() == 2.5);
    REQUIRE(tuple.get<2>() == 'A');
}

// make_tuple()
TEST_CASE("Tuple: make_tuple")
{
    REQUIRE(make_tuple() == Tuple<>());
    REQUIRE(make_tuple(1) == Tuple<int>(1));
    REQUIRE(make_tuple(1, 2, 3) == (Tuple<int, int, int>(1, 2, 3)));
    REQUIRE(make_tuple(1, 2.33, 'A', Tuple<>()) == (Tuple<int, double, char, Tuple<>>(1, 2.33, 'A', {})));
}

// operator<<()
TEST_CASE("Tuple: print")
{
    std::ostringstream oss;

    Tuple<> empty;
    oss << empty;
    REQUIRE(oss.str() == "()");
    oss.str("");

    Tuple<int> one(1);
    oss << one;
    REQUIRE(oss.str() == "(1)");
    oss.str("");

    Tuple<int, double, char> many(1, 2.5, 'A');
    oss << many;
    REQUIRE(oss.str() == "(1, 2.5, A)");
    oss.str("");
}
