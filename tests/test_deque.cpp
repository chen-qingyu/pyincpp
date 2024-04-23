#include "../sources/Deque.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Deque")
{
    SECTION("basics")
    {
        // Deque()
        Deque<int> deque1;
        REQUIRE(deque1.size() == 0);
        REQUIRE(deque1.is_empty());

        // Deque(const std::initializer_list<T>& il)
        Deque<int> deque2 = {1, 2, 3, 4, 5};
        REQUIRE(deque2.size() == 5);
        REQUIRE(!deque2.is_empty());

        // Deque(const InputIt& first, const InputIt& last)
        Deque<int> deque3(deque2.begin(), deque2.end());
        REQUIRE(deque3.size() == 5);
        REQUIRE(!deque3.is_empty());

        // Deque(const Deque& that)
        Deque<int> deque4(deque3);
        REQUIRE(deque4.size() == 5);
        REQUIRE(!deque4.is_empty());

        // Deque(Deque&& that)
        Deque<int> deque5(std::move(deque4));
        REQUIRE(deque5.size() == 5);
        REQUIRE(!deque5.is_empty());

        // ~Deque()
    }

    Deque<int> empty({});
    Deque<int> one({1});
    Deque<int> some({1, 2, 3, 4, 5});

    SECTION("compare")
    {
        // operator==
        Deque<int> eq_deque({1, 2, 3, 4, 5});
        REQUIRE(eq_deque == some);

        // operator!=
        Deque<int> ne_deque({1, 3, 5});
        REQUIRE(ne_deque != some);

        // operator<
        Deque<int> lt_deque({0, 9, 9, 9, 9});
        REQUIRE(lt_deque < some);

        // operator<=
        REQUIRE(lt_deque <= some);
        REQUIRE(eq_deque <= some);

        // operator>
        Deque<int> gt_deque({2});
        REQUIRE(gt_deque > some);

        // operator>=
        REQUIRE(gt_deque >= some);
        REQUIRE(eq_deque >= some);
    }

    SECTION("assignment")
    {
        some = one; // copy
        REQUIRE(some == Deque<int>({1}));
        REQUIRE(one == Deque<int>({1}));

        empty = std::move(one); // move
        REQUIRE(empty == Deque<int>({1}));
        REQUIRE(one == Deque<int>());
    }

    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());

        // for in
        for (int i = 1; const auto& e : some)
        {
            REQUIRE(e == i++);
        }
    }

    SECTION("access")
    {
        REQUIRE_THROWS_MATCHES(empty.back(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(empty.front(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(empty[0], std::runtime_error, Message("Error: Index out of range."));

        REQUIRE(some.back() == 5);
        REQUIRE(some.front() == 1);

        REQUIRE(++some.back() == 6);
        REQUIRE(--some.front() == 0);

        REQUIRE(some[-1] == 6);
        REQUIRE(some[0] == 0);
    }

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

    SECTION("clear")
    {
        REQUIRE(some.clear() == Deque<int>());
        REQUIRE(some.clear() == Deque<int>()); // double clear
    }

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

    SECTION("reverse")
    {
        REQUIRE(Deque<int>().reverse() == Deque<int>());
        REQUIRE(Deque<int>({1}).reverse() == Deque<int>({1}));
        REQUIRE(Deque<int>({1, 2, 3, 4, 5}).reverse() == Deque<int>({5, 4, 3, 2, 1}));
    }

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
