#include <iostream>

#include <Random/Random.hpp>

#include "FibonacciHeap.hpp"

int main() {
    Ng::FibonacciHeap<int, std::greater<>> heap;

    for (int i = 0; i < 11; ++i)
        heap.Push(i);

    std::cout << "IsEmpty: " << heap.IsEmpty() << std::endl;
    std::cout << "Count: " << heap.GetCount() << std::endl;

    heap.Print();

    heap.Exchange(5, 228);

    std::cout << "After exchange!" << std::endl;
    heap.Print();

    heap.PopPeak();

    std::cout << "After PopPeak!" << std::endl;
    heap.Print();

    while (!heap.IsEmpty())
        heap.PopPeak();

    std::cout << "After clearing!" << std::endl;
    heap.Print();

    return 0;
}
