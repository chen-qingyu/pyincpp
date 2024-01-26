#include <deque>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <pyincpp/pyincpp.hpp>

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
benchmark.exe is a Catch2 v3.5.2 host application.
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
std::vector constructor                        100           284     1.9312 ms
                                        67.4894 ns    67.0528 ns    68.1408 ns
                                        2.70618 ns     1.9834 ns    3.57221 ns

pyincpp::List constructor                      100           331     1.9529 ms
                                        59.2417 ns    58.7523 ns    60.6495 ns
                                        3.86208 ns    1.63382 ns    8.28367 ns

std::vector append                             100          2889     1.7334 ms
                                        2.74905 ns    2.56144 ns    2.99308 ns
                                        1.08784 ns    0.88793 ns    1.34118 ns

pyincpp::List append                           100             2     3.2644 ms
                                         21.911 us    21.3435 us    23.2765 us
                                        4.18563 us    1.43306 us    7.45444 us

std::vector access                             100         21060          0 ns
                                       0.912583 ns   0.903371 ns   0.932051 ns
                                      0.0652673 ns  0.0348639 ns   0.119054 ns

pyincpp::List access                           100         20176          0 ns
                                        0.96476 ns   0.960646 ns   0.971451 ns
                                      0.0260298 ns  0.0158245 ns  0.0357711 ns


-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
tests\benchmark.cpp(49)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::set constructor                           100            18      2.007 ms
                                        1.11217 us    1.10444 us    1.12706 us
                                         52.843 ns    32.0772 ns    90.1007 ns

pyincpp::Set constructor                       100            19     2.0197 ms
                                        1.13758 us    1.11174 us    1.17542 us
                                        157.719 ns    117.904 ns    211.503 ns

std::set insert                                100          3577     1.7885 ms
                                        5.45848 ns    5.42801 ns    5.50629 ns
                                       0.191466 ns   0.132749 ns   0.257028 ns

pyincpp::Set insert                            100            21     2.0139 ms
                                        987.048 ns    966.286 ns    1.03395 us
                                        151.926 ns    81.6504 ns    301.407 ns

std::set find                                  100          6941     1.3882 ms
                                        2.92062 ns    2.88705 ns    2.96038 ns
                                       0.187498 ns   0.166752 ns   0.207488 ns

pyincpp::Set find                              100          7789     1.5578 ms
                                        2.63089 ns    2.60868 ns    2.68199 ns
                                       0.162798 ns  0.0869985 ns   0.331952 ns


-------------------------------------------------------------------------------
std::map vs pyincpp::Map
-------------------------------------------------------------------------------
tests\benchmark.cpp(85)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::map constructor                           100            18     1.9998 ms
                                         1.1115 us    1.10622 us    1.12061 us
                                        34.6265 ns    22.1036 ns    50.3892 ns

pyincpp::Map constructor                       100            19     2.0216 ms
                                        1.06816 us    1.06358 us      1.076 us
                                        29.6017 ns    19.6673 ns    42.1316 ns

std::map insert                                100          3503     1.7515 ms
                                        5.54782 ns    5.52498 ns     5.5875 ns
                                       0.150488 ns  0.0937849 ns   0.207308 ns

pyincpp::Map insert                            100            20      1.988 ms
                                         978.95 ns        970 ns     993.15 ns
                                        56.5971 ns    39.8372 ns    77.5328 ns

std::map find                                  100          6772     1.3544 ms
                                         2.8321 ns    2.81527 ns    2.85854 ns
                                       0.106104 ns   0.074107 ns    0.14313 ns

pyincpp::Map find                              100          7260      1.452 ms
                                        2.67424 ns    2.66295 ns    2.69298 ns
                                      0.0725741 ns  0.0477745 ns    0.09943 ns


-------------------------------------------------------------------------------
std::string vs pyincpp::String
-------------------------------------------------------------------------------
tests\benchmark.cpp(121)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::string constructor                        100          9383     1.8766 ms
                                        2.09059 ns    2.07503 ns    2.14153 ns
                                       0.128194 ns  0.0456228 ns   0.284884 ns

pyincpp::String constructor                    100           153     1.9584 ms
                                         127.19 ns    126.667 ns    128.033 ns
                                        3.31416 ns    2.25515 ns    4.52527 ns

std::string append                             100             3      2.478 ms
                                         11.799 us    11.1123 us    14.8947 us
                                        6.27654 us    681.539 ns    14.7866 us

pyincpp::String append                         100             1     3.5409 ms
                                         46.478 us     45.827 us     48.453 us
                                        5.28957 us    1.79323 us    11.6205 us

std::string access                             100         16318     1.6318 ms
                                        1.18685 ns    1.18256 ns    1.19475 ns
                                      0.0293818 ns  0.0185525 ns  0.0420459 ns

pyincpp::String access                         100         20063          0 ns
                                       0.966805 ns   0.962119 ns   0.974979 ns
                                      0.0307606 ns  0.0194745 ns  0.0446113 ns


-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
tests\benchmark.cpp(157)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::tuple constructor                         100         31438          0 ns
                                       0.593708 ns   0.591227 ns    0.59848 ns
                                      0.0170884 ns  0.0100871 ns  0.0257271 ns

pyincpp::Tuple constructor                     100         31938          0 ns
                                       0.594903 ns   0.591709 ns   0.600257 ns
                                      0.0205765 ns  0.0137946 ns  0.0287492 ns

std::tuple compare                             100         32706          0 ns
                                       0.593928 ns   0.591573 ns   0.597933 ns
                                      0.0153503 ns  0.0099567 ns  0.0214216 ns

pyincpp::Tuple compare                         100         32728          0 ns
                                       0.595545 ns   0.592734 ns   0.599853 ns
                                      0.0176167 ns   0.012502 ns   0.023272 ns

std::tuple get                                 100         31692          0 ns
                                       0.612773 ns   0.608986 ns   0.618831 ns
                                      0.0240504 ns  0.0167363 ns  0.0330922 ns

pyincpp::Tuple get                             100         31408          0 ns
                                         0.6186 ns   0.612392 ns   0.630508 ns
                                      0.0423602 ns  0.0262834 ns  0.0646732 ns


-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
tests\benchmark.cpp(193)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::deque constructor                         100            71     1.9596 ms
                                        281.028 ns    279.662 ns    283.268 ns
                                        8.78064 ns    6.03296 ns    12.3899 ns

pyincpp::Deque constructor                     100            46     1.9596 ms
                                        423.326 ns    421.196 ns      426.5 ns
                                        13.1772 ns    9.47976 ns    17.7107 ns

std::deque push_back                           100          1150      1.955 ms
                                        19.5965 ns    17.1287 ns    22.9878 ns
                                        14.6352 ns    11.5227 ns    18.3244 ns

pyincpp::Deque push_back                       100           375       1.95 ms
                                         51.928 ns    45.4507 ns      60.16 ns
                                        37.0752 ns    30.4711 ns    43.4695 ns

std::deque push_front                          100          1037     1.8666 ms
                                        15.3028 ns    13.2999 ns    18.1938 ns
                                        12.1424 ns     9.2369 ns    15.1902 ns

pyincpp::Deque push_front                      100           369     1.9188 ms
                                        52.1843 ns    45.5447 ns    60.7588 ns
                                         38.428 ns     31.548 ns    45.0693 ns


===============================================================================
test cases: 6 | 6 passed
*/
