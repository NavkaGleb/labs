namespace nm_lab2::math {

template <typename T> requires std::signed_integral<T> || std::floating_point<T>
constexpr T Sgn(T integral) {
  return integral < 0 ? -1 : 1;
}

template <std::unsigned_integral T>
constexpr T Sgn(T integral) {
  return 1;
}

} // namespace nm_lab2::math
