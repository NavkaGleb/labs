#include <iostream>

#include "OrderStatisticsTree.hpp"

int main() {
    Ng::OrderStatisticsTree<int> tree;

    std::cout << tree.GetNodes() << std::endl;
    std::cout << tree.GetMin().value_or(221) << std::endl;

    for (int i = 0; i < 20; ++i)
        tree.Push((i * 3 + 321) % 31);

    tree.Print();

    std::cout << "--------------------------------------------------" << std::endl;

    tree.Pop(20);
    tree.Print();

    std::cout << "Height = " << tree.GetHeight() << std::endl;
}
