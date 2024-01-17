/**
 * @file fraction_in_eigen.cpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Use Fraction in Eigen to show exact representation.
 * @date 2024.01.17
 *
 * @copyright Copyright (C) 2024
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

#include <iostream>

#include <Eigen/Dense>
#include <mds/Fraction.hpp>

void fraction_in_eigen()
{
    using Matrix = Eigen::Matrix<mds::Fraction, 2, 2>;

    Matrix A;
    A << 1, 2, 3, 4;
    Matrix B = Matrix::Zero();
    Matrix C = Matrix::Ones();
    Matrix D = Matrix::Identity();

    std::cout << (((A + B) * (C + D)).inverse()) << std::endl;
    /*
    -11/6     5/6
      5/3    -2/3
    */
}
