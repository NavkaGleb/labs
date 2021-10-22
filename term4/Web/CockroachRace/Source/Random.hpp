#pragma once

#include <random>
#include <cstdint>
#include <chrono>
#include <stdexcept>

namespace CockroachRace {

    template <typename T>
    T GetRandomNumber(T min, T max) {
        namespace cr = std::chrono;

        static auto rd   = std::random_device();
        static auto seed = std::seed_seq{ rd(), static_cast<uint32_t>(cr::steady_clock::now().time_since_epoch().count()) };
        static auto mt   = std::mt19937(seed);

        if constexpr (std::is_integral_v<T>)
            return std::uniform_int_distribution<T>(min, max)(mt);

        if constexpr (std::is_floating_point_v<T>)
            return std::uniform_real_distribution<T>(min, max)(mt);

        throw std::invalid_argument("CockroachRace::GetRandomNumber: Not supported type");
    }

} // namespace CockroachRace