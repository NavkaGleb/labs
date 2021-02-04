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

    template <>
    struct UniversalHash<std::string> {
        std::size_t operator ()(const std::string& string, int a, int b, int p, int m) const {
            return std::accumulate(
                string.cbegin(),
                string.cend(),
                0,
                [a, b, p](int sum, char ch) { return (a * (sum + ch) + b) % p; }
            ) % m;
        }

    }; // struct UniversalHash<std::string>

} // namespace Ng