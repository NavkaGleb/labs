#pragma once

#include <chrono>
#include <functional>

namespace Ng {

    template <typename Func, typename... Args>
    int Benchmark(Func func, Args... args) {
        auto start = std::chrono::steady_clock::now();
        func(args...);
        auto end = std::chrono::steady_clock::now();

        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

} // namespace Ng