//! @file pyincpp.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief Unified header file of PyInCpp.
//!
//! @copyright Copyright (C) 2023-present, Chen QingYu

#ifndef PYINCPP_HPP
#define PYINCPP_HPP

#if ((defined(_MSVC_LANG) && _MSVC_LANG > 201703L) || __cplusplus > 201703L)
#include "complex.hpp"
#include "deque.hpp"
#include "dict.hpp"
#include "fraction.hpp"
#include "int.hpp"
#include "list.hpp"
#include "set.hpp"
#include "str.hpp"
#include "tuple.hpp"

#else
#error "Require at least C++20."

#endif

#endif // PYINCPP_HPP
