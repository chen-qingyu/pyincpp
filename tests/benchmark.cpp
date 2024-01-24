#include <deque>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <pytype/pytype.hpp>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("std::vector vs pytype::List")
{
    BENCHMARK("std::vector constructor")
    {
        return std::vector<int>({1, 2, 3, 4, 5});
    };

    BENCHMARK("pytype::List constructor")
    {
        return pytype::List<int>({1, 2, 3, 4, 5});
    };

    auto s = std::vector<int>({1, 2, 3, 4, 5});
    auto p = pytype::List<int>({1, 2, 3, 4, 5});

    BENCHMARK("std::vector append")
    {
        return s.push_back(6);
    };

    BENCHMARK("pytype::List append")
    {
        return p += 6;
    };

    BENCHMARK("std::vector access")
    {
        return s.at(0);
    };

    BENCHMARK("pytype::List access")
    {
        return p[0]; // access with check
    };
}

TEST_CASE("std::set vs pytype::Set")
{
    BENCHMARK("std::set constructor")
    {
        return std::set<int>({1, 2, 3, 4, 5});
    };

    BENCHMARK("pytype::Set constructor")
    {
        return pytype::Set<int>({1, 2, 3, 4, 5});
    };

    auto s = std::set<int>({1, 2, 3, 4, 5});
    auto p = pytype::Set<int>({1, 2, 3, 4, 5});

    BENCHMARK("std::set insert")
    {
        return s.insert(6);
    };

    BENCHMARK("pytype::Set insert")
    {
        return p += 6;
    };

    BENCHMARK("std::set find")
    {
        return s.find(2);
    };

    BENCHMARK("pytype::Set find")
    {
        return p.find(2);
    };
}

TEST_CASE("std::map vs pytype::Map")
{
    BENCHMARK("std::map constructor")
    {
        return std::map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    };

    BENCHMARK("pytype::Map constructor")
    {
        return pytype::Map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    };

    auto s = std::map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    auto p = pytype::Map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});

    BENCHMARK("std::map insert")
    {
        return s.insert({6, 'F'});
    };

    BENCHMARK("pytype::Map insert")
    {
        return p += {6, 'F'};
    };

    BENCHMARK("std::map find")
    {
        return s.find(2);
    };

    BENCHMARK("pytype::Map find")
    {
        return p.find(2);
    };
}

TEST_CASE("std::string vs pytype::String")
{
    BENCHMARK("std::string constructor")
    {
        return std::string("hello");
    };

    BENCHMARK("pytype::String constructor")
    {
        return pytype::String("hello");
    };

    auto s = std::string("hello");
    auto p = pytype::String("hello");

    BENCHMARK("std::string append")
    {
        return s += "world";
    };

    BENCHMARK("pytype::String append")
    {
        return p += "world";
    };

    BENCHMARK("std::string access")
    {
        return s.at(0);
    };

    BENCHMARK("pytype::String access")
    {
        return p[0]; // access with check
    };
}

TEST_CASE("std::tuple vs pytype::Tuple")
{
    BENCHMARK("std::tuple constructor")
    {
        return std::make_tuple(1, 2.33, 'A');
    };

    BENCHMARK("pytype::Tuple constructor")
    {
        return pytype::make_tuple(1, 2.33, 'A');
    };

    auto s = std::make_tuple(1, 2.33, 'A');
    auto p = pytype::make_tuple(1, 2.33, 'A');

    BENCHMARK("std::tuple compare")
    {
        return s == s;
    };

    BENCHMARK("pytype::Tuple compare")
    {
        return p == p;
    };

    BENCHMARK("std::tuple get")
    {
        return std::get<0>(s);
    };

    BENCHMARK("pytype::Tuple get")
    {
        return p.get<0>();
    };
}

