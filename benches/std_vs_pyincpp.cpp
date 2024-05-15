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
    BENCHMARK("construct (std)")
    {
        return std::vector<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(pyincpp::List<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("construct (pyincpp)")
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
    BENCHMARK("access with check (std)")
    {
        return s.at(0);
    };

    REQUIRE(p[0] == 1);
    BENCHMARK("access with check (pyincpp)")
    {
        return p[0];
    };
}

TEST_CASE("std::set vs pyincpp::Set", "[std]")
{
    auto s = std::set<int>{1, 2, 3, 4, 5};
    auto p = pyincpp::Set<int>{1, 2, 3, 4, 5};

    REQUIRE(std::set<int>{1, 2, 3, 4, 5} == s);
    BENCHMARK("construct (std)")
    {
        return std::set<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(pyincpp::Set<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("construct (pyincpp)")
    {
        return pyincpp::Set<int>{1, 2, 3, 4, 5};
    };

    s.insert(6);
    REQUIRE(s == std::set<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("add (std)")
    {
        s.insert(6);
    };

    p.add(6);
    REQUIRE(p == pyincpp::Set<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("add (pyincpp)")
    {
        p.add(6);
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
    BENCHMARK("construct (std)")
    {
        return std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    };

    REQUIRE(pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}} == p);
    BENCHMARK("construct (pyincpp)")
    {
        return pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    };

    s.insert({6, 'F'});
    REQUIRE(s == std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}, {6, 'F'}});
    BENCHMARK("add (std)")
    {
        s.insert({6, 'F'});
    };

    p.add(6, 'F');
    REQUIRE(p == pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}, {6, 'F'}});
    BENCHMARK("add (pyincpp)")
    {
        p.add(6, 'F');
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
    BENCHMARK("construct (std)")
    {
        return std::string("hello world");
    };

    REQUIRE(pyincpp::Str("hello world") == p);
    BENCHMARK("construct (pyincpp)")
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
    BENCHMARK("compare (std)")
    {
        return s == s;
    };

    REQUIRE(p == p);
    BENCHMARK("compare (pyincpp)")
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
    BENCHMARK("construct (std)")
    {
        return std::deque<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(pyincpp::Deque<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("construct (pyincpp)")
    {
        return pyincpp::Deque<int>{1, 2, 3, 4, 5};
    };

    s.push_back(6);
    REQUIRE(s.back() == 6);
    BENCHMARK("push_back (std)")
    {
        s.push_back(6);
    };

    p.push_back(6);
    REQUIRE(p.back() == 6);
    BENCHMARK("push_back (pyincpp)")
    {
        p.push_back(6);
    };

    s.push_front(0);
    REQUIRE(s.front() == 0);
    BENCHMARK("push_front (std)")
    {
        s.push_front(0);
    };

    p.push_front(0);
    REQUIRE(p.front() == 0);
    BENCHMARK("push_front (pyincpp)")
    {
        p.push_front(0);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench --benchmark-no-analysis -i [std]`

Result (2024.05.15, Windows 10, Catch2 v3.5.3):

-------------------------------------------------------------------------------
std::vector vs pyincpp::List
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(13)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (std)                                100           293    74.4744 ns
construct (pyincpp)                            100           290    70.1034 ns
append (std)                                   100          2967     2.7489 ns
append (pyincpp)                               100          2506    2.94852 ns
access with check (std)                        100         21853   0.934929 ns
access with check (pyincpp)                    100         17922    2.76599 ns

-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(57)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (std)                                100            47    428.426 ns
construct (pyincpp)                            100            47    428.064 ns
add (std)                                      100          3098    6.45513 ns
add (pyincpp)                                  100          3075    6.28325 ns
find (std)                                     100          5878    3.25876 ns
find (pyincpp)                                 100          5840    3.41473 ns

-------------------------------------------------------------------------------
std::map vs pyincpp::Dict
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(101)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (std)                                100            46    429.391 ns
construct (pyincpp)                            100            45    451.356 ns
add (std)                                      100          3102    6.44262 ns
add (pyincpp)                                  100           322    62.1584 ns
access (std)                                   100          3536    5.65243 ns
access (pyincpp)                               100          4482    4.69723 ns

-------------------------------------------------------------------------------
std::string vs pyincpp::Str
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(145)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (std)                                100          8386    2.38135 ns
construct (pyincpp)                            100          8409    2.37805 ns
append (std)                                   100          2693      7.241 ns
append (pyincpp)                               100          1317    16.0744 ns
find (std)                                     100          3350     6.1591 ns
find (pyincpp)                                 100          1173     16.451 ns

-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(187)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
make_tuple (std)                               100         33502   0.594562 ns
make_tuple (pyincpp)                           100         33623   0.626803 ns
compare (std)                                  100         33664   0.594522 ns
compare (pyincpp)                              100         33627   0.594195 ns
get (std)                                      100         32451   0.611568 ns
get (pyincpp)                                  100         32469   0.612122 ns

-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(229)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (std)                                100            74      272.5 ns
construct (pyincpp)                            100            74     270.77 ns
push_back (std)                                100          1056    17.4356 ns
push_back (pyincpp)                            100          1066    17.2129 ns
push_front (std)                               100           954    16.8574 ns
push_front (pyincpp)                           100           907     17.215 ns

===============================================================================
*/
