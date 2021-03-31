#pragma once

#include <list>

#include "BinomialTree.hpp"

namespace Ng {

    template <typename T>
    class BinomialHeap {
    public:
        using InnerType = T;

        BinomialHeap() = default;
        virtual ~BinomialHeap() noexcept = default;

        const T& GetPeak() const;
        void PopPeak() const;
        void Push(const T& value);

    private:
        std::list<BinomialTree<T>> m_Trees;

    }; // BinomialHeap

} // namespace Ng

#include "BinomialHeap.inl"