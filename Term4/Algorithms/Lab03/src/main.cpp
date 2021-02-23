#include <iostream>

#include <Random/Random.hpp>

#include "SplayTree.hpp"

int main() {
    Ng::SplayTree<int, int> tree;

    for (int i = 0; i < 10; ++i)
        tree.Push(Ng::Random::Get(i, 50), Ng::Random::Get(-5, 5));

    tree.Print();

    return 0;
}