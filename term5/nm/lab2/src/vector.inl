#include <algorithm>

namespace nm_lab2 {

template <typename T>
constexpr inline bool always_false = false;

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
constexpr std::size_t Vector<T, kSize>::GetSize() const noexcept {
  return kSize;
}

template <typename T, std::size_t kSize>
constexpr typename Vector<T, kSize>::Iterator Vector<T, kSize>::Begin() noexcept {
  return data_.begin();
}

template <typename T, std::size_t kSize>
constexpr typename Vector<T, kSize>::Iterator Vector<T, kSize>::End() noexcept {
  return data_.end();
}

template <typename T, std::size_t kSize>
constexpr typename Vector<T, kSize>::ConstIterator Vector<T, kSize>::Begin() const noexcept {
  return data_.begin();
}

template <typename T, std::size_t kSize>
constexpr typename Vector<T, kSize>::ConstIterator Vector<T, kSize>::End() const noexcept {
  return data_.end();
}

template <typename T, std::size_t kSize>
constexpr T& Vector<T, kSize>::operator [](std::size_t index) {
  return data_[index];
}

template <typename T, std::size_t kSize>
constexpr const T& Vector<T, kSize>::operator [](std::size_t index) const {
  return data_[index];
}

} // namespace nm_lab2
