#pragma once

#include <iostream>
#include <vector>

namespace ng {

//    template <std::size_t c = 2>
    class Heap {
    public:
        // constructor / destructor
        Heap(std::size_t children);
        virtual ~Heap();

        // accessors
        [[nodiscard]] const int& Top() const;

        // public methods
        void Push(int value);
        void Push(std::size_t index, int value);
        const int& PopTop();

        // operators
        friend std::ostream& operator<<(std::ostream& stream, const Heap& heap);

    private:
        // fields
        std::size_t children;
        std::vector<int> data;

        // private accessors
        [[nodiscard]] std::size_t Child(std::size_t parent, std::size_t child) const;
        [[nodiscard]] std::size_t Parent(std::size_t child) const;

        // private methods

        void Up(std::size_t index);
        void Down(std::size_t index);

    }; // class BinaryHeap

} // class BinaryHeap