#include <iostream>

#include <Random/Random.hpp>

#include "OptimalBinarySearchTree.hpp"

int main() {
    std::vector keys          = { 1,    2,    3,    4,   };
    std::vector values        = { 1,    2,    3,    4    };
    std::vector probabilities = { 0.3f, 0.2f, 0.1f, 0.5f };

    Ng::OptimalBinarySearchTree<int, int> tree(keys, values, probabilities);

    tree.PrintTable();

    for (const auto& [key, value] : tree)
        std::cout << key << " -> " << value << std::endl;

    std::cout << tree << std::endl;

    return 0;
}
