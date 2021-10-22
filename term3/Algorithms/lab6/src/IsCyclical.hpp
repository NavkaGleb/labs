#pragma once

#include <string>
#include <vector>

namespace Ng {

    class IsCyclical {
    public:
        // operators
        std::size_t operator()(const std::string& text, const std::string& pattern) const;

    private:
        // aliases
        using PrefixType = std::vector<std::size_t>;

        // static member methods
        static PrefixType Prefix(const std::string& pattern);

    };

} // namespace Ng