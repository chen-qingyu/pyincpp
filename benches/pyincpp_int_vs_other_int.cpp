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
    BENCHMARK("construct (pyincpp)")
    {
        return pyincpp::Int(int1);
    };

    REQUIRE(BigInt(int1) == "987654321000987654321000987654321000987654321000");
    BENCHMARK("construct (BigInt)")
    {
        return BigInt(int1);
    };

    REQUIRE(pint1 + pint2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("+ (pyincpp)")
    {
        return pint1 + pint2;
    };

    REQUIRE(bint1 + bint2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("+ (BigInt)")
    {
        return bint1 + bint2;
    };

    REQUIRE(pint1 - pint2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("- (pyincpp)")
    {
        return pint1 - pint2;
    };

    REQUIRE(bint1 - bint2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("- (BigInt)")
    {
        return bint1 - bint2;
    };

    REQUIRE(pint1 * pint2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("* (pyincpp)")
    {
        return pint1 * pint2;
    };

    REQUIRE(bint1 * bint2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("* (BigInt)")
    {
        return bint1 * bint2;
    };

    REQUIRE(pint1 / pint2 == "8");
    BENCHMARK("/ (pyincpp)")
    {
        return pint1 / pint2;
    };

    REQUIRE(bint1 / bint2 == "8");
    BENCHMARK("/ (BigInt)")
    {
        return bint1 / bint2;
    };

    REQUIRE(pint1 % pint2 == "9000000000009000000000009000000000009000");
    BENCHMARK("% (pyincpp)")
    {
        return pint1 % pint2;
    };

    REQUIRE(bint1 % bint2 == "9000000000009000000000009000000000009000");
    BENCHMARK("% (BigInt)")
    {
        return bint1 % bint2;
    };

    auto pcopy = pyincpp::Int(pint1);
    REQUIRE(++pcopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("++ (pyincpp)")
    {
        return ++pcopy;
    };

    auto bcopy = BigInt(bint1);
    REQUIRE(++bcopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("++ (BigInt)")
    {
        return ++bcopy;
    };

    REQUIRE(pint1 > pint2);
    BENCHMARK("> (pyincpp)")
    {
        return pint1 > pint2;
    };

    REQUIRE(bint1 > bint2);
    BENCHMARK("> (BigInt)")
    {
        return bint1 > bint2;
    };

    REQUIRE(pyincpp::Int::gcd(pint1, pint2) == "9000000000009000000000009000000000009000");
    BENCHMARK("gcd (pyincpp)")
    {
        return pyincpp::Int::gcd(pint1, pint2);
    };

    REQUIRE(gcd(bint1, bint2) == "9000000000009000000000009000000000009000");
    BENCHMARK("gcd (BigInt)")
    {
        return gcd(bint1, bint2);
    };

    REQUIRE(pyincpp::Int::pow(pint1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK("pow (pyincpp)")
    {
        return pyincpp::Int::pow(pint1, 10);
    };

    REQUIRE(pow(bint1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK("pow (BigInt)")
    {
        return pow(bint1, 10);
    };

    REQUIRE(pyincpp::Int::sqrt(pint1) == "993807990006614735669893");
    BENCHMARK("sqrt (pyincpp)")
    {
        return pyincpp::Int::sqrt(pint1);
    };

    REQUIRE(sqrt(bint1) == "993807990006614735669893");
    BENCHMARK("sqrt (BigInt)")
    {
        return sqrt(bint1);
    };

    REQUIRE(pyincpp::Int::random(1000).digits() == 1000);
    BENCHMARK("random (pyincpp)")
    {
        return pyincpp::Int::random(1000);
    };

    REQUIRE(big_random(1000).to_string().size() == 1000);
    BENCHMARK("random (BigInt)")
    {
        return big_random(1000);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench --benchmark-no-analysis -i [int]`

Result (2024.05.15, Windows 10, Catch2 v3.5.3):

-------------------------------------------------------------------------------
pyincpp::Int vs BigInt
-------------------------------------------------------------------------------
benches\pyincpp_int_vs_other_int.cpp(7)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (pyincpp)                            100           142    142.859 ns
construct (BigInt)                             100            80    252.975 ns
+ (pyincpp)                                    100            46    447.435 ns
+ (BigInt)                                     100             5     4.8318 us
- (pyincpp)                                    100            76    280.447 ns
- (BigInt)                                     100             5     4.2616 us
* (pyincpp)                                    100             3    7.59633 us
* (BigInt)                                     100             1    121.847 us
/ (pyincpp)                                    100             8     2.5945 us
/ (BigInt)                                     100             1     44.597 us
% (pyincpp)                                    100             8      2.523 us
% (BigInt)                                     100             1     90.192 us
++ (pyincpp)                                   100           267    75.6442 ns
++ (BigInt)                                    100             5     4.1202 us
> (pyincpp)                                    100          5567     3.6093 ns
> (BigInt)                                     100          1678    12.5703 ns
gcd (pyincpp)                                  100             2    13.2665 us
gcd (BigInt)                                   100             1    288.573 us
pow (pyincpp)                                  100             1    771.099 us
pow (BigInt)                                   100             1    4.19652 ms
sqrt (pyincpp)                                 100             1    819.069 us
sqrt (BigInt)                                  100             1    2.90994 ms
random (pyincpp)                               100             4      6.336 us
random (BigInt)                                100             3    9.62133 us

===============================================================================
*/
