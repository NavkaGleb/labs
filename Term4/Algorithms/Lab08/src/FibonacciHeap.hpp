#pragma once

#include <vector>

#include "FibonacciNode.hpp"

namespace Ng {

    template <typename T>
    class FibonacciHeap {
    public:
        using InnerType = T;

    public:
        FibonacciHeap();
        FibonacciHeap(FibonacciHeap&& other) noexcept;
        ~FibonacciHeap() = default;

        [[nodiscard]] constexpr inline bool IsEmpty() const noexcept { return m_Count == 0; }
        [[nodiscard]] constexpr inline std::size_t GetCount() const noexcept { return m_Count; }

        [[nodiscard]] const T& GetPeak() const;

        void Clear();
        void Push(const T& value);

        void PopPeak();

        void Print() const;

        FibonacciHeap& operator =(FibonacciHeap&& other) noexcept;

    private:
        using Node = FibonacciNode<T>;

    private:
        static void Merge(FibonacciHeap& lhs, FibonacciHeap& rhs);

    private:
        void Push(Node* node);
        void Consolidate();

    private:
        Node*       m_Peak;
        std::size_t m_Count;

    }; // class FibonacciHeap

} // namespace Ng

#include "FibonacciHeap.inl"