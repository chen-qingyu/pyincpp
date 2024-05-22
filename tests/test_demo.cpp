#include "../sources/pyincpp.hpp"

#include "tool.hpp"

#include <unordered_set>

using namespace pyincpp;

TEST_CASE("Usage")
{
    // List support negative index
    REQUIRE(List<int>{1, 2, 3, 4, 5}[-1] == 5);
    // List traversal
    REQUIRE(List<int>{1, 2, 3, 4, 5}.map([](int& x)
                                         { x *= 2; }) == List<int>{2, 4, 6, 8, 10});
    // List uniquify
    REQUIRE(List<int>{1, 2, 3, 1, 2, 3, 1, 2, 3}.uniquify() == List<int>{1, 2, 3});
    // List sort, stable sort, default from small to large, customizable comparator
    REQUIRE(List<int>{1, 2, 3, 4, 5}.sort([](const int& e1, const int& e2)
                                          { return e1 > e2; }) == List<int>{5, 4, 3, 2, 1});

    // test whether a Set is proper subset of another Set
    REQUIRE(Set<int>{5, 1} < Set<int>{1, 2, 3, 4, 5});
    // intersection of Sets, support intersection, union, difference, and symmetric difference
    REQUIRE((Set<int>{1, 2, 3, 4, 5} & Set<int>{1, 3, 5, 7, 9}) == Set<int>{1, 3, 5});

    // Dict access
    REQUIRE(Dict<Str, int>{{"one", 1}, {"two", 2}, {"three", 3}}["one"] == 1);
    // Dict get values
    REQUIRE(Dict<Str, int>{{"one", 1}, {"two", 2}, {"three", 3}}.values() == Set<int>{1, 2, 3});

    // Int basic operation, support +, -, *, /, % and compare
    REQUIRE(Int("18446744073709551617") + Int("18446744073709551617") == "36893488147419103234");
    // Int increment, after my optimization, much faster than `+= 1`
    REQUIRE(++Int("99999999999999") == "100000000000000");
    // Int modular power, very fast
    REQUIRE(Int::pow("1024", "1024", "100") == "76");
    // Int factorial
    REQUIRE(Int("5").factorial().factorial() == "6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000");
    // get random Int, using hardware device to generate true random integer if possible
    REQUIRE(Int::random().digits() <= 4300);
    // calculate the next prime that greater than this
    REQUIRE(Int(0).next_prime() == "2");

    // convert Str to floating-point number, support inf and nan
    REQUIRE(Str(".1e-2").to_decimal() == 0.1e-2);
    // convert Str to integer, support base 2-36
    REQUIRE(Str("-ffffffffffffffff").to_integer(16) == Int("-18446744073709551615"));
    // Str repeat
    REQUIRE(Str("hello! ") * 2 == "hello! hello! ");
    // Str replace
    REQUIRE(Str("hahaha").replace("a", "ooow~ ") == "hooow~ hooow~ hooow~ ");
    // Str slice
    REQUIRE(Str("12345").slice(0, 5, 2) == "135");
    // Str split
    REQUIRE(Str("one, two, three").split(", ") == List<Str>{"one", "two", "three"});
    // Str join
    REQUIRE(Str(".").join({"192", "168", "0", "1"}) == "192.168.0.1");
    // Str format
    REQUIRE(Str("I'm {}, {} years old.").format("Alice", 18) == "I'm Alice, 18 years old.");

    // Tuple index, return type different, so template function is used
    REQUIRE(Tuple<int, double, char>(1, 2.5, 'A').get<2>() == 'A');
    // take the remaining part of the Tuple, the underlying layer is pointer conversion, which is very fast
    REQUIRE(Tuple<int, double, char>(1, 2.5, 'A').rest() == Tuple<double, char>(2.5, 'A'));

    // Complex addition
    REQUIRE(Complex(1, 2) + Complex(1, 3) == Complex(2, 5));
    // Complex power
    REQUIRE(Complex::pow(Complex(1, 2), Complex(-1, 2)) == Complex(0.04281551979798478, 0.023517649351954585));

    // Deque element reference
    REQUIRE(Deque<int>{1, 2, 3, 4, 5}.front() == 1);
    // Deque rotate to right (or left), very vivid!
    REQUIRE((Deque<int>{1, 2, 3, 4, 5} >>= 1) == Deque<int>{5, 1, 2, 3, 4});

    // Fraction addition
    REQUIRE(Fraction(1, 2) + Fraction(1, 3) == Fraction(5, 6));
    // Fraction modulo
    REQUIRE(Fraction(1, 2) % Fraction(1, 3) == Fraction(1, 6));
}

TEST_CASE("Advantage")
{
    // 1. All types can be printed and easily combined:
    Dict<Str, List<Int>> dict = {{"first", {"123", "456"}}, {"second", {"789"}}, {"third", {"12345678987654321", "5"}}};
    std::ostringstream oss;
    oss << dict;
    REQUIRE(oss.str() == "{\"first\": [123, 456], \"second\": [789], \"third\": [12345678987654321, 5]}");
    REQUIRE(dict.keys() == Set<Str>{"first", "second", "third"});
    REQUIRE(dict["third"][-1].factorial() == 120);

    // 2. All container types are iterable:
    for (const auto& [k, v] : Dict<int, int>{{1, 1}, {2, 4}, {3, 9}})
    {
        REQUIRE(k * k == v);
    }

    // 3. All immutable types are hashable:
    std::unordered_set<Int> set1 = {"1", "2", "3", "18446744073709551617"};
    std::unordered_set<Str> set2 = {"hello", "pyincpp"};
    std::unordered_set<Fraction> set3 = {{1, 2}, {3, 4}};
}
