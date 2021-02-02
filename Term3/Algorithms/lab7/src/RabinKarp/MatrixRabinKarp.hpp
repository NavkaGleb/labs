#pragma once

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

        // accessors
        [[nodiscard]] inline const Matrix<T>& GetPattern() const { return m_Pattern; }

        // modifiers
        void SetPattern(const Matrix<T>& pattern);

        // public methods
        std::vector<std::pair<std::size_t, std::size_t>> Search(const Matrix<T>& text) const;

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
        [[nodiscard]] MatrixHashType Hash(const Matrix<T>& matrix, std::size_t startRow, std::size_t startColumn) const;
        [[nodiscard]] bool HashEquality(const MatrixHashType& matrixHash) const;
        void UpdatePatternHash();


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

        UpdatePatternHash();
    }

    // modifiers
    template <typename T>
    void MatrixRabinKarp<T>::SetPattern(const Matrix<T>& pattern) {
        m_Pattern = pattern;
        UpdatePatternHash();
    }

    // public methods
    template <typename T>
    std::vector<std::pair<std::size_t, std::size_t>> MatrixRabinKarp<T>::Search(const Matrix<T>& text) const {
        if (text.Rows() < m_Pattern.Rows() || text.Columns() < m_Pattern.Columns())
            return { };

        std::vector<std::pair<std::size_t, std::size_t>> result;

        for (std::size_t i = 0; i <= text.Rows() - m_Pattern.Rows(); ++i) {
            MatrixHashType textHash = Hash(text, i, 0);

            for (std::size_t j = 0; j <= text.Columns() - m_Pattern.Columns(); ++j) {
                if (HashEquality(textHash))
                    result.emplace_back(i, j);

                if (j == text.Columns() - m_Pattern.Columns())
                    break;

                for (int k = 0; k < textHash.size(); ++k) {
                    textHash[k] = (textHash[k] + m_Prime - m_PolynomialOrder * text.At(i + k, j) % m_Prime) % m_Prime;
                    textHash[k] = (textHash[k] * m_AlphabetSize + text.At(i + k, j + m_Pattern.Columns())) % m_Prime;
                }
            }
        }

        return result;
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

    template <typename T>
    bool MatrixRabinKarp<T>::HashEquality(const MatrixHashType& matrixHash) const {
        for (std::size_t i = 0; i < matrixHash.size(); ++i)
            if (matrixHash[i] != m_PatternHash[i])
                return false;

        return true;
    }

    template <typename T>
    void MatrixRabinKarp<T>::UpdatePatternHash() {
        for (std::size_t i = 0; i < m_Pattern.Columns() - 1; ++i)
            m_PolynomialOrder = (m_AlphabetSize * m_PolynomialOrder) % m_Prime;

        m_PatternHash = Hash(m_Pattern, 0, 0);
    }

} // namespace Ng