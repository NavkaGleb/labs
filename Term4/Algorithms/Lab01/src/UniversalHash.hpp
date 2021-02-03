#pragma once

namespace Ng {

    template <typename T>
    struct UniversalHash {
        virtual std::size_t operator ()(const T& t, int a, int b, int p, int m) = 0;

    }; // class UniversalHash

    template <>
    struct UniversalHash<int> {
        std::size_t operator ()(const int& t, int a, int b, int p, int m) {
            return ((a * t + b) % p) % m;
        }

    }; // class UniversalHash<int>

} // namespace Ng