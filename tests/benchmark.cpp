#include <deque>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "../sources/pyincpp.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("std::vector vs pyincpp::List")
{
    BENCHMARK("std::vector constructor")
    {
        return std::vector<int>({1, 2, 3, 4, 5});
    };

    BENCHMARK("pyincpp::List constructor")
    {
        return pyincpp::List<int>({1, 2, 3, 4, 5});
    };

    auto s = std::vector<int>({1, 2, 3, 4, 5});
    auto p = pyincpp::List<int>({1, 2, 3, 4, 5});

    BENCHMARK("std::vector append")
    {
        return s.push_back(6);
    };

    BENCHMARK("pyincpp::List append")
    {
        return p += 6;
    };

    BENCHMARK("std::vector access")
    {
        return s.at(0);
    };

    BENCHMARK("pyincpp::List access")
    {
        return p[0]; // access with check
    };
}

TEST_CASE("std::set vs pyincpp::Set")
{
    BENCHMARK("std::set constructor")
    {
        return std::set<int>({1, 2, 3, 4, 5});
    };

    BENCHMARK("pyincpp::Set constructor")
    {
        return pyincpp::Set<int>({1, 2, 3, 4, 5});
    };

    auto s = std::set<int>({1, 2, 3, 4, 5});
    auto p = pyincpp::Set<int>({1, 2, 3, 4, 5});

    BENCHMARK("std::set insert")
    {
        return s.insert(6);
    };

    BENCHMARK("pyincpp::Set insert")
    {
        return p += 6;
    };

    BENCHMARK("std::set find")
    {
        return s.find(2);
    };

    BENCHMARK("pyincpp::Set find")
    {
        return p.find(2);
    };
}

TEST_CASE("std::map vs pyincpp::Map")
{
    BENCHMARK("std::map constructor")
    {
        return std::map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    };

    BENCHMARK("pyincpp::Map constructor")
    {
        return pyincpp::Map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    };

    auto s = std::map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    auto p = pyincpp::Map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});

    BENCHMARK("std::map insert")
    {
        return s.insert({6, 'F'});
    };

    BENCHMARK("pyincpp::Map insert")
    {
        return p += {6, 'F'};
    };

    BENCHMARK("std::map find")
    {
        return s.find(2);
    };

    BENCHMARK("pyincpp::Map find")
    {
        return p.find(2);
    };
}

TEST_CASE("std::string vs pyincpp::String")
{
    BENCHMARK("std::string constructor")
    {
        return std::string("hello");
    };

    BENCHMARK("pyincpp::String constructor")
    {
        return pyincpp::String("hello");
    };

    auto s = std::string("hello");
    auto p = pyincpp::String("hello");

    BENCHMARK("std::string append")
    {
        return s += "world";
    };

    BENCHMARK("pyincpp::String append")
    {
        return p += "world";
    };

    BENCHMARK("std::string access")
    {
        return s.at(0);
    };

    BENCHMARK("pyincpp::String access")
    {
        return p[0]; // access with check
    };
}

TEST_CASE("std::tuple vs pyincpp::Tuple")
{
    BENCHMARK("std::tuple constructor")
    {
        return std::make_tuple(1, 2.33, 'A');
    };

    BENCHMARK("pyincpp::Tuple constructor")
    {
        return pyincpp::make_tuple(1, 2.33, 'A');
    };

    auto s = std::make_tuple(1, 2.33, 'A');
    auto p = pyincpp::make_tuple(1, 2.33, 'A');

    BENCHMARK("std::tuple compare")
    {
        return s == s;
    };

    BENCHMARK("pyincpp::Tuple compare")
    {
        return p == p;
    };

    BENCHMARK("std::tuple get")
    {
        return std::get<0>(s);
    };

    BENCHMARK("pyincpp::Tuple get")
    {
        return p.get<0>();
    };
}

