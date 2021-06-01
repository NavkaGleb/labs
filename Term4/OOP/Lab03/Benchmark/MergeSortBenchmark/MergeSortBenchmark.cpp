#include <cmath>
#include <numeric>

#include <benchmark/benchmark.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <uint32_t InsertionSortCutoff>
void BM_MergeSort(benchmark::State& state) {
    std::vector<int> data(state.range());
    std::iota(data.begin(), data.end(), 0);

    for (auto _ : state) {
        state.PauseTiming();
        {
            Lab03::Algorithm::Shuffle(data.begin(), data.end());
        }
        state.ResumeTiming();

        Lab03::Algorithm::MergeSort<InsertionSortCutoff>(data.begin(), data.end(), std::less<>());
    }
}

BENCHMARK_TEMPLATE(BM_MergeSort, 0)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_MergeSort, 40)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_MergeSort, 60)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_MergeSort, 80)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_MergeSort, 100)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_MergeSort, 120)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_MergeSort, 140)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();