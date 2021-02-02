#pragma once

#include <string>
#include <cstdint>

namespace Ng {

    class StringRabinKarp {
    public:
        // constructor / destructor
        explicit StringRabinKarp(std::string&& pattern);

        // accessors
        [[nodiscard]] inline const std::string& getPattern() const { return m_Pattern; }

        // modifiers
        void setPattern(const std::string& pattern);

        // public methods
        [[nodiscard]] std::size_t Search(const std::string& text) const;

    private:
        // aliases
        using HashType = std::int64_t;

        // member data
        std::string m_Pattern;
        HashType    m_PatternHash;
        HashType    m_Prime;
        HashType    m_AlphabetSize;
        HashType    m_PolynomialOrder;

        // member methods
        [[nodiscard]] HashType Hash(const std::string& key, std::size_t length) const;

    }; // class RabinKarp

} // namespace Ng