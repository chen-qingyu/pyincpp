// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/Map.hpp"
#include "../Sources/String.hpp"

#include "my_tools.hpp"

using namespace mdspp;

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

// operator==() operator!=()
TEST(Map, compare)
{
    Map<String, int> map({{"one", 1}, {"two", 2}, {"three", 3}});

    // operator==
    ASSERT_TRUE(map == (Map<String, int>({{"two", 2}, {"one", 1}, {"three", 3}})));

    // operator!=
    ASSERT_TRUE(map != (Map<String, int>({{"one", 1}, {"two", 2}, {"six", 6}})));
}

// operator=()
TEST(Map, copy_assignment)
{
    Map<int, String> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, String> map2({{4, "four"}, {5, "five"}});

    map1 = map2;
    ASSERT_EQ(map1, (Map<int, String>({{4, "four"}, {5, "five"}}))); // need (Map<>({...}))
    ASSERT_EQ(map2, (Map<int, String>({{4, "four"}, {5, "five"}})));
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
    MY_ASSERT_THROWS_MESSAGE(map["four"], std::runtime_error, "ERROR: Key is not found in the map.");
}

// begin() end()
TEST(Map, iterator)
{
    // empty
    Map<int, String> empty;
    ASSERT_EQ(empty.begin(), empty.end());

    // key, value=key^2
    Map<int, int> map({{1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}});

    // for
    int k = 1;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        ASSERT_EQ(it->key(), k);
        ASSERT_EQ(it->value(), k * k);
        ++k;
    }

    // for in
    k = 1;
    for (const auto& e : map)
    {
        ASSERT_EQ(e.key(), k);
        ASSERT_EQ(e.value(), k * k);
        ++k;
    }
}

// find() contains() min() max()
TEST(Map, examination)
{
    Map<int, String> map({{1, "one"}, {2, "two"}, {3, "three"}});

    // find
    ASSERT_EQ(map.find(1), map.begin());
    ASSERT_EQ(map.find(3), --map.end());
    ASSERT_EQ(map.find(0), map.end());

    // contains
    ASSERT_EQ(map.contains(1), true);
    ASSERT_EQ(map.contains(3), true);
    ASSERT_EQ(map.contains(0), false);

    // min
    ASSERT_EQ(map.min(), 1);

    // max
    ASSERT_EQ(map.max(), 3);
}

// keys() values()
TEST(Map, keys_values)
{
    Map<int, String> map({{1, "one"}, {2, "two"}, {3, "three"}});

    ASSERT_EQ(map.keys(), Set<int>({1, 2, 3}));
    ASSERT_EQ(map.values(), Set<String>({"one", "two", "three"}));
}

// operator+=()
TEST(Map, insert)
{
    Map<int, String> map;

    map += {3, "three"};
    map += {1, "one"};
    map += {2, "two"};

    ASSERT_EQ(map, (Map<int, String>({{1, "one"}, {2, "two"}, {3, "three"}})));

    map += {3, "three"};
    map += {1, "one"};
    map += {2, "two"};

    ASSERT_EQ(map, (Map<int, String>({{1, "one"}, {2, "two"}, {3, "three"}})));
}

// operator-=()
TEST(Map, remove)
{
    Map<int, String> map = {{1, "one"}, {2, "two"}, {3, "three"}};

    map -= 3;
    map -= 1;
    map -= 2;

    ASSERT_EQ(map, (Map<int, String>()));

    map -= 2;
    map -= 1;
    map -= 3;

    ASSERT_EQ(map, (Map<int, String>()));
}

// clear()
TEST(Map, clear)
{
    Map<int, String> map = {{1, "one"}, {2, "two"}, {3, "three"}};

    ASSERT_EQ(map.clear(), (Map<int, String>()));

    // double clear
    ASSERT_EQ(map.clear(), (Map<int, String>()));

    // modify after clear
    map += {1, "one"};
    ASSERT_EQ(map, (Map<int, String>({{1, "one"}})));
}

// operator<<()
TEST(Map, print)
{
    std::ostringstream oss;

    Map<int, String> empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "{}"); // string == char*, use eq
    oss.str("");

    Map<int, String> one = {{1, "one"}};
    oss << one;
    ASSERT_EQ(oss.str(), "{1: \"one\"}");
    oss.str("");

    Map<int, String> many = {{1, "one"}, {2, "two"}, {3, "three"}};
    oss << many;
    ASSERT_EQ(oss.str(), "{1: \"one\", 2: \"two\", 3: \"three\"}");
    oss.str("");
}
