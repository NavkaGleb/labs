#include <iostream>

#include <Random/Random.hpp>

#include "OrderStatisticsTree.hpp"

int main() {
    Ng::OrderStatisticsTree<int> tree;

    std::cout << tree.GetSize() << std::endl;

    for (int i = 0; i < 10; ++i) {
        int r = Ng::Random::Get(-10, 10);
        std::cout << "---------------------------- " << r << std::endl;
        tree.Push(r);
        tree.Print();
    }

    tree.Print();

    std::cout << tree.GetSize() << ": ";
    for (int i = 0; i < tree.GetSize(); ++i)
        std::cout << *tree.Get(i) << " ";
    std::cout << std::endl;

    while (!tree.IsEmpty())
        tree.Pop(*tree.Get(0));

    tree.Print();
}
