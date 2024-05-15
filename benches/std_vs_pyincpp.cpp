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
    BENCHMARK("std::vector constructor")
    {
        return std::vector<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(pyincpp::List<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("pyincpp::List constructor")
    {
        return pyincpp::List<int>{1, 2, 3, 4, 5};
    };

    s.push_back(6);
    REQUIRE(s == std::vector<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("std::vector append")
    {
        s.push_back(6);
    };

    p += 6;
    REQUIRE(p == pyincpp::List<int>{1, 2, 3, 4, 5, 6});
    BENCHMARK("pyincpp::List append")
    {
        p += 6;
    };

    REQUIRE(s.at(0) == 1);
    BENCHMARK("std::vector access with check")
    {
        return s.at(0);
    };

    REQUIRE(p[0] == 1);
    BENCHMARK("pyincpp::List access with check")
    {
        return p[0];
    };
}

TEST_CASE("std::set vs pyincpp::Set", "[std]")
{
    auto s = std::set<int>{1, 2, 3, 4, 5};
    auto p = pyincpp::Set<int>{1, 2, 3, 4, 5};

    REQUIRE(std::set<int>{1, 2, 3, 4, 5} == s);
    BENCHMARK("std::set constructor")
    {
        return std::set<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(pyincpp::Set<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("pyincpp::Set constructor")
    {
        return pyincpp::Set<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(s.insert(6).second == true);
    BENCHMARK("std::set add and check")
    {
        return s.insert(6).second;
    };

    REQUIRE(p.add(6) == true);
    BENCHMARK("pyincpp::Set add and check")
    {
        return p.add(6);
    };

    REQUIRE(*s.find(2) == 2);
    BENCHMARK("std::set find")
    {
        return *s.find(2);
    };

    REQUIRE(*p.find(2) == 2);
    BENCHMARK("pyincpp::Set find")
    {
        return *p.find(2);
    };
}

TEST_CASE("std::map vs pyincpp::Dict", "[std]")
{
    auto s = std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    auto p = pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};

    REQUIRE(std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}} == s);
    BENCHMARK("std::map constructor")
    {
        return std::map<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    };

    REQUIRE(pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}} == p);
    BENCHMARK("pyincpp::Dict constructor")
    {
        return pyincpp::Dict<int, char>{{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}};
    };

    REQUIRE(s.insert({6, 'F'}).second == true);
    BENCHMARK("std::map add and check")
    {
        return s.insert({6, 'F'}).second;
    };

    REQUIRE(p.add(6, 'F') == true);
    BENCHMARK("pyincpp::Dict add and check")
    {
        return p.add(6, 'F');
    };

    REQUIRE(s[5] == 'E');
    BENCHMARK("std::map access")
    {
        return s[5];
    };

    REQUIRE(p[5] == 'E');
    BENCHMARK("pyincpp::Dict access")
    {
        return p[5];
    };
}

TEST_CASE("std::string vs pyincpp::Str", "[std]")
{
    auto s = std::string("hello world");
    auto p = pyincpp::Str("hello world");

    REQUIRE(std::string("hello world") == s);
    BENCHMARK("std::string constructor")
    {
        return std::string("hello world");
    };

    REQUIRE(pyincpp::Str("hello world") == p);
    BENCHMARK("pyincpp::Str constructor")
    {
        return pyincpp::Str("hello world");
    };

    REQUIRE(s + "!" == "hello world!");
    BENCHMARK("std::string append")
    {
        return s + "!";
    };

    REQUIRE(p + "!" == "hello world!");
    BENCHMARK("pyincpp::Str append")
    {
        return p + "!";
    };

    REQUIRE(s.find("world") == 6);
    BENCHMARK("std::string find")
    {
        return s.find("world");
    };

    REQUIRE(p.find("world") == 6);
    BENCHMARK("pyincpp::Str find")
    {
        return p.find("world");
    };
}

TEST_CASE("std::tuple vs pyincpp::Tuple", "[std]")
{
    auto s = std::make_tuple(1, 2.33, 'A');
    auto p = pyincpp::make_tuple(1, 2.33, 'A');

    REQUIRE(std::make_tuple(1, 2.33, 'A') == s);
    BENCHMARK("std::tuple make_tuple")
    {
        return std::make_tuple(1, 2.33, 'A');
    };

    REQUIRE(pyincpp::make_tuple(1, 2.33, 'A') == p);
    BENCHMARK("pyincpp::Tuple make_tuple")
    {
        return pyincpp::make_tuple(1, 2.33, 'A');
    };

    REQUIRE(s == s);
    BENCHMARK("std::tuple compare")
    {
        return s == s;
    };

    REQUIRE(p == p);
    BENCHMARK("pyincpp::Tuple compare")
    {
        return p == p;
    };

    REQUIRE(std::get<0>(s) == 1);
    BENCHMARK("std::tuple get")
    {
        return std::get<0>(s);
    };

    REQUIRE(p.get<0>() == 1);
    BENCHMARK("pyincpp::Tuple get")
    {
        return p.get<0>();
    };
}

