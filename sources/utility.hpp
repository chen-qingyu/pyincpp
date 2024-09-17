//! @file utility.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Utility functions of PyInCpp.
//! @date 2023.01.05

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <algorithm>   // std::copy std::find std::rotate ...
#include <climits>     // INT_MAX
#include <cmath>       // std::abs std::pow std::sqrt std::atan2 std::hypot INFINITY NAN
#include <cstring>     // std::strlen
#include <istream>     // std::istream
#include <iterator>    // std::input_iterator
#include <limits>      // std::numeric_limits
#include <ostream>     // std::ostream
#include <random>      // std::random_device std::mt19937 std::uniform_int_distribution
#include <sstream>     // std::ostringstream
#include <stdexcept>   // std::runtime_error
#include <string>      // std::string std::getline
#include <string_view> // std::string_view
#include <utility>     // std::initializer_list std::move
#include <vector>      // std::vector

namespace pyincpp::internal
{

// Check whether the index is valid (begin <= pos < end).
static inline void check_bounds(int pos, int begin, int end)
{
    if (pos < begin || pos >= end)
    {
        throw std::runtime_error("Error: Index out of range.");
    }
}

// Check whether the container is not empty.
static inline void check_empty(int size)
{
    if (size == 0)
    {
        throw std::runtime_error("Error: The container is empty.");
    }
}

// Check whether there is any remaining capacity.
static inline void check_full(int size, int capacity)
{
    if (size >= capacity)
    {
        throw std::runtime_error("Error: The container has reached the maximum size.");
    }
}

// Check whether the number is not zero.
template <typename T>
static inline void check_zero(T number)
{
    if (number == T(0))
    {
        throw std::runtime_error("Error: Divide by zero.");
    }
}

// Print helper for Pair.
// This function can only be placed here because of the header file reference order.
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<const K, V>& pair)
{
    return os << pair.first << ": " << pair.second;
}

// Print helper for range [`first`, `last`).
template <std::input_iterator InputIt>
static inline std::ostream& print(std::ostream& os, const InputIt& first, const InputIt& last, char open, char close)
{
    // This form looks complex, but there is only one judgment in the loop.
    // At the Assembly level (see https://godbolt.org/z/qT9n7GKf8), this is more efficient
    // than the usual short form of the generated machine code under O3-level optimization.
    // The inspiration comes from Java source code.

    if (first == last)
    {
        return os << open << close;
    }

    os << open;
    auto it = first;
    while (true)
    {
        os << *it++;
        if (it == last)
        {
            return os << close;
        }
        os << ", ";
    }
}

template <typename T>
static inline T gcd(T a, T b)
{
    // using Euclidean algorithm

    while (b != 0) // a, b = b, a % b until b == 0
    {
        auto t = b;
        b = a % b;
        a = t;
    }

    return a; // a is the GCD
}

} // namespace pyincpp::internal

#endif // UTILITY_HPP
