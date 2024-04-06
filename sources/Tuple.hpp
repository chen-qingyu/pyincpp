/**
 * @file Tuple.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Tuple template class.
 * @date 2023.03.13
 *
 * @copyright Copyright (C) 2023
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TUPLE_HPP
#define TUPLE_HPP

#include "utility.hpp"

namespace pyincpp
{

// Empty tuple template class.
template <typename... _>
class Tuple
{
public:
    // Check whether two tuples are equal.
    template <typename... X>
    constexpr bool operator==(const Tuple<X...>& that) const
    {
        return that.size() == 0;
    }

    // Check whether two tuples are not equal.
    template <typename... X>
    constexpr bool operator!=(const Tuple<X...>& that) const
    {
        return that.size() != 0;
    }

    // Empty tuple have no element.
    constexpr int size() const
    {
        return 0;
    }
};

/**
 * @brief Tuple template class.
 *
 * @tparam T the type of first element
 * @tparam Ts the type of rest elements
 */
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

    /**
     * @brief Construct a new tuple object.
     *
     * @param value the first element
     * @param values the rest elements
     */
    Tuple<T, Ts...>(const T& value, const Ts&... values)
        : Tuple<Ts...>(values...)
        , value_(value)
    {
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two tuples are equal.
     *
     * @param that another tuple
     * @return true if two tuples are equal
     */
    template <typename... X>
    bool operator==(const Tuple<X...>& that) const
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

    /**
     * @brief Check whether two tuples are not equal.
     *
     * @param that another tuple
     * @return true if two tuples are not equal
     */
    template <typename... X>
    bool operator!=(const Tuple<X...>& that) const
    {
        return !(*this == that);
    }

    /*
     * Access
     */

    /**
     * @brief Get the i-th element of the tuple.
     *
     * @tparam N the index
     * @return the i-th element
     */
    template <int N>
    decltype(auto) get() const
    {
        static_assert(N < 1 + sizeof...(Ts));
        if constexpr (N == 0)
        {
            return value_;
        }
        else
        {
            return Tuple<Ts...>::template get<N - 1>();
        }
    }

    /*
     * Examination
     */

    /**
     * @brief Return the number of elements in the tuple.
     *
     * @return the number of elements in the tuple
     */
    constexpr int size() const
    {
        return 1 + sizeof...(Ts);
    }

    /**
     * @brief Return the rest of the tuple.
     *
     * @return the rest of the tuple
     */
    const Tuple<Ts...>& rest() const
    {
        return *((Tuple<Ts...>*)this);
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Creates a tuple object, deducing the target type from the types of arguments.
 *
 * @tparam Ts the type of elements in the tuple
 * @param values zero or more arguments to construct the tuple from
 * @return a tuple object containing the given values
 */
template <typename... Ts>
decltype(auto) make_tuple(const Ts&... values)
{
    return Tuple<Ts...>(values...);
}

namespace internal
{

template <typename... Ts>
static void print(std::ostream& os, const Tuple<Ts...>& tuple)
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

/**
 * @brief Output tuple data to the specified output stream.
 *
 * @tparam Ts the type of elements in the tuple, must be printable
 * @param os an output stream
 * @param tuple the tuple to be printed to the output stream
 * @return self reference of the output stream
 */
template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const Tuple<Ts...>& tuple)
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
