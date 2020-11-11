#include <iostream>

#include "Matrix.hpp"
#include "Random.hpp"
#include "Strassen.hpp"
#include "Benchmark.hpp"
#include "Console/Color.hpp"

#include <utility>
#include <ratio>

template <typename T>
void Rand(Ng::Matrix<T>& matrix, const T& left, const T& right) {
    for (std::size_t i = 0; i < matrix.Rows(); ++i)
        for (std::size_t j = 0; j < matrix.Columns(); ++j)
            matrix[i][j] = Ng::Random::Irand(left, right);
}

int main() {
    using Color = Ng::Console::Color;

    Ng::Matrix<int> matrix1(321, 658);
    Ng::Matrix<int> matrix2(658, 1023);

    Rand(matrix1, -10, 10);
    Rand(matrix2, -10, 10);

    std::cout << "a" << std::endl;

    if (matrix1 * matrix2 == Ng::Strassen::Run(matrix1, matrix2))
        std::cout << Color::Magenta << "Multiplication is OK" << Color::Reset << std::endl;

    std::cout << "b" << std::endl;

    for (int i = 2; i < 1024; i *= 2) {
        for (std::size_t limit = 1; limit <= i; limit *= 2) {
            Ng::Matrix<int> m1(i, i);
            Ng::Matrix<int> m2(i, i);

            Rand(m1, -10, 10);
            Rand(m2, -10, 10);

            std::cout << "limit = " << limit << std::endl;
            std::cout << "size = " << i << "x" << i << std::endl;
            std::cout << Color::Green << "Ordinary multiplication: "
                      << Color::Blue << Ng::Benchmark<std::micro>([=] { m1 * m2; }).count()
                      << " mc" << std::endl;

            Rand(m1, -10, 10);
            Rand(m2, -10, 10);

            std::cout << Color::Green << "Strassen algorithm: "
                      << Color::Blue << Ng::Benchmark<std::micro>(Ng::Strassen::Run<int>, m1, m2, limit).count()
                      << " mc" << std::endl;

            std::cout << Color::Reset << std::endl;
        }

        std::cout << "--------------" << std::endl;
    }

    return 0;
}
