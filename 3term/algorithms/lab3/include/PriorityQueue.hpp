#pragma once

#include <vector>

namespace ng {

    class PriorityQueue {
    public:
        // constructor / destructor
        PriorityQueue();
        virtual ~PriorityQueue();

        // accessors
        const int& Top() const;

        // public methods
        void Push(int value);
        void Push(std::size_t index, int value);
        const int& Pop();

    private:
        // fields
        std::vector<int> data;

    }; // class PriorityQueue

} // namespace ng