#ifndef TOOL_HPP
#define TOOL_HPP

#include <sstream> // std::ostringstream std::istringstream
#include <string>  // std::string std::to_string

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#define MY_ASSERT_THROW_MESSAGE(expression, exception, message) \
    // ASSERT_THROW(expression, exception);                        \
    // try                                                         \
    // {                                                           \
    //     expression;                                             \
    // }                                                           \
    // catch (const exception& e)                                  \
    // {                                                           \
    //     ASSERT_STREQ(e.what(), message);                        \
    // }

#endif // TOOL_HPP
