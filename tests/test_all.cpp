#include "../sources/pytype.hpp"

#include "tool.hpp"

using namespace pytype;

TEST_CASE("All")
{
    Map<String, List<Integer>> map = {{"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {"12345678987654321", 5}}};

    std::ostringstream oss;
    oss << map;
    REQUIRE(oss.str() == "{\"first\": [123, 456], \"second\": [789], \"third\": [12345678987654321, 5]}");
    oss.str("");

    REQUIRE(map.size() == 3);
    REQUIRE(map.keys() == Set<String>({"first", "second", "third"}));
    REQUIRE(map["third"][-1].factorial() == 120);
}
