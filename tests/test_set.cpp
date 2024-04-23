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

        // Set(const std::initializer_list<T>& il)
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

        // ~Set()
    }

    Set<int> empty;
    Set<int> one = {1};
    Set<int> some = {1, 2, 3, 4, 5};

    SECTION("compare")
    {
        // operator==
        Set<int> eq_set = {5, 4, 3, 2, 1};
        REQUIRE(eq_set == some);

        // operator!=
        Set<int> ne_set = {1, 3, 5, 7, 9};
        REQUIRE(ne_set != some);

        // operator<
        Set<int> lt_set = {5, 1};
        REQUIRE(lt_set < some);
        REQUIRE_FALSE(ne_set < some);

        // operator<=
        REQUIRE(lt_set <= some);
        REQUIRE(eq_set <= some);

        // operator>
        Set<int> gt_set = {1, 2, 3, 4, 5, 6};
        REQUIRE(gt_set > some);
        REQUIRE_FALSE(ne_set > some);

        // operator>=
        REQUIRE(eq_set >= some);
        REQUIRE(gt_set >= some);
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

        // for in
        for (int i = 1; const auto& e : some)
        {
            REQUIRE(e == i++);
        }
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

        REQUIRE(empty == Set<int>({3, 1, 2, 5, 4}));

        REQUIRE(empty.add(4) == false);
        REQUIRE(empty.add(5) == false);
        REQUIRE(empty.add(2) == false);
        REQUIRE(empty.add(1) == false);
        REQUIRE(empty.add(3) == false);

        REQUIRE(empty == Set<int>({1, 2, 3, 4, 5}));
    }

    SECTION("remove")
    {
        REQUIRE(some.remove(3) == true);
        REQUIRE(some.remove(1) == true);
        REQUIRE(some.remove(2) == true);
        REQUIRE(some.remove(5) == true);
        REQUIRE(some.remove(4) == true);

        REQUIRE(some == Set<int>());

        REQUIRE(some.remove(4) == false);
        REQUIRE(some.remove(5) == false);
        REQUIRE(some.remove(2) == false);
        REQUIRE(some.remove(1) == false);
        REQUIRE(some.remove(3) == false);

        REQUIRE(some == Set<int>());
    }

    SECTION("intersection")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 & set2) == Set<int>({1, 3, 5}));
        REQUIRE((set1 &= set2) == Set<int>({1, 3, 5}));
    }

    SECTION("union")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 | set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
        REQUIRE((set1 |= set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
    }

    SECTION("difference")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 - set2) == Set<int>({2, 4}));
        REQUIRE((set1 -= set2) == Set<int>({2, 4}));
    }

    SECTION("symmetric_difference")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 ^ set2) == Set<int>({2, 4, 7, 9}));
        REQUIRE((set1 ^= set2) == Set<int>({2, 4, 7, 9}));
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
