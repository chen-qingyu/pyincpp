#include "../sources/Tuple.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Tuple")
{
    // constructor destructor size()
    SECTION("basics")
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

    Tuple<> empty;
    Tuple<int> one(1);
    Tuple<int, double, char> some(1, 2.5, 'A');

    // operator==() operator!=()
    SECTION("compare")
    {
        REQUIRE(empty == empty);
        REQUIRE(empty != one);
        REQUIRE(empty != some);

        REQUIRE(one != empty);
        REQUIRE(one == one);
        REQUIRE(one != some);

        REQUIRE(some != empty);
        REQUIRE(some != one);
        REQUIRE(some == some);
    }

    // rest()
    SECTION("rest")
    {
        REQUIRE(some.rest() == (Tuple<double, char>(2.5, 'A')));
        REQUIRE(some.rest().rest() == Tuple<char>('A'));
        REQUIRE(some.rest().rest().rest() == empty);
    }

    // get<>()
    SECTION("get")
    {
        REQUIRE(some.get<0>() == 1);
        REQUIRE(some.get<1>() == 2.5);
        REQUIRE(some.get<2>() == 'A');
    }

    // make_tuple()
    SECTION("make_tuple")
    {
        REQUIRE(make_tuple() == empty);
        REQUIRE(make_tuple(1) == one);
        REQUIRE(make_tuple(1, 2, 3) == (Tuple<int, int, int>(1, 2, 3)));
        REQUIRE(make_tuple(1, 2.33, 'A', Tuple<>()) == (Tuple<int, double, char, Tuple<>>(1, 2.33, 'A', {})));
    }

    // operator<<()
    SECTION("print")
    {
        std::ostringstream oss;

        oss << empty;
        REQUIRE(oss.str() == "()");
        oss.str("");

        oss << one;
        REQUIRE(oss.str() == "(1)");
        oss.str("");

        oss << some;
        REQUIRE(oss.str() == "(1, 2.5, A)");
        oss.str("");
    }
}
