#include <iostream>

#include "PersistentTree.hpp"

int main() {
    Ng::PersistentTree<int, int> tree;

    for (int i = 0; i < 1000; ++i) {
        tree = tree.Push(i, 0);
//        tree.Print();
        std::cout << "-------" << std::endl;
    }

    tree.Print();

    return 0;
}
