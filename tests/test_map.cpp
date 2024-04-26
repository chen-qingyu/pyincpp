#include "../sources/Map.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Map")
{
    SECTION("basics")
    {
        // Map()
        Map<int, std::string> map1;
        REQUIRE(map1.size() == 0);
        REQUIRE(map1.is_empty());

        // Map(const std::initializer_list<Pair<K, V>>& init)
        Map<int, std::string> map2({{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}});
        REQUIRE(map2.size() == 5);
        REQUIRE(!map2.is_empty());

        // Map(const InputIt& first, const InputIt& last)
        Map<int, std::string> map3(map2.begin(), map2.end());
        REQUIRE(map3.size() == 5);
        REQUIRE(!map3.is_empty());

        // Map(const Map& that)
        Map<int, std::string> map4(map3);
        REQUIRE(map4.size() == 5);
        REQUIRE(!map4.is_empty());

        // Map(Map&& that)
        Map<int, std::string> map5(std::move(map4));
        REQUIRE(map5.size() == 5);
        REQUIRE(!map5.is_empty());
        REQUIRE(map4.size() == 0);
        REQUIRE(map4.is_empty());

        // test compatibility
        Map<EqLtType, EqType> test = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
        REQUIRE(test.size() == 5);
        REQUIRE(!test.is_empty());

        // ~Map()
    }

    Map<int, std::string> empty;
    Map<int, std::string> one = {{1, "one"}};
    Map<int, std::string> some = {{1, "one"}, {2, "two"}, {3, "three"}};

    SECTION("compare")
    {
        // operator==
        Map<int, std::string> eq_map = {{1, "one"}, {2, "two"}, {3, "three"}};
        REQUIRE(eq_map == some);

        // operator!=
        Map<int, std::string> ne_map = {{1, "one"}, {2, "two"}, {3, "three!"}};
        REQUIRE(ne_map != some);
    }

    SECTION("assignment")
    {
        some = one; // copy
        REQUIRE(some == Map<int, std::string>({{1, "one"}}));
        REQUIRE(one == Map<int, std::string>({{1, "one"}}));

        empty = std::move(one); // move
        REQUIRE(empty == Map<int, std::string>({{1, "one"}}));
        REQUIRE(one == Map<int, std::string>());
    }

    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());

        // for in
        for (const auto& [k, v] : Map<int, int>({{1, 1}, {2, 4}, {3, 9}}))
        {
            REQUIRE(k * k == v);
        }
    }

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

    SECTION("keys_values_items")
    {
        REQUIRE(some.keys() == Set<int>({1, 2, 3}));
        REQUIRE(some.values() == Set<std::string>({"one", "two", "three"}));
        REQUIRE(some.items() == Set<Pair<int, std::string>>({{1, "one"}, {2, "two"}, {3, "three"}}));
    }

    SECTION("add")
    {
        REQUIRE(empty.add(3, "three") == true);
        REQUIRE(empty.add(1, "one") == true);
        REQUIRE(empty.add(2, "two") == true);

        REQUIRE(empty == Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}}));

        REQUIRE(empty.add(3, "three") == false);
        REQUIRE(empty.add(1, "one") == false);
        REQUIRE(empty.add(2, "two") == false);

        REQUIRE(empty == Map<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}}));
    }

    SECTION("remove")
    {
        REQUIRE(some.remove(3) == true);
        REQUIRE(some.remove(1) == true);
        REQUIRE(some.remove(2) == true);

        REQUIRE(some == Map<int, std::string>());

        REQUIRE(some.remove(2) == false);
        REQUIRE(some.remove(1) == false);
        REQUIRE(some.remove(3) == false);

        REQUIRE(some == Map<int, std::string>());
    }

    SECTION("clear")
    {
        some.clear();
        REQUIRE(some == Map<int, std::string>());
        some.clear(); // double clear
        REQUIRE(some == Map<int, std::string>());
    }

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
