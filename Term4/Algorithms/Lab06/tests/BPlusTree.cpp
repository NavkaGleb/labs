#include <doctest/doctest.h>

#include <Random/Random.hpp>

#include "BPlusTree.hpp"

TEST_CASE("BPlusTree Push") {
    for (int i = 0; i < 100; ++i) {
        const int count = Ng::Random::Get(1000, 10000);

        Ng::BPlusTree<int, int, 2> tree;

        std::vector<int>           keys;
        std::vector<int>           actual;

        keys.reserve(count);
        actual.reserve(count);

        for (int j = 0; j < count; ++j) {
            auto key = Ng::Random::Get(0, 100'000);

            keys.push_back(key);
            tree.Push(key, 1);
        }

        for (const auto& [key, value] : tree)
            actual.push_back(key);

        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

        CHECK(actual == keys);
    }
}

TEST_CASE("BPlusTree IsExists") {
    for (int i = 0; i < 100; ++i) {
        const int count = Ng::Random::Get(1000, 10000);
        const int bound = 100'000;

        Ng::BPlusTree<int, int, 3> tree;

        std::vector<bool> expected(bound + 1, false);
        std::vector<bool> actual(bound + 1, false);

        for (int j = 0; j < count; ++j) {
            auto key = Ng::Random::Get(0, bound);

            tree.Push(key, 1);
            expected[key] = true;
        }

        for (int j = 0; j <= bound; ++j)
            actual[j] = tree.IsContains(j);

        CHECK(actual == expected);
    }
}

TEST_CASE("BPlusTree Pop") {
    for (int i = 0; i < 100; ++i) {
        const int count = Ng::Random::Get(1000, 10000);

        Ng::BPlusTree<int, int, 2> tree;

        std::vector<int>           keys;
        std::vector<int>           actual;

        keys.reserve(count);
        actual.reserve(count);

        for (int j = 0; j < count; ++j) {
            auto key = Ng::Random::Get(0, 100'000);

            keys.push_back(key);
            tree.Push(key, 1);
        }

        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

        while (!keys.empty()) {
            auto index = Ng::Random::Get<std::size_t>(0, keys.size() - 1);

            tree.Pop(keys[index]);
            keys.erase(keys.begin() + static_cast<decltype(keys)::difference_type>(index));
        }

        for (const auto& [key, value] : tree)
            actual.push_back(key);

        CHECK(actual == keys);
    }
}