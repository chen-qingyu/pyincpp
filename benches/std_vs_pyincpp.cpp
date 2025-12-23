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

Result (2025.12.23, Windows 11, MSVC, Catch2 v3.11.0):

-------------------------------------------------------------------------------
std::vector vs pyincpp::List
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(13)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100           420    36.4786 ns
ctor (pyincpp)                                 100           392    34.6556 ns
append (std)                                   100          3940    1.45711 ns
append (pyincpp)                               100          3654    1.55419 ns
access (std)                                   100         32072   0.433587 ns
access (pyincpp)                               100         26223    0.60344 ns

-------------------------------------------------------------------------------
std::map vs pyincpp::Dict
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(95)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100            76    179.566 ns
ctor (pyincpp)                                 100            76    177.961 ns
add (std)                                      100          4152    3.25626 ns
add (pyincpp)                                  100          4018    3.38228 ns
access (std)                                   100          5852    2.35236 ns
access (pyincpp)                               100          6779      1.828 ns

-------------------------------------------------------------------------------
std::string vs pyincpp::Str
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(136)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100         59672   0.230276 ns
ctor (pyincpp)                                 100         57692   0.224485 ns
append (std)                                   100          8344    1.64477 ns
append (pyincpp)                               100          1550    9.99935 ns
find (std)                                     100          3167     4.3306 ns
find (pyincpp)                                 100          2418    5.74359 ns

-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(175)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
make_tuple (std)                               100         66253   0.206285 ns
make_tuple (pyincpp)                           100         66282   0.206044 ns
cmp (std)                                      100         34100   0.408504 ns
cmp (pyincpp)                                  100         34829    0.37173 ns
get (std)                                      100         59553   0.237385 ns
get (pyincpp)                                  100         61028   0.214803 ns

-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(54)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100            76    179.474 ns
ctor (pyincpp)                                 100            76    182.987 ns
add (std)                                      100          4086     3.3255 ns
add (pyincpp)                                  100          4553    3.05623 ns
find (std)                                     100          9607    1.62621 ns
find (pyincpp)                                 100          9590    1.33618 ns

-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(214)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (std)                                     100           124    109.887 ns
ctor (pyincpp)                                 100           127     109.78 ns
push_back (std)                                100          1499    7.71181 ns
push_back (pyincpp)                            100          1567     7.8111 ns
push_front (std)                               100          1404    7.40812 ns
push_front (pyincpp)                           100          1246    7.83547 ns

===============================================================================
*/
