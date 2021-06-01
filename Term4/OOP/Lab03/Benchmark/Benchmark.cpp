#include <iostream>
#include <cmath>
#include <numeric>

#include <benchmark/benchmark.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

#define BENCHMARK_SORT(BenchmarkFunctionName, SortFunction)           \
    static void BenchmarkFunctionName(benchmark::State& state) {      \
        std::vector<int> data(state.range());                         \
        std::iota(data.begin(), data.end(), 0);                       \
                                                                      \
        for (auto _ : state) {                                        \
            state.PauseTiming();                                      \
            {                                                         \
                Lab03::Algorithm::Shuffle(data.begin(), data.end());  \
            }                                                         \
            state.ResumeTiming();                                     \
                                                                      \
            SortFunction(data.begin(), data.end());                   \
        }                                                             \
    }

BENCHMARK_SORT(BM_BubbleSort,        Lab03::Algorithm::BubbleSort       )
BENCHMARK_SORT(BM_SelectionSort,     Lab03::Algorithm::SelectionSort    )
BENCHMARK_SORT(BM_InsertionSort,     Lab03::Algorithm::InsertionSort    )
BENCHMARK_SORT(BM_ShellSort,         Lab03::Algorithm::ShellSort        )
BENCHMARK_SORT(BM_StandardSort,      std::sort                          )
BENCHMARK_SORT(BM_QuickSort,         Lab03::Algorithm::QuickSort        )
BENCHMARK_SORT(BM_ParallelQuickSort, Lab03::Algorithm::ParallelQuickSort)
BENCHMARK_SORT(BM_MergeSort,         Lab03::Algorithm::MergeSort        )
BENCHMARK_SORT(BM_BottomUpMergeSort, Lab03::Algorithm::BottomUpMergeSort)
BENCHMARK_SORT(BM_ParallelMergeSort, Lab03::Algorithm::ParallelMergeSort)

BENCHMARK(BM_BubbleSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 16)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_SelectionSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 16)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_InsertionSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 16)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_ShellSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 16)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_StandardSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 25)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_QuickSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 25)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_ParallelQuickSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 25)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_MergeSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 25)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_BottomUpMergeSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 25)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK(BM_ParallelMergeSort)
    ->RangeMultiplier(4)
    ->Range(4096, static_cast<int64_t>(std::pow(2, 25)))
    ->Unit(benchmark::TimeUnit::kMillisecond);

BENCHMARK_MAIN();