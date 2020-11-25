#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <algorithm>
#include <initializer_list>

namespace Ng {

    ////////////////////////////////////////////////////////////////////////////
    /// HEADER
    ////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class Matrix {
    public:
        // alias
        using ValueType = T;

        // constructor / destructor
        explicit Matrix(std::size_t rows = 0, std::size_t columns = 0, const ValueType& value = ValueType());
        Matrix(std::initializer_list<std::initializer_list<ValueType>> initList);
        virtual ~Matrix() = default;

        // accessors
        [[nodiscard]] inline bool Empty() const { return m_Data.emtpy(); }
        [[nodiscard]] inline std::size_t Rows() const { return m_Rows; }
        [[nodiscard]] inline std::size_t Columns() const { return m_Columns; }
        [[nodiscard]] inline std::vector<ValueType>& operator[](std::size_t index) { return m_Data[index]; }
        [[nodiscard]] inline const T& At(std::size_t i, std::size_t j) const { return m_Data.at(i).at(j); }

        // modifiers
        void Rows(std::size_t rows);
        void Columns(std::size_t columns);

        // public methods
        void Clear();
        void Clean();
        void PopRow(std::size_t index);
        void PopLastRow();
        void PopColumn(std::size_t index);
        void PopLastColumn();
        std::size_t RowCount(std::size_t row, const ValueType& value) const;
        std::size_t ColumnCount(std::size_t column, const ValueType& value) const;

        // operators
        Matrix<ValueType> operator+(const Matrix<ValueType>& other) const;
        Matrix<ValueType> operator-(const Matrix<ValueType>& other) const;
        Matrix<ValueType> operator*(const Matrix<ValueType>& other) const;
        bool operator==(const Matrix<ValueType>& other) const;
        template <typename T_> friend std::ostream& operator<<(std::ostream& stream, const Matrix<T_>& matrix);

    private:
        // member data
        std::vector<std::vector<ValueType>> m_Data;
        std::size_t m_Rows;
        std::size_t m_Columns;

    }; // class Matrix

    ////////////////////////////////////////////////////////////////////////////
    /// SOURCE
    ////////////////////////////////////////////////////////////////////////////

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

    // modifiers
    template <typename T>
    void Matrix<T>::Rows(std::size_t rows) {
        m_Rows = rows;
        m_Data.resize(m_Rows);
    }

    template <typename T>
    void Matrix<T>::Columns(std::size_t columns) {
        m_Columns = columns;

        for (auto& row : m_Data)
            row.resize(m_Columns, T());
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

    template <typename T>
    void Matrix<T>::Clean() {
        for (size_t i = m_Rows; i > 0; --i) {
            if (RowCount(i - 1, T()) == m_Columns)
                PopLastRow();
            else
                break;
        }

        for (size_t i = m_Columns; i > 0; --i) {
            if (ColumnCount(i - 1, T()) == m_Rows)
                PopLastColumn();
            else
                break;
        }
    }

    template <typename T>
    void Matrix<T>::PopRow(std::size_t index) {
        m_Data.erase(m_Data.begin() + index);
        --m_Rows;
    }

    template <typename T>
    void Matrix<T>::PopLastRow() { PopRow(m_Rows - 1); }

    template <typename T>
    void Matrix<T>::PopColumn(std::size_t index) {
        for (size_t i = 0; i < m_Rows; ++i)
            m_Data[i].erase(m_Data[i].begin() + index);

        --m_Columns;
    }

    template <typename T>
    void Matrix<T>::PopLastColumn() { PopColumn(m_Columns - 1); }

    template <typename T>
    std::size_t Matrix<T>::RowCount(std::size_t row, const T& value) const {
        return std::count(m_Data[row].begin(), m_Data[row].end(), value);
    }

    template <typename T>
    std::size_t Matrix<T>::ColumnCount(std::size_t column, const T& value) const {
        size_t count = 0;

        for (size_t i = 0; i < m_Rows; ++i)
            if (m_Data[i][column] == value)
                ++count;

        return count;
    }

    // operators
    template <typename T>
    Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
        if (m_Rows != other.m_Rows || m_Columns != other.m_Columns)
            throw std::invalid_argument("Matrix::operator+: Matrices have different size");

        Matrix<T> result(m_Rows, m_Columns);

        for (std::size_t i = 0; i < m_Rows; ++i)
            for (std::size_t j = 0; j < m_Columns; ++j)
                result.m_Data[i][j] = m_Data[i][j] + other.m_Data[i][j];

        return result;
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
        if (m_Rows != other.m_Rows || m_Columns != other.m_Columns)
            throw std::invalid_argument("Matrix::operator-: Matrices have different size");

        Matrix<T> result(m_Rows, m_Columns);

        for (std::size_t i = 0; i < m_Rows; ++i)
            for (std::size_t j = 0; j < m_Columns; ++j)
                result.m_Data[i][j] = m_Data[i][j] - other.m_Data[i][j];

        return result;
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
        if (m_Columns != other.m_Rows)
            throw std::invalid_argument("Matrix::operator*: Matrices have different size");

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

            if (i != matrix.m_Rows - 1)
                stream << std::endl;
        }

        return stream;
    }

} // namespace Ng