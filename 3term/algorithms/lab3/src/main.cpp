#include <iostream>
#include "Heap.hpp"

// 2i + 1, 2i + 2

int main() {
    ng::Heap heap(2);

    for (int i = 0; i < 4; ++i)
        heap.Push(i);

    std::cout << heap << std::endl;

    return 0;
}
