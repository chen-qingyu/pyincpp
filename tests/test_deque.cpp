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

        // Deque(const std::initializer_list<T>& init)
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
        REQUIRE(deque4.size() == 0);
        REQUIRE(deque4.is_empty());

        // test compatibility
        Deque<EqType> test = {1, 2, 3, 4, 5};
        REQUIRE(test.size() == 5);
        REQUIRE(!test.is_empty());

        // ~Deque()
    }

    Deque<int> empty;
    Deque<int> one = {1};
    Deque<int> some = {1, 2, 3, 4, 5};

    SECTION("compare")
    {
        // operator==
        Deque<int> eq_deque{1, 2, 3, 4, 5};
        REQUIRE(eq_deque == some);

        // operator!=
        Deque<int> ne_deque{1, 3, 5};
        REQUIRE(ne_deque != some);

        // operator<
        Deque<int> lt_deque{0, 9, 9, 9, 9};
        REQUIRE(lt_deque < some);

        // operator<=
        REQUIRE(lt_deque <= some);
        REQUIRE(eq_deque <= some);

        // operator>
        Deque<int> gt_deque{2};
        REQUIRE(gt_deque > some);

        // operator>=
        REQUIRE(gt_deque >= some);
        REQUIRE(eq_deque >= some);
    }

    SECTION("assignment")
    {
        some = one; // copy
        REQUIRE(some == Deque<int>{1});
        REQUIRE(one == Deque<int>{1});

        empty = std::move(one); // move
        REQUIRE(empty == Deque<int>{1});
        REQUIRE(one == Deque<int>{});
    }

    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());
        REQUIRE(empty.rbegin() == empty.rend());

        // for in
        int i = 0;
        for (const auto& e : some)
        {
            REQUIRE(e == ++i);
        }
        REQUIRE(i == 5);

        // reversed for
        for (auto it = some.rbegin(); it != some.rend(); ++it)
        {
            REQUIRE(*it == i--);
        }
        REQUIRE(i == 0);
    }

    SECTION("access")
    {
        REQUIRE_THROWS_MATCHES(empty.back(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(empty.front(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(empty[0], std::runtime_error, Message("Error: Index out of range."));

        REQUIRE(++some.back() == 6);
        REQUIRE(--some.front() == 0);

        REQUIRE(++some[-1] == 7);
        REQUIRE(--some[0] == -1);
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

    SECTION("extend")
    {
        // extend_back
        empty.extend_back(empty.begin(), empty.end());
        REQUIRE(empty == Deque<int>{});

        empty.extend_back(one.begin(), one.end());
        REQUIRE(empty == Deque<int>{1});

        empty.extend_back(some.begin(), some.end());
        REQUIRE(empty == Deque<int>{1, 1, 2, 3, 4, 5});

        // extend_front
        empty.extend_front(empty.begin(), empty.end());
        REQUIRE(empty == Deque<int>{1, 1, 2, 3, 4, 5, 1, 1, 2, 3, 4, 5});

        empty.extend_front(one.begin(), one.end());
        REQUIRE(empty == Deque<int>{1, 1, 1, 2, 3, 4, 5, 1, 1, 2, 3, 4, 5});

        empty.extend_front(some.begin(), some.end());
        REQUIRE(empty == Deque<int>{1, 2, 3, 4, 5, 1, 1, 1, 2, 3, 4, 5, 1, 1, 2, 3, 4, 5});

        // extend from other container
        std::vector<int> v = {0, 9};

        empty.extend_back(v.begin(), v.end());
        REQUIRE(empty == Deque<int>{1, 2, 3, 4, 5, 1, 1, 1, 2, 3, 4, 5, 1, 1, 2, 3, 4, 5, 0, 9});

        empty.extend_front(v.begin(), v.end());
        REQUIRE(empty == Deque<int>{0, 9, 1, 2, 3, 4, 5, 1, 1, 1, 2, 3, 4, 5, 1, 1, 2, 3, 4, 5, 0, 9});
    }

    SECTION("rotate")
    {
        REQUIRE((empty >>= 1) == Deque<int>{});
        REQUIRE((empty >>= 2) == Deque<int>{});
        REQUIRE((empty <<= 1) == Deque<int>{});
        REQUIRE((empty <<= 2) == Deque<int>{});

        empty.push_back(1);

        REQUIRE((empty >>= 1) == Deque<int>{1});
        REQUIRE((empty >>= 2) == Deque<int>{1});
        REQUIRE((empty <<= 1) == Deque<int>{1});
        REQUIRE((empty <<= 2) == Deque<int>{1});

        empty.push_back(2);
        empty.push_back(3);
        empty.push_back(4);
        empty.push_back(5);

        REQUIRE((empty >>= 1) == Deque<int>{5, 1, 2, 3, 4});
        REQUIRE((empty >>= 2) == Deque<int>{3, 4, 5, 1, 2});
        REQUIRE((empty <<= 1) == Deque<int>{4, 5, 1, 2, 3});
        REQUIRE((empty <<= 2) == Deque<int>{1, 2, 3, 4, 5});

        REQUIRE((empty >>= 233) == Deque<int>{3, 4, 5, 1, 2});
        REQUIRE((empty >>= -233) == Deque<int>{1, 2, 3, 4, 5});

        REQUIRE((empty <<= 233) == Deque<int>{4, 5, 1, 2, 3});
        REQUIRE((empty <<= -233) == Deque<int>{1, 2, 3, 4, 5});
    }

    SECTION("reverse")
    {
        REQUIRE(empty.reverse() == Deque<int>{});
        REQUIRE(one.reverse() == Deque<int>{1});
        REQUIRE(some.reverse() == Deque<int>{5, 4, 3, 2, 1});
    }

    SECTION("clear")
    {
        some.clear();
        REQUIRE(some == Deque<int>{});
        some.clear(); // double clear
        REQUIRE(some == Deque<int>{});
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
