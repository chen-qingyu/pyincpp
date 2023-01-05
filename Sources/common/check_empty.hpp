#ifndef CHECK_EMPTY_HPP
#define CHECK_EMPTY_HPP

#include <stdexcept>

namespace mdspp
{

namespace common
{

// Check whether is not empty.
static inline void check_empty(int size)
{
    if (size == 0)
    {
        throw std::runtime_error("ERROR: The container is empty.");
    }
}

} // namespace common

} // namespace mdspp

#endif // CHECK_EMPTY_HPP
