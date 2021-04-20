#include <iostream>

#include "PersistentTree.hpp"

int main() {
    Ng::PersistentTree<int, int> tree;

    tree.Push(1, 1);
    tree.Push(2, 1);
    tree.Push(3, 1);
    tree.Push(4, 1);

    tree.Print();

    return 0;
}
