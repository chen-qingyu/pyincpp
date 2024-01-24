#include "../sources/Deque.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor size() is_empty()
TEST_CASE("Deque: basics")
{
    // Deque()
    Deque<int> deque1;
    REQUIRE(deque1.size() == 0);
    REQUIRE(deque1.is_empty());

    // Deque(const std::initializer_list<T> &il)
    Deque<int> deque2 = {1, 2, 3, 4, 5};
    REQUIRE(deque2.size() == 5);
    REQUIRE(!deque2.is_empty());

    // Deque(const Deque<T> &that)
    Deque<int> deque3(deque2);
    REQUIRE(deque3.size() == 5);
    REQUIRE(!deque3.is_empty());

    // Deque(Deque<T> &&that)
    Deque<int> deque4(std::move(deque3));
    REQUIRE(deque4.size() == 5);
    REQUIRE(!deque4.is_empty());
    REQUIRE(deque3.size() == 0);
    REQUIRE(deque3.is_empty());

    // ~Deque()
}

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST_CASE("Deque: compare")
{
    Deque<int> deque({1, 2, 3, 4, 5});

    // operator==
    Deque<int> eq_deque({1, 2, 3, 4, 5});
    REQUIRE(eq_deque == deque);

    // operator!=
    Deque<int> ne_deque({1, 3, 5});
    REQUIRE(ne_deque != deque);

    // operator<
    Deque<int> lt_deque({1, 2, 3});
    Deque<int> lt_deque2({0, 9, 9, 9, 9});
    REQUIRE(lt_deque < deque);
    REQUIRE(lt_deque2 < deque);

    // operator<=
    REQUIRE(lt_deque <= deque);
    REQUIRE(lt_deque2 <= deque);
    REQUIRE(eq_deque <= deque);

    // operator>
    Deque<int> gt_deque({1, 2, 3, 4, 5, 6});
    Deque<int> gt_deque2({2});
    REQUIRE(gt_deque > deque);
    REQUIRE(gt_deque2 > deque);

    // operator>=
    REQUIRE(gt_deque >= deque);
    REQUIRE(gt_deque2 >= deque);
    REQUIRE(eq_deque >= deque);
}

// operator=()
TEST_CASE("Deque: copy_assignment")
{
    Deque<int> deque1 = {1, 2, 3, 4, 5};
    Deque<int> deque2 = {6, 7, 8, 9};

    deque1 = deque2;
    REQUIRE(deque1 == Deque<int>({6, 7, 8, 9}));
    REQUIRE(deque2 == Deque<int>({6, 7, 8, 9}));
}

// operator=()
TEST_CASE("Deque: move_assignment")
{
    Deque<int> deque1 = {1, 2, 3, 4, 5};
    Deque<int> deque2 = {6, 7, 8, 9};

    deque1 = std::move(deque2);
    REQUIRE(deque1 == Deque<int>({6, 7, 8, 9}));
    REQUIRE(deque2 == Deque<int>());
}

// back() front()
TEST_CASE("Deque: peek")
{
    Deque<int> empty;

    MY_ASSERT_THROW_MESSAGE(empty.back(), std::runtime_error, "Error: The container is empty.");

    MY_ASSERT_THROW_MESSAGE(empty.front(), std::runtime_error, "Error: The container is empty.");

    Deque<int> deque = {1, 2, 3, 4, 5};

    REQUIRE(deque.back() == 5);
    REQUIRE(deque.front() == 1);

    REQUIRE(++deque.back() == 6);
    REQUIRE(--deque.front() == 0);
}

