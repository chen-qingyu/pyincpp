#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <deque>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "../sources/pyincpp.hpp"

TEST_CASE("std::vector vs pyincpp::List", "[std]")
{
    auto s = std::vector<int>{1, 2, 3, 4, 5};
    auto p = pyincpp::List<int>{1, 2, 3, 4, 5};

    REQUIRE(std::vector<int>{1, 2, 3, 4, 5} == s);
    BENCHMARK("ctor (std)")
    {
        return std::vector<int>{1, 2, 3, 4, 5};
    };
    REQUIRE(pyincpp::List<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("ctor (pyincpp)")
    {
        return pyincpp::List<int>{1, 2, 3, 4, 5};
    };

    s.push_back(6);
    REQUIRE(s == std::vector<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("append (std)")
    {
        s.push_back(6);
    };
    p += 6;
    REQUIRE(p == pyincpp::List<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("append (pyincpp)")
    {
        p += 6;
    };

    REQUIRE(s.at(0) == 1);
    BENCHMARK("access (std)")
    {
        return s.at(0);
    };
    REQUIRE(p[0] == 1);
    BENCHMARK("access (pyincpp)")
    {
        return p[0]; // with boundary check
    };
}

TEST_CASE("std::set vs pyincpp::Set", "[std]")
{
    auto s = std::set<int>{1, 2, 3, 4, 5};
    auto p = pyincpp::Set<int>{1, 2, 3, 4, 5};

    REQUIRE(std::set<int>{1, 2, 3, 4, 5} == s);
    BENCHMARK("ctor (std)")
    {
        return std::set<int>{1, 2, 3, 4, 5};
    };
    REQUIRE(pyincpp::Set<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("ctor (pyincpp)")
    {
        return pyincpp::Set<int>{1, 2, 3, 4, 5};
    };

    s.insert(6);
    REQUIRE(s == std::set<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("add (std)")
    {
        return s.insert(6);
    };
    p.add(6);
    REQUIRE(p == pyincpp::Set<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("add (pyincpp)")
    {
        return p.add(6);
    };

    REQUIRE(*s.find(2) == 2);
    BENCHMARK("find (std)")
    {
        return *s.find(2);
    };
    REQUIRE(*p.find(2) == 2);
    BENCHMARK("find (pyincpp)")
    {
        return *p.find(2);
    };
}

TEST_CASE("std::map vs pyincpp::Dict", "[std]")
{
    auto s = std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    auto p = pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};

    REQUIRE(std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}} == s);
    BENCHMARK("ctor (std)")
    {
        return std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    };
    REQUIRE(pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}} == p);
    BENCHMARK("ctor (pyincpp)")
    {
        return pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    };

    s.insert({6, 'F'});
    REQUIRE(s == std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}, {6, 'F'}});
    BENCHMARK("add (std)")
    {
        return s.insert({6, 'F'});
    };
    p.add(6, 'F');
    REQUIRE(p == pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}, {6, 'F'}});
    BENCHMARK("add (pyincpp)")
    {
        return p.add(6, 'F');
    };

    REQUIRE(s[5] == 'E');
    BENCHMARK("access (std)")
    {
        return s[5];
    };
    REQUIRE(p[5] == 'E');
    BENCHMARK("access (pyincpp)")
    {
        return p[5];
    };
}

TEST_CASE("std::string vs pyincpp::Str", "[std]")
{
    auto s = std::string("hello world");
    auto p = pyincpp::Str("hello world");

    REQUIRE(std::string("hello world") == s);
    BENCHMARK("ctor (std)")
    {
        return std::string("hello world");
    };
    REQUIRE(pyincpp::Str("hello world") == p);
    BENCHMARK("ctor (pyincpp)")
    {
        return pyincpp::Str("hello world");
    };

    REQUIRE(s + "!" == "hello world!");
    BENCHMARK("append (std)")
    {
        return s + "!";
    };
    REQUIRE(p + "!" == "hello world!");
    BENCHMARK("append (pyincpp)")
    {
        return p + "!";
    };

    REQUIRE(s.find("world") == 6);
    BENCHMARK("find (std)")
    {
        return s.find("world");
    };
    REQUIRE(p.find("world") == 6);
    BENCHMARK("find (pyincpp)")
    {
        return p.find("world");
    };
}

