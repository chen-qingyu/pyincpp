#include "../sources/Set.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Set")
{
    SECTION("basics")
    {
        // Set()
        Set<int> set1;
        REQUIRE(set1.size() == 0);
        REQUIRE(set1.is_empty());

        // Set(const std::initializer_list<T>& init)
        Set<int> set2 = {1, 2, 3, 4, 5};
        REQUIRE(set2.size() == 5);
        REQUIRE(!set2.is_empty());

        // Set(const InputIt& first, const InputIt& last)
        Set<int> set3(set2.begin(), set2.end());
        REQUIRE(set3.size() == 5);
        REQUIRE(!set3.is_empty());

        // Set(const Set& that)
        Set<int> set4(set3);
        REQUIRE(set4.size() == 5);
        REQUIRE(!set4.is_empty());

        // Set(Set&& that)
        Set<int> set5(std::move(set4));
        REQUIRE(set5.size() == 5);
        REQUIRE(!set5.is_empty());
        REQUIRE(set4.size() == 0);
        REQUIRE(set4.is_empty());

        // test compatibility
        Set<EqLtType> test = {1, 2, 3, 4, 5};
        REQUIRE(test.size() == 5);
        REQUIRE(!test.is_empty());

        // ~Set()
    }

    Set<int> empty;
    Set<int> one = {1};
    Set<int> some = {1, 2, 3, 4, 5};

    SECTION("compare")
    {
        // operator==
        REQUIRE(Set<int>{} == empty);
        REQUIRE(Set<int>{5, 4, 3, 2, 1, 2, 3, 4, 5} == some);

        // operator!=
        REQUIRE(one != some);
        REQUIRE(empty != one);

        // operator<
        REQUIRE(Set<int>{5, 1} < some);
        REQUIRE(empty < one);

        // operator<=
        REQUIRE(some <= some);
        REQUIRE(empty <= one);

        // operator>
        REQUIRE(Set<int>{0, 1, 2, 3, 4, 5} > some);
        REQUIRE(one > empty);

        // operator>=
        REQUIRE(some >= some);
        REQUIRE(one >= empty);

        // neither a subset nor a superset
        Set<int> set1{0, 1}, set2{2, 3};
        REQUIRE_FALSE(set1 < set2);
        REQUIRE_FALSE(set1 > set2);
        REQUIRE_FALSE(set1 == set2);
    }

    SECTION("assignment")
    {
        some = one; // copy
        REQUIRE(some == Set<int>({1}));
        REQUIRE(one == Set<int>({1}));

        empty = std::move(one); // move
        REQUIRE(empty == Set<int>({1}));
        REQUIRE(one == Set<int>());
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

    SECTION("examination")
    {
        // find
        REQUIRE(*some.find(1) == 1);
        REQUIRE(some.find(0) == some.end());

        // contains
        REQUIRE(some.contains(1) == true);
        REQUIRE(some.contains(0) == false);

        // min
        REQUIRE(some.min() == 1);
        REQUIRE_THROWS_MATCHES(empty.min(), std::runtime_error, Message("Error: The container is empty."));

        // max
        REQUIRE(some.max() == 5);
        REQUIRE_THROWS_MATCHES(empty.max(), std::runtime_error, Message("Error: The container is empty."));
    }

    SECTION("add")
    {
        REQUIRE(empty.add(3) == true);
        REQUIRE(empty.add(1) == true);
        REQUIRE(empty.add(2) == true);
        REQUIRE(empty.add(5) == true);
        REQUIRE(empty.add(4) == true);

        REQUIRE(empty == some);

        REQUIRE(empty.add(4) == false);
        REQUIRE(empty.add(5) == false);
        REQUIRE(empty.add(2) == false);
        REQUIRE(empty.add(1) == false);
        REQUIRE(empty.add(3) == false);

        REQUIRE(empty == some);
    }

    SECTION("remove")
    {
        REQUIRE(some.remove(3) == true);
        REQUIRE(some.remove(1) == true);
        REQUIRE(some.remove(2) == true);
        REQUIRE(some.remove(5) == true);
        REQUIRE(some.remove(4) == true);

        REQUIRE(some == empty);

        REQUIRE(some.remove(4) == false);
        REQUIRE(some.remove(5) == false);
        REQUIRE(some.remove(2) == false);
        REQUIRE(some.remove(1) == false);
        REQUIRE(some.remove(3) == false);

        REQUIRE(some == empty);
    }

    SECTION("pop")
    {
        REQUIRE(some.pop() == 1);
        REQUIRE(some.pop() == 2);
        REQUIRE(some.pop() == 3);
        REQUIRE(some.pop() == 4);
        REQUIRE(some.pop() == 5);
        REQUIRE_THROWS_MATCHES(some.pop(), std::runtime_error, Message("Error: The container is empty."));
    }

    SECTION("ops")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 & set2) == Set<int>({1, 3, 5}));
        REQUIRE((set1 | set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
        REQUIRE((set1 - set2) == Set<int>({2, 4}));
        REQUIRE((set1 ^ set2) == Set<int>({2, 4, 7, 9}));

        REQUIRE((empty & empty) == empty);
        REQUIRE((empty | empty) == empty);
        REQUIRE((empty - empty) == empty);
        REQUIRE((empty ^ empty) == empty);
    }

    SECTION("extend")
    {
        empty.extend(empty.begin(), empty.end());
        REQUIRE(empty == Set<int>{});

        empty.extend(one.begin(), one.end());
        REQUIRE(empty == Set<int>{1});

        empty.extend(some.begin(), some.end());
        REQUIRE(empty == Set<int>{1, 2, 3, 4, 5});

        // extend from other container
        std::vector<int> v = {0, 9};
        empty.extend(v.begin(), v.end());
        REQUIRE(empty == Set<int>{0, 1, 2, 3, 4, 5, 9});
    }

    SECTION("clear")
    {
        some.clear();
        REQUIRE(some == Set<int>());
        some.clear(); // double clear
        REQUIRE(some == Set<int>());
    }

    SECTION("print")
    {
        std::ostringstream oss;

        oss << empty;
        REQUIRE(oss.str() == "{}");
        oss.str("");

        oss << one;
        REQUIRE(oss.str() == "{1}");
        oss.str("");

        oss << some;
        REQUIRE(oss.str() == "{1, 2, 3, 4, 5}");
        oss.str("");
    }
}
