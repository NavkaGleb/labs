#include "RabinKarp.hpp"

#include <utility>
#include <iostream>

namespace Ng {

    // constructor / destructor
    RabinKarp::RabinKarp(std::string&& pattern)
        : m_Pattern(std::move(pattern)),
          m_Prime(15487469),
          m_AlphabetSize(256),
          m_RM(1) {

        for (std::size_t i = 0; i < m_Pattern.size() - 1; ++i)
            m_RM = (m_AlphabetSize * m_RM) % m_Prime;

        m_PatternHash = Hash(m_Pattern, m_Pattern.size());
    }

    // public methods
    std::size_t RabinKarp::Search(const std::string& text) {
        if (text.size() < m_Pattern.size())
            return std::string::npos;

        auto textHash = Hash(text, m_Pattern.size());

        if (textHash == m_PatternHash)
            return 0;

        for (std::size_t i = m_Pattern.size(); i < text.size(); ++i) {
            textHash = (textHash + m_Prime - m_RM * text[i - m_Pattern.size()] % m_Prime) % m_Prime;
            textHash = (textHash * m_AlphabetSize + text[i]) % m_Prime;

            if (textHash == m_PatternHash)
                return i - m_Pattern.size() + 1;
        }

        return std::string::npos;
    }

    // member methods
    std::int64_t RabinKarp::Hash(const std::string& key, std::size_t length) const {
        std::int64_t hash = 0;

        for (std::size_t i = 0; i < length; ++i)
            hash = (m_AlphabetSize * hash + key[i]) % m_Prime;

        return hash;
    }

} // namespace Ng