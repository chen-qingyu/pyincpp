#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../sources/pyincpp.hpp"
#include "BigInt.hpp" // https://github.com/faheel/BigInt/releases/download/v0.5.0-dev/BigInt.hpp

TEST_CASE("pyincpp::Int vs BigInt", "[int]")
{
    const char* int1 = "987654321000987654321000987654321000987654321000";
    const char* int2 = "123456789000123456789000123456789000123456789000";

    pyincpp::Int pint1(int1);
    pyincpp::Int pint2(int2);

    BigInt bint1(int1);
    BigInt bint2(int2);

    REQUIRE(pyincpp::Int(int1) == "987654321000987654321000987654321000987654321000");
    BENCHMARK("pyincpp::Int ctor")
    {
        return pyincpp::Int(int1);
    };

    REQUIRE(BigInt(int1) == "987654321000987654321000987654321000987654321000");
    BENCHMARK("BigInt ctor")
    {
        return BigInt(int1);
    };

    REQUIRE(pint1 + pint2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("pyincpp::Int +")
    {
        return pint1 + pint2;
    };

    REQUIRE(bint1 + bint2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("BigInt +")
    {
        return bint1 + bint2;
    };

    REQUIRE(pint1 - pint2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("pyincpp::Int -")
    {
        return pint1 - pint2;
    };

    REQUIRE(bint1 - bint2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("BigInt -")
    {
        return bint1 - bint2;
    };

    REQUIRE(pint1 * pint2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("pyincpp::Int *")
    {
        return pint1 * pint2;
    };

    REQUIRE(bint1 * bint2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("BigInt *")
    {
        return bint1 * bint2;
    };

    REQUIRE(pint1 / pint2 == "8");
    BENCHMARK("pyincpp::Int /")
    {
        return pint1 / pint2;
    };

    REQUIRE(bint1 / bint2 == "8");
    BENCHMARK("BigInt /")
    {
        return bint1 / bint2;
    };

    REQUIRE(pint1 % pint2 == "9000000000009000000000009000000000009000");
    BENCHMARK("pyincpp::Int %")
    {
        return pint1 % pint2;
    };

    REQUIRE(bint1 % bint2 == "9000000000009000000000009000000000009000");
    BENCHMARK("BigInt %")
    {
        return bint1 % bint2;
    };

    auto pcopy = pyincpp::Int(pint1);
    REQUIRE(++pcopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("pyincpp::Int ++")
    {
        return ++pcopy;
    };

    auto bcopy = BigInt(bint1);
    REQUIRE(++bcopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("BigInt ++")
    {
        return ++bcopy;
    };

    REQUIRE(pint1 > pint2);
    BENCHMARK("pyincpp::Int >")
    {
        return pint1 > pint2;
    };

    REQUIRE(bint1 > bint2);
    BENCHMARK("BigInt >")
    {
        return bint1 > bint2;
    };

    REQUIRE(pyincpp::Int::gcd(pint1, pint2) == "9000000000009000000000009000000000009000");
    BENCHMARK("pyincpp::Int gcd")
    {
        return pyincpp::Int::gcd(pint1, pint2);
    };

    REQUIRE(gcd(bint1, bint2) == "9000000000009000000000009000000000009000");
    BENCHMARK("BigInt gcd")
    {
        return gcd(bint1, bint2);
    };

    REQUIRE(pyincpp::Int::pow(pint1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK("pyincpp::Int pow")
    {
        return pyincpp::Int::pow(pint1, 10);
    };

    REQUIRE(pow(bint1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK("BigInt pow")
    {
        return pow(bint1, 10);
    };

    REQUIRE(pyincpp::Int::sqrt(pint1) == "993807990006614735669893");
    BENCHMARK("pyincpp::Int sqrt")
    {
        return pyincpp::Int::sqrt(pint1);
    };

    REQUIRE(sqrt(bint1) == "993807990006614735669893");
    BENCHMARK("BigInt sqrt")
    {
        return sqrt(bint1);
    };

    REQUIRE(pyincpp::Int::random(1000).digits() == 1000);
    BENCHMARK("pyincpp::Int random")
    {
        return pyincpp::Int::random(1000);
    };

    REQUIRE(big_random(1000).to_string().size() == 1000);
    BENCHMARK("BigInt random")
    {
        return big_random(1000);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench -i [int] --benchmark-no-analysis`

Result (2024.05.15, Windows 10, Catch2 v3.5.3):

-------------------------------------------------------------------------------
pyincpp::Int vs BigInt
-------------------------------------------------------------------------------
benches\pyincpp_int_vs_other_int.cpp(7)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
pyincpp::Int ctor                              100           139    141.813 ns
BigInt ctor                                    100            82    247.866 ns
pyincpp::Int +                                 100            43    536.395 ns
BigInt +                                       100             6    3.40883 us
pyincpp::Int -                                 100            64    269.844 ns
BigInt -                                       100             5       3.66 us
pyincpp::Int *                                 100             3      7.614 us
BigInt *                                       100             1    122.567 us
pyincpp::Int /                                 100             9    2.58344 us
BigInt /                                       100             1     36.993 us
pyincpp::Int %                                 100             9    2.52556 us
BigInt %                                       100             1     78.301 us
pyincpp::Int ++                                100           276    76.8768 ns
BigInt ++                                      100             6    3.40133 us
pyincpp::Int >                                 100          5812    3.58723 ns
BigInt >                                       100          1864    11.2827 ns
pyincpp::Int gcd                               100             2    13.8315 us
BigInt gcd                                     100             1    250.251 us
pyincpp::Int pow                               100             1    767.197 us
BigInt pow                                     100             1    4.08365 ms
pyincpp::Int sqrt                              100             1    799.926 us
BigInt sqrt                                    100             1    2.99672 ms
pyincpp::Int random                            100             4    6.43075 us
BigInt random                                  100             3    9.93367 us

===============================================================================
All tests passed (24 assertions in 1 test case)
*/
