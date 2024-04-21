#include "../sources/Map.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Map")
{
    // constructor destructor size() is_empty()
    SECTION("basics")
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

    Map<int, std::string> empty;
    Map<int, std::string> one = {{1, "one"}};
    Map<int, std::string> some = {{1, "one"}, {2, "two"}, {3, "three"}};

    // operator==() operator!=()
    SECTION("compare")
    {
        // operator==
        Map<int, std::string> eq_map = {{1, "one"}, {2, "two"}, {3, "three"}};
        REQUIRE(eq_map == some);

        // operator!=
        Map<int, std::string> ne_map = {{1, "one"}, {2, "two"}, {3, "three!"}};
        REQUIRE(ne_map != some);
    }

    // operator=()
    SECTION("copy_assignment")
    {
        some = one;
        REQUIRE(some == (Map<int, std::string>({{1, "one"}})));
        REQUIRE(one == (Map<int, std::string>({{1, "one"}})));
    }

    // operator=()
    SECTION("move_assignment")
    {
        some = std::move(one);
        REQUIRE(some == (Map<int, std::string>({{1, "one"}})));
        REQUIRE(one == (Map<int, std::string>()));
    }

    // operator[]() get()
    SECTION("access")
    {
        Map<std::string, int> map({{"one", 1}, {"two", 2}, {"three", 3}});

        // get
        REQUIRE(map.get("one", 233) == 1);
        REQUIRE(map.get("not exist", 233) == 233);

        // access
        REQUIRE(map["one"] == 1);
        REQUIRE(map["two"] == 2);
        REQUIRE(map["three"] == 3);

        // assignment
        map["one"] = 1111;
        REQUIRE(map["one"] == 1111);

        // check key
        REQUIRE_THROWS_MATCHES(map["four"], std::runtime_error, Message("Error: Key is not found in the map."));

        // const access
        const Map<std::string, int> const_map({{"one", 1}, {"two", 2}, {"three", 3}});
        REQUIRE(const_map["one"] == 1);
    }

    // begin() end()
    SECTION("iterator")
    {
        // empty
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

        map.begin()->value() = 233;
        REQUIRE(map.begin()->value() == 233);
        (*map.begin()).value() = 999;
        REQUIRE(map.begin()->value() == 999);
    }

    // find() contains() min() max()
    SECTION("examination")
    {
        // find
        REQUIRE(some.find(1) == some.begin());
        REQUIRE(some.find(3) == --some.end());
        REQUIRE(some.find(0) == some.end());

        // contains
        REQUIRE(some.contains(1) == true);
        REQUIRE(some.contains(3) == true);
        REQUIRE(some.contains(0) == false);

        // min
        REQUIRE(some.min() == 1);

        // max
        REQUIRE(some.max() == 3);
    }

    // keys() values() items()
    SECTION("keys_values_items")
    {
        REQUIRE(some.keys() == Set<int>({1, 2, 3}));
        REQUIRE(some.values() == Set<std::string>({"one", "two", "three"}));
        REQUIRE(some.items() == Set<Pair<int, std::string>>({{1, "one"}, {2, "two"}, {3, "three"}}));
    }

    // operator+=()
    SECTION("insert")
    {
        empty += {3, "three"};
        empty += {1, "one"};
        empty += {2, "two"};

        REQUIRE(empty == (Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}})));

        empty += {3, "three"};
        empty += {1, "one"};
        empty += {2, "two"};

        REQUIRE(empty == (Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}})));
    }

    // operator-=()
    SECTION("remove")
    {
        some -= 3;
        some -= 1;
        some -= 2;

        REQUIRE(some == (Map<int, std::string>()));

        some -= 2;
        some -= 1;
        some -= 3;

        REQUIRE(some == (Map<int, std::string>()));
    }

    // clear()
    SECTION("clear")
    {
        REQUIRE(some.clear() == (Map<int, std::string>()));

        // double clear
        REQUIRE(some.clear() == (Map<int, std::string>()));

        // modify after clear
        some += {1, "one"};
        REQUIRE(some == (Map<int, std::string>({{1, "one"}})));
    }

    // operator<<()
    SECTION("print")
    {
        std::ostringstream oss;

        oss << empty;
        REQUIRE(oss.str() == "{}");
        oss.str("");

        oss << one;
        REQUIRE(oss.str() == "{1: one}");
        oss.str("");

        oss << some;
        REQUIRE(oss.str() == "{1: one, 2: two, 3: three}");
        oss.str("");
    }
}
