#include <iostream>

#include <Random/Random.hpp>

#include "FibonacciHeap.hpp"

int main() {
    Ng::FibonacciHeap<int> heap;

    for (int i = 0; i < 11; ++i)
        heap.Push(i);

    heap.Print();

    std::cout << "IsEmpty: " << heap.IsEmpty() << std::endl;
    std::cout << "Count: " << heap.GetCount() << std::endl;

//    heap.PopPeak();
    heap.Print();

    while (!heap.IsEmpty()) {
        std::cout << "-- Pop = " << heap.GetPeak() << std::endl;
        heap.PopPeak();
        heap.Print();

        std::cout << "--------------------------------------------------------- " << heap.GetCount() << std::endl;
    }

    return 0;
}
