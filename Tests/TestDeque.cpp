#include "pch.h"

#include <sstream> // std::ostringstream
#include <string>  // std::string std::to_string

#include "../Sources/Deque.hpp"

using mdspp::Deque;

// constructor destructor size() is_empty()
TEST(Deque, basics)
{
    // Deque()
    Deque<int> deque;
    ASSERT_EQ(deque.size(), 0);
    ASSERT_TRUE(deque.is_empty());
}

// operator=()
TEST(Deque, copy_assignment)
{
    Deque<int> deque1;
    Deque<int> deque2;

    deque2.push_back(1);
    deque2.push_back(2);
    deque2.push_back(3);

    deque1 = deque2;
    ASSERT_EQ(deque1, Deque<int>().push_back(1).push_back(2).push_back(3));
    ASSERT_EQ(deque2, Deque<int>().push_back(1).push_back(2).push_back(3));
}

// operator=()
TEST(Deque, move_assignment)
{
    Deque<int> deque1;
    Deque<int> deque2;

    deque2.push_back(1);
    deque2.push_back(2);
    deque2.push_back(3);

    deque1 = std::move(deque2);
    ASSERT_EQ(deque1, Deque<int>().push_back(1).push_back(2).push_back(3));
    ASSERT_EQ(deque2, Deque<int>());
}

// operator==() operator!=()
TEST(Deque, compare)
{
    Deque<int> deque;
    deque.push_back(1).push_back(2).push_back(3);

    // operator==
    Deque<int> eq_deque;
    eq_deque.push_back(1).push_back(2).push_back(3);
    ASSERT_TRUE(deque == eq_deque);

    // operator!=
    Deque<int> ne_deque;
    ne_deque.push_back(0);
    ASSERT_TRUE(deque != ne_deque);
}

// back() front()
TEST(Deque, peek)
{
    Deque<int> deque;
    deque.push_back(1).push_back(2).push_back(3);

    ASSERT_EQ(deque.back(), 3);
    ASSERT_EQ(deque.front(), 1);
}

// push_back() push_front() pop_back() pop_front()
TEST(Deque, push_pop)
{
    int size = 20;

    Deque<int> deque;

    // push_back
    for (int i = 0; i <= size; ++i)
    {
        deque.push_back(i);
    }
    ASSERT_EQ(deque.size(), size + 1);

    // pop_back
    for (int i = 0; i <= size; ++i)
    {
        ASSERT_EQ(deque.pop_back(), 20 - i);
    }
    ASSERT_EQ(deque.size(), 0);

    // push_front
    for (int i = 0; i <= size; ++i)
    {
        deque.push_front(i);
    }
    ASSERT_EQ(deque.size(), size + 1);

    // pop_front
    for (int i = 0; i <= size; ++i)
    {
        ASSERT_EQ(deque.pop_front(), 20 - i);
    }
    ASSERT_EQ(deque.size(), 0);
}

// clear()
TEST(Deque, clear)
{
    Deque<int> deque;
    deque.push_back(1).push_back(2).push_back(3);

    deque.clear();
    ASSERT_EQ(deque, Deque<int>());

    deque.clear(); // double clear
    ASSERT_EQ(deque, Deque<int>());
}

// operator>>=() operator<<=()
TEST(Deque, rotate)
{
    Deque<int> deque;
    deque.push_back(1).push_back(2).push_back(3).push_back(4).push_back(5);

    ASSERT_EQ(deque >>= 1, Deque<int>().push_back(5).push_back(1).push_back(2).push_back(3).push_back(4));
    ASSERT_EQ(deque >>= 2, Deque<int>().push_back(3).push_back(4).push_back(5).push_back(1).push_back(2));
    ASSERT_EQ(deque <<= 1, Deque<int>().push_back(4).push_back(5).push_back(1).push_back(2).push_back(3));
    ASSERT_EQ(deque <<= 2, Deque<int>().push_back(1).push_back(2).push_back(3).push_back(4).push_back(5));
}
