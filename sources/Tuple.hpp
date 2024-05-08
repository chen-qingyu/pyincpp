//! @file Tuple.hpp
//! @author Qingyu Chen (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
//! @brief Tuple template class.
//! @date 2023.03.13
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

#ifndef TUPLE_HPP
#define TUPLE_HPP

#include "utility.hpp"

namespace pyincpp
{

/// Empty tuple template class.
template <typename... _>
class Tuple
{
public:
    /// Check whether two tuples are equal.
    template <typename... X>
    constexpr bool operator==(const Tuple<X...>& that) const
    {
        return that.size() == 0;
    }

    /// Check whether two tuples are not equal.
    template <typename... X>
    constexpr bool operator!=(const Tuple<X...>& that) const
    {
        return that.size() != 0;
    }

    /// Empty tuple have no element.
    constexpr int size() const
    {
        return 0;
    }
};

/// Tuple template class.
template <typename T, typename... Ts>
class Tuple<T, Ts...> : public Tuple<Ts...>
{
    friend class Tuple;

private:
    // The value stored in current level.
    T value_;

public:
    /*
     * Constructor / Destructor
     */

    /// Construct a new tuple object.
    constexpr Tuple<T, Ts...>(const T& value, const Ts&... values)
        : Tuple<Ts...>(values...)
        , value_(value)
    {
    }

    /*
     * Comparison
     */

    /// Check whether two tuples are equal.
    template <typename... X>
    constexpr bool operator==(const Tuple<X...>& that) const
    {
        if constexpr (sizeof...(X) > 0)
        {
            return value_ == that.value_ ? (rest() == that.rest()) : false;
        }
        else
        {
            return false;
        }
    }

    /*
     * Access
     */

    /// Get the i-th element of the tuple.
    template <int I>
    constexpr decltype(auto) get() const
    {
        static_assert(I < 1 + sizeof...(Ts));
        if constexpr (I == 0)
        {
            return value_;
        }
        else
        {
            return Tuple<Ts...>::template get<I - 1>();
        }
    }

    /*
     * Examination
     */

    /// Return the number of elements in the tuple.
    constexpr int size() const
    {
        return 1 + sizeof...(Ts);
    }

    /// Return the rest of the tuple.
    constexpr const Tuple<Ts...>& rest() const
    {
        return *((Tuple<Ts...>*)this);
    }
};

/*
 * Non-member functions
 */

/// Creates a tuple object, deducing the target type from the types of arguments.
template <typename... Ts>
constexpr decltype(auto) make_tuple(const Ts&... values)
{
    return Tuple<Ts...>(values...);
}

namespace internal
{

template <typename... Ts>
constexpr static void print(std::ostream& os, const Tuple<Ts...>& tuple)
{
    if constexpr (sizeof...(Ts) > 0)
    {
        os << tuple.template get<0>() << (tuple.size() == 1 ? "" : ", ");
        print(os, tuple.rest());
    }
}

} // namespace internal

/*
 * Print
 */

/// Output the tuple to the specified output stream.
template <typename... Ts>
constexpr std::ostream& operator<<(std::ostream& os, const Tuple<Ts...>& tuple)
{
    if constexpr (sizeof...(Ts) == 1)
    {
        return os << "(" << tuple.template get<0>() << ",)";
    }

    os << "(";
    internal::print(os, tuple);
    os << ")";

    return os;
}

} // namespace pyincpp

#endif // TUPLE_HPP
