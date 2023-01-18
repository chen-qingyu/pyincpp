// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/Map.hpp"
#include "../Sources/String.hpp"

using mdspp::Map;
using mdspp::String;

// constructor destructor size() is_empty()
TEST(Map, basics)
{
    // Map()
    Map<int, String> map1;
    ASSERT_EQ(map1.size(), 0);
    ASSERT_TRUE(map1.is_empty());

    // Map(const std::initializer_list<Pair>& il)
    Map<int, String> map2({{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}});
    ASSERT_EQ(map2.size(), 5);
    ASSERT_FALSE(map2.is_empty());

    // Map(const Map& that)
    Map<int, String> map3(map2);
    ASSERT_EQ(map3.size(), 5);
    ASSERT_FALSE(map3.is_empty());

    // Map(Map&& that)
    Map<int, String> map4(std::move(map3));
    ASSERT_EQ(map4.size(), 5);
    ASSERT_FALSE(map4.is_empty());
    ASSERT_EQ(map3.size(), 0);
    ASSERT_TRUE(map3.is_empty());

    // ~Map()
}

// operator=()
TEST(Map, copy_assignment)
{
    Map<int, String> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, String> map2({{4, "four"}, {5, "five"}});

    map1 = map2;
    map2 += {6, "six"};
    ASSERT_EQ(map1, (Map<int, String>({{4, "four"}, {5, "five"}}))); // need (Map<>({...}))
    ASSERT_EQ(map2, (Map<int, String>({{4, "four"}, {5, "five"}, {6, "six"}})));
}

// operator=()
TEST(Map, move_assignment)
{
    Map<int, String> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, String> map2({{4, "four"}, {5, "five"}});

    map1 = std::move(map2);
    ASSERT_EQ(map1, (Map<int, String>({{4, "four"}, {5, "five"}})));
    ASSERT_EQ(map2, (Map<int, String>()));
}

// operator[]()
TEST(Map, access)
{
    Map<String, int> map({{"one", 1}, {"two", 2}, {"three", 3}});

    // access
    ASSERT_EQ(map["one"], 1);
    ASSERT_EQ(map["two"], 2);
    ASSERT_EQ(map["three"], 3);

    // assignment
    map["one"] = 1111;
    ASSERT_EQ(map["one"], 1111);

    // check key
    ASSERT_THROW(map["four"], std::runtime_error);
    try
    {
        map["four"];
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Key is not found in the map.");
    }
}

// operator==() operator!=()
TEST(Map, compare)
{
    Map<String, int> map({{"one", 1}, {"two", 2}, {"three", 3}});

    // operator==
    ASSERT_TRUE(map == (Map<String, int>({{"two", 2}, {"one", 1}, {"three", 3}})));

    // operator!=
    ASSERT_TRUE(map != (Map<String, int>({{"one", 1}, {"two", 2}, {"six", 6}})));
}
