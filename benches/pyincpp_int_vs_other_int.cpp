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

Result (2024.12.09, Windows 10, Catch2 v3.7.1):

-------------------------------------------------------------------------------
pyincpp::Int vs Other
-------------------------------------------------------------------------------
benches\pyincpp_int_vs_other_int.cpp(12)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
construct (pyincpp)                            100            95    190.895 ns
construct (BigInt)                             100            72    251.833 ns
construct (infint)                             100            41    419.976 ns
construct (bigint)                             100            31     599.29 ns
construct (BigNumber)                          100           118    153.178 ns
+ (pyincpp)                                    100            75    245.707 ns
+ (BigInt)                                     100             6     3.4215 us
+ (infint)                                     100            99    181.838 ns
+ (bigint)                                     100           145    125.614 ns
+ (BigNumber)                                  100             8    2.51575 us
- (pyincpp)                                    100            72    251.111 ns
- (BigInt)                                     100             5     3.6316 us
- (infint)                                     100           103    175.524 ns
- (bigint)                                     100           163    111.773 ns
- (BigNumber)                                  100             1     37.369 us
* (pyincpp)                                    100            23    799.652 ns
* (BigInt)                                     100             1    121.082 us
* (infint)                                     100            49     376.02 ns
* (bigint)                                     100             8     2.2785 us
* (BigNumber)                                  100             1    217.865 us
/ (pyincpp)                                    100            12    1.63258 us
/ (BigInt)                                     100             1     36.917 us
/ (infint)                                     100             1     23.516 us
/ (BigNumber)                                  100             1    285.437 us
% (pyincpp)                                    100            12    1.63858 us
% (BigInt)                                     100             1     77.951 us
% (infint)                                     100             1     23.789 us
++ (pyincpp)                                   100           249    72.3655 ns
++ (BigInt)                                    100             6       3.44 us
++ (infint)                                    100           209         86 ns
++ (BigNumber)                                 100             8      2.424 us
> (pyincpp)                                    100          4361    3.88191 ns
> (BigInt)                                     100          1622    10.9673 ns
> (infint)                                     100          4846     3.8376 ns
> (bigint)                                     100          4558     4.1312 ns
> (BigNumber)                                  100            38    499.711 ns
gcd (pyincpp)                                  100             3      8.403 us
gcd (BigInt)                                   100             1    237.707 us
pow (pyincpp)                                  100             1     51.987 us
pow (BigInt)                                   100             1    4.02956 ms
pow (bigint)                                   100             1    5.38032 ms
pow (BigNumber)                                100             1    20.8601 ms
sqrt (pyincpp)                                 100             1    149.425 us
sqrt (BigInt)                                  100             1    2.91216 ms
sqrt (infint)                                  100             1    1.01735 ms
random (pyincpp)                               100             1    308.696 us
random (BigInt)                                100             2     9.6545 us
factorial (pyincpp)                            100             1     33.216 us
factorial (bigint)                             100             2    11.9695 us

===============================================================================
*/
