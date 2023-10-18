#ifndef TOOL_HPP
#define TOOL_HPP

#include <sstream> // std::ostringstream std::istringstream
#include <string>  // std::string std::to_string

#include <gtest/gtest.h>

#define MY_ASSERT_THROWS_MESSAGE(expression, exception, message) \
    ASSERT_THROW(expression, exception);                         \
    try                                                          \
    {                                                            \
        expression;                                              \
    }                                                            \
    catch (const exception& e)                                   \
    {                                                            \
        ASSERT_STREQ(e.what(), message);                         \
    }

#endif // TOOL_HPP
