#include <iostream>
#include "Heap.hpp"
#include "Random.hpp"

// 2i + 1, 2i + 2

int main() {
    ng::Heap heap(2);

    for (int i = 0; i < 4; ++i)
        heap.Push(ng::random::irand(-10, 10));

    std::cout << heap << std::endl;
    std::cout << heap.Top()

    return 0;
}
