#include "StringRabinKarp.hpp"

namespace Ng {

    // constructor / destructor
    StringRabinKarp::StringRabinKarp(std::string&& pattern)
        : m_Pattern(std::move(pattern)),
          m_Prime(15487469),
          m_AlphabetSize(256),
          m_PolynomialOrder(1) {

        for (std::size_t i = 0; i < m_Pattern.size() - 1; ++i)
            m_PolynomialOrder = (m_AlphabetSize * m_PolynomialOrder) % m_Prime;

        m_PatternHash = Hash(m_Pattern, m_Pattern.size());
    }

    // modifiers
    void StringRabinKarp::setPattern(const std::string& pattern) {
        m_Pattern = pattern;
    }

    // public methods
    std::size_t StringRabinKarp::Search(const std::string& text) const {
        if (text.size() < m_Pattern.size())
            return std::string::npos;

        auto textHash = Hash(text, m_Pattern.size());

        if (textHash == m_PatternHash)
            return 0;

        for (std::size_t i = m_Pattern.size(); i < text.size(); ++i) {
            textHash = (textHash + m_Prime - m_PolynomialOrder * text[i - m_Pattern.size()] % m_Prime) % m_Prime;
            textHash = (textHash * m_AlphabetSize + text[i]) % m_Prime;

            if (textHash == m_PatternHash)
                return i - m_Pattern.size() + 1;
        }

        return std::string::npos;
    }

    // member methods
    StringRabinKarp::HashType StringRabinKarp::Hash(const std::string& key, std::size_t length) const {
        HashType hash = 0;

        for (std::size_t i = 0; i < length; ++i)
            hash = (m_AlphabetSize * hash + key[i]) % m_Prime;

        return hash;
    }

} // namespace Ng