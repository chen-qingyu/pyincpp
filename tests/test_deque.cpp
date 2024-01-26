#include "../sources/Deque.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Deque")
{
    // constructor destructor size() is_empty()
    SECTION("basics")
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

    Deque<int> empty({});
    Deque<int> one({1});
    Deque<int> some({1, 2, 3, 4, 5});

    // operator==() operator!=() operator<() operator<=() operator>() operator>=()
    SECTION("compare")
    {
        // operator==
        Deque<int> eq_deque({1, 2, 3, 4, 5});
        REQUIRE(eq_deque == some);

        // operator!=
        Deque<int> ne_deque({1, 3, 5});
        REQUIRE(ne_deque != some);

        // operator<
        Deque<int> lt_deque({1, 2, 3});
        Deque<int> lt_deque2({0, 9, 9, 9, 9});
        REQUIRE(lt_deque < some);
        REQUIRE(lt_deque2 < some);

        // operator<=
        REQUIRE(lt_deque <= some);
        REQUIRE(lt_deque2 <= some);
        REQUIRE(eq_deque <= some);

        // operator>
        Deque<int> gt_deque({1, 2, 3, 4, 5, 6});
        Deque<int> gt_deque2({2});
        REQUIRE(gt_deque > some);
        REQUIRE(gt_deque2 > some);

        // operator>=
        REQUIRE(gt_deque >= some);
        REQUIRE(gt_deque2 >= some);
        REQUIRE(eq_deque >= some);
    }

    // operator=()
    SECTION("copy_assignment")
    {
        some = one;
        REQUIRE(some == Deque<int>({1}));
        REQUIRE(one == Deque<int>({1}));
    }

    // operator=()
    SECTION("move_assignment")
    {
        some = std::move(one);
        REQUIRE(some == Deque<int>({1}));
        REQUIRE(one == Deque<int>());
    }

    // back() front()
    SECTION("peek")
    {
        REQUIRE_THROWS_MATCHES(empty.back(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(empty.front(), std::runtime_error, Message("Error: The container is empty."));

        REQUIRE(some.back() == 5);
        REQUIRE(some.front() == 1);

        REQUIRE(++some.back() == 6);
        REQUIRE(--some.front() == 0);
    }

    // push_back() push_front() pop_back() pop_front()
    SECTION("push_pop")
    {
        REQUIRE_THROWS_MATCHES(empty.pop_back(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(empty.pop_front(), std::runtime_error, Message("Error: The container is empty."));

        const int size = 99;

        // push_back
        for (int i = 0; i <= size; ++i)
        {
            empty.push_back(i);
        }
        REQUIRE(empty.size() == size + 1);

        // pop_back
        for (int i = 0; i <= size; ++i)
        {
            REQUIRE(empty.pop_back() == size - i);
        }
        REQUIRE(empty.size() == 0);

        // push_front
        for (int i = 0; i <= size; ++i)
        {
            empty.push_front(i);
        }
        REQUIRE(empty.size() == size + 1);

        // pop_front
        for (int i = 0; i <= size; ++i)
        {
            REQUIRE(empty.pop_front() == size - i);
        }
        REQUIRE(empty.size() == 0);
    }

    // clear()
    SECTION("clear")
    {
        REQUIRE(some.clear() == Deque<int>());

        // double clear
        REQUIRE(some.clear() == Deque<int>());

        // modify after clear
        some.push_back(233);
        REQUIRE(some == Deque<int>({233}));
    }

    // operator>>=() operator<<=()
    SECTION("rotate")
    {
        REQUIRE((empty >>= 1) == Deque<int>());
        REQUIRE((empty >>= 2) == Deque<int>());
        REQUIRE((empty <<= 1) == Deque<int>());
        REQUIRE((empty <<= 2) == Deque<int>());

        empty.push_back(1);

        REQUIRE((empty >>= 1) == Deque<int>({1}));
        REQUIRE((empty >>= 2) == Deque<int>({1}));
        REQUIRE((empty <<= 1) == Deque<int>({1}));
        REQUIRE((empty <<= 2) == Deque<int>({1}));

        empty.push_back(2);
        empty.push_back(3);
        empty.push_back(4);
        empty.push_back(5);

        REQUIRE((empty >>= 1) == Deque<int>({5, 1, 2, 3, 4}));
        REQUIRE((empty >>= 2) == Deque<int>({3, 4, 5, 1, 2}));
        REQUIRE((empty <<= 1) == Deque<int>({4, 5, 1, 2, 3}));
        REQUIRE((empty <<= 2) == Deque<int>({1, 2, 3, 4, 5}));

        REQUIRE((empty >>= 233) == Deque<int>({3, 4, 5, 1, 2}));
        REQUIRE((empty >>= -233) == Deque<int>({1, 2, 3, 4, 5}));

        REQUIRE((empty <<= 233) == Deque<int>({4, 5, 1, 2, 3}));
        REQUIRE((empty <<= -233) == Deque<int>({1, 2, 3, 4, 5}));
    }

    // reverse()
    SECTION("reverse")
    {
        REQUIRE(Deque<int>().reverse() == Deque<int>());
        REQUIRE(Deque<int>({1}).reverse() == Deque<int>({1}));
        REQUIRE(Deque<int>({1, 2, 3, 4, 5}).reverse() == Deque<int>({5, 4, 3, 2, 1}));
    }

    // to_list() to_set()
    SECTION("to_list_set")
    {
        REQUIRE(Deque<int>({1, 2, 3, 4, 5}).to_list() == List<int>({1, 2, 3, 4, 5}));

        REQUIRE(Deque<int>({1, 2, 3, 4, 5}).to_set() == Set<int>({1, 2, 3, 4, 5}));
    }

    // operator<<()
    SECTION("print")
    {
        std::ostringstream oss;

        oss << empty;
        REQUIRE(oss.str() == "<>");
        oss.str("");

        oss << one;
        REQUIRE(oss.str() == "<1>");
        oss.str("");

        oss << some;
        REQUIRE(oss.str() == "<1, 2, 3, 4, 5>");
        oss.str("");
    }
}
