#pragma once

#include "Matrix.hpp"

#include <iostream>
#include <functional>

namespace Ng::Strassen {

    namespace _Internal {

        using std::size_t;

        template <typename T>
        Matrix<T> SquareN2(const Matrix<T>& matrix) {
            size_t size = std::max(matrix.Rows(), matrix.Columns());
            size_t newSize = 1;

            while (newSize < size)
                newSize *= 2;

            Matrix<T> result(newSize, newSize);

            for (size_t i = 0; i < matrix.Rows(); ++i)
                for (size_t j = 0; j < matrix.Columns(); ++j)
                    result(i, j) = matrix.At(i, j);

            return result;
        }

        template <typename T>
        std::vector<Matrix<T>> Split(const Matrix<T>& matrix) {
            size_t size = matrix.Rows() / 2;
            std::vector<Matrix<T>> result(4, Matrix<T>(size, size));

            std::function<void(Matrix<T>&, size_t, size_t, size_t, size_t)> Fill =
                [&matrix](Matrix<T>& to, size_t rowBegin, size_t rowEnd, size_t columnBegin, size_t columnEnd) {

                for (size_t i = 0; i < rowEnd - rowBegin; ++i)
                    for (size_t j = 0; j < columnEnd - columnBegin; ++j)
                        to(i, j) = matrix.At(i + rowBegin, j + columnBegin);
            };

            Fill(result[0], 0, size, 0, size);
            Fill(result[1], 0, size, size, size * 2);
            Fill(result[2], size, size * 2, 0, size);
            Fill(result[3], size, size * 2, size, size * 2);

            return result;
        }

        template <typename T>
        Matrix<T> Collect(const std::vector<Matrix<T>>& splits) {
            Matrix<T> result(splits.front().Rows() * 2);

            
        }

        template <typename T>
        Matrix<T> Run(const Matrix<T>& left, const Matrix<T>& right) {
            if (left.Rows() <= 64)
                return left * right;

            auto leftSplit = Split(left);
            auto rightSplit = Split(right);

            Matrix<T> a = Run(leftSplit[0] + leftSplit[3], rightSplit[0] + rightSplit[3]);
            Matrix<T> b = Run(leftSplit[2] + leftSplit[3], rightSplit[0]                );
            Matrix<T> c = Run(leftSplit[0]               , rightSplit[1] - rightSplit[3]);
            Matrix<T> d = Run(leftSplit[3]               , rightSplit[2] - rightSplit[0]);
            Matrix<T> e = Run(leftSplit[0] + leftSplit[1], rightSplit[3]                );
            Matrix<T> f = Run(leftSplit[2] - leftSplit[0], rightSplit[0] + rightSplit[1]);
            Matrix<T> g = Run(leftSplit[1] - leftSplit[3], rightSplit[2] + rightSplit[3]);

            leftSplit[0] = a + d - e + g;
            leftSplit[1] = c + e;
            leftSplit[2] = b + d;
            leftSplit[3] = a + c - b + f;

            return Collect(leftSplit);
        }

    } // namespace _Internal

    template <typename T>
    Matrix<T> Run(const Matrix<T>& left, const Matrix<T>& right) {
        if (left.Columns() != right.Rows())
            throw std::invalid_argument("Strassen");

        Matrix<T> _left = _Internal::SquareN2(left);
        Matrix<T> _right = _Internal::SquareN2(right);
        Matrix<T> result = _Internal::Run(_left, _right);

        return result;
    }

} // namespace Ng::Strassen