#include <iostream>

#include "Matrix.hpp"
#include "Random.hpp"

int main() {

    Ng::Matrix<int> matrix(4, 4);

    std::cout << matrix.Rows() << " " << matrix.Columns() << std::endl;

    for (std::size_t i = 0; i < matrix.Rows(); ++i)
        for (std::size_t j = 0; j < matrix.Columns(); ++j)
            matrix[i][j] = Ng::Random::Irand(-10, 10);

    std::cout << matrix << std::endl;

    return 0;
}
