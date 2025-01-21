#include <format>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../sources/pyincpp.hpp"

// Some big integer libraries with more than 100 stars on Github.
#include "BigInt.hpp"  // https://github.com/faheel/BigInt
#include "InfInt.h"    // https://github.com/sercantutar/infint
#include "bigint.h"    // https://github.com/kasparsklavins/bigint
#include "bignumber.h" // https://github.com/Limeoats/BigNumber

template <typename T>
inline void constructor(const char* a)
{
    REQUIRE(T(a) == T("987654321000987654321000987654321000987654321000"));
    BENCHMARK(std::format("ctor ({})", typeid(T).name()))
    {
        return T(a);
    };
}

template <typename T>
inline void add(T a, T b)
{
    REQUIRE(a + b == T("1111111110001111111110001111111110001111111110000"));
    BENCHMARK(std::format("+ ({})", typeid(T).name()))
    {
        return a + b;
    };
}

template <typename T>
inline void sub(T a, T b)
{
    REQUIRE(a - b == T("864197532000864197532000864197532000864197532000"));
    BENCHMARK(std::format("- ({})", typeid(T).name()))
    {
        return a - b;
    };
}

template <typename T>
inline void mul(T a, T b)
{
    REQUIRE(a * b == T("121932631112879134262225636335893338393537524450906873893338149672262225392470631112635269000000"));
    BENCHMARK(std::format("* ({})", typeid(T).name()))
    {
        return a * b;
    };
}

template <typename T>
inline void div(T a, T b)
{
    REQUIRE(a / b == T("8"));
    BENCHMARK(std::format("/ ({})", typeid(T).name()))
    {
        return a / b;
    };
}

template <typename T>
inline void mod(T a, T b)
{
    REQUIRE(a % b == T("9000000000009000000000009000000000009000"));
    BENCHMARK(std::format("% ({})", typeid(T).name()))
    {
        return a % b;
    };
}

template <typename T>
inline void inc(T a)
{
    REQUIRE(++a == T("987654321000987654321000987654321000987654321001"));
    BENCHMARK(std::format("++ ({})", typeid(T).name()))
    {
        return ++a;
    };
}

template <typename T>
inline void cmp(T a, T b)
{
    REQUIRE(a > b);
    BENCHMARK(std::format("> ({})", typeid(T).name()))
    {
        return a > b;
    };
}

