#include <iostream>

#include "RabinKarp/StringRabinKarp.hpp"
#include "RabinKarp/MatrixRabinKarp.hpp"

int main() {
    Ng::Matrix<char> pattern = {
        { 'b', 'c' }
    };

    Ng::Matrix<char> text = {
        { 'a', 'b', 'c' },
        { 'a', 'b', 'c' },
        { 'a', 'b', 'c' }
    };

    Ng::MatrixRabinKarp rabinKarp(pattern);

    auto a = rabinKarp.Search(text);

    for (const auto& p : a)
        std::cout << "--i = " << p.first << ", j = " << p.second << std::endl;

    return 0;
}
