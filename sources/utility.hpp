//! @file utility.hpp
//! @author Qingyu Chen (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
//! @brief Utility functions for PyInCpp.
//! @date 2023.01.05
//!
//! @copyright Copyright (C) 2023 - 2024
//!
//! This program is free software: you can redistribute it and/or modify
//! it under the terms of the GNU General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//!
//! This program is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//! GNU General Public License for more details.
//!
//! You should have received a copy of the GNU General Public License
//! along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <algorithm>   // std::copy std::find std::rotate ...
#include <climits>     // INT_MAX
#include <cmath>       // std::abs std::pow INFINITY NAN
#include <cstring>     // std::strlen
#include <iostream>    // std::istream std::ostream
#include <iterator>    // std::input_iterator
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
constexpr static inline void check_bounds(int pos, int begin, int end)
{
    if (pos < begin || pos >= end)
    {
        throw std::runtime_error("Error: Index out of range.");
    }
}

// Check whether is not empty.
constexpr static inline void check_empty(int size)
{
    if (size == 0)
    {
        throw std::runtime_error("Error: The container is empty.");
    }
}

// Check whether there is any remaining capacity.
constexpr static inline void check_full(int size, int capacity)
{
    if (size >= capacity)
    {
        throw std::runtime_error("Error: The container has reached the maximum size.");
    }
}

// Print helper for range [`first`, `last`).
constexpr static inline std::ostream& print(std::ostream& os, std::input_iterator auto first, std::input_iterator auto last, char open, char close)
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

} // namespace pyincpp::internal

#endif // UTILITY_HPP
