#ifndef CHECK_FULL_HPP
#define CHECK_FULL_HPP

#include <stdexcept>

namespace mdspp
{

namespace common
{

// Check whether there is any remaining capacity.
static inline void check_full(int size, int capacity)
{
    if (size >= capacity)
    {
        throw std::runtime_error("ERROR: The container has reached the maximum size.");
    }
}

} // namespace common

} // namespace mdspp

#endif // CHECK_FULL_HPP
