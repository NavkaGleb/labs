#include "IsCyclical.hpp"

namespace Ng {

    bool IsCyclical(const std::string& left, const std::string& right) {
        if (left.size() != right.size())
            return false;

        for (std::size_t i = 0; i < left.size(); ++i)
            for (std::size_t j = 0; j < left.size() && left[j] == right[i % left.size()]; ++i, ++j)
                if (j == left.size() - 1)
                    return true;

        return false;
    }

} // namespace Ng