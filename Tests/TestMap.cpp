#include "../Sources/Map.hpp"

#include "tool.hpp"

using namespace mdspp;

// constructor destructor size() is_empty()
TEST(Map, basics)
{
    // Map()
    Map<int, std::string> map1;
    ASSERT_EQ(map1.size(), 0);
    ASSERT_TRUE(map1.is_empty());

    // Map(const std::initializer_list<Pair>& il)
    Map<int, std::string> map2({{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}});
    ASSERT_EQ(map2.size(), 5);
    ASSERT_FALSE(map2.is_empty());

    // Map(const Map& that)
    Map<int, std::string> map3(map2);
    ASSERT_EQ(map3.size(), 5);
    ASSERT_FALSE(map3.is_empty());

    // Map(Map&& that)
    Map<int, std::string> map4(std::move(map3));
    ASSERT_EQ(map4.size(), 5);
    ASSERT_FALSE(map4.is_empty());
    ASSERT_EQ(map3.size(), 0);
    ASSERT_TRUE(map3.is_empty());

    // ~Map()
}

// operator==() operator!=() operator<=() operator<() operator>=() operator>()
TEST(Map, compare)
{
    Map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};

    // operator==
    Map<std::string, int> eq_map = {{"three", 3}, {"three", 3}, {"one", 1}, {"two", 2}};
    ASSERT_TRUE(eq_map == map);

    // operator!=
    Map<std::string, int> ne_map = {{"one", 1}, {"three", 3}, {"five", 5}};
    ASSERT_TRUE(ne_map != map);

    // operator<
    Map<std::string, int> lt_map = {{"one", 1}, {"two", 2}};
    ASSERT_TRUE(lt_map < map);

    // operator<=
    ASSERT_TRUE(lt_map <= map);
    ASSERT_TRUE(eq_map <= map);

    // operator>
    Map<std::string, int> gt_map = {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}};
    ASSERT_TRUE(gt_map > map);

    // operator>=
    ASSERT_TRUE(eq_map >= map);
    ASSERT_TRUE(gt_map >= map);
}

// operator=()
TEST(Map, copy_assignment)
{
    Map<int, std::string> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, std::string> map2({{4, "four"}, {5, "five"}});

    map1 = map2;
    ASSERT_EQ(map1, (Map<int, std::string>({{4, "four"}, {5, "five"}}))); // need (Map<>({...}))
    ASSERT_EQ(map2, (Map<int, std::string>({{4, "four"}, {5, "five"}})));
}

// operator=()
TEST(Map, move_assignment)
{
    Map<int, std::string> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, std::string> map2({{4, "four"}, {5, "five"}});

    map1 = std::move(map2);
    ASSERT_EQ(map1, (Map<int, std::string>({{4, "four"}, {5, "five"}})));
    ASSERT_EQ(map2, (Map<int, std::string>()));
}

// operator[]() get()
TEST(Map, access)
{
    Map<std::string, int> map({{"one", 1}, {"two", 2}, {"three", 3}});

    // get
    ASSERT_EQ(map.get("one"), 1);
    ASSERT_EQ(map.get("not exist"), 0);
    ASSERT_EQ(map.get("not exist", 233), 233);

    // access
    ASSERT_EQ(map["one"], 1);
    ASSERT_EQ(map["two"], 2);
    ASSERT_EQ(map["three"], 3);

    // assignment
    map["one"] = 1111;
    ASSERT_EQ(map["one"], 1111);

    // check key
    MY_ASSERT_THROW_MESSAGE(map["four"], std::runtime_error, "Error: Key is not found in the map.");
}

// begin() end()
TEST(Map, iterator)
{
    // empty
    Map<int, std::string> empty;
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
    Map<int, std::string> map({{1, "one"}, {2, "two"}, {3, "three"}});

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
    Map<int, std::string> map({{1, "one"}, {2, "two"}, {3, "three"}});

    ASSERT_EQ(map.keys(), Set<int>({1, 2, 3}));
    ASSERT_EQ(map.values(), Set<std::string>({"one", "two", "three"}));
}

// operator+=()
TEST(Map, insert)
{
    Map<int, std::string> map;

    map += {3, "three"};
    map += {1, "one"};
    map += {2, "two"};

    ASSERT_EQ(map, (Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}})));

    map += {3, "three"};
    map += {1, "one"};
    map += {2, "two"};

    ASSERT_EQ(map, (Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}})));
}

// operator-=()
TEST(Map, remove)
{
    Map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};

    map -= 3;
    map -= 1;
    map -= 2;

    ASSERT_EQ(map, (Map<int, std::string>()));

    map -= 2;
    map -= 1;
    map -= 3;

    ASSERT_EQ(map, (Map<int, std::string>()));
}

// clear()
TEST(Map, clear)
{
    Map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};

    ASSERT_EQ(map.clear(), (Map<int, std::string>()));

    // double clear
    ASSERT_EQ(map.clear(), (Map<int, std::string>()));

    // modify after clear
    map += {1, "one"};
    ASSERT_EQ(map, (Map<int, std::string>({{1, "one"}})));
}

// operator<<()
TEST(Map, print)
{
    std::ostringstream oss;

    Map<int, std::string> empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "{}"); // string == char*, use eq
    oss.str("");

    Map<int, std::string> one = {{1, "one"}};
    oss << one;
    ASSERT_EQ(oss.str(), "{1: one}");
    oss.str("");

    Map<int, std::string> many = {{1, "one"}, {2, "two"}, {3, "three"}};
    oss << many;
    ASSERT_EQ(oss.str(), "{1: one, 2: two, 3: three}");
    oss.str("");
}
