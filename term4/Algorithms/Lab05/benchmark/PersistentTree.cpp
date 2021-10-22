#include <cmath>

#include <benchmark/benchmark.h>

#include <Random/Random.hpp>

#include "PersistentTree.hpp"

static void BM_PersistentTreePush(benchmark::State& state) {
    const int64_t count = state.range(0);

    Ng::PersistentTree<int, int> tree;

    for (auto _ : state) {
        for (int64_t i = 0; i < count; ++i) {
            const auto key = Ng::Random::Get<int>();
            benchmark::DoNotOptimize(tree = tree.Push(key, key));
        }
    }
}

static void BM_PersistentTreePop(benchmark::State& state) {
    const int64_t count = state.range(0);

    Ng::PersistentTree<int, int> tree;
    Ng::PersistentTree<int, int> original;
    std::vector<int>             keys;

    keys.reserve(count);

    for (int64_t i = 0; i < count; ++i) {
        const auto key = Ng::Random::Get<int>();

        tree = tree.Push(key, key);
        keys.push_back(key);
    }

    original = tree;

    for (auto _ : state) {
        for (const auto& key : keys)
            tree = tree.Pop(key);

        tree = original;
    }
}

BENCHMARK(BM_PersistentTreePush)
    ->RangeMultiplier(2)
    ->Range(1, static_cast<int64_t>(std::pow(2, 16)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_PersistentTreePop)
    ->RangeMultiplier(2)
    ->Range(1, static_cast<int64_t>(std::pow(2, 16)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();