TEST_CASE("std::tuple vs pyincpp::Tuple", "[std]")
{
    auto s = std::make_tuple(1, 2.33, 'A');
    auto p = pyincpp::make_tuple(1, 2.33, 'A');

    REQUIRE(std::make_tuple(1, 2.33, 'A') == s);
    BENCHMARK("make_tuple (std)")
    {
        return std::make_tuple(1, 2.33, 'A');
    };
    REQUIRE(pyincpp::make_tuple(1, 2.33, 'A') == p);
    BENCHMARK("make_tuple (pyincpp)")
    {
        return pyincpp::make_tuple(1, 2.33, 'A');
    };

    REQUIRE(s == s);
    BENCHMARK("cmp (std)")
    {
        return s == s;
    };
    REQUIRE(p == p);
    BENCHMARK("cmp (pyincpp)")
    {
        return p == p;
    };

    REQUIRE(std::get<0>(s) == 1);
    BENCHMARK("get (std)")
    {
        return std::get<0>(s);
    };
    REQUIRE(p.get<0>() == 1);
    BENCHMARK("get (pyincpp)")
    {
        return p.get<0>();
    };
}

TEST_CASE("std::deque vs pyincpp::Deque", "[std]")
{
    auto s = std::deque<int>{1, 2, 3, 4, 5};
    auto p = pyincpp::Deque<int>{1, 2, 3, 4, 5};

    REQUIRE(std::deque<int>{1, 2, 3, 4, 5} == s);
    BENCHMARK("ctor (std)")
    {
        return std::deque<int>{1, 2, 3, 4, 5};
    };
    REQUIRE(pyincpp::Deque<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("ctor (pyincpp)")
    {
        return pyincpp::Deque<int>{1, 2, 3, 4, 5};
    };

    s.push_back(6);
    REQUIRE(s.back() == 6);
    BENCHMARK("push_back (std)")
    {
        return s.push_back(6);
    };
    p.push_back(6);
    REQUIRE(p.back() == 6);
    BENCHMARK("push_back (pyincpp)")
    {
        return p.push_back(6);
    };

    s.push_front(0);
    REQUIRE(s.front() == 0);
    BENCHMARK("push_front (std)")
    {
        return s.push_front(0);
    };
    p.push_front(0);
    REQUIRE(p.front() == 0);
    BENCHMARK("push_front (pyincpp)")
    {
        return p.push_front(0);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench --benchmark-no-analysis -i [std]`

Result (2024.12.24, Windows 10, Catch2 v3.7.1):

-------------------------------------------------------------------------------
std::vector vs pyincpp::List
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(13)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100           203    67.0542 ns
ctor (pyincpp)                                 100           271    68.4022 ns
append (std)                                   100          2596    2.68952 ns
append (pyincpp)                               100          2407      3.086 ns
access (std)                                   100         19754   0.914397 ns
access (pyincpp)                               100         16606    1.11484 ns

-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(54)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100            44    431.159 ns
ctor (pyincpp)                                 100            44    430.295 ns
add (std)                                      100          2829    7.26935 ns
add (pyincpp)                                  100          2077    7.49206 ns
find (std)                                     100          5949    3.11548 ns
find (pyincpp)                                 100          5854    3.14315 ns

-------------------------------------------------------------------------------
std::map vs pyincpp::Dict
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(95)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100            43     431.86 ns
ctor (pyincpp)                                 100            44    434.977 ns
add (std)                                      100          2832    6.57804 ns
add (pyincpp)                                  100          2067    8.04838 ns
access (std)                                   100          3165    5.69826 ns
access (pyincpp)                               100          3657    4.76347 ns

-------------------------------------------------------------------------------
std::string vs pyincpp::Str
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(136)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100          7765    2.37411 ns
ctor (pyincpp)                                 100          7779     2.3773 ns
append (std)                                   100          2399    7.93497 ns
append (pyincpp)                               100          1209    15.7279 ns
find (std)                                     100          2709    6.83204 ns
find (pyincpp)                                 100          1286    15.0101 ns

-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(175)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
make_tuple (std)                               100         30953   0.597293 ns
make_tuple (pyincpp)                           100         31151   0.593496 ns
cmp (std)                                      100         18348   0.979398 ns
cmp (pyincpp)                                  100         17862    1.07384 ns
get (std)                                      100         30168   0.613697 ns
get (pyincpp)                                  100         30145   0.620335 ns

-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(214)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100            68    270.691 ns
ctor (pyincpp)                                 100            70    268.557 ns
push_back (std)                                100           984    17.0264 ns
push_back (pyincpp)                            100           984    16.6839 ns
push_front (std)                               100           889    17.0484 ns
push_front (pyincpp)                           100           849    18.3133 ns

===============================================================================
*/
