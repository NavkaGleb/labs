#include <iostream>
#include <numeric>

#include <Random/Random.hpp>

#include "PersistentTree.hpp"

int main() {
    constexpr int count = 10;

    Ng::PersistentTree<int, int>              tree;
    std::vector<int>                          keys(count);
    std::vector<Ng::PersistentTree<int, int>> trees;

    std::iota(keys.begin(), keys.end(), 0);

    trees.resize(count + 1);

    for (int i = 0; i < count; ++i) {
        std::cout << "------------------------------ " << i + 1 << std::endl;
        trees[i + 1] = trees[i].Push(i, i);
        trees[i + 1].Print();
    }

    trees[5] = trees[5].Pop(3);

    std::cout << "########################################################################" << std::endl;
    for (int i = 4; i < 7; ++i) {
        std::cout << "------------------------------ " << i << std::endl;
        trees[i].Print();
    }

//    for (auto& [key, value] : tree)
//        std::cout << key << " -> " << value << std::endl;
//
//    std::cout << "At 32: " << *tree.At(32) << std::endl;
//    std::cout << "At 10001: " << tree.At(10001).has_value() << std::endl;
//
//    std::cout << "MinKey: " << *tree.GetMinKey() << std::endl;
//    std::cout << "MaxKey: " << *tree.GetMaxKey() << std::endl;
//
//    for (int i = 0; i < count - 1; ++i) {
//        auto index = Ng::Random::Get<std::size_t>(0, keys.size() - 1);
//
//        tree = tree.Pop(keys[index]);
//        keys.erase(keys.begin() + static_cast<decltype(keys)::difference_type>(index));
//    }
//
//    tree.Print();

    return 0;
}