// push_back() push_front() pop_back() pop_front()
TEST_CASE("Deque: push_pop")
{
    Deque<int> empty;

    MY_ASSERT_THROW_MESSAGE(empty.pop_back(), std::runtime_error, "Error: The container is empty.");

    MY_ASSERT_THROW_MESSAGE(empty.pop_front(), std::runtime_error, "Error: The container is empty.");

    Deque<int> deque;
    const int size = 99;

    // push_back
    for (int i = 0; i <= size; ++i)
    {
        deque.push_back(i);
    }
    REQUIRE(deque.size() == size + 1);

    // pop_back
    for (int i = 0; i <= size; ++i)
    {
        REQUIRE(deque.pop_back() == size - i);
    }
    REQUIRE(deque.size() == 0);

    // push_front
    for (int i = 0; i <= size; ++i)
    {
        deque.push_front(i);
    }
    REQUIRE(deque.size() == size + 1);

    // pop_front
    for (int i = 0; i <= size; ++i)
    {
        REQUIRE(deque.pop_front() == size - i);
    }
    REQUIRE(deque.size() == 0);
}

// clear()
TEST_CASE("Deque: clear")
{
    Deque<int> deque = {1, 2, 3, 4, 5};

    REQUIRE(deque.clear() == Deque<int>());

    // double clear
    REQUIRE(deque.clear() == Deque<int>());

    // modify after clear
    deque.push_back(233);
    REQUIRE(deque == Deque<int>({233}));
}

// operator>>=() operator<<=()
TEST_CASE("Deque: rotate")
{
    Deque<int> deque;

    REQUIRE((deque >>= 1) == Deque<int>());
    REQUIRE((deque >>= 2) == Deque<int>());
    REQUIRE((deque <<= 1) == Deque<int>());
    REQUIRE((deque <<= 2) == Deque<int>());

    deque.push_back(1);

    REQUIRE((deque >>= 1) == Deque<int>({1}));
    REQUIRE((deque >>= 2) == Deque<int>({1}));
    REQUIRE((deque <<= 1) == Deque<int>({1}));
    REQUIRE((deque <<= 2) == Deque<int>({1}));

    deque.push_back(2);
    deque.push_back(3);
    deque.push_back(4);
    deque.push_back(5);

    REQUIRE((deque >>= 1) == Deque<int>({5, 1, 2, 3, 4}));
    REQUIRE((deque >>= 2) == Deque<int>({3, 4, 5, 1, 2}));
    REQUIRE((deque <<= 1) == Deque<int>({4, 5, 1, 2, 3}));
    REQUIRE((deque <<= 2) == Deque<int>({1, 2, 3, 4, 5}));

    REQUIRE((deque >>= 233) == Deque<int>({3, 4, 5, 1, 2}));
    REQUIRE((deque >>= -233) == Deque<int>({1, 2, 3, 4, 5}));

    REQUIRE((deque <<= 233) == Deque<int>({4, 5, 1, 2, 3}));
    REQUIRE((deque <<= -233) == Deque<int>({1, 2, 3, 4, 5}));
}

// reverse()
TEST_CASE("Deque: reverse")
{
    REQUIRE(Deque<int>().reverse() == Deque<int>());
    REQUIRE(Deque<int>({1}).reverse() == Deque<int>({1}));
    REQUIRE(Deque<int>({1, 2, 3, 4, 5}).reverse() == Deque<int>({5, 4, 3, 2, 1}));
}

// to_list() to_set()
TEST_CASE("Deque: to_list_set")
{
    REQUIRE(Deque<int>({1, 2, 3, 4, 5}).to_list() == List<int>({1, 2, 3, 4, 5}));

    REQUIRE(Deque<int>({1, 2, 3, 4, 5}).to_set() == Set<int>({1, 2, 3, 4, 5}));
}

// operator<<()
TEST_CASE("Deque: print")
{
    std::ostringstream oss;

    Deque<int> empty;
    oss << empty;
    REQUIRE(oss.str() == "<>");
    oss.str("");

    Deque<int> one = {1};
    oss << one;
    REQUIRE(oss.str() == "<1>");
    oss.str("");

    Deque<int> many = {1, 2, 3, 4, 5};
    oss << many;
    REQUIRE(oss.str() == "<1, 2, 3, 4, 5>");
    oss.str("");
}
