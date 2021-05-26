#include <vector>

#include <doctest/doctest.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <std::size_t Count, typename T>
void RunShellSortTest() {
    std::vector<T> expected(Count);
    std::vector<T> actual(Count);

    // Init
    for (auto& e : expected)
        e = Ziben::Random::Get<T>();

    actual = expected;

    // Custom Sort
    Lab03::Algorithm::ShellSort(expected.begin(), expected.end());

    // Std sort
    std::sort(actual.begin(), actual.end());

    CHECK(expected == actual);
}

template <std::size_t Count, typename T, typename Comparator>
void RunShellSortTest() {
    std::vector<T> expected(Count);
    std::vector<T> actual(Count);

    // Init
    for (auto& e : expected)
        e = Ziben::Random::Get<T>();

    actual = expected;

    // Custom Sort
    Lab03::Algorithm::ShellSort(expected.begin(), expected.end(), Comparator());

    // Std sort
    std::sort(actual.begin(), actual.end(), Comparator());

    CHECK(expected == actual);
}

TEST_CASE("ShellSort Test <100, int>") {
    RunShellSortTest<100, int>();
}

TEST_CASE("ShellSort Test <1000, uint8_t>") {
    RunShellSortTest<1000, uint8_t>();
}

TEST_CASE("ShellSort Test <100, float, std::greater<>>") {
    RunShellSortTest<10000, float, std::greater<>>();
}
