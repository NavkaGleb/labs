#pragma once

#include <list>

#include "BinomialTree.hpp"

namespace Ng {

    template <typename T, typename Comparator = std::less<>>
    class BinomialHeap {
    public:
        using InnerType = T;
        using Node      = typename BinomialTree<T>::Node;

        static void Merge(BinomialHeap& lhs, BinomialHeap& rhs);

        BinomialHeap();
        virtual ~BinomialHeap() noexcept = default;

        [[nodiscard]] inline bool IsEmpty() const { return m_Count == 0; }
        [[nodiscard]] inline int GetCount() const { return m_Count; }
        [[nodiscard]] const T& GetPeak() const;

        void Clear();

        void Push(const T& value);
        void Exchange(std::size_t index, const T& value);
        void PopPeak();
        void Pop(std::size_t index);

        void Print() const;

    private:
        using TreeContainer     = std::list<BinomialTree<T>>;
        using TreeIterator      = typename TreeContainer::iterator;
        using TreeConstIterator = typename TreeContainer::const_iterator;

        static void Merge(BinomialHeap& lhs, TreeContainer& rhs);

    private:
        [[nodiscard]] TreeIterator GetPeakIterator();
        [[nodiscard]] TreeConstIterator GetPeakIterator() const;
        [[nodiscard]] std::pair<TreeIterator, Node*> GetHandle(std::size_t index);

        void PopTree(TreeIterator it);
        void Union();
        void SwimToRoot(Node* node);
        void Increase(Node* node);
        void Decrease(Node* node);

    private:
        TreeContainer m_Trees;
        Comparator    m_Comparator;
        int           m_Count;

    }; // BinomialHeap

} // namespace Ng

#include "BinomialHeap.inl"