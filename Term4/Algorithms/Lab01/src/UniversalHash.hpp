#pragma once

namespace Ng {

    template <typename T>
    struct UniversalHash {
        virtual std::size_t operator ()(const T& t, int a, int b, int p, int m) const = 0;

    }; // struct UniversalHash

    template <>
    struct UniversalHash<int> {
        std::size_t operator ()(const int& t, int a, int b, int p, int m) const {
            return ((a * t + b) % p) % m;
        }

    }; // struct UniversalHash<int>

} // namespace Ng