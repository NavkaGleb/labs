#pragma once

#include "FibonacciNode.hpp"

namespace Ng {

    template <typename T, typename Comparator = std::less<>>
    class FibonacciHeap {
    public:
        using InnerType = T;

    public:
        static void Merge(FibonacciHeap& lhs, FibonacciHeap& rhs);

    public:
        FibonacciHeap();
        FibonacciHeap(const FibonacciHeap& other) = delete;
        FibonacciHeap(FibonacciHeap&& other) noexcept;
        ~FibonacciHeap() = default;

        [[nodiscard]] constexpr inline bool IsEmpty() const noexcept { return m_Count == 0; }
        [[nodiscard]] constexpr inline std::size_t GetCount() const noexcept { return m_Count; }

        [[nodiscard]] const T& GetPeak() const;

        void Clear();
        void Push(const T& value);
        void Exchange(const T& oldValue, const T& newValue);

        void PopPeak();
        void Pop(const T& value);

        void Print() const;

        FibonacciHeap& operator =(const FibonacciHeap& other) = delete;
        FibonacciHeap& operator =(FibonacciHeap&& other) noexcept;

    private:
        using Node = FibonacciNode<T>;

    private:
        Node* GetHandle(const T& value);

        void Push(Node* node);
        void Pop(Node* node);

        void Consolidate();
        void Cut(Node* node);
        void CascadingCut(Node* node);

    private:
        Node*       m_Peak;
        std::size_t m_Count;
        Comparator  m_Comparator;

    }; // class FibonacciHeap

} // namespace Ng

#include "FibonacciHeap.inl"