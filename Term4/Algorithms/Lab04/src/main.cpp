#include <iostream>
#include <vector>
#include <string>

#include <OptimalBinarySearchTree.hpp>

int main() {
    std::vector keys = { 1,    2,    3,     4,   };
    std::vector prob = { 4.0f, 2.0f, 6.0f, 3.0f };

    Ng::OptimalBinarySearchTree<int, int> tree(prob);

    tree.Print();

    return 0;
}
