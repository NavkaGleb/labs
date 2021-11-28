#include <algorithm>

namespace dc_lab6 {

template <typename T, std::size_t kSize>
template <std::same_as<T>... Args>
constexpr Vector<T, kSize>::Vector(Args&&... args) {
  constexpr auto args_count = sizeof...(args);

  if constexpr (args_count == 0) {
    data_.fill(T());
  } else if constexpr (args_count == 1) {
    data_.fill(std::forward<Args>(args)...);
  } else if constexpr (args_count == kSize) {
    data_ = { std::forward<T>(args)... };
  } else {
    static_assert(false && sizeof...(Args));
  }
}

template <typename T, std::size_t kSize>
constexpr bool Vector<T, kSize>::operator ==(const Vector<T, kSize>& other) const {
  for (std::size_t i = 0; i < kSize; ++i) {
    if (data_[i] != other.data_[i]) {
      return false;
    }
  }

  return true;
}

} // namespace dc_lab6
