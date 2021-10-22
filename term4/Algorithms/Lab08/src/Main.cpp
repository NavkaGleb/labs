#include <iostream>

#include <Random/Random.hpp>

#include "FibonacciHeap.hpp"

int main() {
    Ng::FibonacciHeap<int, std::greater<>> heap;

    for (int i = 0; i < 18; ++i)
        heap.Push(i);

    std::cout << "IsEmpty: " << heap.IsEmpty() << std::endl;
    std::cout << "Count: " << heap.GetCount() << std::endl;

    heap.Print();

    for (int i = 0; i < 4; ++i)
        heap.Exchange(i, 228);

    std::cout << "After exchange!" << std::endl;
    heap.Print();

    heap.PopPeak();

    std::cout << "After PopPeak!" << std::endl;
    heap.Print();

    heap.Pop(17);

    std::cout << "After Pop!" << std::endl;
    heap.Print();

    while (!heap.IsEmpty()) {
        std::cout << "Peak: " << heap.GetPeak() << std::endl;
        heap.PopPeak();
    }

    std::cout << "After clearing!" << std::endl;
    heap.Print();

    return 0;
}
