#include <iostream>

#include "RabinKarp/MatrixRabinKarp.hpp"
#include "Random/Random.hpp"

int main() {
//    Ng::Matrix<char> pattern = {
//        { '3', 'd' },
//    };
//
//    Ng::Matrix<char> text = {
//        { 'a', 'b', 'c', 't' },
//        { '1', '2', '3', 'd' },
//        { 'd', 'f', 'g', 'k' },
//        { 'a', 'b', 'c', 'i' },
//    };
//
//    Ng::MatrixRabinKarp rabinKarp(pattern);
//
//    auto positions = rabinKarp.Search(text);
//
//    if (positions.empty())
//        std::cout << "Nothing found" << std::endl;
//
//    for (const auto& position : positions)
//        std::cout << "i = " << position.first << ", j = " << position.second << std::endl;

    std::size_t sizeText = 40;
    std::size_t sizePattern = 3;

    Ng::Matrix<char> text1(sizeText, sizeText);
    Ng::Matrix<char> pattern1(sizePattern, sizePattern);

    for (std::size_t i = 0; i < sizeText; ++i) {
        for (std::size_t j = 0; j < sizeText; ++j) {
            text1[i][j] = static_cast<char>(Ng::Random::Irand(0, 1) + '0');
        }
    }

    for (std::size_t i = 0; i < sizePattern; ++i) {
        for (std::size_t j = 0; j < sizePattern; ++j) {
            pattern1[i][j] = static_cast<char>(Ng::Random::Irand(0, 1) + '0');
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
