#include <cmath>
#include <numeric>

#include <benchmark/benchmark.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <uint32_t AsyncCutoff>
void BM_ParallelQuickSort(benchmark::State& state) {
    std::vector<int> data(state.range());
    std::iota(data.begin(), data.end(), 0);

    for (auto _ : state) {
        state.PauseTiming();
        {
            Lab03::Algorithm::Shuffle(data.begin(), data.end());
        }
        state.ResumeTiming();

        Lab03::Algorithm::ParallelQuickSort<35, AsyncCutoff>(data.begin(), data.end());
    }
}

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 100)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 3000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 3500)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 4000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 5000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 8000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_TEMPLATE(BM_ParallelQuickSort, 20000)
    ->RangeMultiplier(2)
    ->Range(1048576, static_cast<int64_t>(std::pow(2, 24)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();