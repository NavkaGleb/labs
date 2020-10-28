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
        void Data(const std::vector<Pair>& _data);

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
        std::vector<Pair> data;

        // private methods
        void Clear();

    }; // class Solution

    // accessors
    template <typename T>
    const std::vector<typename Ng::Solution<T>::Pair>& Solution<T>::Data() const { return this->data; }

    // modifiers
    template <typename T>
    void Solution<T>::Data(const std::vector<Pair>& _data) { this->data = _data; }

    // public methods
    template <typename T>
    void Solution<T>::Rand(std::size_t count) {
        this->Clear();
        this->data.resize(count);

        for (std::size_t i = 0; i < count; ++i)
            this->data[i] = { Random::Irand(0, 1), Random::Irand(-10, 10) };
    }

    template <typename T>
    void Solution<T>::Shuffle() {
        for (std::size_t i = 0; i < this->data.size(); ++i)
            std::swap(this->data[i], this->data[Ng::Random::Irand(0, static_cast<int>(i))]);
    }

    template <typename T>
    void Solution<T>::Sort12() {
        if (this->data.empty())
            return;

        std::vector<std::list<int>> count(2);

        for (const auto& pair : this->data)
            count[pair.key].emplace_back(pair.value);

        for (std::size_t key = 0, j = 0; key < count.size(); ++key)
            for (const auto& value : count[key])
                this->data[j++] = Pair(key, value);
    }

    template <typename T>
    void Solution<T>::Sort13() {
        if (this->data.empty())
            return;

        std::size_t left = 0;
        std::size_t right = this->data.size() - 1;

        while (left < right) {
            bool next = false;

            if (this->data[left].key == 0) {
                ++left;
                next = true;
            }

            if (this->data[right].key == 1) {
                --right;
                next = true;
            }

            if (!next)
                std::swap(this->data[left], this->data[right]);
        }

    }

    template <typename T>
    void Solution<T>::Sort23() {
        for (std::size_t i = 0; i < this->data.size(); ++i)
            for (std::size_t j = i; j > 0 && this->data[j] < this->data[j - 1]; --j)
                std::swap(this->data[j], this->data[j - 1]);
    }

    // operators
    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const Solution<T>& solution) {
        if (solution.data.empty())
            return stream;

        stream << "[";

        for (bool first = true; const auto& pair : solution.data) {
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
        this->data.clear();
        this->data.shrink_to_fit();
    }

} // namespace Ng