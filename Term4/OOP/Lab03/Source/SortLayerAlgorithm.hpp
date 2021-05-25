#pragma once

#include <glm/glm.hpp>

#include "Algorithm.hpp"
#include "Observer.hpp"
#include "SortableQuad.hpp"

namespace Lab03 {

    using SwapQuadFunction = std::function<void(Quad&, Quad&)>;

    class SortLayerObserver : public IObserver {
    public:
        SortLayerObserver();
        ~SortLayerObserver() override = default;

    public:
        void OnNotify(const void* data) override;

    public:
        virtual void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const = 0;

    protected:
        bool m_IsShutdown;

    }; // class SortLayerObserver

    class BubbleSortQuadsFunctor : public SortLayerObserver {
    public:
        BubbleSortQuadsFunctor() = default;
        ~BubbleSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class BubbleSortQuadsFunctor

    class SelectionSortQuadsFunctor : public SortLayerObserver {
    public:
        SelectionSortQuadsFunctor() = default;
        ~SelectionSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class SelectionSortQuadsFunctor

    class InsertionSortQuadsFunctor : public SortLayerObserver {
    public:
        InsertionSortQuadsFunctor() = default;
        ~InsertionSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class InsertionSortQuadsFunctor

    class ShellSortQuadsFunctor : public SortLayerObserver {
    public:
        ShellSortQuadsFunctor() = default;
        ~ShellSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class ShellSortQuadsFunctor

    class IQuickSortQuadsFunctor : public SortLayerObserver {
    public:
        IQuickSortQuadsFunctor() = default;
        ~IQuickSortQuadsFunctor() override = default;

    protected:
        [[nodiscard]] static QuadIterator Partition(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap);

    }; // IQuickSortQuads

    class QuickSortQuadsFunctor : public IQuickSortQuadsFunctor {
    public:
        QuickSortQuadsFunctor() = default;
        ~QuickSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class ShellSortQuadsFunctor

    class ParallelQuickSortQuadsFunctor : public IQuickSortQuadsFunctor {
    public:
        ParallelQuickSortQuadsFunctor() = default;
        ~ParallelQuickSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class ShellSortQuadsFunctor

    class IMergeSortQuadsFunctor : public SortLayerObserver {
    public:
        IMergeSortQuadsFunctor() = default;
        ~IMergeSortQuadsFunctor() override = default;

    protected:
        static void Merge(QuadIterator begin, QuadIterator middle, QuadIterator end, const SwapQuadFunction& swap);

    }; // IQuickSortQuads

    class MergeSortQuadsFunctor : public IMergeSortQuadsFunctor {
    public:
        MergeSortQuadsFunctor() = default;
        ~MergeSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class ShellSortQuadsFunctor

    class BottomUpMergeSortQuadsFunctor : public IMergeSortQuadsFunctor {
    public:
        BottomUpMergeSortQuadsFunctor() = default;
        ~BottomUpMergeSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class ShellSortQuadsFunctor

    class ParallelMergeSortQuadsFunctor : public IMergeSortQuadsFunctor {
    public:
        ParallelMergeSortQuadsFunctor() = default;
        ~ParallelMergeSortQuadsFunctor() override = default;

    public:
        void operator ()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const override;

    }; // class ShellSortQuadsFunctor

} // namespace Lab03