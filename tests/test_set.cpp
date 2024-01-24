#include "../sources/Set.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor size() is_empty()
TEST_CASE("Set: basics")
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

// operator==() operator!=() operator<=() operator<() operator>=() operator>()
TEST_CASE("Set: compare")
{
    Set<int> set = {1, 2, 3, 4, 5};

    // operator==
    Set<int> eq_set = {5, 4, 3, 2, 1};
    REQUIRE(eq_set == set);

    // operator!=
    Set<int> ne_set = {1, 3, 5};
    REQUIRE(ne_set != set);

    // operator<
    Set<int> lt_set = {1, 2, 3};
    REQUIRE(lt_set < set);

    // operator<=
    REQUIRE(lt_set <= set);
    REQUIRE(eq_set <= set);

    // operator>
    Set<int> gt_set = {1, 2, 3, 4, 5, 6};
    REQUIRE(gt_set > set);

    // operator>=
    REQUIRE(eq_set >= set);
    REQUIRE(gt_set >= set);
}

// begin() end()
TEST_CASE("Set: iterator")
{
    // empty
    Set<int> empty;
    REQUIRE(empty.begin() == empty.end());

    Set<int> set = {1, 2, 3, 4, 5};

    // for
    int i = 1;
    for (auto it = set.begin(); it != set.end(); ++it)
    {
        REQUIRE(*it == i++);
    }

    // for in
    i = 1;
    for (const auto& e : set)
    {
        REQUIRE(e == i++);
    }
}

// operator=()
TEST_CASE("Set: copy_assignment")
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {6, 7, 8, 9};

    set1 = set2;
    REQUIRE(set1 == Set<int>({6, 7, 8, 9}));
    REQUIRE(set2 == Set<int>({6, 7, 8, 9}));
}

// operator=()
TEST_CASE("Set: move_assignment")
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {6, 7, 8, 9};

    set1 = std::move(set2);
    REQUIRE(set1 == Set<int>({6, 7, 8, 9}));
    REQUIRE(set2 == Set<int>());
}

// find() contains() min() max()
TEST_CASE("Set: examination")
{
    Set<int> set = {1, 2, 3, 4, 5};

    // find
    REQUIRE(set.find(1) == set.begin());
    REQUIRE(set.find(5) == --set.end());
    REQUIRE(set.find(0) == set.end());

    // contains
    REQUIRE(set.contains(1) == true);
    REQUIRE(set.contains(5) == true);
    REQUIRE(set.contains(0) == false);

    // min
    REQUIRE(set.min() == 1);

    // max
    REQUIRE(set.max() == 5);
}

// operator+=()
TEST_CASE("Set: insert")
{
    Set<int> set;

    set += 3;
    set += 1;
    set += 2;
    set += 5;
    set += 4;

    REQUIRE(set == Set<int>({3, 1, 2, 5, 4}));

    set += 4;
    set += 5;
    set += 2;
    set += 1;
    set += 3;

    REQUIRE(set == Set<int>({1, 2, 3, 4, 5}));
}

// operator-=()
TEST_CASE("Set: remove")
{
    Set<int> set = {1, 2, 3, 4, 5};

    set -= 3;
    set -= 1;
    set -= 2;
    set -= 5;
    set -= 4;

    REQUIRE(set == Set<int>());

    set -= 4;
    set -= 5;
    set -= 2;
    set -= 1;
    set -= 3;

    REQUIRE(set == Set<int>());
}

// clear()
TEST_CASE("Set: clear")
{
    Set<int> set = {1, 2, 3, 4, 5};

    REQUIRE(set.clear() == Set<int>());

    // double clear
    REQUIRE(set.clear() == Set<int>());

    // modify after clear
    set += 233;
    REQUIRE(set == Set<int>({233}));
}

// operator&=() operator&()
TEST_CASE("Set: intersect")
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    REQUIRE((set1 & set2) == Set<int>({1, 3, 5}));
    REQUIRE((set1 &= set2) == Set<int>({1, 3, 5}));
}

// operator|=() operator|()
TEST_CASE("Set: union")
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    REQUIRE((set1 | set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
    REQUIRE((set1 |= set2) == Set<int>({1, 2, 3, 4, 5, 7, 9}));
}

// operator-=() operator-()
TEST_CASE("Set: difference")
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    REQUIRE((set1 - set2) == Set<int>({2, 4}));
    REQUIRE((set1 -= set2) == Set<int>({2, 4}));
}

// operator^=() operator^()
TEST_CASE("Set: symmetric_difference")
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    REQUIRE((set1 ^ set2) == Set<int>({2, 4, 7, 9}));
    REQUIRE((set1 ^= set2) == Set<int>({2, 4, 7, 9}));
}

// to_list() to_deque()
TEST_CASE("Set: to_list_deque")
{
    REQUIRE(Set<int>({1, 2, 3, 4, 5}).to_list() == List<int>({1, 2, 3, 4, 5}));

    REQUIRE(Set<int>({1, 2, 3, 4, 5}).to_deque() == Deque<int>({1, 2, 3, 4, 5}));
}

// operator<<()
TEST_CASE("Set: print")
{
    std::ostringstream oss;

    Set<int> empty;
    oss << empty;
    REQUIRE(oss.str() == "{}");
    oss.str("");

    Set<int> one = {1};
    oss << one;
    REQUIRE(oss.str() == "{1}");
    oss.str("");

    Set<int> many = {5, 4, 3, 2, 1};
    oss << many;
    REQUIRE(oss.str() == "{1, 2, 3, 4, 5}");
    oss.str("");
}

TEST_CASE("Set: all")
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
    MY_ASSERT_THROW_MESSAGE(set.max(), std::runtime_error, "Error: The container is empty.");
    MY_ASSERT_THROW_MESSAGE(set.min(), std::runtime_error, "Error: The container is empty.");
}
