//! @file tuple.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Tuple template class.
//! @date 2023.03.13

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
    template <typename... X>
    friend class Tuple;

private:
    // The value stored in current level.
    T value_;

public:
    /*
     * Constructor
     */

    /// Construct a new tuple.
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
        return *(static_cast<const Tuple<Ts...>*>(this));
    }
};

/*
 * Non-member functions
 */

/// Create a tuple, deducing the target type from the types of arguments.
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
