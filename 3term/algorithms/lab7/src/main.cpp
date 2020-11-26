#include <iostream>

#include "RabinKarp/StringRabinKarp.hpp"
#include "RabinKarp/MatrixRabinKarp.hpp"

int main() {
    Ng::Matrix<char> pattern = {
        { 'c', 'a' },
        { 'a', 'b' }
    };

    Ng::Matrix<char> text = {
        { 'a', 'b', 'c' },
        { 'b', 'c', 'a' },
        { 'd', 'a', 'b' }
    };

    Ng::MatrixRabinKarp rabinKarp(pattern);

    std::pair<std::size_t, std::size_t> p = rabinKarp.Search(text);

    std::cout << "--i = " << p.first << ", j = " << p.second << std::endl;

    return 0;
}
