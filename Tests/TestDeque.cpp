// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/Deque.hpp"

using mdspp::Deque;

// constructor destructor size() is_empty()
TEST(Deque, basics)
{
    // Deque()
    Deque<int> deque1;
    ASSERT_EQ(deque1.size(), 0);
    ASSERT_TRUE(deque1.is_empty());

    // Deque(const std::initializer_list<T> &il)
    Deque<int> deque2 = {1, 2, 3, 4, 5};
    ASSERT_EQ(deque2.size(), 5);
    ASSERT_FALSE(deque2.is_empty());

    // Deque(const Deque<T> &that)
    Deque<int> deque3(deque2);
    ASSERT_EQ(deque3.size(), 5);
    ASSERT_FALSE(deque3.is_empty());

    // Deque(Deque<T> &&that)
    Deque<int> deque4(std::move(deque3));
    ASSERT_EQ(deque4.size(), 5);
    ASSERT_FALSE(deque4.is_empty());
    ASSERT_EQ(deque3.size(), 0);
    ASSERT_TRUE(deque3.is_empty());

    // ~Deque()
}

// operator=()
TEST(Deque, copy_assignment)
{
    Deque<int> deque1 = {1, 2, 3, 4, 5};
    Deque<int> deque2 = {6, 7, 8, 9};

    deque1 = deque2;
    deque2.push_back(10);
    ASSERT_EQ(deque1, Deque<int>({6, 7, 8, 9}));
    ASSERT_EQ(deque2, Deque<int>({6, 7, 8, 9, 10}));
}

// operator=()
TEST(Deque, move_assignment)
{
    Deque<int> deque1 = {1, 2, 3, 4, 5};
    Deque<int> deque2 = {6, 7, 8, 9};

    deque1 = std::move(deque2);
    ASSERT_EQ(deque1, Deque<int>({6, 7, 8, 9}));
    ASSERT_EQ(deque2, Deque<int>());
}

// operator==() operator!=()
TEST(Deque, compare)
{
    Deque<int> deque = {1, 2, 3, 4, 5};

    // operator==
    ASSERT_TRUE(deque == Deque<int>({1, 2, 3, 4, 5}));

    // operator!=
    ASSERT_TRUE(deque != Deque<int>({1, 3, 5}));
}

// back() front()
TEST(Deque, peek)
{
    Deque<int> empty;

    ASSERT_THROW(empty.back(), std::runtime_error);
    try
    {
        empty.back();
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }

    ASSERT_THROW(empty.front(), std::runtime_error);
    try
    {
        empty.front();
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }

    Deque<int> deque = {1, 2, 3, 4, 5};

    ASSERT_EQ(deque.back(), 5);
    ASSERT_EQ(deque.front(), 1);

    ASSERT_EQ(++deque.back(), 6);
    ASSERT_EQ(--deque.front(), 0);
}

// push_back() push_front() pop_back() pop_front()
TEST(Deque, push_pop)
{
    Deque<int> empty;

    ASSERT_THROW(empty.pop_back(), std::runtime_error);
    try
    {
        empty.pop_back();
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }

    ASSERT_THROW(empty.pop_front(), std::runtime_error);
    try
    {
        empty.pop_front();
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }

    Deque<int> deque;
    const int size = 99;

    // push_back
    for (int i = 0; i <= size; ++i)
    {
        deque.push_back(i);
    }
    ASSERT_EQ(deque.size(), size + 1);

    // pop_back
    for (int i = 0; i <= size; ++i)
    {
        ASSERT_EQ(deque.pop_back(), size - i);
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
        ASSERT_EQ(deque.pop_front(), size - i);
    }
    ASSERT_EQ(deque.size(), 0);
}

// clear()
TEST(Deque, clear)
{
    Deque<int> deque = {1, 2, 3, 4, 5};

    ASSERT_EQ(deque.clear(), Deque<int>());

    // double clear
    ASSERT_EQ(deque.clear(), Deque<int>());

    // modify after clear
    deque.push_back(233);
    ASSERT_EQ(deque, Deque<int>({233}));
}

// operator>>=() operator<<=()
TEST(Deque, rotate)
{
    Deque<int> deque;

    ASSERT_EQ(deque >>= 1, Deque<int>());
    ASSERT_EQ(deque >>= 2, Deque<int>());
    ASSERT_EQ(deque <<= 1, Deque<int>());
    ASSERT_EQ(deque <<= 2, Deque<int>());

    deque.push_back(1);

    ASSERT_EQ(deque >>= 1, Deque<int>({1}));
    ASSERT_EQ(deque >>= 2, Deque<int>({1}));
    ASSERT_EQ(deque <<= 1, Deque<int>({1}));
    ASSERT_EQ(deque <<= 2, Deque<int>({1}));

    deque.push_back(2);
    deque.push_back(3);
    deque.push_back(4);
    deque.push_back(5);

    ASSERT_EQ(deque >>= 1, Deque<int>({5, 1, 2, 3, 4}));
    ASSERT_EQ(deque >>= 2, Deque<int>({3, 4, 5, 1, 2}));
    ASSERT_EQ(deque <<= 1, Deque<int>({4, 5, 1, 2, 3}));
    ASSERT_EQ(deque <<= 2, Deque<int>({1, 2, 3, 4, 5}));

    ASSERT_EQ(deque >>= 233, Deque<int>({3, 4, 5, 1, 2}));
    ASSERT_EQ(deque >>= -233, Deque<int>({1, 2, 3, 4, 5}));

    ASSERT_EQ(deque <<= 233, Deque<int>({4, 5, 1, 2, 3}));
    ASSERT_EQ(deque <<= -233, Deque<int>({1, 2, 3, 4, 5}));
}

// operator<<()
TEST(Deque, print)
{
    std::ostringstream oss;

    Deque<int> empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "<>"); // string == char*, use eq
    oss.str("");

    Deque<int> one = {1};
    oss << one;
    ASSERT_EQ(oss.str(), "<1>");
    oss.str("");

    Deque<int> many = {1, 2, 3, 4, 5};
    oss << many;
    ASSERT_EQ(oss.str(), "<1, 2, 3, 4, 5>");
    oss.str("");
}
