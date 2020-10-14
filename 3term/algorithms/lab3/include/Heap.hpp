#pragma once

#include <iostream>
#include <vector>

namespace Ng {

    template <typename T, std::size_t c = 2, typename C = std::greater<T>>
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
        template <typename T_, std::size_t ch_, typename C_>
        friend std::ostream& operator<<(std::ostream& stream, const Heap<T_, ch_, C_>& heap);

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
    template <typename T, std::size_t ch, typename C>
    Heap<T, ch, C>::Heap() : children(ch) {}

    // accessors
    template <typename T, std::size_t ch, typename C>
    const T& Heap<T, ch, C>::Peak() const {
        if (this->data.empty())
            throw std::out_of_range("Heap: index out of range");

        return this->data.front();
    }

    // public methods
    template <typename T, std::size_t ch, typename C>
    void Heap<T, ch, C>::Push(const T& value) {
        this->data.emplace_back(value);
        this->Up(this->data.size() - 1);
    }

    template <typename T, std::size_t ch, typename C>
    void Heap<T, ch, C>::Increase(std::size_t index, const T& value) {
        if (this->data.empty() || index > this->data.size() - 1)
            throw std::out_of_range("Heap: index out of range");

        this->data[index] += value;
        C()(value, 0) ? this->Up(index) : this->Down(index);
    }

    template <typename T, std::size_t ch, typename C>
    T Heap<T, ch, C>::PopPeak() {
        int peak = this->Peak();

        std::swap(this->data.front(), this->data.back());
        this->data.pop_back();
        this->Down(0);

        return peak;
    }

    // operators
    template <typename T, std::size_t ch, typename C>
    std::ostream& operator<<(std::ostream& stream, const Heap<T, ch, C>& heap) {
        bool first = true;

        stream << "[";
        for (const auto& e : heap.data) {
            if (!first)
                stream << ", ";
            else
                first = false;

            stream << e;
        }

        return stream << "]";
    }

    // private accessors
    template <typename T, std::size_t ch, typename C>
    std::size_t Heap<T, ch, C>::Child(std::size_t parent, std::size_t child) const {
        return parent * this->children + 1 + child;
    }

    template <typename T, std::size_t ch, typename C>
    std::size_t Heap<T, ch, C>::Parent(std::size_t child) const { return (child - 1) / this->children; }

    // private methods
    template <typename T, std::size_t ch, typename C>
    void Heap<T, ch, C>::Up(std::size_t index) {
        while (index > 0 && C()(this->data[index], this->data[this->Parent(index)])) {
            std::swap(this->data[index], this->data[this->Parent(index)]);
            index = this->Parent(index);
        }
    }

    template <typename T, std::size_t ch, typename C>
    void Heap<T, ch, C>::Down(std::size_t index) {
        while (this->Child(index, 0) < this->data.size()) {
            int max_child = this->Child(index, 0);

            for (int i = 1; i < this->children; ++i)
                if (this->Child(index, i) < this->data.size() &&
                    C()(this->data[this->Child(index, i)], this->data[max_child]))
                    max_child = this->Child(index, i);

            if (C()(this->data[max_child], this->data[index])) {
                std::swap(this->data[index], this->data[max_child]);
                index = max_child;
            } else {
                return;
            }
        }
    }

} // namespace Ng