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

    std::cout << "iterators: " << std::endl;
    for (auto it = tree.Begin(); it != tree.End(); ++it)
        std::cout << (*it).first << " ";
    std::cout << std::endl;

    std::cout << "At1: " << *tree.At(32) << std::endl;
    std::cout << "At2: " << tree.At(10001).has_value() << std::endl;

//    for (int i = 0; i < count - 1; ++i) {
//        auto index = Ng::Random::Get<std::size_t>(0, keys.size() - 1);
//
//        tree = tree.Pop(keys[index]);
//        keys.erase(keys.begin() + index);
//    }
//
//    tree.Print();

    return 0;
}
