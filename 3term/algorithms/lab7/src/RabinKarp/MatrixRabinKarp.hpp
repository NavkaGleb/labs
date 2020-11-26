#pragma once

#include <tuple>
#include <cstdint>

#include "Matrix/Matrix.hpp"

namespace Ng {

    ////////////////////////////////////////////////////////////////////////////
    /// HEADER
    ////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class MatrixRabinKarp {
    public:
        // constructor / destructor
        explicit MatrixRabinKarp(const Matrix<T>& pattern);

        // public methods
        std::pair<std::size_t, std::size_t> Search(const Matrix<T>& text) const;

    private:
        // aliases
        using HashType = std::int64_t;
        using MatrixHashType = std::vector<HashType>;

        // member data
        Matrix<T>      m_Pattern;
        MatrixHashType m_PatternHash;
        HashType       m_Prime;
        HashType       m_AlphabetSize;
        HashType       m_PolynomialOrder;

        // member methods
        MatrixHashType Hash(const Matrix<T>& matrix, std::size_t startRow = 0, std::size_t startColumn = 0) const;

    }; // class MatrixRabinKarp

    ////////////////////////////////////////////////////////////////////////////
    /// SOURCE
    ////////////////////////////////////////////////////////////////////////////

    // constructor / destructor
    template <typename T>
    MatrixRabinKarp<T>::MatrixRabinKarp(const Matrix<T>& pattern)
        : m_Pattern(pattern),
          m_Prime(15487469),
          m_AlphabetSize(256),
          m_PolynomialOrder(1) {

        for (std::size_t i = 0; i < m_Pattern.Columns() - 1; ++i)
            m_PolynomialOrder = (m_AlphabetSize * m_PolynomialOrder) % m_Prime;

        m_PatternHash = Hash(m_Pattern);
    }

    // public methods
    template <typename T>
    std::pair<std::size_t, std::size_t> MatrixRabinKarp<T>::Search(const Matrix<T>& text) const {
        if (text.Rows() < m_Pattern.Rows() || text.Columns() < m_Pattern.Columns())
            return { -1, -1 };

        for (std::size_t i = 0; i <= text.Rows() - m_Pattern.Rows(); ++i) {
            MatrixHashType textHash = Hash(text, i, 0);

            for (std::size_t j = 0; j <= text.Columns() - m_Pattern.Columns(); ++j) {
                if (textHash == m_PatternHash)
                    return { i, j };

                if (j == text.Columns() - m_Pattern.Columns())
                    break;

                for (int k = 0; k < textHash.size(); ++k) {
                    textHash[k] = (textHash[k] + m_Prime - m_PolynomialOrder * text.At(i + k, j) % m_Prime) % m_Prime;
                    textHash[k] = (textHash[k] * m_AlphabetSize + text.At(i + k, j + m_Pattern.Columns())) % m_Prime;
                }
            }
        }

        return { -1, -1 };
    }

    // member methods
    template <typename T>
    typename MatrixRabinKarp<T>::MatrixHashType MatrixRabinKarp<T>::Hash(const Matrix<T>& matrix,
                                                                         std::size_t startRow,
                                                                         std::size_t startColumn) const {
        std::vector<HashType> hash(m_Pattern.Rows(), 0);

        for (std::size_t i = 0; i < m_Pattern.Rows(); ++i)
            for (std::size_t j = 0; j < m_Pattern.Columns(); ++j)
                hash[i] = (m_AlphabetSize * hash[i] + matrix.At(i + startRow, j + startColumn)) % m_Prime;

        return hash;
    }

} // namespace Ng