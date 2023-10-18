#include "../Sources/Deque.hpp"

#include "tool.hpp"

using namespace mdspp;

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

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST(Deque, compare)
{
    Deque<int> deque({1, 2, 3, 4, 5});

    // operator==
    Deque<int> eq_deque({1, 2, 3, 4, 5});
    ASSERT_TRUE(eq_deque == deque);

    // operator!=
    Deque<int> ne_deque({1, 3, 5});
    ASSERT_TRUE(ne_deque != deque);

    // operator<
    Deque<int> lt_deque({1, 2, 3});
    Deque<int> lt_deque2({0, 9, 9, 9, 9});
    ASSERT_TRUE(lt_deque < deque);
    ASSERT_TRUE(lt_deque2 < deque);

    // operator<=
    ASSERT_TRUE(lt_deque <= deque);
    ASSERT_TRUE(lt_deque2 <= deque);
    ASSERT_TRUE(eq_deque <= deque);

    // operator>
    Deque<int> gt_deque({1, 2, 3, 4, 5, 6});
    Deque<int> gt_deque2({2});
    ASSERT_TRUE(gt_deque > deque);
    ASSERT_TRUE(gt_deque2 > deque);

    // operator>=
    ASSERT_TRUE(gt_deque >= deque);
    ASSERT_TRUE(gt_deque2 >= deque);
    ASSERT_TRUE(eq_deque >= deque);
}

// operator=()
TEST(Deque, copy_assignment)
{
    Deque<int> deque1 = {1, 2, 3, 4, 5};
    Deque<int> deque2 = {6, 7, 8, 9};

    deque1 = deque2;
    ASSERT_EQ(deque1, Deque<int>({6, 7, 8, 9}));
    ASSERT_EQ(deque2, Deque<int>({6, 7, 8, 9}));
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

// back() front()
TEST(Deque, peek)
{
    Deque<int> empty;

    MY_ASSERT_THROWS_MESSAGE(empty.back(), std::runtime_error, "ERROR: The container is empty.");

    MY_ASSERT_THROWS_MESSAGE(empty.front(), std::runtime_error, "ERROR: The container is empty.");

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

    MY_ASSERT_THROWS_MESSAGE(empty.pop_back(), std::runtime_error, "ERROR: The container is empty.");

    MY_ASSERT_THROWS_MESSAGE(empty.pop_front(), std::runtime_error, "ERROR: The container is empty.");

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

// reverse()
TEST(Deque, reverse)
{
    ASSERT_EQ(Deque<int>().reverse(), Deque<int>());
    ASSERT_EQ(Deque<int>({1}).reverse(), Deque<int>({1}));
    ASSERT_EQ(Deque<int>({1, 2, 3, 4, 5}).reverse(), Deque<int>({5, 4, 3, 2, 1}));
}

// to_list() to_set()
TEST(Deque, to_list_set)
{
    ASSERT_EQ(Deque<int>({1, 2, 3, 4, 5}).to_list(), List<int>({1, 2, 3, 4, 5}));

    ASSERT_EQ(Deque<int>({1, 2, 3, 4, 5}).to_set(), Set<int>({1, 2, 3, 4, 5}));
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
