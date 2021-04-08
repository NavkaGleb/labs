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

TEST_CASE("BPlusTree IsExists") {
    for (int j = 0; j < 100; ++j) {
        Ng::BPlusTree<int, int, 3> tree;

        std::vector<bool>          expected(10000 + 1, false);
        std::vector<bool>          actual(10000 + 1, false);

        for (int i = 0; i < Ng::Random::Get(300, 10000); ++i) {
            auto key = Ng::Random::Get(0, 10000);

            tree.Push(key, 1);
            expected[key] = true;
        }

        for (int i = 0; i <= 10000; ++i)
            actual[i] = tree.IsExists(i);

        CHECK(actual == expected);
    }
}

TEST_CASE("BPlusTree Pop") {
    for (int j = 0; j < 100; ++j) {
        Ng::BPlusTree<int, int, 2> tree;
        std::vector<int>           vector;
        std::set<int>              keys;
        std::set<int>              actual;

        for (int i = 0; i < Ng::Random::Get(100, 200); ++i) {
            auto key = Ng::Random::Get(0, 10000);

            tree.Push(key, 1);
            keys.insert(key);
            vector.push_back(key);
        }

        std::sort(vector.begin(), vector.end());

        auto to = Ng::Random::Get(0, 10000);

//        for (int i = 0; i <= to; ++i) {
//            tree.Pop(i);
//            keys.erase(i);
//        }

        while (tree.GetCount() != 0) {
            if (vector.empty())
                break;

            auto index = Ng::Random::Get<int>(0, vector.size() - 1);

            auto key = vector[index];
            vector.erase(vector.begin() + index);

            tree.Pop(key);
            keys.erase(key);

            for (auto [vectorIt, keyIt] = std::make_pair(vector.begin(), keys.begin());
                vectorIt != vector.end() && keyIt != keys.end(); ++vectorIt, ++keyIt) {

                if (*vectorIt != *keyIt) {
                    std::cout << "Pop Key = " << key << std::endl;
                    tree.Print();
                    return;
                }
            }
        }

        for (const auto& key : tree)
            actual.insert(key);

        tree.Print();

        std::cout << "Actual:   ";
        for (const auto& key : actual)
            std::cout << key << " ";
        std::cout << std::endl;

        std::cout << "Expected: ";
        for (const auto& key : keys)
            std::cout << key << " ";
        std::cout << std::endl;

        CHECK(actual == keys);
    }
}