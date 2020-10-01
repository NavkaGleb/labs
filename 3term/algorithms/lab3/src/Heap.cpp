#include "Heap.hpp"

#include <stdexcept>

namespace ng {

    // constructor / destructor
    Heap::Heap(std::size_t children) : children(children) {}

    Heap::~Heap() {}

    // accessors
    const int& Heap::Top() const {
        if (this->data.empty())
            throw std::out_of_range("PriorityQueue is empty");

        return this->data.front();
    }

    // public methods
    void Heap::Push(int value) {
        this->data.emplace_back(value);
        this->Up(this->data.size() - 1);
    }

    void Heap::Push(std::size_t index, int value) {

    }

    const int& Heap::PopTop() {
        return this->data.front();
    }

    // operators
    std::ostream& operator<<(std::ostream& stream, const Heap& heap) {
        stream << "[ ";
        for (const auto& e : heap.data)
            stream << e << " ";
        stream << "]";

        return stream;
    }

    // private accessors
    std::size_t Heap::Child(std::size_t parent, std::size_t child) const {
        return parent * children + 1 + child;
    }

    std::size_t Heap::Parent(std::size_t child) const {
        return (child - 1) / children;
    }

    // private methods
    void Heap::Up(std::size_t index) {
        while (index > 0 && this->data[Parent(index)] < this->data[index]) {
            std::swap(this->data[index], this->data[Parent(index)]);
            index = Parent(index);
        }
    }

    void Heap::Down(std::size_t index) {
        while (Child(index, 0) < this->data.size()) {
            int max_child = Child(index, 0);

            for (int i = 1; i < this->children; ++i)
                if (Child(index, i) < this->data.size() && this->data[Child(index, i)] > this->data[max_child])
                    max_child = Child(index, i);

            if (this->data[index] < this->data[max_child]) {
                std::swap(this->data[index], this->data[max_child]);
                index = max_child;
            } else {
                return;
            }
        }
    }

} // namespace ng