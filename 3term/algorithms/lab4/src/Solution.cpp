#include "Lab4/Solution.hpp"

#include <list>
#include <functional>
#include <algorithm>

#include "Lab4/Random.hpp"

namespace Ng {

    /* --------------- class Solution::Pair --------------- */

    // operators
    bool Solution::Pair::operator<(const Pair& other) const { return this->key < other.key; }

    bool Solution::Pair::operator>(const Pair& other) const { return this->key > other.key; }

    /* --------------- class Solution --------------- */

    // accessors
    const std::vector<Ng::Solution::Pair>& Solution::Data() const { return this->data; }

    // modifiers
    void Solution::Data(const std::vector<Pair>& _data) { this->data = _data; }

    // public methods
    void Solution::Rand(std::size_t count) {
        this->Clear();
        this->data.resize(count);

        for (std::size_t i = 0; i < count; ++i)
            this->data[i] = { Random::Irand(0, 1), Random::Irand(-10, 10) };
    }


    void Solution::Shuffle() {
        for (std::size_t i = 0; i < this->data.size(); ++i)
            std::swap(this->data[i], this->data[Ng::Random::Irand(0, static_cast<int>(i))]);
    }

    void Solution::Sort12() {
        if (this->data.empty())
            return;

        std::vector<std::list<int>> count(2);

        for (const auto& pair : this->data)
            count[pair.key].emplace_back(pair.value);

        for (std::size_t key = 0, j = 0; key < count.size(); ++key)
            for (const auto& value : count[key])
                this->data[j++] = Pair(key, value);
    }

    void Solution::Sort13() {
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

    void Solution::Sort23() {
        for (std::size_t i = 0; i < this->data.size(); ++i)
            for (std::size_t j = i; j > 0 && this->data[j] < this->data[j - 1]; --j)
                std::swap(this->data[j], this->data[j - 1]);
    }

    // operators
    std::ostream& operator<<(std::ostream& stream, const Solution& solution) {
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
    void Solution::Clear() {
        this->data.clear();
        this->data.shrink_to_fit();
    }

} // namespace Ng