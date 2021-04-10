#pragma once

namespace Ng {

    template <typename T>
    class FibonacciHeap;

    template <typename T>
    class FibonacciNode {
    public:
        friend FibonacciHeap<T>;

    public:
        explicit FibonacciNode(const T& value);
        ~FibonacciNode();

        [[nodiscard]] inline const T& GetValue() const { return m_Value; }
        [[nodiscard]] inline const FibonacciNode* GetParent() const { return m_Parent; };
        [[nodiscard]] inline std::size_t GetDegree() const { return m_Degree; }

        [[nodiscard]] inline bool HasParent() const { return m_Parent; }
        [[nodiscard]] inline bool HasChild() const { return m_Child; }
        [[nodiscard]] inline bool IsAlone() const { return m_LeftSibling == this; }

        void StayAlone();
        void PushSibling(FibonacciNode* sibling);
        void PushChild(FibonacciNode* child);

        void Print(int level = 0) const;

    private:
        T              m_Value;
        FibonacciNode* m_Parent;
        FibonacciNode* m_LeftSibling;
        FibonacciNode* m_RightSibling;
        FibonacciNode* m_Child;
        std::size_t    m_Degree;
        bool           m_IsMarked;

    };

} // namespace Ng

#include "FibonacciNode.inl"