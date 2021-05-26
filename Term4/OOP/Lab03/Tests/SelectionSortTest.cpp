#include <vector>

#include <doctest/doctest.h>

#include <Ziben/Utility/Random.hpp>

#include "Algorithm.hpp"

template <std::size_t Count, typename T>
void RunSelectionSortTest() {
    std::vector<T> expected(Count);
    std::vector<T> actual(Count);

    // Init
    for (auto& e : expected)
        e = Ziben::Random::Get<T>();

    actual = expected;

    // Custom Sort
    Lab03::Algorithm::SelectionSort(expected.begin(), expected.end());

    // Std sort
    std::sort(actual.begin(), actual.end());

    CHECK(expected == actual);
}

template <std::size_t Count, typename T, typename Comparator>
void RunSelectionSortTest() {
    std::vector<T> expected(Count);
    std::vector<T> actual(Count);

    // Init
    for (auto& e : expected)
        e = Ziben::Random::Get<T>();

    actual = expected;

    // Custom Sort
    Lab03::Algorithm::SelectionSort(expected.begin(), expected.end(), Comparator());

    // Std sort
    std::sort(actual.begin(), actual.end(), Comparator());

    CHECK(expected == actual);
}

TEST_CASE("SelectionSort Test <100, int>") {
    RunSelectionSortTest<100, int>();
}

TEST_CASE("SelectionSort Test <1000, uint8_t>") {
    RunSelectionSortTest<1000, uint8_t>();
}

TEST_CASE("SelectionSort Test <100, float, std::greater<>>") {
    RunSelectionSortTest<10000, float, std::greater<>>();
}
