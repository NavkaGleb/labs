#include <iostream>

#include "RabinKarp/MatrixRabinKarp.hpp"

int main() {
    Ng::Matrix<char> pattern = {
        { '3', 'd' },
    };

    Ng::Matrix<char> text = {
        { 'a', 'b', 'c', 't' },
        { '1', '2', '3', 'd' },
        { 'd', 'f', 'g', 'k' },
        { 'a', 'b', 'c', 'i' },
    };

    Ng::MatrixRabinKarp rabinKarp(pattern);

    auto positions = rabinKarp.Search(text);

    if (positions.empty())
        std::cout << "Nothing found" << std::endl;

    for (const auto& position : positions)
        std::cout << "i = " << position.first << ", j = " << position.second << std::endl;

    return 0;
}
