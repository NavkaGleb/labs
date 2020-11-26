#include <iostream>

#include "Algorithms/RabinKarp.hpp"

int main() {
    Ng::RabinKarp rabinKarp("Hello");

    std::cout << rabinKarp.Search(" Hello, world!Hello") << std::endl;
    return 0;
}
