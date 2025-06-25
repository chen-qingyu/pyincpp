#ifndef TOOL_HPP
#define TOOL_HPP

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

using Catch::Approx;
using Catch::Matchers::Message;

// 1. Only overloaded == to test compatibility.
// 2. No default constructor to test compatibility.
struct EqType
{
    EqType(int)
    {
    }
    bool operator==(const EqType& that) const
    {
        return true;
    }
};

// 1. Only overloaded == and < to test compatibility.
// 2. No default constructor to test compatibility.
struct EqLtType
{
    int value;
    EqLtType(int v)
        : value(v)
    {
    }
    bool operator==(const EqLtType& that) const
    {
        return value == that.value;
    }
    bool operator<(const EqLtType& that) const
    {
        return value < that.value;
    }
};

#endif // TOOL_HPP
