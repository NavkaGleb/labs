#include <doctest/doctest.h>

#include <Random/Random.hpp>

#include "PersistentTree.hpp"

TEST_CASE("PersistentTree Push") {
    for (int i = 0; i < 100; ++i) {
        int count = Ng::Random::Get(1000, 10000);

        Ng::PersistentTree<int, int> tree;
        std::vector<int>             keys(count);
        std::vector<int>             actual;

        std::iota(keys.begin(), keys.end(), 0);

        for (int j = 0; j < count; ++j)
            tree = tree.Push(j, j);

        actual.reserve(count);

        for (const auto& [key, value] : tree)
            actual.emplace_back(key);

        CHECK(actual == keys);
    }
}

TEST_CASE("PersistentTree Pop") {
    for (int i = 0; i < 100; ++i) {
        int count = Ng::Random::Get(1000, 10000);

        Ng::PersistentTree<int, int> tree;
        std::vector<int>             keys;
        std::vector<int>             actual;

        keys.reserve(count);
        actual.reserve(count);

        for (int j = 0; j < count; ++j) {
            auto key = Ng::Random::Get(0, 100'000);

            tree = tree.Push(key, j);
            keys.emplace_back(key);
        }

        for (const auto& [key, value] : tree)
            actual.emplace_back(key);

        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

        CHECK(actual == keys);

        while (!keys.empty()) {
            auto index = Ng::Random::Get<std::size_t>(0, keys.size() - 1);

            tree = tree.Pop(keys[index]);
            keys.erase(keys.begin() + static_cast<decltype(keys)::difference_type>(index));
        }

        actual.clear();

        for (const auto& [key, value] : tree)
            actual.emplace_back(key);

        CHECK(actual == keys);
    }
}