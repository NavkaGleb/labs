#include <iostream>

#include "RabinKarp/MatrixRabinKarp.hpp"

int main() {
    Ng::Matrix<char> pattern = {
        { '3', 'd' },
        { 'g', 'k' }
    };

    Ng::Matrix<char> text = {
        { 'a', 'b', 'c', 't' },
        { '1', '2', '3', 'd' },
        { 'd', 'f', 'g', 'k' },
        { 'a', 'b', 'c', 'i' },
    };

    Ng::MatrixRabinKarp rabinKarp(pattern);

    auto a = rabinKarp.Search(text);

    for (const auto& p : a)
        std::cout << "--i = " << p.first << ", j = " << p.second << std::endl;

    return 0;
}
