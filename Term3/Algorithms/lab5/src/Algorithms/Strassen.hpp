#pragma once

// std lib
#include <iostream>
#include <functional>
#include <cmath>

// own files
#include "Matrix/Matrix.hpp"

namespace Ng::Strassen {

    namespace _Internal {

        using std::size_t;

        template <typename T>
        bool CheckSize(const Matrix<T>& left, const Matrix<T>& right) {
            return left.Rows() == right.Rows() && left.Columns() == right.Columns();
        }

        template <typename T>
        bool CheckSquareN2(const Matrix<T>& matrix) {
            return matrix.Rows() == matrix.Columns() &&
                   matrix.Rows() != 0 &&
                   (matrix.Rows() & (matrix.Rows() - 1)) == 0;
        }

        template <typename T>
        Matrix<T> SquareN2(const Matrix<T>& matrix, size_t size) {
            auto newSize = static_cast<size_t>(pow(2, std::ceil(std::log2(size))));
            Matrix<T> result(newSize, newSize);

            for (size_t i = 0; i < matrix.Rows(); ++i)
                for (size_t j = 0; j < matrix.Columns(); ++j)
                    result[i][j] = matrix.At(i, j);

            return result;
        }

        template <typename T>
        std::vector<Matrix<T>> Split(const Matrix<T>& matrix) {
            size_t size = matrix.Rows() / 2;
            std::vector<Matrix<T>> result(4, Matrix<T>(size, size));

            for (size_t i = 0; i < size; ++i) {
                for (size_t j = 0; j < size; ++j) {
                    result[0][i][j] = matrix.At(i,        j       );
                    result[1][i][j] = matrix.At(i,        j + size);
                    result[2][i][j] = matrix.At(i + size, j       );
                    result[3][i][j] = matrix.At(i + size, j + size);
                }
            }

            return result;
        }

        template <typename T>
        Matrix<T> Collect(const std::vector<Matrix<T>>& splits) {
            size_t size = splits.front().Rows();
            Matrix<T> result(size * 2, size * 2);

            for (size_t i = 0; i < size; ++i) {
                for (size_t j = 0; j < size; ++j) {
                    result[i][j]               = splits[0].At(i, j);
                    result[i][j + size]        = splits[1].At(i, j);
                    result[i + size][j]        = splits[2].At(i, j);
                    result[i + size][j + size] = splits[3].At(i, j);
                }
            }

            return result;
        }

        template <typename T>
        Matrix<T> Run(const Matrix<T>& left, const Matrix<T>& right, std::size_t limit) {
           if (left.Rows() <= limit)
                return left * right;

            auto leftSplit = Split(left);
            auto rightSplit = Split(right);

            Matrix<T> a = Run(leftSplit[0] + leftSplit[3], rightSplit[0] + rightSplit[3], limit);
            Matrix<T> b = Run(leftSplit[2] + leftSplit[3], rightSplit[0]                , limit);
            Matrix<T> c = Run(leftSplit[0]               , rightSplit[1] - rightSplit[3], limit);
            Matrix<T> d = Run(leftSplit[3]               , rightSplit[2] - rightSplit[0], limit);
            Matrix<T> e = Run(leftSplit[0] + leftSplit[1], rightSplit[3]                , limit);
            Matrix<T> f = Run(leftSplit[2] - leftSplit[0], rightSplit[0] + rightSplit[1], limit);
            Matrix<T> g = Run(leftSplit[1] - leftSplit[3], rightSplit[2] + rightSplit[3], limit);

            leftSplit[0] = a + d - e + g;
            leftSplit[1] = c + e;
            leftSplit[2] = b + d;
            leftSplit[3] = a + c - b + f;

            return Collect(leftSplit);
        }

    } // namespace _Internal

    template <typename T>
    Matrix<T> Run(const Matrix<T>& left, const Matrix<T>& right, std::size_t limit = 64) {
        if (left.Columns() != right.Rows())
            throw std::invalid_argument("Ng::Strassen: Matrices have different size");

        if (_Internal::CheckSize(left, right) && _Internal::CheckSquareN2(left) && _Internal::CheckSquareN2(right))
            return _Internal::Run(left, right, limit);

        size_t size = std::max({ left.Rows(), left.Columns(), right.Rows(), right.Columns() });
        Matrix<T> result;

        if (!_Internal::CheckSquareN2(left) && !_Internal::CheckSquareN2(right))
            result = _Internal::Run(_Internal::SquareN2(left, size), _Internal::SquareN2(right, size), limit);
        else if (!_Internal::CheckSquareN2(left))
            result = _Internal::Run(_Internal::SquareN2(left, size), right, limit);
        else
            result = _Internal::Run(left, _Internal::SquareN2(right, size), limit);

        result.Clean();
        return result;
    }

} // namespace Ng::Strassen