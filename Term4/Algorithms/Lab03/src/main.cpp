#include <iostream>

#include <Random/Random.hpp>

#include "SplayTree.hpp"

#include <map>

int main() {
    Ng::SplayTree<int, int> tree;

    for (int i = 0; i < 10; ++i) {
        auto key   = Ng::Random::Get(i, 50);
        auto value = Ng::Random::Get(-5, 5);

        tree.Push(key, value);
    }

    std::cout << "Size: " << tree.GetSize() << std::endl;
    std::cout << "Height: " << tree.GetHeight() << std::endl;
    std::cout << tree << std::endl;

    for (const auto& [key, value] : tree)
        std::cout << "{ " << key << ", " << value << " } ";
    std::cout << std::endl;

    while (!tree.IsEmpty())
        tree.Pop(tree.GetRoot()->GetKey());

    std::cout << tree << std::endl;

    return 0;
}