#include <doctest/doctest.h>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

TEST_CASE("BPlusTree Push") {
    for (int j = 0; j < 100; ++j) {
        Ng::BPlusTree<int, int, 2> tree;
        std::set<int>              keys;
        std::set<int>              actual;

        for (int i = 0; i < Ng::Random::Get(300, 2000); ++i) {
            auto key = Ng::Random::Get(0, 10000);

            tree.Push(key, 1);
            keys.insert(key);
        }

        auto currentKey = keys.begin();

        for (const auto& key : tree)
            actual.insert(key);

        CHECK(actual == keys);
    }
}

//TEST_CASE("BPlusTree IsExists") {
//    for (int j = 0; j < 10; ++j) {
//        Ng::BPlusTree<int, int, 3> tree;
//
//        std::vector<bool>          expected(10000 + 1, false);
//        std::vector<bool>          actual(10000 + 1, false);
//
//        for (int i = 0; i < Ng::Random::Get(300, 10000); ++i) {
//            auto key = Ng::Random::Get(0, 10000);
//
//            tree.Push(key, 1);
//            expected[i] = true;
//        }
//
//        for (int i = 0; i <= 10000; ++i)
//            actual[i] = tree.IsExists(i);
//
//        CHECK(actual == expected);
//    }
//}

TEST_CASE("BPlusTree Pop") {
    for (int j = 0; j < 100; ++j) {
        Ng::BPlusTree<int, int, 2> tree;
        std::set<int>              keys;
        std::set<int>              actual;

        for (int i = 0; i < Ng::Random::Get(300, 2000); ++i) {
            auto key = Ng::Random::Get(0, 10000);

            tree.Push(key, 1);
            keys.insert(key);
        }

        auto to = Ng::Random::Get(0, 10000);

        while (tree.GetCount() != 0) {
            auto key = Ng::Random::Get(0, 10000);

            tree.Pop(key);
            keys.erase(key);
        }

        for (const auto& key : tree)
            actual.insert(key);

        CHECK(actual == keys);
    }
}