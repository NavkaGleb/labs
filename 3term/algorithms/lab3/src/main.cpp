#include <iostream>
#include "Heap.hpp"
#include "Random.hpp"

int main() {
    ng::Heap<int, 4> heap;

    heap.Push(4);
    heap.Push(32);
    heap.Push(-7);
    heap.Push(0);
    heap.Push(5);

    std::cout << "Peak = " << heap.Peak() << std::endl;
    std::cout << heap << std::endl;

    heap.PopPeak();
    std::cout << heap << std::endl;

    heap.Increase(2, 10);
    std::cout << heap << std::endl;

    return 0;
}
