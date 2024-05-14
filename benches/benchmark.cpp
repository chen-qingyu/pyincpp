#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <deque>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "../sources/pyincpp.hpp"

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
        return p.add(6);
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

TEST_CASE("std::map vs pyincpp::Dict")
{
    BENCHMARK("std::map constructor")
    {
        return std::map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    };

    BENCHMARK("pyincpp::Dict constructor")
    {
        return pyincpp::Dict<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    };

    auto s = std::map<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});
    auto p = pyincpp::Dict<int, char>({{1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}});

    BENCHMARK("std::map insert")
    {
        return s.insert({6, 'F'});
    };

    BENCHMARK("pyincpp::Dict insert")
    {
        return p.add(6, 'F');
    };

    BENCHMARK("std::map find")
    {
        return s.find(2);
    };

    BENCHMARK("pyincpp::Dict find")
    {
        return p.find(2);
    };
}

TEST_CASE("std::string vs pyincpp::Str")
{
    BENCHMARK("std::string constructor")
    {
        return std::string("hello");
    };

    BENCHMARK("pyincpp::Str constructor")
    {
        return pyincpp::Str("hello");
    };

    auto s = std::string("hello");
    auto p = pyincpp::Str("hello");

    BENCHMARK("std::string append")
    {
        return s + "world";
    };

    BENCHMARK("pyincpp::Str append")
    {
        return p + "world";
    };

    BENCHMARK("std::string access")
    {
        return s.at(0);
    };

    BENCHMARK("pyincpp::Str access")
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
std::vector constructor                        100           302     2.1442 ms
                                        75.8808 ns    72.1788 ns    82.6093 ns
                                        24.6142 ns    15.2081 ns    38.2256 ns

pyincpp::List constructor                      100           302     2.1442 ms
                                         72.798 ns    70.9636 ns    76.1523 ns
                                        12.3321 ns    7.73523 ns    18.7859 ns

std::vector append                             100          3022     2.1154 ms
                                        2.55493 ns    2.40834 ns    2.75778 ns
                                        0.87289 ns   0.691952 ns    1.15907 ns

pyincpp::List append                           100             4      2.576 ms
                                        8.87775 us    8.17175 us     10.695 us
                                        5.06282 us    643.512 ns    9.13123 us

std::vector access                             100         23494          0 ns
                                       0.909934 ns   0.903933 ns   0.917213 ns
                                      0.0337354 ns  0.0294148 ns  0.0419337 ns

pyincpp::List access                           100         19198     1.9198 ms
                                        1.11361 ns    1.10933 ns    1.12053 ns
                                      0.0269401 ns  0.0162647 ns  0.0369729 ns


-------------------------------------------------------------------------------
std::set vs pyincpp::Set
-------------------------------------------------------------------------------
tests\benchmark.cpp(49)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::set constructor                           100            19     2.1584 ms
                                        1.11553 us    1.10989 us    1.12589 us
                                        37.8264 ns    24.8811 ns      60.83 ns

pyincpp::Set constructor                       100            20       2.24 ms
                                         1.1195 us      1.114 us    1.12925 us
                                        36.2607 ns     24.387 ns    59.5459 ns

std::set insert                                100          3941     1.9705 ms
                                        5.48769 ns    5.43745 ns    5.60188 ns
                                       0.367171 ns   0.163831 ns   0.655751 ns

pyincpp::Set insert                            100          3996      1.998 ms
                                        5.33934 ns    5.31907 ns    5.37287 ns
                                       0.130487 ns  0.0820592 ns   0.180527 ns

std::set find                                  100          7658     1.5316 ms
                                        2.76965 ns    2.75764 ns    2.78715 ns
                                      0.0730347 ns  0.0535259 ns  0.0971304 ns

pyincpp::Set find                              100          7644     1.5288 ms
                                           2.75 ns    2.73927 ns    2.76766 ns
                                      0.0688498 ns  0.0479064 ns  0.0941097 ns


-------------------------------------------------------------------------------
std::map vs pyincpp::Dict
-------------------------------------------------------------------------------
tests\benchmark.cpp(85)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::map constructor                           100            20      2.248 ms
                                        1.11905 us    1.11355 us     1.1287 us
                                        36.4588 ns    23.9397 ns    54.8529 ns

pyincpp::Dict constructor                      100            19     2.1736 ms
                                        1.11374 us    1.10858 us    1.12474 us
                                        36.7615 ns    21.9786 ns    66.0705 ns

std::map insert                                100          3788      1.894 ms
                                        5.60428 ns    5.54488 ns    5.82154 ns
                                       0.516391 ns   0.132115 ns    1.18894 ns

pyincpp::Dict insert                           100           353      2.118 ms
                                         60.864 ns    60.6176 ns    61.2436 ns
                                        1.54356 ns    1.05821 ns    2.05627 ns

std::map find                                  100          7593     1.5186 ms
                                        2.80785 ns    2.79586 ns    2.82629 ns
                                      0.0752833 ns  0.0526472 ns    0.10071 ns

pyincpp::Dict find                             100          7663     1.5326 ms
                                        2.85998 ns    2.81026 ns    3.04776 ns
                                        0.42798 ns   0.100659 ns   0.981021 ns


-------------------------------------------------------------------------------
std::string vs pyincpp::Str
-------------------------------------------------------------------------------
tests\benchmark.cpp(121)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::string constructor                        100         10312     2.0624 ms
                                        2.08136 ns    2.07283 ns    2.09513 ns
                                      0.0545595 ns  0.0371228 ns   0.076778 ns

pyincpp::Str constructor                       100         10282     2.0564 ms
                                        2.07742 ns    2.06934 ns    2.09045 ns
                                      0.0507527 ns  0.0310144 ns  0.0698221 ns

std::string append                             100          2858     2.0006 ms
                                        7.67075 ns    7.58118 ns    7.82715 ns
                                       0.583981 ns   0.395532 ns    1.07269 ns

pyincpp::Str append                            100          1360       2.04 ms
                                         16.175 ns    15.9485 ns    16.6228 ns
                                        1.55607 ns   0.916294 ns    2.66859 ns

std::string access                             100         17959     1.7959 ms
                                       0.998051 ns   0.965143 ns     1.0426 ns
                                       0.193357 ns   0.152494 ns   0.232142 ns

pyincpp::Str access                            100         18740      1.874 ms
                                        1.36713 ns    1.29445 ns    1.44856 ns
                                       0.391853 ns   0.354692 ns   0.446096 ns


-------------------------------------------------------------------------------
std::tuple vs pyincpp::Tuple
-------------------------------------------------------------------------------
tests\benchmark.cpp(157)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::tuple constructor                         100         36070          0 ns
                                       0.596784 ns   0.592847 ns   0.607984 ns
                                       0.031138 ns  0.0134988 ns  0.0665321 ns

pyincpp::Tuple constructor                     100         35771          0 ns
                                       0.593609 ns   0.591289 ns   0.597327 ns
                                      0.0145126 ns 0.00879798 ns  0.0199404 ns

std::tuple compare                             100         35796          0 ns
                                       0.599341 ns   0.594592 ns   0.610962 ns
                                      0.0355219 ns  0.0183045 ns  0.0710582 ns

pyincpp::Tuple compare                         100         35978          0 ns
                                       0.594669 ns   0.592084 ns   0.599255 ns
                                      0.0170509 ns  0.0106411 ns  0.0246243 ns

std::tuple get                                 100         34684          0 ns
                                       0.613222 ns   0.610743 ns   0.617345 ns
                                      0.0159992 ns  0.0103045 ns  0.0220868 ns

pyincpp::Tuple get                             100         34825          0 ns
                                       0.614731 ns   0.611342 ns   0.620675 ns
                                      0.0224441 ns   0.014839 ns  0.0330294 ns


-------------------------------------------------------------------------------
std::deque vs pyincpp::Deque
-------------------------------------------------------------------------------
tests\benchmark.cpp(193)
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
std::deque constructor                         100            36     2.1492 ms
                                        270.306 ns    268.278 ns    274.222 ns
                                         13.836 ns    8.56579 ns    22.0835 ns

pyincpp::Deque constructor                     100            80      2.168 ms
                                         269.45 ns    268.125 ns    271.525 ns
                                        8.30215 ns    5.93057 ns    11.4488 ns

std::deque push_back                           100          1258     2.1386 ms
                                        15.1614 ns    13.3887 ns    17.5771 ns
                                        10.4986 ns    8.34186 ns    12.6616 ns

pyincpp::Deque push_back                       100          1117     2.1223 ms
                                        16.8675 ns    15.0483 ns    19.4056 ns
                                        10.8587 ns    8.42586 ns    13.3053 ns

std::deque push_front                          100          1093     2.0767 ms
                                        15.2525 ns    13.4328 ns    17.8756 ns
                                          11.01 ns    8.53439 ns    13.6402 ns

pyincpp::Deque push_front                      100          1124     2.1356 ms
                                        15.2322 ns    13.3906 ns    17.8096 ns
                                        11.0676 ns    8.55655 ns    13.5966 ns


===============================================================================
test cases: 6 | 6 passed
*/
