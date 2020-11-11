#pragma once

#include "Timer.hpp"

namespace Ng {

    // alias
    template <typename Period>
    using Duration = std::chrono::duration<int64_t, Period>;



    template <typename Period, typename Func, typename... Args>
    void Benchmark(Duration<Period>& time, Func func, Args... args) {
        Timer timer(time);
        func(args...);
    }

    template <typename Period, typename Func, typename... Args>
    Duration<Period> Benchmark(Func func, Args... args) {
        Duration<Period> time;

        {
            Timer timer(time);
            func(args...);
        }

        return time;
    }

} // namespace Ng