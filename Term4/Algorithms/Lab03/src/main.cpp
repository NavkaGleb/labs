#include <iostream>

#include <Random/Random.hpp>

#include "SplayTree.hpp"

enum class Category : int {
    None = 0,
    Sport,
    Politics,
    Science,
    Economics
};

std::ostream& operator <<(std::ostream& ostream, const Category& category) {
    switch (category) {
        case Category::None:      ostream << "None";      break;
        case Category::Sport:     ostream << "Sport";     break;
        case Category::Politics:  ostream << "Politics";  break;
        case Category::Science:   ostream << "Science";   break;
        case Category::Economics: ostream << "Economics"; break;
    }

    return ostream;
}

int main() {
    Ng::SplayTree<int, Category> tree;

    // Push Key / Value
    for (int i = 0; i < 10; ++i) {
        auto key   = Ng::Random::Get(0, 50);
        auto value = static_cast<Category>(Ng::Random::Get(1, 4));

        tree.Push(key, value);
    }

    // Operator []
    tree[60];
    tree[-1] = Category::Science;

    std::cout << "Size: " << tree.GetSize() << std::endl;
    std::cout << "Height: " << tree.GetHeight() << std::endl;
    std::cout << tree << std::endl;

    // Range based for
    for (auto& [key, value] : tree)
        std::cout << "{ " << key << ", " << value << " }" << std::endl;

    // Clear tree
    while (!tree.IsEmpty())
        tree.Pop(tree.GetRoot()->GetKey());

    std::cout << std::endl;
    std::cout << "Size: " << tree.GetSize() << std::endl;
    std::cout << "Height: " << tree.GetHeight() << std::endl;
    std::cout << tree << std::endl;

    return 0;
}