#include <doctest/doctest.h>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

TEST_CASE("BPlusTree Push") {
    for (int j = 0; j < 10; ++j) {
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

        REQUIRE(actual == keys);
    }
}

TEST_CASE("BPlusTree IsExists") {
    for (int j = 0; j < 10; ++j) {
        Ng::BPlusTree<int, int, 2> tree;
        std::set<int>              keys;

        for (int i = 0; i < Ng::Random::Get(300, 10000); ++i) {
            auto key = Ng::Random::Get(0, 10000);

            tree.Push(key, 1);
            keys.insert(key);
        }

        auto currentKey = keys.begin();

        for (int i = 0; i < 10000; ++i) {
            INFO(i);
            CHECK(tree.IsExists(i) == keys.contains(i));
        }
    }
}