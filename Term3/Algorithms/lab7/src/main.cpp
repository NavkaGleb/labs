#include <iostream>

#include <Random/Random.hpp>

#include "RabinKarp/MatrixRabinKarp.hpp"

int main() {
    std::size_t sizeText = 40;
    std::size_t sizePattern = 3;

    Ng::Matrix<char> text1(sizeText, sizeText);
    Ng::Matrix<char> pattern1(sizePattern, sizePattern);

    for (std::size_t i = 0; i < sizeText; ++i) {
        for (std::size_t j = 0; j < sizeText; ++j) {
            text1[i][j] = static_cast<char>(Ng::Random::Get(0, 1) + '0');
        }
    }

    for (std::size_t i = 0; i < sizePattern; ++i) {
        for (std::size_t j = 0; j < sizePattern; ++j) {
            pattern1[i][j] = static_cast<char>(Ng::Random::Get(0, 1) + '0');
        }
    }

    Ng::MatrixRabinKarp<char> rabinKarp(pattern1);

    auto positions = rabinKarp.Search(text1);

    if (positions.empty())
        std::cout << "Nothing found" << std::endl;

    for (const auto& position : positions)
        std::cout << "i = " << position.first << ", j = " << position.second << std::endl;

    return 0;
}
