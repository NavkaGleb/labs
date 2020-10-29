#include <iostream>

#include "Matrix.hpp"
#include "Random.hpp"
#include "Strassen.hpp"

int main() {

    Ng::Matrix<int> matrix1 = {
        { 2, 2, 1 },
        { 1, 2, 1 },
        { 3, 5, 7 }
    };

    Ng::Matrix<int> matrix2 = {
        { 1 },
        { 1 },
        { 1 }
    };

    std::cout << matrix1 * matrix2 << std::endl;


    return 0;
}
