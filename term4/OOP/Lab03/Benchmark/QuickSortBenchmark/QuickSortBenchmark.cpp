#include <cmath>
#include <numeric>

#include <benchmark/benchmark.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <uint32_t InsertionSortCutoff>
void BM_QuickSort(benchmark::State& state) {
    std::vector<int> data(state.range());
    std::iota(data.begin(), data.end(), 0);

    for (auto _ : state) {
        state.PauseTiming();
        {
            Lab03::Algorithm::Shuffle(data.begin(), data.end());
        }
        state.ResumeTiming();

        Lab03::Algorithm::QuickSort<InsertionSortCutoff>(data.begin(), data.end());
    }
}

BENCHMARK_TEMPLATE(BM_QuickSort, 10)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 20)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 40)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 60)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 80)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 100)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 140)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_QuickSort, 200)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();