TEST_CASE("std::deque vs pyincpp::Deque")
{
    BENCHMARK("std::deque constructor")
    {
        return std::deque<int>({1, 2, 3, 4, 5});
    };

    BENCHMARK("pyincpp::Deque constructor")
    {
        return pyincpp::Deque<int>({1, 2, 3, 4, 5});
    };

    auto s = std::deque<int>({1, 2, 3, 4, 5});
    auto p = pyincpp::Deque<int>({1, 2, 3, 4, 5});

    BENCHMARK("std::deque push_back")
    {
        return s.push_back(6);
    };

    BENCHMARK("pyincpp::Deque push_back")
    {
        return p.push_back(6);
    };

    BENCHMARK("std::deque push_front")
    {
        return s.push_front(6);
    };

    BENCHMARK("pyincpp::Deque push_front")
    {
        return p.push_front(6);
    };
}

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
benchmark.exe is a Catch2 v3.5.3 host application.
Run with -? for options

-------------------------------------------------------------------------------
std::vector vs pyincpp::List
-------------------------------------------------------------------------------
tests\benchmark.cpp(13)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::vector constructor                        100           290      1.943 ms
                                        68.0793 ns    67.5759 ns    68.7379 ns
                                        2.91895 ns    2.42381 ns    3.55439 ns

pyincpp::List constructor                      100           325       1.95 ms
                                        63.2185 ns    61.0431 ns      66.68 ns
                                        13.6884 ns    9.60682 ns    18.7119 ns

std::vector append                             100          2564     1.7948 ms
                                        2.64314 ns    2.49766 ns    2.83541 ns
                                       0.845982 ns   0.695176 ns    1.04057 ns

pyincpp::List append                           100             3     2.0739 ms
                                        9.62867 us       9.04 us     12.237 us
                                        5.28229 us    335.444 ns    12.4067 us

std::vector access                             100         21504          0 ns
                                       0.912481 ns   0.905552 ns   0.921596 ns
                                      0.0403162 ns  0.0326209 ns  0.0507803 ns

pyincpp::List access                           100         20335          0 ns
                                       0.970838 ns   0.964101 ns   0.988739 ns
                                      0.0518939 ns  0.0242792 ns    0.10827 ns


-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
tests\benchmark.cpp(49)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::set constructor                           100            18     2.0232 ms
                                        1.11706 us    1.11167 us    1.12617 us
                                        34.7622 ns    23.7073 ns    51.4365 ns

pyincpp::Set constructor                       100            18     2.0484 ms
                                        1.12883 us    1.12078 us    1.14178 us
                                        51.2726 ns     37.479 ns    75.9518 ns

std::set insert                                100          3028     1.8168 ms
                                        5.43263 ns    5.40555 ns    5.48217 ns
                                       0.180082 ns   0.109504 ns   0.275621 ns

pyincpp::Set insert                            100            42     2.0034 ms
                                        1.01681 us    970.976 ns    1.20757 us
                                        414.857 ns    71.4958 ns    975.402 ns

std::set find                                  100          6903     1.3806 ms
                                        2.80646 ns    2.79342 ns    2.82616 ns
                                      0.0807788 ns  0.0562349 ns   0.108786 ns

pyincpp::Set find                              100          7029     1.4058 ms
                                        2.79855 ns    2.78674 ns    2.81619 ns
                                      0.0726402 ns   0.051389 ns  0.0988542 ns


-------------------------------------------------------------------------------
std::map vs pyincpp::Map
-------------------------------------------------------------------------------
tests\benchmark.cpp(85)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::map constructor                           100            18     2.0214 ms
                                        1.12978 us    1.12139 us    1.14339 us
                                        53.4222 ns    36.4334 ns    74.4705 ns

pyincpp::Map constructor                       100            18     2.0106 ms
                                        1.12644 us    1.11583 us     1.1585 us
                                         85.141 ns    33.5583 ns    185.113 ns

std::map insert                                100          3392      1.696 ms
                                         5.5513 ns    5.52683 ns    5.59316 ns
                                        0.15977 ns   0.101424 ns   0.220604 ns

pyincpp::Map insert                            100            41     1.9844 ms
                                        478.146 ns    475.902 ns     481.78 ns
                                         14.313 ns    9.93387 ns    19.1901 ns

