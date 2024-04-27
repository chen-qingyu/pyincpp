#include "../sources/Dict.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Dict")
{
    SECTION("basics")
    {
        // Dict()
        Dict<int, std::string> dict1;
        REQUIRE(dict1.size() == 0);
        REQUIRE(dict1.is_empty());

        // Dict(const std::initializer_list<Pair<K, V>>& init)
        Dict<int, std::string> dict2({{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}});
        REQUIRE(dict2.size() == 5);
        REQUIRE(!dict2.is_empty());

        // Dict(const InputIt& first, const InputIt& last)
        Dict<int, std::string> dict3(dict2.begin(), dict2.end());
        REQUIRE(dict3.size() == 5);
        REQUIRE(!dict3.is_empty());

        // Dict(const Dict& that)
        Dict<int, std::string> dict4(dict3);
        REQUIRE(dict4.size() == 5);
        REQUIRE(!dict4.is_empty());

        // Dict(Dict&& that)
        Dict<int, std::string> dict5(std::move(dict4));
        REQUIRE(dict5.size() == 5);
        REQUIRE(!dict5.is_empty());
        REQUIRE(dict4.size() == 0);
        REQUIRE(dict4.is_empty());

        // test compatibility
        Dict<EqLtType, EqType> test = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
        REQUIRE(test.size() == 5);
        REQUIRE(!test.is_empty());

        // ~Dict()
    }

    Dict<int, std::string> empty;
    Dict<int, std::string> one = {{1, "one"}};
    Dict<int, std::string> some = {{1, "one"}, {2, "two"}, {3, "three"}};

    SECTION("compare")
    {
        // operator==
        Dict<int, std::string> eq_dict = {{1, "one"}, {2, "two"}, {3, "three"}};
        REQUIRE(eq_dict == some);

        // operator!=
        Dict<int, std::string> ne_dict = {{1, "one"}, {2, "two"}, {3, "three!"}};
        REQUIRE(ne_dict != some);
    }

    SECTION("assignment")
    {
        some = one; // copy
        REQUIRE(some == Dict<int, std::string>({{1, "one"}}));
        REQUIRE(one == Dict<int, std::string>({{1, "one"}}));

        empty = std::move(one); // move
        REQUIRE(empty == Dict<int, std::string>({{1, "one"}}));
        REQUIRE(one == Dict<int, std::string>());
    }

    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());
        REQUIRE(empty.rbegin() == empty.rend());

        // for in
        for (const auto& [k, v] : Dict<int, int>({{1, 1}, {2, 4}, {3, 9}}))
        {
            REQUIRE(k * k == v);
        }

        // reversed for
        auto it = some.rbegin();
        REQUIRE(*it++ == Pair{3, "three"});
        REQUIRE(*it++ == Pair{2, "two"});
        REQUIRE(*it++ == Pair{1, "one"});
        REQUIRE(it == some.rend());
    }

    SECTION("access")
    {
        Dict<std::string, int> dict({{"one", 1}, {"two", 2}, {"three", 3}});

        // get
        REQUIRE(dict.get("one", 233) == 1);
        REQUIRE(dict.get("not exist", 233) == 233);

        // access
        REQUIRE(dict["one"] == 1);
        REQUIRE(dict["two"] == 2);
        REQUIRE(dict["three"] == 3);

        // assignment
        dict["one"] = 1111;
        REQUIRE(dict["one"] == 1111);

        // check key
        REQUIRE_THROWS_MATCHES(dict["four"], std::runtime_error, Message("Error: Key is not found in the dictionary."));

        // const access
        const Dict<std::string, int> const_dict({{"one", 1}, {"two", 2}, {"three", 3}});
        REQUIRE(const_dict["one"] == 1);
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

        REQUIRE(empty == Dict<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}}));

        REQUIRE(empty.add(3, "three") == false);
        REQUIRE(empty.add(1, "one") == false);
        REQUIRE(empty.add(2, "two") == false);

        REQUIRE(empty == Dict<int, std::string>({{1, "one"}, {2, "two"}, {3, "three"}}));
    }

    SECTION("remove")
    {
        REQUIRE(some.remove(3) == true);
        REQUIRE(some.remove(1) == true);
        REQUIRE(some.remove(2) == true);

        REQUIRE(some == Dict<int, std::string>());

        REQUIRE(some.remove(2) == false);
        REQUIRE(some.remove(1) == false);
        REQUIRE(some.remove(3) == false);

        REQUIRE(some == Dict<int, std::string>());
    }

    SECTION("clear")
    {
        some.clear();
        REQUIRE(some == Dict<int, std::string>());
        some.clear(); // double clear
        REQUIRE(some == Dict<int, std::string>());
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
