#pragma once

#include <iostream>
#include <vector>

namespace ng {

    template <typename T, std::size_t c = 2>
    class Heap {
    public:
        // constructor / destructor
        Heap();
        virtual ~Heap() = default;

        // accessors
        [[nodiscard]] const T& Peak() const;

        // public methods
        void Push(const T& value);
        void Increase(std::size_t index, const T& value);
        T PopPeak();

        // operators
        template <typename T_, std::size_t c_>
        friend std::ostream& operator<<(std::ostream& stream, const Heap<T_, c_>& heap);

    private:
        // fields
        std::size_t children;
        std::vector<T> data;

        // private accessors
        [[nodiscard]] std::size_t Child(std::size_t parent, std::size_t child) const;
        [[nodiscard]] std::size_t Parent(std::size_t child) const;

        // private methods
        void Up(std::size_t index);
        void Down(std::size_t index);

    }; // class BinaryHeap

    // constructor
    template <typename T, std::size_t c>
    Heap<T, c>::Heap() : children(c) {}

    // accessors
    template <typename T, std::size_t c>
    const T& Heap<T, c>::Peak() const {
        if (this->data.empty())
            throw std::out_of_range("Heap: index out of range");

        return this->data.front();
    }

    // public methods
    template <typename T, std::size_t c>
    void Heap<T, c>::Push(const T& value) {
        this->data.emplace_back(value);
        this->Up(this->data.size() - 1);
    }

    template <typename T, std::size_t c>
    void Heap<T, c>::Increase(std::size_t index, const T& value) {
        if (this->data.empty() || index > this->data.size() - 1)
            throw std::out_of_range("Heap: index out of range");

        this->data[index] += value;
        value > 0 ? this->Up(index) : this->Down(index);
    }

    template <typename T, std::size_t c>
    T Heap<T, c>::PopPeak() {
        int peak = this->Peak();
        std::swap(this->data.front(), this->data.back());
        this->data.pop_back();

        return peak;
    }

    // operators
    template <typename T, std::size_t c>
    std::ostream& operator<<(std::ostream& stream, const Heap<T, c>& heap) {
        stream << "[ ";
        for (const auto& e : heap.data)
            stream << e << " ";
        stream << "]";

        return stream;
    }

    // private accessors
    template <typename T, std::size_t c>
    std::size_t Heap<T, c>::Child(std::size_t parent, std::size_t child) const {
        return parent * this->children + 1 + child;
    }

    template <typename T, std::size_t c>
    std::size_t Heap<T, c>::Parent(std::size_t child) const { return (child - 1) / this->children; }

    // private methods
    template <typename T, std::size_t c>
    void Heap<T, c>::Up(std::size_t index) {
        while (index > 0 && this->data[Parent(index)] < this->data[index]) {
            std::swap(this->data[index], this->data[Parent(index)]);
            index = Parent(index);
        }
    }

    template <typename T, std::size_t c>
    void Heap<T, c>::Down(std::size_t index) {
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

} // class BinaryHeap