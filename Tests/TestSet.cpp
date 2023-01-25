// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/Set.hpp"

using mdspp::Set;

// constructor destructor size() is_empty()
TEST(Set, basics)
{
    // Set()
    Set<int> set1;
    ASSERT_EQ(set1.size(), 0);
    ASSERT_TRUE(set1.is_empty());

    // Set(const std::initializer_list<T> &il)
    Set<int> set2 = {1, 2, 3, 4, 5};
    ASSERT_EQ(set2.size(), 5);
    ASSERT_FALSE(set2.is_empty());

    // Set(const Set<T> &that)
    Set<int> set3(set2);
    ASSERT_EQ(set3.size(), 5);
    ASSERT_FALSE(set3.is_empty());

    // Set(Set<T> &&that)
    Set<int> set4(std::move(set3));
    ASSERT_EQ(set4.size(), 5);
    ASSERT_FALSE(set4.is_empty());
    ASSERT_EQ(set3.size(), 0);
    ASSERT_TRUE(set3.is_empty());

    // ~Set()
}

// begin() end()
TEST(Set, iterator)
{
    // empty
    Set<int> empty;
    ASSERT_EQ(empty.begin(), empty.end());

    Set<int> set = {1, 2, 3, 4, 5};

    // for
    int i = 1;
    for (auto it = set.begin(); it != set.end(); ++it)
    {
        ASSERT_EQ(*it, i++);
    }

    // for in
    i = 1;
    for (const auto& e : set)
    {
        ASSERT_EQ(e, i++);
    }
}

// operator=()
TEST(Set, copy_assignment)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {6, 7, 8, 9};

    set1 = set2;
    set2 += 10;
    ASSERT_EQ(set1, Set<int>({6, 7, 8, 9}));
    ASSERT_EQ(set2, Set<int>({6, 7, 8, 9, 10}));
}

// operator=()
TEST(Set, move_assignment)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {6, 7, 8, 9};

    set1 = std::move(set2);
    ASSERT_EQ(set1, Set<int>({6, 7, 8, 9}));
    ASSERT_EQ(set2, Set<int>());
}

// operator==() operator!=() operator<=() operator<() operator>=() operator>()
TEST(Set, compare)
{
    Set<int> set = {1, 2, 3, 4, 5};

    // operator==
    Set<int> eq_set = {5, 4, 3, 2, 1};
    ASSERT_TRUE(eq_set == set);

    // operator!=
    Set<int> ne_set = {1, 3, 5};
    ASSERT_TRUE(ne_set != set);

    // operator<
    Set<int> lt_set = {1, 2, 3};
    ASSERT_TRUE(lt_set < set);

    // operator<=
    ASSERT_TRUE(lt_set <= set);
    ASSERT_TRUE(eq_set <= set);

    // operator>
    Set<int> gt_set = {1, 2, 3, 4, 5, 6};
    ASSERT_TRUE(gt_set > set);

    // operator>=
    ASSERT_TRUE(eq_set >= set);
    ASSERT_TRUE(gt_set >= set);
}

// find() contains() min() max()
TEST(Set, examination)
{
    Set<int> set = {1, 2, 3, 4, 5};

    // find
    ASSERT_EQ(set.find(1), set.begin());
    ASSERT_EQ(set.find(5), --set.end());
    ASSERT_EQ(set.find(0), set.end());

    // contains
    ASSERT_EQ(set.contains(1), true);
    ASSERT_EQ(set.contains(5), true);
    ASSERT_EQ(set.contains(0), false);

    // min
    ASSERT_EQ(set.min(), 1);

    // max
    ASSERT_EQ(set.max(), 5);
}

// operator+=()
TEST(Set, insert)
{
    Set<int> set;

    set += 3;
    set += 1;
    set += 2;
    set += 5;
    set += 4;

    ASSERT_EQ(set, Set<int>({3, 1, 2, 5, 4}));

    set += 4;
    set += 5;
    set += 2;
    set += 1;
    set += 3;

    ASSERT_EQ(set, Set<int>({1, 2, 3, 4, 5}));
}

// operator-=()
TEST(Set, remove)
{
    Set<int> set = {1, 2, 3, 4, 5};

    set -= 3;
    set -= 1;
    set -= 2;
    set -= 5;
    set -= 4;

    ASSERT_EQ(set, Set<int>());

    set -= 4;
    set -= 5;
    set -= 2;
    set -= 1;
    set -= 3;

    ASSERT_EQ(set, Set<int>());
}

// clear()
TEST(Set, clear)
{
    Set<int> set = {1, 2, 3, 4, 5};

    ASSERT_EQ(set.clear(), Set<int>());

    // double clear
    ASSERT_EQ(set.clear(), Set<int>());

    // modify after clear
    set += 233;
    ASSERT_EQ(set, Set<int>({233}));
}

// operator&=() operator&()
TEST(Set, intersect)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    ASSERT_EQ(set1 & set2, Set<int>({1, 3, 5}));
    ASSERT_EQ(set1 &= set2, Set<int>({1, 3, 5}));
}

// operator|=() operator|()
TEST(Set, union)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    ASSERT_EQ(set1 | set2, Set<int>({1, 2, 3, 4, 5, 7, 9}));
    ASSERT_EQ(set1 |= set2, Set<int>({1, 2, 3, 4, 5, 7, 9}));
}

// operator-=() operator-()
TEST(Set, difference)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    ASSERT_EQ(set1 - set2, Set<int>({2, 4}));
    ASSERT_EQ(set1 -= set2, Set<int>({2, 4}));
}

// operator^=() operator^()
TEST(Set, symmetric_difference)
{
    Set<int> set1 = {1, 2, 3, 4, 5};
    Set<int> set2 = {1, 3, 5, 7, 9};

    ASSERT_EQ(set1 ^ set2, Set<int>({2, 4, 7, 9}));
    ASSERT_EQ(set1 ^= set2, Set<int>({2, 4, 7, 9}));
}

// operator<<()
TEST(Set, print)
{
    std::ostringstream oss;

    Set<int> empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "{}"); // string == char*, use eq
    oss.str("");

    Set<int> one = {1};
    oss << one;
    ASSERT_EQ(oss.str(), "{1}");
    oss.str("");

    Set<int> many = {5, 4, 3, 2, 1};
    oss << many;
    ASSERT_EQ(oss.str(), "{1, 2, 3, 4, 5}");
    oss.str("");
}
