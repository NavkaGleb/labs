#include "PriorityQueue.hpp"

#include <stdexcept>

namespace ng {

    // constructor / destructor
    PriorityQueue::PriorityQueue() {

    }

    PriorityQueue::~PriorityQueue() {

    }

    // accessors
    const int& PriorityQueue::Top() const {
        if (this->data.empty())
            throw std::out_of_range("PriorityQueue is empty");

        return this->data.front();
    }

    // public methods
    void PriorityQueue::Push(int value) {

    }

} // namespace ng