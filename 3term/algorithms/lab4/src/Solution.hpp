#pragma once

#include <iostream>
#include <vector>
#include <list>

#include "Random.hpp"

namespace Ng {

    template <typename T>
    class Solution {
    public:
        // inner structs
        struct Pair {
            // fields
            int16_t key;
            T value;

            // constructor
            Pair() : key(0), value(T()) {}
            Pair(int _key, int _value) : key(_key), value(_value) {}

            // operators
            bool operator<(const Pair& other) const { return this->key < other.key; };
            bool operator>(const Pair& other) const { return this->key > other.key; };
        };

        // accessors
        [[nodiscard]] const std::vector<Pair>& Data() const;

        // modifiers
        void Data(const std::vector<Pair>& data);

        // public methods
        void Rand(std::size_t count);
        void Shuffle();
        void Sort12();
        void Sort13();
        void Sort23();

        // operators
        template <typename T_>
        friend std::ostream& operator<<(std::ostream&, const Solution<T_>& solution);

    private:
        // member data
        std::vector<Pair> m_Data;

        // private methods
        void Clear();

    }; // class Solution

    // accessors
    template <typename T>
    const std::vector<typename Ng::Solution<T>::Pair>& Solution<T>::Data() const { return m_Data; }

    // modifiers
    template <typename T>
    void Solution<T>::Data(const std::vector<Pair>& data) { m_Data = data; }

    // public methods
    template <typename T>
    void Solution<T>::Rand(std::size_t count) {
        Clear();
        m_Data.resize(count);

        for (std::size_t i = 0; i < count; ++i)
            m_Data[i] = { Random::Irand(0, 1), Random::Irand(-10, 10) };
    }

    template <typename T>
    void Solution<T>::Shuffle() {
        for (std::size_t i = 0; i < m_Data.size(); ++i)
            std::swap(m_Data[i], m_Data[Ng::Random::Irand(0, static_cast<int>(i))]);
    }

    template <typename T>
    void Solution<T>::Sort12() {
        if (m_Data.empty())
            return;

        std::vector<std::list<int>> count(2);

        for (const auto& pair : m_Data)
            count[pair.key].emplace_back(pair.value);

        for (std::size_t key = 0, j = 0; key < count.size(); ++key)
            for (const auto& value : count[key])
                m_Data[j++] = Pair(key, value);
    }

    template <typename T>
    void Solution<T>::Sort13() {
        if (m_Data.empty())
            return;

        std::size_t left = 0;
        std::size_t right = m_Data.size() - 1;

        while (left < right) {
            bool next = false;

            if (m_Data[left].key == 0) {
                ++left;
                next = true;
            }

            if (m_Data[right].key == 1) {
                --right;
                next = true;
            }

            if (!next)
                std::swap(m_Data[left], m_Data[right]);
        }

    }

    template <typename T>
    void Solution<T>::Sort23() {
        for (std::size_t i = 0; i < m_Data.size(); ++i)
            for (std::size_t j = i; j > 0 && m_Data[j] < m_Data[j - 1]; --j)
                std::swap(m_Data[j], m_Data[j - 1]);
    }

    // operators
    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const Solution<T>& solution) {
        if (solution.m_Data.empty())
            return stream;

        stream << "[";

        for (bool first = true; const auto& pair : solution.m_Data) {
            if (!first)
                stream << ", ";
            else
                first = false;

            stream << "(" << pair.key << " " << pair.value << ")";
        }

        return stream << "]";
    }

    // private methods
    template <typename T>
    void Solution<T>::Clear() {
        m_Data.clear();
        m_Data.shrink_to_fit();
    }

} // namespace Ng