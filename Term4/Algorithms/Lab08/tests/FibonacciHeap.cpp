#include <doctest/doctest.h>

#include <Random/Random.hpp>

#include "FibonacciHeap.hpp"

TEST_CASE("FibonacciHeap Push / PopPeak") {
    for (int i = 0; i < 100; ++i) {
        Ng::FibonacciHeap<int> heap;
        std::vector<int>       keys;
        std::vector<int>       actual;

        const int count = Ng::Random::Get(200, 1000);

        keys.reserve(count);
        actual.reserve(count);

        for (int j = 0; j < count; ++j) {
            auto key = Ng::Random::Get(0, 100'000);

            heap.Push(key);
            keys.push_back(key);
        }

        std::sort(keys.begin(), keys.end());

        while (!heap.IsEmpty()) {
            actual.push_back(heap.GetPeak());
            heap.PopPeak();
        }

        CHECK(actual == keys);
    }
}

TEST_CASE("FibonacciHeap Push / PopPeak / Exchange") {
    for (int i = 0; i < 100; ++i) {
        Ng::FibonacciHeap<int> heap;
        std::vector<int>       keys;
        std::vector<int>       actual;

        const int count = Ng::Random::Get(200, 1000);

        keys.reserve(count);
        actual.reserve(count);

        for (int j = 0; j < count; ++j) {
            auto key = Ng::Random::Get(0, 100'000);

            heap.Push(key);
            keys.push_back(key);
        }

        for (int j = 0; j < 150; ++j) {
            auto key   = Ng::Random::Get(-100, -10);

            heap.Exchange(keys[j], key);
            keys[j] = key;
        }

        std::sort(keys.begin(), keys.end());

        while (!heap.IsEmpty()) {
            actual.push_back(heap.GetPeak());
            heap.PopPeak();
        }

        CHECK(actual == keys);
    }
}