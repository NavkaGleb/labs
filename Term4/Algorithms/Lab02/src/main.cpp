#include <iostream>

#include <Random/Random.hpp>

#include "OrderStatisticsTree.hpp"

int main() {
    Ng::OrderStatisticsTree<int> tree;

    std::cout << tree.GetSize() << std::endl;

    for (int i = 0; i < 10; ++i)
        tree.Push(Ng::Random::Get(-10, 10));

    tree.Print();

    for (int i = 0; i < tree.GetSize(); ++i)
        std::cout << *tree.Get(i) << " ";
    std::cout << std::endl;

    for (int i = 0; i < 10; ++i)
        tree.Pop(*tree.Get(0));

    tree.Print();
}
