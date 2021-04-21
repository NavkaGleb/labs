#include <iostream>
#include <numeric>

#include <Random/Random.hpp>

#include "PersistentTree.hpp"

int main() {
    constexpr int count = 100;

    Ng::PersistentTree<int, int> tree;
    std::vector<int>             keys(count);

    std::iota(keys.begin(), keys.end(), 0);

    for (int i = 0; i < count; ++i) {
        tree = tree.Push(i, i);
        std::cout << "-------" << std::endl;
    }

    tree.Print();

    for (auto& [key, value] : tree)
        std::cout << key << " ";
    std::cout << std::endl;

    std::cout << "At 32: " << *tree.At(32) << std::endl;
    std::cout << "At 10001: " << tree.At(10001).has_value() << std::endl;

    for (int i = 0; i < count - 1; ++i) {
        auto index = Ng::Random::Get<std::size_t>(0, keys.size() - 1);

        tree = tree.Pop(keys[index]);
        keys.erase(keys.begin() + index);
    }

    tree.Print();

    return 0;
}
