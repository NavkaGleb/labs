#include <iostream>

#include <Random/Random.hpp>

#include "SplayTree.hpp"

int main() {
    Ng::SplayTree<int, int> tree;

    int count = 0;
    int exists = 0;

    for (int i = 0; i < 20; ++i) {
        auto key   = Ng::Random::Get(i, 50);
        auto value = Ng::Random::Get(-5, 5);

        if (tree.IsExists(key)) {
            std::cout << "exists!" << std::endl;
            ++exists;
        }

        std::cout << "----------- Push { " << key << ", " << value << " }" << std::endl;
        tree.Push(key, value);
        tree.Print();
        std::cout << "------------------------------- " << tree.GetSize() << std::endl;

        if (tree.GetRoot().GetKey() == key)
            ++count;
        else
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Exists: " << exists << std::endl;
    std::cout << "Count: " << count << std::endl;
    std::cout << "Size: " << tree.GetSize() << std::endl;

    return 0;
}