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

    // // Set(const std::initializer_list<T> &il)
    // Set<int> set2 = {1, 2, 3, 4, 5};
    // ASSERT_EQ(set2.size(), 5);
    // ASSERT_FALSE(set2.is_empty());

    // // Set(const Set<T> &that)
    // Set<int> set3(set2);
    // ASSERT_EQ(set3.size(), 5);
    // ASSERT_FALSE(set3.is_empty());

    // // Set(Set<T> &&that)
    // Set<int> set4(std::move(set3));
    // ASSERT_EQ(set4.size(), 5);
    // ASSERT_FALSE(set4.is_empty());
    // ASSERT_EQ(set3.size(), 0);
    // ASSERT_TRUE(set3.is_empty());

    // ~Set()
}