TEST_CASE("pyincpp::Int vs Other", "[int]")
{
    const char* a = "987654321000987654321000987654321000987654321000";
    const char* b = "123456789000123456789000123456789000123456789000";

    pyincpp::Int a1(a), b1(b);
    BigInt a2(a), b2(b);
    InfInt a3(a), b3(b);
    Dodecahedron::Bigint a4(a), b4(b);
    BigNumber a5(a), b5(b);

    constructor<pyincpp::Int>(a);
    constructor<BigInt>(a);
    constructor<InfInt>(a);
    constructor<Dodecahedron::Bigint>(a);
    constructor<BigNumber>(a);

    add<pyincpp::Int>(a1, b1);
    add<BigInt>(a2, b2);
    add<InfInt>(a3, b3);
    add<Dodecahedron::Bigint>(a4, b4);
    add<BigNumber>(a5, b5);

    sub<pyincpp::Int>(a1, b1);
    sub<BigInt>(a2, b2);
    sub<InfInt>(a3, b3);
    sub<Dodecahedron::Bigint>(a4, b4);
    sub<BigNumber>(a5, b5);

    mul<pyincpp::Int>(a1, b1);
    mul<BigInt>(a2, b2);
    mul<InfInt>(a3, b3);
    mul<Dodecahedron::Bigint>(a4, b4);
    mul<BigNumber>(a5, b5);

    div<pyincpp::Int>(a1, b1);
    div<BigInt>(a2, b2);
    div<InfInt>(a3, b3);
    // div<Dodecahedron::Bigint>(a4, b4); // not supported
    div<BigNumber>(a5, b5);

    mod<pyincpp::Int>(a1, b1);
    mod<BigInt>(a2, b2);
    mod<InfInt>(a3, b3);
    // mod<Dodecahedron::Bigint>(a4, b4); // not supported
    // mod<BigNumber>(a5, b5); // not supported

    inc<pyincpp::Int>(a1);
    inc<BigInt>(a2);
    inc<InfInt>(a3);
    // inc<Dodecahedron::Bigint>(a4); // not supported
    inc<BigNumber>(a5);

    cmp<pyincpp::Int>(a1, b1);
    cmp<BigInt>(a2, b2);
    cmp<InfInt>(a3, b3);
    cmp<Dodecahedron::Bigint>(a4, b4);
    cmp<BigNumber>(a5, b5);

    // Now, it is not convenient to use template functions, and some classes do not support the relevant function is omitted.

    REQUIRE(pyincpp::Int::gcd(a1, b1) == "9000000000009000000000009000000000009000");
    BENCHMARK(std::format("gcd ({})", typeid(pyincpp::Int).name()))
    {
        return pyincpp::Int::gcd(a1, b1);
    };
    REQUIRE(gcd(a2, b2) == "9000000000009000000000009000000000009000");
    BENCHMARK(std::format("gcd ({})", typeid(BigInt).name()))
    {
        return gcd(a2, b2);
    };

    REQUIRE(pyincpp::Int::pow(a1, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK(std::format("pow ({})", typeid(pyincpp::Int).name()))
    {
        return pyincpp::Int::pow(a1, 10);
    };
    REQUIRE(pow(a2, 10) == "883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000");
    BENCHMARK(std::format("pow ({})", typeid(BigInt).name()))
    {
        return pow(a2, 10);
    };
    REQUIRE(Dodecahedron::Bigint(a4).pow(10) == std::string("883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000"));
    BENCHMARK(std::format("pow ({})", typeid(Dodecahedron::Bigint).name()))
    {
        return Dodecahedron::Bigint(a4).pow(10); // Dodecahedron::Bigint::pow will modify this
    };
    REQUIRE(a5.pow(10) == std::string("883180926273197021952264207133587213024757297231319710324392613444786407476937172097269547752144587748034859385950214986131578524631947404097159758634059531453759956655784361184056650195934688971795597131494125041289325199580638365345730937031303573757092905409889931409326947349999958418006278835787126126617733545641071231406892102718589117039785666092599334122903993297569621489728460437976845055240356743070703495190012103572275987661566940511201000000000000000000000000000000"));
    BENCHMARK(std::format("pow ({})", typeid(BigNumber).name()))
    {
        return a5.pow(10);
    };

    REQUIRE(pyincpp::Int::sqrt(a1) == "993807990006614735669893");
    BENCHMARK(std::format("sqrt ({})", typeid(pyincpp::Int).name()))
    {
        return pyincpp::Int::sqrt(a1);
    };
    REQUIRE(sqrt(a2) == "993807990006614735669893");
    BENCHMARK(std::format("sqrt ({})", typeid(BigInt).name()))
    {
        return sqrt(a2);
    };
    REQUIRE(a3.intSqrt() == "993807990006614735669893");
    BENCHMARK(std::format("sqrt ({})", typeid(InfInt).name()))
    {
        return a3.intSqrt();
    };

    REQUIRE(pyincpp::Int::random(1000).digits() == 1000);
    BENCHMARK(std::format("rand ({})", typeid(pyincpp::Int).name()))
    {
        return pyincpp::Int::random(1000);
    };
    REQUIRE(big_random(1000).to_string().size() == 1000);
    BENCHMARK(std::format("rand ({})", typeid(BigInt).name()))
    {
        return big_random(1000);
    };

    REQUIRE(pyincpp::Int(120).factorial() == "6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000");
    BENCHMARK(std::format("fac ({})", typeid(pyincpp::Int).name()))
    {
        return pyincpp::Int(120).factorial();
    };
    REQUIRE(Dodecahedron::factorial(120) == std::string("6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000"));
    BENCHMARK(std::format("fac ({})", typeid(Dodecahedron::Bigint).name()))
    {
        return Dodecahedron::factorial(120);
    };
}

/*
Run with: `xmake config -m release && xmake build bench && xmake run bench --benchmark-no-analysis -i [int]`

Result (2025.01.22, Windows 10, MSVC, Catch2 v3.7.1):

-------------------------------------------------------------------------------
pyincpp::Int vs Other
-------------------------------------------------------------------------------
benches\pyincpp_int_vs_other_int.cpp(12)
...............................................................................

benchmark name                            samples    iterations          mean
-------------------------------------------------------------------------------
ctor (class pyincpp::Int)                      100           143     96.007 ns
ctor (class BigInt)                            100           139    102.317 ns
ctor (class InfInt)                            100            78    173.628 ns
ctor (class Dodecahedron::Bigint)              100            62    217.065 ns
ctor (class BigNumber)                         100           236    58.4788 ns
+ (class pyincpp::Int)                         100           160     91.475 ns
+ (class BigInt)                               100            10     1.6001 us
+ (class InfInt)                               100           188    86.7606 ns
+ (class Dodecahedron::Bigint)                 100           322    43.6211 ns
+ (class BigNumber)                            100            13    1.23977 us
- (class pyincpp::Int)                         100           121    114.074 ns
- (class BigInt)                               100             9    1.44844 us
- (class InfInt)                               100           198    81.9242 ns
- (class Dodecahedron::Bigint)                 100           356    38.2079 ns
- (class BigNumber)                            100             1     17.256 us
* (class pyincpp::Int)                         100            77    178.455 ns
* (class BigInt)                               100             1     56.818 us
* (class InfInt)                               100            66    215.742 ns
* (class Dodecahedron::Bigint)                 100            20     662.65 ns
* (class BigNumber)                            100             1    100.011 us
/ (class pyincpp::Int)                         100            34    406.735 ns
/ (class BigInt)                               100             1     21.111 us
/ (class InfInt)                               100             2     9.2145 us
/ (class BigNumber)                            100             1    135.492 us
% (class pyincpp::Int)                         100            34    404.324 ns
% (class BigInt)                               100             1     34.249 us
% (class InfInt)                               100             2      9.144 us
++ (class pyincpp::Int)                        100           508     26.811 ns
++ (class BigInt)                              100            10     1.4853 us
++ (class InfInt)                              100           359    37.8496 ns
++ (class BigNumber)                           100            12    1.15642 us
> (class pyincpp::Int)                         100          8150    1.65853 ns
> (class BigInt)                               100          2748    5.01456 ns
> (class InfInt)                               100         10800    1.24796 ns
> (class Dodecahedron::Bigint)                 100          9183    1.51029 ns
> (class BigNumber)                            100            68    200.471 ns
gcd (class pyincpp::Int)                       100             8    1.84112 us
gcd (class BigInt)                             100             1    104.218 us
pow (class pyincpp::Int)                       100             3      6.264 us
pow (class BigInt)                             100             1     1.8282 ms
pow (class Dodecahedron::Bigint)               100             1     18.511 us
pow (class BigNumber)                          100             1    10.1835 ms
sqrt (class pyincpp::Int)                      100             1     96.477 us
sqrt (class BigInt)                            100             1    1.49139 ms
sqrt (class InfInt)                            100             1    418.793 us
rand (class pyincpp::Int)                      100             7    2.02043 us
rand (class BigInt)                            100             3    4.71167 us
fac (class pyincpp::Int)                       100             2    10.0405 us
fac (class Dodecahedron::Bigint)               100             5     3.0828 us

===============================================================================
*/
