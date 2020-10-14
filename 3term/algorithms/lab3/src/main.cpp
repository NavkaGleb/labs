#include <iostream>

#include "Heap.hpp"
#include "Random.hpp"

int main() {
    Ng::Heap<int, 4, std::less<>> heap;

    for (int i = 0; i < 10; ++i)
        heap.Push(Ng::Random::Irand(-10, 10));

    std::cout << "Peak = " << heap.Peak() << std::endl;
    std::cout << heap << std::endl;

    std::cout << "Pop peak" << std::endl;
    heap.PopPeak();
    std::cout << heap << std::endl;

    std::cout << "Increase 2 item" << std::endl;
    heap.Increase(2, 10);
    std::cout << heap << std::endl;

    std::cout << "Decrease 2 item" << std::endl;
    heap.Decrease(2, 10);
    std::cout << heap << std::endl;

    return 0;
}