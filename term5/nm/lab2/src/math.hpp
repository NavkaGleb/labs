#pragma once

#include <concepts>

namespace nm_lab2::math {

template <typename T> requires std::signed_integral<T> || std::floating_point<T>
constexpr T Sgn(T integral);

template <std::unsigned_integral T>
constexpr T Sgn(T integral);

} // namespace nm_lab2::math

#include "math.inl"
