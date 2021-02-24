#include <iostream>

#include <Random/Random.hpp>

#include "SplayTree.hpp"

int main() {
    Ng::SplayTree<int, int> tree;

    for (int i = 0; i < 10; ++i) {
        auto key   = Ng::Random::Get(i, 50);
        auto value = Ng::Random::Get(-5, 5);

        tree.Push(key, value);
    }

    std::cout << "Size: " << tree.GetSize() << std::endl;
    tree.Print();

    while (!tree.IsEmpty()) {
        tree.Pop(tree.GetRoot()->GetKey());
        std::cout << "Size after pop: " << tree.GetSize() << std::endl;
    }

    return 0;
}