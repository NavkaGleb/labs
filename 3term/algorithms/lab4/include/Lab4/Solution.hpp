#pragma once

#include <iostream>
#include <vector>

namespace Ng {

    class Solution {
    public:
        // inner structs
        struct Pair {
            // fields
            int key;
            int value;

            // constructor
            Pair() : key(0), value(0) {}
            Pair(int _key, int _value) : key(_key), value(_value) {}

            // operators
            bool operator<(const Pair& other) const;
            bool operator>(const Pair& other) const;
        };

        // accessors
        const std::vector<Pair>& Data() const;

        // modifiers
        void Data(const std::vector<Pair>& _data);

        // public methods
        void Rand(std::size_t count);
        void Shuffle();
        void Sort12();
        void Sort13();
        void Sort23();

        // operators
        friend std::ostream& operator<<(std::ostream&, const Solution& solution);

    private:
        // fields
        std::vector<Pair> data;

        // private methods
        void Clear();

    }; // class Solution

} // namespace Ng