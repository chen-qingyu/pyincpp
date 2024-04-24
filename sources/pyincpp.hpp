/**
 * @file pyincpp.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Unified header file.
 * @date 2023.11.01
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

#ifndef PYINCPP_HPP
#define PYINCPP_HPP

#if ((defined(_MSVC_LANG) && _MSVC_LANG > 201703L) || __cplusplus > 201703L)

#include "Deque.hpp"
#include "Fraction.hpp"
#include "Integer.hpp"
#include "List.hpp"
#include "Map.hpp"
#include "Set.hpp"
#include "String.hpp"
#include "Tuple.hpp"

#else
#error "Require at least C++20."

#endif // C++17

#endif // PYINCPP_HPP
