#pragma once

#include <chrono>

namespace Ng {

    template <typename Rep, typename Period>
    class Timer {
    public:
        // constructor / destructor
        explicit Timer(std::chrono::duration<Rep, Period>& time);
        ~Timer();

    private:
        // aliases
        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

        // member data
        TimePoint m_Start;
        std::chrono::duration<Rep, Period>& m_Time;

    }; // class Timer

    // constructor / destructor
    template <typename Rep, typename Period>
    Timer<Rep, Period>::Timer(std::chrono::duration<Rep, Period>& time)
        : m_Start(std::chrono::high_resolution_clock::now()), m_Time(time) {}

    template <typename Rep, typename Period>
    Timer<Rep, Period>::~Timer() {
        TimePoint end = std::chrono::high_resolution_clock::now();
        m_Time = std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(end - m_Start);
    }

} // namespace Ng