#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <initializer_list>

namespace Ng {

    template <typename T>
    class Matrix {
    public:
        // constructor / destructor
        explicit Matrix(std::size_t rows = 0, std::size_t columns = 0, const T& value = T());
        Matrix(std::initializer_list<std::initializer_list<T>> initList);
        virtual ~Matrix() = default;

        // accessors
        [[nodiscard]] inline bool Empty() const { return m_Data.emtpy(); }
        [[nodiscard]] inline std::size_t Rows() const { return m_Rows; }
        [[nodiscard]] inline std::size_t Columns() const { return m_Columns; }
        [[nodiscard]] inline T& operator()(std::size_t i, std::size_t j) { return m_Data[i][j]; }
        [[nodiscard]] inline const T& At(std::size_t i, std::size_t j) const { return m_Data.at(i).at(j); }

        // modifiers
        // public methods
        void Clear();

        // operators
        Matrix<T> operator+(const Matrix<T>& other) const;
        Matrix<T> operator-(const Matrix<T>& other) const;
        Matrix<T> operator*(const Matrix<T>& other) const;
        bool operator==(const Matrix<T>& other) const;
        template <typename T_> friend std::ostream& operator<<(std::ostream& stream, const Matrix<T_>& matrix);

    private:
        // member data
        std::vector<std::vector<T>> m_Data;
        std::size_t m_Rows;
        std::size_t m_Columns;
    }; // class Matrix

    // constructor / destructor
    template <typename T>
    Matrix<T>::Matrix(std::size_t rows, std::size_t columns, const T& value) : m_Rows(rows), m_Columns(columns) {
        m_Data.resize(m_Rows);

        for (int i = 0; i < m_Rows; ++i) {
            m_Data[i].resize(m_Columns);

            for (int j = 0; j < m_Columns; ++j)
                m_Data[i][j] = value;
        }
    }

    template <typename T>
    Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> initList) {
        m_Rows = initList.size();
        m_Columns = initList.begin()->size();
        m_Data.reserve(initList.size());

        for (const auto& row : initList)
            m_Data.emplace_back(row);
    }

    // public methods
    template <typename T>
    void Matrix<T>::Clear() {
        for (auto& row : m_Data) {
            row.clear();
            row.shrink_to_fit();
        }

        m_Data.clear();
        m_Data.shrink_to_fit();
    }

    // operators
    template <typename T>
    Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
        if (m_Rows != other.m_Rows || m_Columns != other.m_Columns)
            throw std::invalid_argument("matrix have different size");

        Matrix<T> result(m_Rows, m_Columns);

        for (std::size_t i = 0; i < m_Rows; ++i)
            for (std::size_t j = 0; j < m_Columns; ++j)
                result.m_Data[i][j] = m_Data[i][j] + other.m_Data[i][j];

        return result;
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
        if (m_Rows != other.m_Rows || m_Columns != other.m_Columns)
            throw std::invalid_argument("matrix have different size");

        Matrix<T> result(m_Rows, m_Columns);

        for (std::size_t i = 0; i < m_Rows; ++i)
            for (std::size_t j = 0; j < m_Columns; ++j)
                result.m_Data[i][j] = m_Data[i][j] - other.m_Data[i][j];

        return result;
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
        if (m_Columns != other.m_Rows)
            throw std::invalid_argument("matrix have different size");

        Matrix<T> result(m_Rows, other.m_Columns);

        for (std::size_t i = 0; i < m_Rows; ++i)
            for (std::size_t j = 0; j < other.m_Columns; ++j)
                for (std::size_t k = 0; k < m_Columns; ++k)
                    result.m_Data[i][j] += m_Data[i][k] * other.m_Data[k][j];

        return result;
    }

    template <typename T>
    bool Matrix<T>::operator==(const Matrix<T>& other) const {
        if (m_Rows != other.m_Rows || m_Columns != other.m_Columns)
            return false;

        for (std::size_t i = 0; i < m_Rows; ++i)
            for (std::size_t j = 0; j < m_Columns; ++j)
                if (m_Data[i][j] != other.m_Data[i][j])
                    return false;

        return true;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix) {
        for (int i = 0; i < matrix.m_Rows; ++i) {
            for (int j = 0; j < matrix.m_Columns; ++j)
                stream << matrix.m_Data[i][j] << " ";

            stream << std::endl;
        }

        return stream;
    }

} // namespace Ng