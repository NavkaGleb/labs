#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

namespace ch = std::chrono;

namespace ng {

    class Solution {
    public:
        // constructor / destructor
        Solution();
        virtual ~Solution() = default;
        // public methods
        void Solve();

    private:
        // fields
        std::vector<int> bolts;
        std::vector<int> nuts;

        // private methods
        void Init();
        void Shuffle();
        int Partition(int left, int right);
        void Sort(int left, int right);
        void Print();
    };

}