#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../sources/pyincpp.hpp"

// Some big integer libraries with more than 100 stars on Github.
#include "BigInt.hpp"  // https://github.com/faheel/BigInt
#include "InfInt.h"    // https://github.com/sercantutar/infint
#include "bigint.h"    // https://github.com/kasparsklavins/bigint
#include "bignumber.h" // https://github.com/Limeoats/BigNumber

TEST_CASE("pyincpp::Int vs Other", "[int]")
{
    const char* int1 = "987654321000987654321000987654321000987654321000";
    const char* int2 = "123456789000123456789000123456789000123456789000";

    pyincpp::Int a1(int1);
    pyincpp::Int a2(int2);

    BigInt b1(int1);
    BigInt b2(int2);

    InfInt c1(int1);
    InfInt c2(int2);

    Dodecahedron::Bigint d1(int1);
    Dodecahedron::Bigint d2(int2);

    BigNumber e1(int1);
    BigNumber e2(int2);

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

    REQUIRE(InfInt(int1) == "987654321000987654321000987654321000987654321000");
    BENCHMARK("construct (infint)")
    {
        return InfInt(int1);
    };

    REQUIRE(Dodecahedron::Bigint(int1) == std::string("987654321000987654321000987654321000987654321000"));
    BENCHMARK("construct (bigint)")
    {
        return Dodecahedron::Bigint(int1);
    };

    REQUIRE(BigNumber(int1) == "987654321000987654321000987654321000987654321000");
    BENCHMARK("construct (BigNumber)")
    {
        return BigNumber(int1);
    };

    REQUIRE(a1 + a2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("+ (pyincpp)")
    {
        return a1 + a2;
    };

    REQUIRE(b1 + b2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("+ (BigInt)")
    {
        return b1 + b2;
    };

    REQUIRE(c1 + c2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("+ (infint)")
    {
        return c1 + c2;
    };

    REQUIRE(d1 + d2 == std::string("1111111110001111111110001111111110001111111110000"));
    BENCHMARK("+ (bigint)")
    {
        return d1 + d2;
    };

    REQUIRE(e1 + e2 == "1111111110001111111110001111111110001111111110000");
    BENCHMARK("+ (BigNumber)")
    {
        return e1 + e2;
    };

    REQUIRE(a1 - a2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("- (pyincpp)")
    {
        return a1 - a2;
    };

    REQUIRE(b1 - b2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("- (BigInt)")
    {
        return b1 - b2;
    };

    REQUIRE(c1 - c2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("- (infint)")
    {
        return c1 - c2;
    };

    REQUIRE(d1 - d2 == std::string("864197532000864197532000864197532000864197532000"));
    BENCHMARK("- (bigint)")
    {
        return d1 - d2;
    };

    REQUIRE(e1 - e2 == "864197532000864197532000864197532000864197532000");
    BENCHMARK("- (BigNumber)")
    {
        return e1 - e2;
    };

    REQUIRE(a1 * a2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("* (pyincpp)")
    {
        return a1 * a2;
    };

    REQUIRE(b1 * b2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("* (BigInt)")
    {
        return b1 * b2;
    };

    REQUIRE(c1 * c2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("* (infint)")
    {
        return c1 * c2;
    };

    REQUIRE(d1 * d2 == std::string("121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000"));
    BENCHMARK("* (bigint)")
    {
        return d1 * d2;
    };

    REQUIRE(e1 * e2 == "121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000");
    BENCHMARK("* (BigNumber)")
    {
        return e1 * e2;
    };

    REQUIRE(a1 / a2 == "8");
    BENCHMARK("/ (pyincpp)")
    {
        return a1 / a2;
    };

    REQUIRE(b1 / b2 == "8");
    BENCHMARK("/ (BigInt)")
    {
        return b1 / b2;
    };

    REQUIRE(c1 / c2 == "8");
    BENCHMARK("/ (infint)")
    {
        return c1 / c2;
    };

    // Not supported
    // REQUIRE(d1 / d2 == std::string("8"));
    // BENCHMARK("/ (bigint)")
    // {
    //     return d1 / d2;
    // };

    REQUIRE(e1 / e2 == "8");
    BENCHMARK("/ (BigNumber)")
    {
        return e1 / e2;
    };

    REQUIRE(a1 % a2 == "9000000000009000000000009000000000009000");
    BENCHMARK("% (pyincpp)")
    {
        return a1 % a2;
    };

    REQUIRE(b1 % b2 == "9000000000009000000000009000000000009000");
    BENCHMARK("% (BigInt)")
    {
        return b1 % b2;
    };

    REQUIRE(c1 % c2 == "9000000000009000000000009000000000009000");
    BENCHMARK("% (infint)")
    {
        return c1 % c2;
    };

    // Not supported
    // REQUIRE(d1 % d2 == std::string("9000000000009000000000009000000000009000"));
    // BENCHMARK("% (bigint)")
    // {
    //     return d1 % d2;
    // };

    // Not supported
    // REQUIRE(e1 % e2 == "9000000000009000000000009000000000009000");
    // BENCHMARK("% (BigNumber)")
    // {
    //     return e1 % e2;
    // };

    auto acopy = pyincpp::Int(a1);
    REQUIRE(++acopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("++ (pyincpp)")
    {
        return ++acopy;
    };

    auto bcopy = BigInt(b1);
    REQUIRE(++bcopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("++ (BigInt)")
    {
        return ++bcopy;
    };

    auto ccopy = InfInt(c1);
    REQUIRE(++ccopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("++ (infint)")
    {
        return ++ccopy;
    };

    // Not supported
    // auto dcopy = Dodecahedron::Bigint(d1);
    // REQUIRE(++dcopy == "987654321000987654321000987654321000987654321001");
    // BENCHMARK("++ (bigint)")
    // {
    //     return ++dcopy;
    // };

    auto ecopy = BigNumber(e1);
    REQUIRE(++ecopy == "987654321000987654321000987654321000987654321001");
    BENCHMARK("++ (BigNumber)")
    {
        return ++ecopy;
    };

    REQUIRE(a1 > a2);
    BENCHMARK("> (pyincpp)")
    {
        return a1 > a2;
    };

    REQUIRE(b1 > b2);
    BENCHMARK("> (BigInt)")
    {
        return b1 > b2;
    };

    REQUIRE(c1 > c2);
    BENCHMARK("> (infint)")
    {
        return c1 > c2;
    };

    REQUIRE(d1 > d2);
    BENCHMARK("> (bigint)")
    {
        return d1 > d2;
    };

    REQUIRE(e1 > e2);
    BENCHMARK("> (BigNumber)")
    {
        return e1 > e2;
    };

    REQUIRE(pyincpp::Int::gcd(a1, a2) == "9000000000009000000000009000000000009000");
    BENCHMARK("gcd (pyincpp)")
    {
        return pyincpp::Int::gcd(a1, a2);
    };

    REQUIRE(gcd(b1, b2) == "9000000000009000000000009000000000009000");
    BENCHMARK("gcd (BigInt)")
    {
        return gcd(b1, b2);
    };

    REQUIRE(pyincpp::Int::pow(a1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK("pow (pyincpp)")
    {
        return pyincpp::Int::pow(a1, 10);
    };

    REQUIRE(pow(b1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK("pow (BigInt)")
    {
        return pow(b1, 10);
    };

    REQUIRE(d1.pow(10) == std::string("883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000"));
    BENCHMARK("pow (bigint)")
    {
        return Dodecahedron::Bigint(d1).pow(10); // pow will modify this
    };

    REQUIRE(e1.pow(10) == std::string("883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000"));
    BENCHMARK("pow (BigNumber)")
    {
        return e1.pow(10);
    };

    REQUIRE(pyincpp::Int::sqrt(a1) == "993807990006614735669893");
    BENCHMARK("sqrt (pyincpp)")
    {
        return pyincpp::Int::sqrt(a1);
    };

    REQUIRE(sqrt(b1) == "993807990006614735669893");
    BENCHMARK("sqrt (BigInt)")
    {
        return sqrt(b1);
    };

    REQUIRE(c1.intSqrt() == "993807990006614735669893");
    BENCHMARK("sqrt (infint)")
    {
        return c1.intSqrt();
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

    REQUIRE(pyincpp::Int(120).factorial() == "6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000");
    BENCHMARK("factorial (pyincpp)")
    {
        return pyincpp::Int(120).factorial();
    };

    REQUIRE(Dodecahedron::factorial(120) == std::string("6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000"));
    BENCHMARK("factorial (bigint)")
    {
        return Dodecahedron::factorial(120);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench --benchmark-no-analysis -i [int]`

Result (2024.12.10, Windows 10, Catch2 v3.7.1):

-------------------------------------------------------------------------------
pyincpp::Int vs Other
-------------------------------------------------------------------------------
benches\pyincpp_int_vs_other_int.cpp(12)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (pyincpp)                            100            93    194.892 ns
construct (BigInt)                             100            70      243.8 ns
construct (infint)                             100            44    413.636 ns
construct (bigint)                             100            31    1.15077 us
construct (BigNumber)                          100           120    150.258 ns
+ (pyincpp)                                    100            75     242.68 ns
+ (BigInt)                                     100             6      3.412 us
+ (infint)                                     100           100     178.96 ns
+ (bigint)                                     100           143    129.021 ns
+ (BigNumber)                                  100             8     2.4655 us
- (pyincpp)                                    100            72    249.472 ns
- (BigInt)                                     100             5     3.6136 us
- (infint)                                     100           104    172.212 ns
- (bigint)                                     100           161    110.404 ns
- (BigNumber)                                  100             1      36.91 us
* (pyincpp)                                    100            23    807.174 ns
* (BigInt)                                     100             1      120.1 us
* (infint)                                     100            48    373.583 ns
* (bigint)                                     100             8    2.27013 us
* (BigNumber)                                  100             1    215.286 us
/ (pyincpp)                                    100            11    1.63845 us
/ (BigInt)                                     100             1     36.553 us
/ (infint)                                     100             1      23.56 us
/ (BigNumber)                                  100             1    285.638 us
% (pyincpp)                                    100            11      1.639 us
% (BigInt)                                     100             1     77.325 us
% (infint)                                     100             1     23.462 us
++ (pyincpp)                                   100           250      71.72 ns
++ (BigInt)                                    100             6     3.4155 us
++ (infint)                                    100           209    85.4211 ns
++ (BigNumber)                                 100             8    2.38063 us
> (pyincpp)                                    100          4629    3.86261 ns
> (BigInt)                                     100          1641    10.8873 ns
> (infint)                                     100          4004    4.46079 ns
> (bigint)                                     100          5025    3.56378 ns
> (BigNumber)                                  100            37    485.676 ns
gcd (pyincpp)                                  100             3      8.345 us
gcd (BigInt)                                   100             1    238.947 us
pow (pyincpp)                                  100             1     48.379 us
pow (BigInt)                                   100             1    4.01147 ms
pow (bigint)                                   100             1    5.38484 ms
pow (BigNumber)                                100             1    20.9088 ms
sqrt (pyincpp)                                 100             1     93.623 us
sqrt (BigInt)                                  100             1    2.87853 ms
sqrt (infint)                                  100             1    1.00549 ms
random (pyincpp)                               100             6    3.13183 us
random (BigInt)                                100             2     9.5865 us
factorial (pyincpp)                            100             1     32.737 us
factorial (bigint)                             100             2     11.877 us

===============================================================================
*/
