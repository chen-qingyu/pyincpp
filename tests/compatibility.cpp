// g++ -std=c++20 -Wall -Wextra -Wold-style-cast -Wpedantic -Wno-sign-compare -o .\build\compatibility.exe .\tests\compatibility.cpp && .\build\compatibility.exe

#include "../sources/pyincpp.hpp"

#include <cassert>
#include <iostream>
#include <unordered_set>

using namespace pyincpp;

int main()
{
    // 1. All types can be printed and easily combined:
    Dict<Str, List<Int>> dict = {{"first", {"123", "456"}}, {"second", {"789"}}, {"third", {"12345678987654321", "5"}}};
    std::ostringstream oss;
    oss << dict;
    assert(oss.str() == "{\"first\": [123, 456], \"second\": [789], \"third\": [12345678987654321, 5]}");
    assert(dict.keys() == (Set<Str>{"first", "second", "third"}));
    assert(dict["third"][-1].factorial() == 120);

    // 2. All container types are iterable:
    for (const auto& [k, v] : Dict<int, int>{{1, 1}, {2, 4}, {3, 9}})
    {
        assert(k * k == v);
    }

    // 3. All immutable types are hashable:
    std::unordered_set<Int> set1 = {"1", "2", "3", "18446744073709551617"};
    std::unordered_set<Str> set2 = {"hello", "pyincpp"};
    std::unordered_set<Fraction> set3 = {{1, 2}, {3, 4}};

    assert(pyincpp::make_tuple(std::string("hello")) != pyincpp::make_tuple(1, 2, 3));
    assert(pyincpp::make_tuple(std::string("hello")) != pyincpp::make_tuple(233));

    assert(Complex::pow(Complex(1, 2), Complex(3, 4)) == Complex(0.12900959407446697, 0.03392409290517014));

    std::cout << "OK!\n";
}
