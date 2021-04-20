#pragma once

#include <functional>

namespace Ng {

    template <typename T, typename Comparator>
    class FibonacciHeap;

    template <typename T>
    class FibonacciNode {
    public:
        template <class T_, typename Comparator>
        friend class FibonacciHeap;

    public:
        explicit FibonacciNode(const T& value);
        ~FibonacciNode();

        [[nodiscard]] inline const T& GetValue() const { return m_Value; }
        [[nodiscard]] inline FibonacciNode* GetParent() const { return m_Parent; };
        [[nodiscard]] inline FibonacciNode* GetLeftSibling() const { return m_LeftSibling; }
        [[nodiscard]] inline FibonacciNode* GetRightSibling() const { return m_RightSibling; }
        [[nodiscard]] inline FibonacciNode* GetChild() const { return m_Child; }
        [[nodiscard]] inline std::size_t GetDegree() const { return m_Degree; }

        [[nodiscard]] inline bool HasParent() const { return m_Parent; }
        [[nodiscard]] inline bool HasChild() const { return m_Child; }
        [[nodiscard]] inline bool IsAlone() const { return m_LeftSibling == this; }

        inline void SetValue(const T& value) { m_Value = value; }
        inline void SetParent(FibonacciNode* parent) { m_Parent = parent; }
        inline void SetLeftSibling(FibonacciNode* leftSibling) { m_LeftSibling = leftSibling; }
        inline void SetRightSibling(FibonacciNode* rightSibling) { m_RightSibling = rightSibling; }
        inline void SetChild(FibonacciNode* child) { m_Child = child; }
        inline void IsMarked(bool isMarked) { m_IsMarked = isMarked; }

        FibonacciNode* GetHandle(const T& value);

        void StayAlone();
        void PushSibling(FibonacciNode* sibling);
        void PushChild(FibonacciNode* child);
        void PopChild(FibonacciNode* child);

        void Print(const std::string& indent = std::string()) const;

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