TEST_CASE("std::deque vs pytype::Deque")
{
    BENCHMARK("std::deque constructor")
    {
        return std::deque<int>({1, 2, 3, 4, 5});
    };

    BENCHMARK("pytype::Deque constructor")
    {
        return pytype::Deque<int>({1, 2, 3, 4, 5});
    };

    auto s = std::deque<int>({1, 2, 3, 4, 5});
    auto p = pytype::Deque<int>({1, 2, 3, 4, 5});

    BENCHMARK("std::deque push_back")
    {
        return s.push_back(6);
    };

    BENCHMARK("pytype::Deque push_back")
    {
        return p.push_back(6);
    };

    BENCHMARK("std::deque push_front")
    {
        return s.push_front(6);
    };

    BENCHMARK("pytype::Deque push_front")
    {
        return p.push_front(6);
    };
}

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
benchmark.exe is a Catch2 v3.5.2 host application.
Run with -? for options

-------------------------------------------------------------------------------
std::vector vs pytype::List
-------------------------------------------------------------------------------
benchmark\pytype_vs_std.cpp(13)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::vector constructor                        100           293     1.9631 ms
                                        67.4164 ns    67.0307 ns    68.1263 ns
                                        2.57777 ns    1.72177 ns    4.27671 ns

pytype::List constructor                       100           336     1.9824 ms
                                        61.2113 ns    59.2351 ns    66.5863 ns
                                        15.2517 ns    5.21958 ns     31.364 ns

std::vector append                             100          2657     1.8599 ms
                                         2.6703 ns    2.49228 ns    2.92473 ns
                                        1.07646 ns   0.846387 ns    1.48264 ns

pytype::List append                            100             2     3.2414 ms
                                        22.1985 us    21.5025 us     23.885 us
                                        5.15181 us     2.4043 us    10.0196 us

std::vector access                             100         21683          0 ns
                                       0.925933 ns   0.919153 ns    0.93322 ns
                                      0.0357973 ns  0.0326128 ns  0.0428182 ns

pytype::List access                            100         20438          0 ns
                                       0.964331 ns   0.960808 ns   0.970839 ns
                                      0.0239285 ns  0.0149728 ns  0.0343968 ns


-------------------------------------------------------------------------------
std::set vs pytype::Set
-------------------------------------------------------------------------------
benchmark\pytype_vs_std.cpp(49)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::set constructor                           100            47     1.9928 ms
                                        425.298 ns    420.809 ns    441.809 ns
                                        38.9405 ns     10.205 ns    89.4653 ns

pytype::Set constructor                        100            49     1.9796 ms
                                         397.51 ns    395.408 ns    401.367 ns
                                        14.1831 ns    9.16242 ns    23.2231 ns

std::set insert                                100          3596      1.798 ms
                                        5.49166 ns    5.43632 ns    5.64433 ns
                                       0.430383 ns   0.182751 ns   0.913027 ns

pytype::Set insert                             100            21     2.0076 ms
                                        982.286 ns    967.333 ns    1.01167 us
                                        103.359 ns    64.5068 ns    193.803 ns

std::set find                                  100          5954     1.7862 ms
                                        3.52855 ns    3.48975 ns    3.55895 ns
                                       0.174379 ns   0.129672 ns   0.225881 ns

pytype::Set find                               100          7951     1.5902 ms
                                         2.5964 ns    2.58068 ns    2.61766 ns
                                       0.092264 ns  0.0721619 ns   0.118717 ns


-------------------------------------------------------------------------------
std::map vs pytype::Map
-------------------------------------------------------------------------------
benchmark\pytype_vs_std.cpp(85)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::map constructor                           100            47     2.0069 ms
                                        423.255 ns    421.511 ns    426.234 ns
                                        11.4099 ns    7.54462 ns    16.0276 ns

pytype::Map constructor                        100            47     2.0022 ms
                                        405.766 ns    403.894 ns    408.809 ns
                                        11.9789 ns    7.85749 ns    16.1967 ns

std::map insert                                100          3533     1.7665 ms
                                        5.61534 ns    5.58053 ns    5.66035 ns
                                       0.201271 ns   0.166985 ns    0.24777 ns

pytype::Map insert                             100            21     2.0223 ms
                                        958.095 ns    954.095 ns    965.429 ns
                                         26.986 ns    17.1603 ns    41.2915 ns

std::map find                                  100          5905     1.7715 ms
                                        3.52312 ns    3.47604 ns    3.56952 ns
                                       0.236864 ns   0.190883 ns    0.29265 ns

