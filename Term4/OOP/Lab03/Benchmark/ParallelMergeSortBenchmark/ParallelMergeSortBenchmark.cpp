#include <cmath>
#include <numeric>

#include <benchmark/benchmark.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <uint32_t AsyncCutoff>
void BM_ParallelMergeSort(benchmark::State& state) {
    std::vector<int> data(state.range());
    std::iota(data.begin(), data.end(), 0);

    for (auto _ : state) {
        state.PauseTiming();
        {
            Lab03::Algorithm::Shuffle(data.begin(), data.end());
        }
        state.ResumeTiming();

        Lab03::Algorithm::ParallelMergeSort<100, AsyncCutoff>(data.begin(), data.end());
    }
}

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 100)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 2000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 3000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 4000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 5000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 8000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelMergeSort, 20000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();