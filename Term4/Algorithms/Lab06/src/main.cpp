#include <iostream>

#include "BPlusTree.hpp"

#include <Random/Random.hpp>

int main() {
    Ng::BPlusTree<int, int, 2> tree;
    std::set<int> set;

    for (int i = 0; i < 20; ++i) {
        auto key = Ng::Random::Get(0, 20);
//        auto key = 9 - i;
        set.insert(key);
        tree.Push(key, 1);
    }

    std::cout << "keys: ";
    for (const auto& key : set)
        std::cout << key << " ";
    std::cout << std::endl;

    tree.Print();

    std::cout << "IsExists" << std::endl;
    for (int i = 0; i < 20; ++i)
        std::cout << i << " -> " << tree.IsExists(i) << std::endl;

//    //                    0, 1, 2, 3, 4
//    std::set<int> set = { 1, 2 };
//    //                   0  1  2  3  4  5
//
//    std::cout << (std::prev(set.lower_bound(-1)) == set.end()) << std::endl;
//    std::cout << *set.lower_bound(0) << std::endl;
//
//    std::cout << "---" << std::endl;
//
//    for (int i = 0; i < 4; ++i) {
//        std::cout << i << " -> " << *--set.upper_bound(i) << std::endl;
//
//    }

    return 0;
}
