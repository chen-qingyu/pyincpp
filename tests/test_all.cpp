#include "../sources/pyincpp.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("All")
{
    Dict<Str, List<Int>> dict = {{"first", {"123", "456"}}, {"second", {"789"}}, {"third", {"12345678987654321", "5"}}};

    std::ostringstream oss;
    oss << dict;
    REQUIRE(oss.str() == "{\"first\": [123, 456], \"second\": [789], \"third\": [12345678987654321, 5]}");
    REQUIRE(dict.keys() == Set<Str>({"first", "second", "third"}));
    REQUIRE(dict["third"][-1].factorial() == 120);
}
