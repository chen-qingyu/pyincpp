#ifndef SWAP_HPP
#define SWAP_HPP

#include <utility> // std::move

namespace mdspp
{

namespace common
{

// Swap the content of the two elements.
template <typename T>
static inline void swap(T& e1, T& e2)
{
    auto tmp = std::move(e1);
    e1 = std::move(e2);
    e2 = std::move(tmp);
}

} // namespace common

} // namespace mdspp

#endif // SWAP_HPP
