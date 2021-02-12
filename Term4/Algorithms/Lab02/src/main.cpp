#include <iostream>

#include <Random/Random.hpp>

#include "OrderStatisticsTree.hpp"

int main() {
    Ng::OrderStatisticsTree<int> tree;

    std::cout << tree.GetSize() << std::endl;

    for (int i = 0; i < 30; ++i)
        tree.Push(Ng::Random::Get(-20, 20));

    tree.Print();

    std::cout << tree.GetSize() << ": ";
    for (int i = 0; i < tree.GetSize(); ++i)
        std::cout << *tree.Get(i) << " ";
    std::cout << std::endl;

    int size = tree.GetSize();

    for (int i = 0; i < size - 1; ++i) {
        tree.Pop(*tree.Get(0));
    }

    std::cout << "######################################################" << std::endl;
    tree.Print();
}
