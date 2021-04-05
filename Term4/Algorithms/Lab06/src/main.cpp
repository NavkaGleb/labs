#include <iostream>

#include "BPlusTree.hpp"

int main() {
    Ng::BPlusTree<int, int, 2> tree;

    for (int i = 0; i < 10; ++i) {
        tree.Push(9 - i, 1);
    }

    tree.Print();

    return 0;
}
