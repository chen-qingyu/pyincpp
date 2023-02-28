#ifndef MY_TOOLS_HPP
#define MY_TOOLS_HPP

#define MY_ASSERT_THROWS_MESSAGE(expr, exception, msg) \
    ASSERT_THROW(expr, exception);                     \
    try                                                \
    {                                                  \
        expr;                                          \
    }                                                  \
    catch (const exception& e)                         \
    {                                                  \
        ASSERT_STREQ(e.what(), msg);                   \
    }

#endif // MY_TOOLS_HPP
