// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/mdspp.hpp"

#include "my_tools.hpp"

using namespace mdspp;

TEST(All, demo)
{
    Map<String, List<Integer>> map = {{"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {123456789, 5}}};

    std::ostringstream oss;
    oss << map;
    ASSERT_EQ(oss.str(), "{\"first\": [123, 456], \"second\": [789], \"third\": [123456789, 5]}");
    oss.str("");

    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map.keys(), Set<String>({"first", "second", "third"}));
    ASSERT_EQ(map["third"][-1].factorial(), 120);
}
