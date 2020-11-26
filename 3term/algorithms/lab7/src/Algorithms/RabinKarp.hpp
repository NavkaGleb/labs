#pragma once

#include <string>
#include <cstdint>

namespace Ng {

    class RabinKarp {
    public:
        // aliases

        // constructor / destructor
        explicit RabinKarp(std::string&&  pattern);

        // public methods
        std::size_t Search(const std::string& text);

    private:
        // member data
        std::string  m_Pattern;
        std::int64_t m_PatternHash;
        std::int64_t m_Prime;
        std::int64_t m_AlphabetSize;
        std::int64_t m_RM;

        // member methods
        [[nodiscard]] std::int64_t Hash(const std::string& key, std::size_t length) const;

    };

} // namespace Ng