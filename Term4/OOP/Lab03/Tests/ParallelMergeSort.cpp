#include <vector>

#include <doctest/doctest.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <std::size_t Count, typename T>
void RunParallelMergeSortTest() {
    std::vector<T> expected(Count);
    std::vector<T> actual(Count);

    // Init
    for (auto& e : expected)
        e = Ziben::Random::Get<T>();

    actual = expected;

    // Custom Sort
    Lab03::Algorithm::ParallelMergeSort(expected.begin(), expected.end());

    // Std sort
    std::sort(actual.begin(), actual.end());

    CHECK(expected == actual);
}

template <std::size_t Count, typename T, typename Comparator>
void RunParallelMergeSortTest() {
    std::vector<T> expected(Count);
    std::vector<T> actual(Count);

    // Init
    for (auto& e : expected)
        e = Ziben::Random::Get<T>();

    actual = expected;

    // Custom Sort
    Lab03::Algorithm::ParallelMergeSort(expected.begin(), expected.end(), Comparator());

    // Std sort
    std::sort(actual.begin(), actual.end(), Comparator());

    CHECK(expected == actual);
}

TEST_CASE("ParallelMergeSort Test <100, int>") {
    RunParallelMergeSortTest<100, int>();
}

TEST_CASE("ParallelMergeSort Test <1000, uint8_t>") {
    RunParallelMergeSortTest<1000, uint8_t>();
}

TEST_CASE("ParallelMergeSort Test <100, float, std::greater<>>") {
    RunParallelMergeSortTest<10000, float, std::greater<>>();
}