std::map find                                  100          7039     1.4078 ms
                                        3.30118 ns    3.16025 ns    3.49766 ns
                                       0.844483 ns   0.670635 ns    1.26917 ns

pyincpp::Map find                              100          6947     1.3894 ms
                                         2.7907 ns    2.77703 ns    2.81244 ns
                                      0.0868424 ns  0.0600235 ns   0.123149 ns


-------------------------------------------------------------------------------
std::string vs pyincpp::String
-------------------------------------------------------------------------------
tests\benchmark.cpp(121)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::string constructor                        100          9380      1.876 ms
                                         2.0887 ns    2.07665 ns    2.11119 ns
                                      0.0811621 ns  0.0513611 ns    0.13361 ns

pyincpp::String constructor                    100           167     1.9706 ms
                                        116.102 ns    115.569 ns    117.024 ns
                                        3.48715 ns    2.30111 ns    5.09537 ns

std::string append                             100             2     2.0356 ms
                                        12.3335 us    11.4915 us     15.227 us
                                        7.03314 us    2.11791 us    15.8587 us

pyincpp::String append                         100             3     2.4978 ms
                                        10.6217 us    10.0747 us    11.9897 us
                                        4.08474 us    1.37533 us    7.44033 us

std::string access                             100         16495     1.6495 ms
                                        1.18794 ns     1.1829 ns    1.19606 ns
                                      0.0315599 ns  0.0190143 ns  0.0432977 ns

pyincpp::String access                         100         20362          0 ns
                                       0.964542 ns    0.96076 ns   0.971516 ns
                                      0.0255764 ns   0.015556 ns  0.0372162 ns


-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
tests\benchmark.cpp(157)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::tuple constructor                         100         33044          0 ns
                                       0.594662 ns    0.59212 ns   0.598717 ns
                                      0.0161938 ns   0.010996 ns   0.021944 ns

pyincpp::Tuple constructor                     100         33056          0 ns
                                       0.593871 ns    0.59133 ns   0.597955 ns
                                      0.0158439 ns 0.00954727 ns   0.021774 ns

std::tuple compare                             100         32406          0 ns
                                       0.593193 ns   0.591033 ns   0.597173 ns
                                      0.0147875 ns 0.00947732 ns  0.0212018 ns

pyincpp::Tuple compare                         100         33035          0 ns
                                        0.59555 ns   0.592674 ns   0.600303 ns
                                      0.0185433 ns  0.0122758 ns  0.0255572 ns

std::tuple get                                 100         31577          0 ns
                                       0.610159 ns   0.607277 ns    0.61472 ns
                                      0.0181923 ns  0.0125619 ns  0.0244981 ns

pyincpp::Tuple get                             100         31896          0 ns
                                       0.609199 ns   0.606502 ns   0.613807 ns
                                      0.0176826 ns  0.0115222 ns  0.0244966 ns


-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
tests\benchmark.cpp(193)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::deque constructor                         100            70      1.967 ms
                                        280.657 ns    278.471 ns    286.786 ns
                                        17.1302 ns    7.23477 ns    36.5183 ns

pyincpp::Deque constructor                     100            69     1.9734 ms
                                        278.928 ns    277.667 ns    281.014 ns
                                        8.09878 ns    5.43297 ns     11.097 ns

std::deque push_back                           100          1089     1.9602 ms
                                        15.5216 ns    13.6107 ns    18.2287 ns
                                        11.5227 ns    8.90792 ns    14.1806 ns

pyincpp::Deque push_back                       100          1043     1.8774 ms
                                        17.1304 ns    15.1755 ns    19.9473 ns
                                        11.8468 ns    8.98871 ns    15.1086 ns

std::deque push_front                          100          1019     1.9361 ms
                                        15.1256 ns     13.315 ns    17.7841 ns
                                        11.0708 ns    8.29119 ns    13.9211 ns

pyincpp::Deque push_front                      100          1019     1.9361 ms
                                        15.5378 ns     13.473 ns    18.4711 ns
                                        12.4601 ns    9.48078 ns    15.6352 ns


===============================================================================
test cases: 6 | 6 passed
*/
