#include "SortLayerAlgorithm.hpp"

namespace Lab03 {

    SortLayerObserver::SortLayerObserver()
        : m_IsShutdown(false) {}

    void SortLayerObserver::OnNotify(const void* data) {
        m_IsShutdown = true;
    }

    void BubbleSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        for (auto [i, isSwapped] = std::make_pair(begin, false); i != end - 1; ++i) {
            isSwapped = false;

            for (auto j = begin; j != end - std::distance(begin, i) - 1; ++j) {
                if (*(j + 1) < *j) {
                    isSwapped = true;

                    swap(*(j + 1), *j);
                }
            }

            if (!isSwapped)
                break;
        }
    }

    void SelectionSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        for (auto i = begin, min = QuadIterator(); i != end - 1; ++i) {
            min = i;

            for (auto j = i; j < end; ++j)
                if (*j < *min)
                    min = j;

            swap(*i, *min);
        }
    }

    void InsertionSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        for (auto i = begin + 1; i != end; ++i)
            for (auto j = i; j != begin && *j < *(j - 1); --j)
                swap(*j, *(j - 1));
    }

    void ShellSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        std::iterator_traits<QuadIterator>::difference_type h = 1;

        while (h < (end - begin) / 3)
            h = 3 * h + 1;

        while (h >= 1) {
            for (auto i = begin + h; i != end; ++i)
                for (auto j = i; j >= begin + h && *j < *(j - h); j -= h)
                    swap(*j, *(j - h));

            h = h / 3;
        }
    }

    QuadIterator IQuickSortQuadsFunctor::Partition(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) {
        auto i = begin - 1;
        auto j = end;

        while (true) {
            while (*(++i) < *end)
                if (i == end)
                    break;

            while (*end < *(--j))
                if (j == begin)
                    break;

            if (j <= i)
                break;

            swap(*i, *j);
        }

        swap(*end, *i);

        return i;
    }

    void QuickSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return InsertionSortQuadsFunctor()(begin, end, swap);

        auto partition = Partition(begin, end - 1, swap);

        (*this)(begin,         partition, swap);
        (*this)(partition + 1, end,       swap);
    }

    void ParallelQuickSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return InsertionSortQuadsFunctor()(begin, end, swap);

        auto partition = Partition(begin, end - 1, swap);

        if (end - begin >= 20) {
            auto left = AsyncRun([&] { return (*this)(begin, partition, swap); });
            (*this)(partition + 1, end, swap);

            left.wait();
        } else {
            (*this)(begin,         partition, swap);
            (*this)(partition + 1, end,       swap);
        }
    }

    void IMergeSortQuadsFunctor::Merge(QuadIterator begin, QuadIterator middle, QuadIterator end, const SwapQuadFunction& swap) {
        std::vector<Quad> temp(std::distance(begin, end));

        auto i = begin;
        auto j = middle;

        for (auto& k : temp) {
            if ((*i < *j && i < middle) || j >= end)
                k = *(i++);
            else if ((*j < *i && j < end) || i >= middle)
                k = *(j++);
        }

        for (auto k = temp.begin(); k != temp.end(); ++k)
            swap(*(begin + std::distance(temp.begin(), k)), *k);
    }

    void MergeSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return InsertionSortQuadsFunctor()(begin, end, swap);

        auto middle = begin + std::distance(begin, end) / 2;

        (*this)(begin,  middle, swap);
        (*this)(middle, end,    swap);

        Merge(begin, middle, end, swap);
    }

    void BottomUpMergeSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        for (std::iterator_traits<QuadIterator>::difference_type size = 1; size < end - begin; size += size)
            for (auto left = begin; left < end - size; left += size * 2)
                Merge(left, left + size, std::min(left + size + size, end), swap);
    }

    void ParallelMergeSortQuadsFunctor::operator()(QuadIterator begin, QuadIterator end, const SwapQuadFunction& swap) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return InsertionSortQuadsFunctor()(begin, end, swap);

        auto middle = begin + std::distance(begin, end) / 2;

        if (end - begin >= 20) {
            auto left = AsyncRun([&] { return (*this)(begin, middle, swap); });
            (*this)(middle, end, swap);

            left.wait();
        } else {
            (*this)(begin,  middle, swap);
            (*this)(middle, end,    swap);
        }

        Merge(begin, middle, end, swap);
    }

} // namespace Lab03