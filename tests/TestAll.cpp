#include "../sources/mds.hpp"

#include "tool.hpp"

using namespace mds;

TEST(All, demo)
{
    Map<String, List<Integer>> map = {{"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {"12345678987654321", 5}}};

    std::ostringstream oss;
    oss << map;
    ASSERT_EQ(oss.str(), "{\"first\": [123, 456], \"second\": [789], \"third\": [12345678987654321, 5]}");
    oss.str("");

    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map.keys(), Set<String>({"first", "second", "third"}));
    ASSERT_EQ(map["third"][-1].factorial(), 120);
}
