#include "../sources/Map.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor size() is_empty()
TEST_CASE("Map: basics")
{
    // Map()
    Map<int, std::string> map1;
    REQUIRE(map1.size() == 0);
    REQUIRE(map1.is_empty());

    // Map(const std::initializer_list<Pair>& il)
    Map<int, std::string> map2({{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}});
    REQUIRE(map2.size() == 5);
    REQUIRE(!map2.is_empty());

    // Map(const Map& that)
    Map<int, std::string> map3(map2);
    REQUIRE(map3.size() == 5);
    REQUIRE(!map3.is_empty());

    // Map(Map&& that)
    Map<int, std::string> map4(std::move(map3));
    REQUIRE(map4.size() == 5);
    REQUIRE(!map4.is_empty());
    REQUIRE(map3.size() == 0);
    REQUIRE(map3.is_empty());

    // ~Map()
}

// operator==() operator!=() operator<=() operator<() operator>=() operator>()
TEST_CASE("Map: compare")
{
    Map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};

    // operator==
    Map<std::string, int> eq_map = {{"three", 3}, {"three", 3}, {"one", 1}, {"two", 2}};
    REQUIRE(eq_map == map);

    // operator!=
    Map<std::string, int> ne_map = {{"one", 1}, {"three", 3}, {"five", 5}};
    REQUIRE(ne_map != map);

    // operator<
    Map<std::string, int> lt_map = {{"one", 1}, {"two", 2}};
    REQUIRE(lt_map < map);

    // operator<=
    REQUIRE(lt_map <= map);
    REQUIRE(eq_map <= map);

    // operator>
    Map<std::string, int> gt_map = {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}};
    REQUIRE(gt_map > map);

    // operator>=
    REQUIRE(eq_map >= map);
    REQUIRE(gt_map >= map);
}

// operator=()
TEST_CASE("Map: copy_assignment")
{
    Map<int, std::string> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, std::string> map2({{4, "four"}, {5, "five"}});

    map1 = map2;
    REQUIRE(map1 == (Map<int, std::string>({{4, "four"}, {5, "five"}})));
    REQUIRE(map2 == (Map<int, std::string>({{4, "four"}, {5, "five"}})));
}

// operator=()
TEST_CASE("Map: move_assignment")
{
    Map<int, std::string> map1({{1, "one"}, {2, "two"}, {3, "three"}});
    Map<int, std::string> map2({{4, "four"}, {5, "five"}});

    map1 = std::move(map2);
    REQUIRE(map1 == (Map<int, std::string>({{4, "four"}, {5, "five"}})));
    REQUIRE(map2 == (Map<int, std::string>()));
}

// operator[]() get()
TEST_CASE("Map: access")
{
    Map<std::string, int> map({{"one", 1}, {"two", 2}, {"three", 3}});

    // get
    REQUIRE(map.get("one") == 1);
    REQUIRE(map.get("not exist") == 0);
    REQUIRE(map.get("not exist", 233) == 233);

    // access
    REQUIRE(map["one"] == 1);
    REQUIRE(map["two"] == 2);
    REQUIRE(map["three"] == 3);

    // assignment
    map["one"] = 1111;
    REQUIRE(map["one"] == 1111);

    // check key
    MY_ASSERT_THROW_MESSAGE(map["four"], std::runtime_error, "Error: Key is not found in the map.");
}

// begin() end()
TEST_CASE("Map: iterator")
{
    // empty
    Map<int, std::string> empty;
    REQUIRE(empty.begin() == empty.end());

    // key, value=key^2
    Map<int, int> map({{1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}});

    // for
    int k = 1;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        REQUIRE(it->key() == k);
        REQUIRE(it->value() == k * k);
        ++k;
    }

    // for in
    k = 1;
    for (const auto& e : map)
    {
        REQUIRE(e.key() == k);
        REQUIRE(e.value() == k * k);
        ++k;
    }
}

// find() contains() min() max()
TEST_CASE("Map: examination")
{
    Map<int, std::string> map({{1, "one"}, {2, "two"}, {3, "three"}});

    // find
    REQUIRE(map.find(1) == map.begin());
    REQUIRE(map.find(3) == --map.end());
    REQUIRE(map.find(0) == map.end());

    // contains
    REQUIRE(map.contains(1) == true);
    REQUIRE(map.contains(3) == true);
    REQUIRE(map.contains(0) == false);

    // min
    REQUIRE(map.min() == 1);

    // max
    REQUIRE(map.max() == 3);
}

// keys() values()
TEST_CASE("Map: keys_values")
{
    Map<int, std::string> map({{1, "one"}, {2, "two"}, {3, "three"}});

    REQUIRE(map.keys() == Set<int>({1, 2, 3}));
    REQUIRE(map.values() == Set<std::string>({"one", "two", "three"}));
}

// operator+=()
TEST_CASE("Map: insert")
{
    Map<int, std::string> map;

    map += {3, "three"};
    map += {1, "one"};
    map += {2, "two"};

    REQUIRE(map == (Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}})));

    map += {3, "three"};
    map += {1, "one"};
    map += {2, "two"};

    REQUIRE(map == (Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}})));
}

// operator-=()
TEST_CASE("Map: remove")
{
    Map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};

    map -= 3;
    map -= 1;
    map -= 2;

    REQUIRE(map == (Map<int, std::string>()));

    map -= 2;
    map -= 1;
    map -= 3;

    REQUIRE(map == (Map<int, std::string>()));
}

// clear()
TEST_CASE("Map: clear")
{
    Map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};

    REQUIRE(map.clear() == (Map<int, std::string>()));

    // double clear
    REQUIRE(map.clear() == (Map<int, std::string>()));

    // modify after clear
    map += {1, "one"};
    REQUIRE(map == (Map<int, std::string>({{1, "one"}})));
}

// operator<<()
TEST_CASE("Map: print")
{
    std::ostringstream oss;

    Map<int, std::string> empty;
    oss << empty;
    REQUIRE(oss.str() == "{}");
    oss.str("");

    Map<int, std::string> one = {{1, "one"}};
    oss << one;
    REQUIRE(oss.str() == "{1: one}");
    oss.str("");

    Map<int, std::string> many = {{1, "one"}, {2, "two"}, {3, "three"}};
    oss << many;
    REQUIRE(oss.str() == "{1: one, 2: two, 3: three}");
    oss.str("");
}
