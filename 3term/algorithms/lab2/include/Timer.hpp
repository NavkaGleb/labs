#pragma once

#include <chrono>

namespace ch = std::chrono;

namespace ng {

    class Timer {
    public:
        // constructor / destructor
        Timer(double& data);
        virtual ~Timer();

    private:
        ch::time_point<ch::steady_clock> start;
        double& data;

    }; // class Time

} // namespace ng