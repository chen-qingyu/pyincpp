#include "../sources/Set.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Set")
{
    // constructor destructor size() is_empty()
    SECTION("basics")
    {
        // Set()
        Set<int> set1;
        REQUIRE(set1.size() == 0);
        REQUIRE(set1.is_empty());

        // Set(const std::initializer_list<T> &il)
        Set<int> set2 = {1, 2, 3, 4, 5};
        REQUIRE(set2.size() == 5);
        REQUIRE(!set2.is_empty());

        // Set(const Set<T> &that)
        Set<int> set3(set2);
        REQUIRE(set3.size() == 5);
        REQUIRE(!set3.is_empty());

        // Set(Set<T> &&that)
        Set<int> set4(std::move(set3));
        REQUIRE(set4.size() == 5);
        REQUIRE(!set4.is_empty());
        REQUIRE(set3.size() == 0);
        REQUIRE(set3.is_empty());

        // ~Set()
    }

    Set<int> empty;
    Set<int> one = {1};
    Set<int> some = {1, 2, 3, 4, 5};

    // operator==() operator!=() operator<=() operator<() operator>=() operator>()
    SECTION("compare")
    {
        // operator==
        Set<int> eq_set = {5, 4, 3, 2, 1};
        REQUIRE(eq_set == some);

        // operator!=
        Set<int> ne_set = {1, 3, 5};
        REQUIRE(ne_set != some);

        // operator<
        Set<int> lt_set = {1, 2, 3};
        REQUIRE(lt_set < some);

        // operator<=
        REQUIRE(lt_set <= some);
        REQUIRE(eq_set <= some);

        // operator>
        Set<int> gt_set = {1, 2, 3, 4, 5, 6};
        REQUIRE(gt_set > some);

        // operator>=
        REQUIRE(eq_set >= some);
        REQUIRE(gt_set >= some);
    }

    // begin() end()
    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());

        // for
        int i = 1;
        for (auto it = some.begin(); it != some.end(); ++it)
        {
            REQUIRE(*it == i++);
        }

        // for in
        i = 1;
        for (const auto& e : some)
        {
            REQUIRE(e == i++);
        }
    }

    // operator=()
    SECTION("copy_assignment")
    {
        some = one;
        REQUIRE(some == Set<int>({1}));
        REQUIRE(one == Set<int>({1}));
    }

    // operator=()
    SECTION("move_assignment")
    {
        some = std::move(one);
        REQUIRE(some == Set<int>({1}));
        REQUIRE(one == Set<int>());
    }

    // find() contains() min() max()
    SECTION("examination")
    {
        // find
        REQUIRE(some.find(1) == some.begin());
        REQUIRE(some.find(5) == --some.end());
        REQUIRE(some.find(0) == some.end());

        // contains
        REQUIRE(some.contains(1) == true);
        REQUIRE(some.contains(5) == true);
        REQUIRE(some.contains(0) == false);

        // min
        REQUIRE(some.min() == 1);

        // max
        REQUIRE(some.max() == 5);
    }

    // operator+=()
    SECTION("insert")
    {
        empty += 3;
        empty += 1;
        empty += 2;
        empty += 5;
        empty += 4;

        REQUIRE(empty == Set<int>({3, 1, 2, 5, 4}));

        empty += 4;
        empty += 5;
        empty += 2;
        empty += 1;
        empty += 3;

        REQUIRE(empty == Set<int>({1, 2, 3, 4, 5}));

        REQUIRE(one.insert(2) == true);
        REQUIRE(one.insert(2) == false);
    }

    // operator-=()
    SECTION("remove")
    {
        some -= 3;
        some -= 1;
        some -= 2;
        some -= 5;
        some -= 4;

        REQUIRE(some == Set<int>());

        some -= 4;
        some -= 5;
        some -= 2;
        some -= 1;
        some -= 3;

        REQUIRE(some == Set<int>());

        REQUIRE(one.remove(1) == true);
        REQUIRE(one.remove(1) == false);
    }

    // clear()
    SECTION("clear")
    {
        REQUIRE(some.clear() == Set<int>());

        // double clear
        REQUIRE(some.clear() == Set<int>());

        // modify after clear
        some += 233;
        REQUIRE(some == Set<int>({233}));
    }

    // operator&=() operator&()
    SECTION("intersect")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 & set2) == Set<int>({1, 3, 5}));
        REQUIRE((set1 &= set2) == Set<int>({1, 3, 5}));
    }

    // operator|=() operator|()
    SECTION("union")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 | set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
        REQUIRE((set1 |= set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
    }

    // operator-=() operator-()
    SECTION("difference")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 - set2) == Set<int>({2, 4}));
        REQUIRE((set1 -= set2) == Set<int>({2, 4}));
    }

    // operator^=() operator^()
    SECTION("symmetric_difference")
    {
        Set<int> set1 = {1, 2, 3, 4, 5};
        Set<int> set2 = {1, 3, 5, 7, 9};

        REQUIRE((set1 ^ set2) == Set<int>({2, 4, 7, 9}));
        REQUIRE((set1 ^= set2) == Set<int>({2, 4, 7, 9}));
    }

    // operator<<()
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

    SECTION("all")
    {
        Set<int> set = {1, 5, 10, 11, 9};
        REQUIRE(set.max() == 11);
        REQUIRE(set.min() == 1);

        set -= 11;
        set -= 10;
        REQUIRE(set.max() == 9);

        set -= 1;
        set -= 5;
        REQUIRE(set.min() == 9);

        set -= 9;
        set -= 9;
        REQUIRE_THROWS_MATCHES(set.max(), std::runtime_error, Message("Error: The container is empty."));
        REQUIRE_THROWS_MATCHES(set.min(), std::runtime_error, Message("Error: The container is empty."));
    }
}
