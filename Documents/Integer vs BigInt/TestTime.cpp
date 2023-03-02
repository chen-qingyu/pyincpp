// Roughly compare the speed of two integer classes: mdspp::Integer vs BigInt
// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h" // GoogleTest pre-compiled header

#include "../Sources/Integer.hpp" // MDSPP

#define or ||         // for BigInt.hpp
#define and &&        // for BigInt.hpp
#include "BigInt.hpp" // https://github.com/faheel/BigInt

using namespace mdspp; // mdspp::*

#define LOOP 100

TEST(Integer, time_bigint_add)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("18446744073709551616") + BigInt("18446744073709551616"), BigInt("36893488147419103232"));
}

TEST(Integer, time_mdspp_add)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("18446744073709551616") + Integer("18446744073709551616"), Integer("36893488147419103232"));
}

TEST(Integer, time_bigint_sub)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("36893488147419103232") - BigInt("18446744073709551616"), BigInt("18446744073709551616"));
}

TEST(Integer, time_mdspp_sub)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("36893488147419103232") - Integer("18446744073709551616"), Integer("18446744073709551616"));
}

TEST(Integer, time_bigint_mul)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("18446744073709551616") * BigInt("2"), BigInt("36893488147419103232"));
}

TEST(Integer, time_mdspp_mul)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("18446744073709551616") * Integer("2"), Integer("36893488147419103232"));
}

TEST(Integer, time_bigint_div)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("36893488147419103232") / BigInt("2"), BigInt("18446744073709551616"));
}

TEST(Integer, time_mdspp_div)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("36893488147419103232") / Integer("2"), Integer("18446744073709551616"));
}

TEST(Integer, time_bigint_mod)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("36893488147419103232") % BigInt("2"), BigInt("0"));
}

TEST(Integer, time_mdspp_mod)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("36893488147419103232") % Integer("2"), Integer("0"));
}

TEST(Integer, time_bigint_inc_dec)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(++BigInt("36893488147419103232"), BigInt("36893488147419103233"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(--BigInt("36893488147419103232"), BigInt("36893488147419103231"));
}

TEST(Integer, time_mdspp_inc_dec)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(++Integer("36893488147419103232"), Integer("36893488147419103233"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(--Integer("36893488147419103232"), Integer("36893488147419103231"));
}

TEST(Integer, time_bigint_compare)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(BigInt("36893488147419103232000") == BigInt("36893488147419103232000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(BigInt("36893488147419103232000") > BigInt("18446744073709551616000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(BigInt("36893488147419103232") < BigInt("18446744073709551616000"));
}

TEST(Integer, time_mdspp_compare)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(Integer("36893488147419103232000") == Integer("36893488147419103232000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(Integer("36893488147419103232000") > Integer("18446744073709551616000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(Integer("36893488147419103232") < Integer("18446744073709551616000"));
}

TEST(Integer, time_bigint_gcd_lcm)
{
    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(gcd(BigInt("12345678987654321"), BigInt("98765432123456789")), BigInt("1"));

    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(lcm(BigInt("12345678987654321"), BigInt("98765432123456789")), BigInt("1219326320073159566072245112635269"));
}

TEST(Integer, time_mdspp_gcd_lcm)
{
    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(gcd(Integer("12345678987654321"), Integer("98765432123456789")), Integer("1"));

    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(lcm(Integer("12345678987654321"), Integer("98765432123456789")), Integer("1219326320073159566072245112635269"));
}

TEST(Integer, time_bigint_pow)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(pow(BigInt("2"), 100), BigInt("1267650600228229401496703205376"));
}

TEST(Integer, time_mdspp_pow)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("2").pow(Integer("100")), Integer("1267650600228229401496703205376"));
}

TEST(Integer, time_bigint_sqrt)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(sqrt(BigInt("9801")), BigInt("99"));
}

TEST(Integer, time_mdspp_sqrt)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("9801").sqrt(), Integer("99"));
}

/*
Run results:

[ RUN      ] Integer.time_bigint_add
[       OK ] Integer.time_bigint_add (5 ms)
[ RUN      ] Integer.time_mdspp_add
[       OK ] Integer.time_mdspp_add (1 ms)
[ RUN      ] Integer.time_bigint_sub
[       OK ] Integer.time_bigint_sub (5 ms)
[ RUN      ] Integer.time_mdspp_sub
[       OK ] Integer.time_mdspp_sub (1 ms)
[ RUN      ] Integer.time_bigint_mul
[       OK ] Integer.time_bigint_mul (52 ms)
[ RUN      ] Integer.time_mdspp_mul
[       OK ] Integer.time_mdspp_mul (1 ms)
[ RUN      ] Integer.time_bigint_div
[       OK ] Integer.time_bigint_div (162 ms)
[ RUN      ] Integer.time_mdspp_div
[       OK ] Integer.time_mdspp_div (28 ms)
[ RUN      ] Integer.time_bigint_mod
[       OK ] Integer.time_bigint_mod (195 ms)
[ RUN      ] Integer.time_mdspp_mod
[       OK ] Integer.time_mdspp_mod (29 ms)
[ RUN      ] Integer.time_bigint_inc_dec
[       OK ] Integer.time_bigint_inc_dec (9 ms)
[ RUN      ] Integer.time_mdspp_inc_dec
[       OK ] Integer.time_mdspp_inc_dec (2 ms)
[ RUN      ] Integer.time_bigint_compare
[       OK ] Integer.time_bigint_compare (2 ms)
[ RUN      ] Integer.time_mdspp_compare
[       OK ] Integer.time_mdspp_compare (2 ms)
[ RUN      ] Integer.time_bigint_gcd_lcm
[       OK ] Integer.time_bigint_gcd_lcm (25 ms)
[ RUN      ] Integer.time_mdspp_gcd_lcm
[       OK ] Integer.time_mdspp_gcd_lcm (58 ms)
[ RUN      ] Integer.time_bigint_pow
[       OK ] Integer.time_bigint_pow (105 ms)
[ RUN      ] Integer.time_mdspp_pow
[       OK ] Integer.time_mdspp_pow (13 ms)
[ RUN      ] Integer.time_bigint_sqrt
[       OK ] Integer.time_bigint_sqrt (32 ms)
[ RUN      ] Integer.time_mdspp_sqrt
[       OK ] Integer.time_mdspp_sqrt (41 ms)
*/