pytype::Map find                               100          7373     1.4746 ms
                                        2.75926 ns    2.67557 ns    3.13007 ns
                                         0.7634 ns  0.0985621 ns    1.79872 ns


-------------------------------------------------------------------------------
std::string vs pytype::String
-------------------------------------------------------------------------------
benchmark\pytype_vs_std.cpp(121)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::string constructor                        100          9463     1.8926 ms
                                        2.12776 ns    2.09109 ns    2.23365 ns
                                       0.288654 ns   0.113171 ns   0.616065 ns

pytype::String constructor                     100           151     1.9781 ms
                                        131.437 ns    130.245 ns    135.391 ns
                                         9.8959 ns    3.43541 ns    22.1282 ns

std::string append                             100             3     2.5212 ms
                                         11.992 us    11.3233 us    15.0587 us
                                         6.1724 us    442.526 ns    14.6051 us

pytype::String append                          100             1     3.7775 ms
                                          48.04 us     47.533 us     48.564 us
                                        2.62554 us    2.39601 us    3.00549 us

std::string access                             100         16612     1.6612 ms
                                        1.18866 ns    1.18348 ns    1.19763 ns
                                       0.034069 ns  0.0224381 ns  0.0493206 ns

pytype::String access                          100         20408          0 ns
                                        0.96472 ns   0.960947 ns   0.971972 ns
                                      0.0260193 ns  0.0154444 ns   0.038926 ns


-------------------------------------------------------------------------------
std::tuple vs pytype::Tuple
-------------------------------------------------------------------------------
benchmark\pytype_vs_std.cpp(157)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::tuple constructor                         100         33188          0 ns
                                       0.593799 ns   0.591268 ns   0.597867 ns
                                      0.0157341 ns 0.00979613 ns  0.0218088 ns

pytype::Tuple constructor                      100         33131          0 ns
                                       0.594187 ns   0.591651 ns   0.598563 ns
                                      0.0164099 ns  0.0108208 ns  0.0230526 ns

std::tuple compare                             100         31378          0 ns
                                       0.813086 ns   0.786252 ns   0.835872 ns
                                       0.125523 ns   0.109304 ns   0.139425 ns

pytype::Tuple compare                          100         33090          0 ns
                                       0.597552 ns   0.592656 ns   0.613025 ns
                                      0.0395327 ns   0.014359 ns  0.0876173 ns

std::tuple get                                 100         32138          0 ns
                                        0.61488 ns   0.612359 ns   0.619018 ns
                                      0.0160136 ns 0.00978418 ns  0.0219471 ns

pytype::Tuple get                              100         32176          0 ns
                                       0.625031 ns   0.615365 ns   0.650516 ns
                                      0.0733957 ns  0.0255806 ns   0.144736 ns


-------------------------------------------------------------------------------
std::deque vs pytype::Deque
-------------------------------------------------------------------------------
benchmark\pytype_vs_std.cpp(193)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::deque constructor                         100            72     1.9944 ms
                                        273.542 ns    272.153 ns    275.917 ns
                                        9.08774 ns    5.98132 ns    12.9186 ns

pytype::Deque constructor                      100            48     2.0064 ms
                                        417.312 ns    415.354 ns    420.396 ns
                                         12.348 ns    8.45379 ns    16.7553 ns

std::deque push_back                           100          1157     1.9669 ms
                                        15.3025 ns     13.401 ns    17.9888 ns
                                        11.4581 ns    8.98813 ns    14.3357 ns

pytype::Deque push_back                        100           384     1.9584 ms
                                        50.4557 ns    44.6615 ns     58.224 ns
                                        34.0514 ns    27.7503 ns    40.6018 ns

std::deque push_front                          100           841     1.9343 ms
                                        15.5767 ns    13.2319 ns    19.1736 ns
                                        14.5309 ns    10.5901 ns    18.8346 ns

pytype::Deque push_front                       100           320      1.952 ms
                                        56.3969 ns    47.9219 ns    68.4688 ns
                                         51.305 ns     39.653 ns    66.8777 ns


===============================================================================
test cases: 6 | 6 passed
*/
