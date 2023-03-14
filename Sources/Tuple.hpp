/**
 * @file Tuple.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Tuple template class.
 * @version 1.0
 * @date 2023.03.13
 *
 * @copyright Copyright (c) 2023
 */

#ifndef TUPLE_HPP
#define TUPLE_HPP

namespace mdspp
{

template <typename... Args>
class Tuple;

/**
 * @brief Empty tuple template class.
 */
template <>
class Tuple<>
{
public:
    // Two empty tuples are equal.
    bool operator==(const Tuple<>& that) const
    {
        return true;
    }

    // Two empty tuples are equal.
    bool operator!=(const Tuple<>& that) const
    {
        return false;
    }

    // Empty tuple have no element.
    int size() const
    {
        return 0;
    }
};

/**
 * @brief Tuple template class.
 *
 * @tparam T the first element
 * @tparam Ts the rest elements
 */
template <typename T, typename... Ts>
class Tuple<T, Ts...> : public Tuple<Ts...>
{
    template <int N, typename... Args>
    friend auto get(const Tuple<Args...>& tuple);

private:
    // The value stored in current level.
    T value_;

    // Check whether two tuples are equal.
    template <typename... Args>
    static bool eq(const Tuple<Args...>& tuple1, const Tuple<Args...>& tuple2)
    {
        // tuple1.value_ == tuple2.value_ compile error
        return get<0>(tuple1) == get<0>(tuple2) ? eq(tuple1.rest(), tuple2.rest()) : false;
    }

    // Two empty tuples are equal. Must appear after eq(Tuple<Args...>, Tuple<Args...>)
    template <>
    static bool eq(const Tuple<>& tuple1, const Tuple<>& tuple2)
    {
        return true;
    }

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
    bool operator==(const Tuple<T, Ts...>& that) const
    {
        return size() == that.size() ? eq(*this, that) : false;
    }

    /**
     * @brief Check whether two tuples are not equal.
     *
     * @param that another tuple
     * @return true if two tuples are not equal
     */
    bool operator!=(const Tuple<T, Ts...>& that) const
    {
        return !(*this == that);
    }

    /*
     * Examination (will not change the object itself)
     */

    /**
     * @brief Return the number of elements in the tuple.
     *
     * @return the number of elements in the tuple
     */
    int size() const
    {
        return 1 + Tuple<Ts...>::size();
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

// Underlying implementation
namespace impl
{

template <int N, typename... Args>
struct Get;

template <typename T, typename... Ts>
struct Get<0, Tuple<T, Ts...>>
{
    using class_type = Tuple<T, Ts...>;
};

template <int N, typename T, typename... Ts>
struct Get<N, Tuple<T, Ts...>> : Get<N - 1, Tuple<Ts...>>
{
};

template <typename... Args>
void print(std::ostream& os, const Tuple<Args...>& tuple)
{
    os << get<0>(tuple) << (tuple.size() == 1 ? "" : ", ");
    print(os, tuple.rest());
}

// Must appear after print(std::ostream, Tuple<Args...>)
template <>
void print(std::ostream& os, const Tuple<>& tuple)
{
}

} // namespace impl

/**
 * @brief Get the i-th element of the tuple.
 *
 * @tparam N the index
 * @param tuple the tuple
 * @return the i-th element
 */
template <int N, typename... _>
auto get(const Tuple<_...>& tuple)
{
    return ((typename impl::Get<N, Tuple<_...>>::class_type&)tuple).value_;
}

/**
 * @brief Output tuple data to the specified output stream.
 *
 * @param os an output stream
 * @param tuple the tuple to be printed to the output stream
 * @return self reference of the output stream
 */
template <typename... _>
std::ostream& operator<<(std::ostream& os, const Tuple<_...>& tuple)
{
    if (tuple.size() == 0)
    {
        return os << "()";
    }

    os << "(";
    impl::print(os, tuple);
    os << ")";

    return os;
}

} // namespace mdspp

#endif // TUPLE_HPP
