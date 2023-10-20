// Roughly compare the speed of two integer classes: mds::Integer vs BigInt

#include <gtest/gtest.h>

#include "../../Sources/Integer.hpp" // mds::Integer
#include "BigInt.hpp"                // https://github.com/faheel/BigInt

using namespace mds;

#define LOOP 100

TEST(Speed, bigint_add)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("18446744073709551616") + BigInt("18446744073709551616"), BigInt("36893488147419103232"));
}

TEST(Speed, integer_add)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("18446744073709551616") + Integer("18446744073709551616"), Integer("36893488147419103232"));
}

TEST(Speed, bigint_sub)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("36893488147419103232") - BigInt("18446744073709551616"), BigInt("18446744073709551616"));
}

TEST(Speed, integer_sub)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("36893488147419103232") - Integer("18446744073709551616"), Integer("18446744073709551616"));
}

TEST(Speed, bigint_mul)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("18446744073709551616") * BigInt("2"), BigInt("36893488147419103232"));
}

TEST(Speed, integer_mul)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("18446744073709551616") * Integer("2"), Integer("36893488147419103232"));
}

TEST(Speed, bigint_div)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("36893488147419103232") / BigInt("2"), BigInt("18446744073709551616"));
}

TEST(Speed, integer_div)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("36893488147419103232") / Integer("2"), Integer("18446744073709551616"));
}

TEST(Speed, bigint_mod)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(BigInt("36893488147419103232") % BigInt("2"), BigInt("0"));
}

TEST(Speed, integer_mod)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("36893488147419103232") % Integer("2"), Integer("0"));
}

TEST(Speed, bigint_inc_dec)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(++BigInt("36893488147419103232"), BigInt("36893488147419103233"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(--BigInt("36893488147419103232"), BigInt("36893488147419103231"));
}

TEST(Speed, integer_inc_dec)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(++Integer("36893488147419103232"), Integer("36893488147419103233"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(--Integer("36893488147419103232"), Integer("36893488147419103231"));
}

TEST(Speed, bigint_compare)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(BigInt("36893488147419103232000") == BigInt("36893488147419103232000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(BigInt("36893488147419103232000") > BigInt("18446744073709551616000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(BigInt("36893488147419103232") < BigInt("18446744073709551616000"));
}

TEST(Speed, integer_compare)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(Integer("36893488147419103232000") == Integer("36893488147419103232000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(Integer("36893488147419103232000") > Integer("18446744073709551616000"));

    for (int i = 0; i < LOOP; i++)
        ASSERT_TRUE(Integer("36893488147419103232") < Integer("18446744073709551616000"));
}

TEST(Speed, bigint_gcd_lcm)
{
    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(gcd(BigInt("12345678987654321"), BigInt("98765432123456789")), BigInt("1"));

    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(lcm(BigInt("12345678987654321"), BigInt("98765432123456789")), BigInt("1219326320073159566072245112635269"));
}

TEST(Speed, integer_gcd_lcm)
{
    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(gcd(Integer("12345678987654321"), Integer("98765432123456789")), Integer("1"));

    for (int i = 0; i < LOOP / 2; i++)
        ASSERT_EQ(lcm(Integer("12345678987654321"), Integer("98765432123456789")), Integer("1219326320073159566072245112635269"));
}

TEST(Speed, bigint_pow)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(pow(BigInt("2"), 100), BigInt("1267650600228229401496703205376"));
}

TEST(Speed, integer_pow)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("2").pow(Integer("100")), Integer("1267650600228229401496703205376"));
}

TEST(Speed, bigint_sqrt)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(sqrt(BigInt("9801")), BigInt("99"));
}

TEST(Speed, integer_sqrt)
{
    for (int i = 0; i < LOOP; i++)
        ASSERT_EQ(Integer("9801").sqrt(), Integer("99"));
}

/*
Run results (update 2023.10.19):

[ RUN      ] Speed.bigint_add
[       OK ] Speed.bigint_add (2 ms)
[ RUN      ] Speed.integer_add
[       OK ] Speed.integer_add (0 ms)
[ RUN      ] Speed.bigint_sub
[       OK ] Speed.bigint_sub (3 ms)
[ RUN      ] Speed.integer_sub
[       OK ] Speed.integer_sub (0 ms)
[ RUN      ] Speed.bigint_mul
[       OK ] Speed.bigint_mul (35 ms)
[ RUN      ] Speed.integer_mul
[       OK ] Speed.integer_mul (0 ms)
[ RUN      ] Speed.bigint_div
[       OK ] Speed.bigint_div (108 ms)
[ RUN      ] Speed.integer_div
[       OK ] Speed.integer_div (15 ms)
[ RUN      ] Speed.bigint_mod
[       OK ] Speed.bigint_mod (139 ms)
[ RUN      ] Speed.integer_mod
[       OK ] Speed.integer_mod (15 ms)
[ RUN      ] Speed.bigint_inc_dec
[       OK ] Speed.bigint_inc_dec (7 ms)
[ RUN      ] Speed.integer_inc_dec
[       OK ] Speed.integer_inc_dec (0 ms)
[ RUN      ] Speed.bigint_compare
[       OK ] Speed.bigint_compare (1 ms)
[ RUN      ] Speed.integer_compare
[       OK ] Speed.integer_compare (0 ms)
[ RUN      ] Speed.bigint_gcd_lcm
[       OK ] Speed.bigint_gcd_lcm (16 ms)
[ RUN      ] Speed.integer_gcd_lcm
[       OK ] Speed.integer_gcd_lcm (30 ms)
[ RUN      ] Speed.bigint_pow
[       OK ] Speed.bigint_pow (80 ms)
[ RUN      ] Speed.integer_pow
[       OK ] Speed.integer_pow (7 ms)
[ RUN      ] Speed.bigint_sqrt
[       OK ] Speed.bigint_sqrt (26 ms)
[ RUN      ] Speed.integer_sqrt
[       OK ] Speed.integer_sqrt (23 ms)
*/
