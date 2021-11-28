#include <algorithm>

namespace dc_lab6 {

template <typename T, std::size_t kSize>
DynamicVector<T, kSize>::DynamicVector() noexcept {
  data_.resize(kSize);
}

template <typename T, std::size_t kSize>
DynamicVector<T, kSize>::DynamicVector(const T& scalar) noexcept {
  data_.resize(kSize);
  std::fill_n(data_.begin(), kSize, scalar);
}

template <typename T, std::size_t kSize>
DynamicVector<T, kSize>::DynamicVector(std::initializer_list<T> list) noexcept {
  data_.resize(kSize);

  for (std::size_t i = 0; i < list.size(); ++i) {
    data_[i] = std::move(*(list.begin() + i));
  }
}

template <typename T, std::size_t kSize>
bool DynamicVector<T, kSize>::operator ==(const DynamicVector<T, kSize>& other) const noexcept {
  for (std::size_t i = 0; i < kSize; ++i) {
    if (data_[i] != other.data_[i]) {
      return false;
    }
  }

  return true;
}

} // namespace dc_lab6
