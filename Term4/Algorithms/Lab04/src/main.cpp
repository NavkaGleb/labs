#include <iostream>

#include <OptimalBinarySearchTree.hpp>

int main() {
    std::vector keys = { 1,    2,    3,     4,   };
    std::vector values = { 1, 2, 3, 4 };
    std::vector prob = { 4.0f, 2.0f, 6.0f, 3.0f };

    Ng::OptimalBinarySearchTree<int, int> tree(prob, keys, values);

    tree.PrintTable();

    for (const auto& [key, value] : tree)
        std::cout << key << " -> " << value << std::endl;

    std::cout << tree << std::endl;

    return 0;
}
