#ifndef CHECK_BOUNDS_HPP
#define CHECK_BOUNDS_HPP

#include <stdexcept>

namespace mdspp
{

namespace common
{

// Check whether the index is valid (begin <= pos < end).
static inline void check_bounds(int pos, int begin, int end)
{
    if (pos < begin || pos >= end)
    {
        throw std::runtime_error("ERROR: Out of range.");
    }
}

} // namespace common

} // namespace mdspp

#endif // CHECK_BOUNDS_HPP
