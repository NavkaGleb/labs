#pragma once

#include <iostream>
#include <vector>

namespace ng {

    class Solution {
    public:
        // constructor / destructor
        Solution();
        virtual ~Solution() = default;
        // public methods
        void Solve(bool print);

    private:
        // fields
        std::vector<int> bolts;
        std::vector<int> nuts;
        double time;

        // private methods
        void Init();
        void Shuffle();
        int Partition(int left, int right);
        void Sort(int left, int right);
        void Print(const char* title = nullptr) const;
        void PrintTime() const;

    }; // class Solution

} // namespace ng