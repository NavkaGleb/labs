#include <iostream>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

constexpr int COUNT = 100;

int main() {
//    Ng::BPlusTree<int, int, 3> tree;
//    std::set<int> set;
//
//    std::vector<int> data;
//
//    data.reserve(COUNT);
//
//    for (int i = 0; i < COUNT; ++i) {
//        auto key = Ng::Random::Get(0, 1000);
//
//        data.push_back(key);
//
//        set.insert(key);
//        tree.Push(key, 1);
////        tree.Print();
////        std::cout << " - -- - --------------------------------- -- - --  - -- " << std::endl;
////        std::cout << std::endl;
//    }
//
//    tree.Print();
//    std::cout << " - -- - - -- - --  - -- " << std::endl;
//
//    while (!data.empty()) {
//        auto index = Ng::Random::Get<decltype(data)::difference_type>(0, data.size() - 1);
//        auto key   = data[index];
//
//        data.erase(data.begin() + index);
//
//        std::cout << "-- pop = " << key << std::endl;
//        tree.Pop(key);
//        std::cout << "---------------------" << std::endl;
//        tree.Print();
//        std::cout << "--------------------- end" << std::endl;
//        set.erase(key);
//        std::cout << std::endl;
//    }
//
//    tree.Print();
//
//    std::cout << "Keys: ";
//    for (const auto& key : tree) {
//        std::cout << key << " ";
//    }
//    std::cout << std::endl;
//
//    std::cout << "Set:  ";
//    for (const auto& key : set) {
//        std::cout << key << " ";
//    }
//    std::cout << std::endl;

    Ng::BPlusTree<int, int, 3> tree;

    for (int i = 0; i < 40; ++i) {
        auto key = Ng::Random::Get(0, 100);

        tree.Push(key, 1);

    }

    for (auto&& [key, value] : tree) {
        std::cout << "Key: " << key << ", Value: " << (value = 123) << std::endl;
    }

    return 0;
}
