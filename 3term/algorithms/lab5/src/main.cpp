#include <iostream>

#include "Matrix.hpp"
#include "Random.hpp"
#include "Strassen.hpp"

#include <functional>
#include <memory>
#include <algorithm>
#include <utility>

template <typename T = int>
void Rand(Ng::Matrix<T>& matrix, const T& left, const T& right) {
    for (std::size_t i = 0; i < matrix.Rows(); ++i)
        for (std::size_t j = 0; j < matrix.Columns(); ++j)
            matrix[i][j] = Ng::Random::Irand(left, right);
}

int main() {
    Ng::Matrix<int> matrix1(321, 658);
    Ng::Matrix<int> matrix2(658, 1023);

    std::vector<Ng::Matrix<int>> m;

    std::sort(m.begin(), m.end(), std::bind(&Ng::Matrix<int>::Rows, _1) < std::bind(&Ng::Matrix<int>::Rows, _2));

    Rand(matrix1, -10, 10);
    Rand(matrix2, -10, 10);

//    matrix1 * matrix2;

    if (matrix1 * matrix2 == Ng::Strassen::Run(matrix1, matrix2))
        std::cout << "fuck this shit" << std::endl;

    return 0;
}
