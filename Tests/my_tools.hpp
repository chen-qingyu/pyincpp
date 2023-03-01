#ifndef MY_TOOLS_HPP
#define MY_TOOLS_HPP

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

#endif // MY_TOOLS_HPP
