#pragma once

#include <random>
#include <typeinfo>
#include <ctime>
#include <cmath>

namespace ng {

    class random {
    public:
        // static methods
        template <typename T>
        static T irand(const T& left, const T& right);

        template <typename T>
        static T frand(const T& left, const T& right, const int& fpart = 1);

    }; // class random

    template <typename T>
    T random::irand(const T& left, const T& right) {
        static std::random_device rd;
        static std::seed_seq seq{rd(), static_cast<unsigned>(time(nullptr))};
        static std::mt19937 mt(seq);

        std::uniform_int_distribution dist(left, right);
        return dist(mt);
    }

    template <typename T>
    T random::frand(const T& left, const T& right, const int& fpart) {
        static std::random_device rd;
        static std::seed_seq seq{rd(), static_cast<unsigned>(time(nullptr))};
        static std::mt19937 mt(seq);

        std::uniform_real_distribution dist(left, right);

        if (typeid(T) == typeid(float))
            return roundf(dist(mt) * std::pow(10, fpart)) / std::pow(10, fpart);

        if (typeid(T) == typeid(double))
            return round(dist(mt) * std::pow(10, fpart)) / std::pow(10, fpart);

        return roundl(dist(mt) * std::pow(10, fpart)) / std::pow(10, fpart);
    }

} // namespace ng