TEST_CASE("std::deque vs pyincpp::Deque", "[std]")
{
    auto s = std::deque<int>{1, 2, 3, 4, 5};
    auto p = pyincpp::Deque<int>{1, 2, 3, 4, 5};

    REQUIRE(std::deque<int>{1, 2, 3, 4, 5} == s);
    BENCHMARK("std::deque constructor")
    {
        return std::deque<int>{1, 2, 3, 4, 5};
    };

    REQUIRE(pyincpp::Deque<int>{1, 2, 3, 4, 5} == p);
    BENCHMARK("pyincpp::Deque constructor")
    {
        return pyincpp::Deque<int>{1, 2, 3, 4, 5};
    };

    s.push_back(6);
    REQUIRE(s.back() == 6);
    BENCHMARK("std::deque push_back")
    {
        return s.push_back(6);
    };

    p.push_back(6);
    REQUIRE(p.back() == 6);
    BENCHMARK("pyincpp::Deque push_back")
    {
        return p.push_back(6);
    };

    s.push_front(0);
    REQUIRE(s.front() == 0);
    BENCHMARK("std::deque push_front")
    {
        s.push_front(0);
    };

    p.push_front(0);
    REQUIRE(p.front() == 0);
    BENCHMARK("pyincpp::Deque push_front")
    {
        p.push_front(0);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench -i [std] --benchmark-no-analysis`

Result (2024.05.15, Windows 10, Catch2 v3.5.3):

-------------------------------------------------------------------------------
std::vector vs pyincpp::List
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(13)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
std::vector constructor                        100           310    68.2903 ns
pyincpp::List constructor                      100           303     68.868 ns
std::vector append                             100          2700    2.64741 ns
pyincpp::List append                           100          2351     2.8843 ns
std::vector access with check                  100         22765   0.948649 ns
pyincpp::List access with check                100         18637    1.13103 ns

-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(57)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
std::set constructor                           100            49    425.735 ns
pyincpp::Set constructor                       100            49    434.694 ns
std::set add and check                         100          2839    6.81825 ns
pyincpp::Set add and check                     100          3240    6.75247 ns
std::set find                                  100          6081    3.35076 ns
pyincpp::Set find                              100          6192     3.3876 ns

-------------------------------------------------------------------------------
std::map vs pyincpp::Dict
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(99)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
std::map constructor                           100            45    446.067 ns
pyincpp::Dict constructor                      100            43    464.186 ns
std::map add and check                         100          2968    6.96799 ns
pyincpp::Dict add and check                    100           324    62.3858 ns
std::map access                                100          3446    5.66599 ns
pyincpp::Dict access                           100          4215    5.42278 ns

-------------------------------------------------------------------------------
std::string vs pyincpp::Str
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(141)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
std::string constructor                        100          8335    2.38164 ns
pyincpp::Str constructor                       100          8599    2.56297 ns
std::string append                             100          2818     7.2885 ns
pyincpp::Str append                            100          1328     15.808 ns
std::string find                               100          4025    5.24571 ns
pyincpp::Str find                              100          1285    15.7642 ns

-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(183)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
std::tuple make_tuple                          100         35033   0.594639 ns
pyincpp::Tuple make_tuple                      100         34955   0.610556 ns
std::tuple compare                             100         35051   0.598927 ns
pyincpp::Tuple compare                         100         34986    0.59601 ns
std::tuple get                                 100         33877   0.613366 ns
pyincpp::Tuple get                             100         33709   0.614554 ns

-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
benches\std_vs_pyincpp.cpp(225)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
std::deque constructor                         100            76    266.224 ns
pyincpp::Deque constructor                     100            79    266.873 ns
std::deque push_back                           100          1102     17.167 ns
pyincpp::Deque push_back                       100          1034    17.7776 ns
std::deque push_front                          100           929    17.5748 ns
pyincpp::Deque push_front                      100           938    17.9797 ns

===============================================================================
All tests passed (36 assertions in 6 test cases)
*/
