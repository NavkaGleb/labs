#pragma once

#include "Matrix.hpp"

#include <iostream>

namespace Ng::Strassen {

    namespace _Internal {

        template <typename T>
        Matrix<T> SquareN2(const Matrix<T>& matrix) {
            std::size_t size = std::max(matrix.Rows(), matrix.Columns());
            std::size_t newSize = 1;

            while (newSize < size)
                newSize *= 2;

            Matrix<T> result(newSize, newSize);

            for (std::size_t i = 0; i < matrix.Rows(); ++i)
                for (std::size_t j = 0; j < matrix.Columns(); ++j)
                    result(i, j) = matrix.At(i, j);

            return result;
        }

        template <typename T>
        Matrix<T> Run(const Matrix<T>& left, const Matrix<T>& right) {

        }

    }

    template <typename T>
    Matrix<T> Run(const Matrix<T>& left, const Matrix<T>& right) {
        if (left.Columns() != right.Rows())
            throw std::invalid_argument("Strassen");

        std::cout << "rows = " << left.Rows() << ", columns = " << left.Columns() << std::endl;
        std::cout << left << std::endl;

        Matrix<T> _left = _Internal::SquareN2(left);
        Matrix<T> _right = _Internal::SquareN2(right);

        std::cout << "rows = " << _left.Rows() << ", columns = " << _left.Columns() << std::endl;
        std::cout << _left << std::endl;
    }

} // namespace Ng::Strassen