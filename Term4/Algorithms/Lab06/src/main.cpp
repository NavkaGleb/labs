#include <iostream>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

int main() {
    Ng::BPlusTree<int, int, 2> tree;

    for (int i = 0; i < 14; ++i)
        tree.Push(Ng::Random::Get(0, 40), 1);

    tree.Print();
    std::cout << " - -- - - -- - --  - -- " << std::endl;

    for (int i = 0; i < 30; ++i) {
//        std::cout << "-- pop " << i << std::endl;
        tree.Pop(i);
//        tree.Print();
//        std::cout << "---------------------" << std::endl;
    }

    tree.Print();

    return 0;
}
