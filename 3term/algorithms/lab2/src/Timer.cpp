#include "Timer.hpp"

namespace ng {

    // constructor / destructor
    Timer::Timer(double& data) : data(data) { this->start = ch::steady_clock::now(); }

    Timer::~Timer() {
        ch::time_point<ch::steady_clock> end = ch::steady_clock::now();
        this->data = ch::duration_cast<ch::milliseconds>(end - start).count() * 0.001;
    }

} // namespace ng