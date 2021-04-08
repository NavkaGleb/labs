#include <iostream>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

int main() {
    Ng::BPlusTree<int, int, 3> tree;
    std::set<int> set;

    for (int i = 0; i < 80; ++i) {
        auto key = Ng::Random::Get(0, 100);
        set.insert(key);
        tree.Push(key, 1);
//        tree.Print();
//        std::cout << " - -- - --------------------------------- -- - --  - -- " << std::endl;
//        std::cout << std::endl;
    }

    tree.Print();
    std::cout << " - -- - - -- - --  - -- " << std::endl;

//    while (tree.GetCount() != 0) {
//        auto key = Ng::Random::Get(0, 100);
//
//        std::cout << "-- pop " << key << std::endl;
//
//
//        if (!set.contains(key)) {
//            std::cout << "---- Not exists! " << tree.GetCount() << std::endl;
//            continue;
//        }
//
//        tree.Pop(key);
//        set.erase(key);
//
//        tree.Print();
//        std::cout << "---------------------" << std::endl;
//    }

//    for (int i = 40; i >= 0; --i) {
//        std::cout << "-- pop " << i << std::endl;
//        tree.Pop(i);
//        tree.Print();
//        std::cout << "---------------------" << std::endl;
//        set.erase(i);
//        std::cout << std::endl;
//    }

//    for (int i = 0; i <= 200; ++i) {
//        if (!set.contains(i))
//            continue;
//
//        std::cout << "-- pop " << i << std::endl;
//        tree.Pop(i);
//        tree.Print();
//        std::cout << "---------------------" << std::endl;
//        set.erase(i);
//        std::cout << std::endl;
//    }

    while (tree.GetCount() != 0) {
        if (set.size() != tree.GetCount())
            break;

        int key;

        std::cout << "Key:";
        std::cin >> key;

        std::cout << "-- key = " << key << std::endl;

        if (!set.contains(key))
            continue;

        std::cout << "-- pop " << key << std::endl;

        tree.Pop(key);
        set.erase(key);

        tree.Print();
        std::cout << "---------------------" << std::endl;
    }

    tree.Print();

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
