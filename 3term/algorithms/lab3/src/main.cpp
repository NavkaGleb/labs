#include <iostream>
#include "Heap.hpp"
#include "Random.hpp"

int main() {
    ng::Heap<int, 3> heap;
    heap.Increase(-1, 32);

    for (int i = 0; i < 6; ++i)
        heap.Push(ng::random::irand(-10, 10));

    std::cout << heap << std::endl;

    heap.Increase(3, 10);

    std::cout << heap << std::endl;

    return 0;
}
