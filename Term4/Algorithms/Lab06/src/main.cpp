#include <iostream>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

int main() {
    Ng::BPlusTree<int, int, 2> tree;
    std::set<int> set;

    for (int i = 0; i < 20; ++i) {
        auto key = Ng::Random::Get(0, 100);
        set.insert(key);
        tree.Push(key, 1);
    }

    tree.Print();
    std::cout << " - -- - - -- - --  - -- " << std::endl;

    for (int i = 0; i < 100; ++i) {
        std::cout << "-- pop " << i << std::endl;
        tree.Pop(i);
        tree.Print();
        std::cout << "---------------------" << std::endl;
        set.erase(i);
    }

    tree.Print();

    auto currentKey = set.begin();

    std::cout << "Keys: ";
    for (const auto& key : tree) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "Set:  ";
    for (const auto& key : set) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    return 0;
}
