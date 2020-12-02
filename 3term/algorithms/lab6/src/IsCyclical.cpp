#include "IsCyclical.hpp"

#include <iostream>

namespace Ng {

    // operators
    std::size_t IsCyclical::operator()(const std::string& text, const std::string& pattern) const {
        if (text.size() != pattern.size())
            return std::string::npos;

        PrefixType prefix = IsCyclical::Prefix(pattern);
        std::string doubleText = text + text;
        std::size_t j = 0;

        for (std::size_t i = 1; i < doubleText.size(); ++i) {
            while (j > 0 && pattern[j] != doubleText[i])
                j = prefix[j - 1];

            if (pattern[j] == doubleText[i])
                ++j;

            if (j == pattern.size()) {
                return i - pattern.size() + 1;
            }
        }

        return std::string::npos;
    }

    // static member methods
    IsCyclical::PrefixType IsCyclical::Prefix(const std::string& pattern) {
        PrefixType prefixes(pattern.size(), 0);

        for (std::size_t i = 1; i < pattern.size(); ++i) {
            std::size_t j = prefixes[i - 1];

            while (j > 0 && pattern[i] != pattern[j])
                j = prefixes[j - 1];

            if (pattern[i] == pattern[j])
                ++j;

            prefixes[i] = j;
        }

        return prefixes;
    }

} // namespace Ng