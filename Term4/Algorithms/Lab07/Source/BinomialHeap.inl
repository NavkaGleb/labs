#include <stdexcept>
#include <algorithm>

namespace Ng {

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Merge(BinomialHeap& lhs, BinomialHeap& rhs) {
        lhs.m_Trees.merge(rhs.m_Trees, [](auto& lhs, auto& rhs) {
            return lhs.GetRoot()->GetDegree() < rhs.GetRoot()->GetDegree();
        });

        rhs.Clear();
    }

    template <typename T, typename Comparator>
    BinomialHeap<T, Comparator>::BinomialHeap()
        : m_Comparator(Comparator())
        , m_Count(0) {}

    template <typename T, typename Comparator>
    const T& BinomialHeap<T, Comparator>::GetPeak() const {
        return GetPeakIterator()->GetRoot()->GetValue();
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Clear() {
        m_Trees.clear();
        m_Count = 0;
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Push(const T& value) {
        m_Trees.push_front(BinomialTree<T>(new typename BinomialTree<T>::Node(value)));
        ++m_Count;

        Union();
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Exchange(std::size_t index, const T& value) {
        auto* node = GetHandle(index).second;
        auto  oldValue = node->m_Value;

        node->m_Value = value;

        if (m_Comparator(oldValue, value))
            Increase(node);
        else
            Decrease(node);
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::PopPeak() {
        return PopTree(GetPeakIterator());
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Pop(std::size_t index) {
        auto [tree, node] = GetHandle(index);

        SwimToRoot(node);
        PopTree(tree);
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Print() const {
        for (const auto& tree : m_Trees)
            tree.Print();
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Merge(BinomialHeap& lhs, TreeContainer& rhs) {
        lhs.m_Trees.merge(rhs, [](auto& lhs, auto& rhs) {
            return lhs.GetRoot()->GetDegree() < rhs.GetRoot()->GetDegree();
        });
    }

    template <typename T, typename Comparator>
    typename BinomialHeap<T, Comparator>::TreeIterator BinomialHeap<T, Comparator>::GetPeakIterator() {
        if (m_Trees.empty())
            throw std::out_of_range("Ng::BinomialHeap::GetMinTree: Heap is empty!");

        return std::min_element(m_Trees.begin(), m_Trees.end(), [this](const auto& lhs, const auto& rhs) {
            return m_Comparator(lhs.GetRoot()->GetValue(), rhs.GetRoot()->GetValue());
        });
    }

    template <typename T, typename Comparator>
    typename BinomialHeap<T, Comparator>::TreeConstIterator BinomialHeap<T, Comparator>::GetPeakIterator() const {
        if (m_Trees.empty())
            throw std::out_of_range("Ng::BinomialHeap::GetMinTree: Heap is empty!");

        return std::min_element(m_Trees.cbegin(), m_Trees.cend(), [this](const auto& lhs, const auto& rhs) {
            return m_Comparator(lhs.GetRoot()->GetValue(), rhs.GetRoot()->GetValue());
        });
    }

    template <typename T, typename Comparator>
    std::pair<
        typename BinomialHeap<T, Comparator>::TreeIterator,
        typename BinomialHeap<T, Comparator>::Node*
    > BinomialHeap<T, Comparator>::GetHandle(std::size_t index) {
        std::size_t position = 0;

        for (auto it = m_Trees.begin(); it != m_Trees.end(); ++it) {
            if (position + it->GetCount() <= index)
                position += it->GetCount();
            else
                return { it, it->GetRoot()->Get(index - position) };
        }

        return { m_Trees.end(), nullptr };
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::PopTree(TreeIterator it) {
        TreeContainer tempTrees;

        auto peakChildren = it->GetRoot()->GiveAwayChildren();

        for (auto& child : peakChildren)
            tempTrees.emplace_front(child);

        m_Trees.erase(it);
        --m_Count;

        Merge(*this, tempTrees);
        Union();
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Union() {
        auto prev    = m_Trees.begin();
        auto current = std::next(prev);
        auto next    = std::next(current);

        auto IsExists = [&](const auto& it) { return it != m_Trees.end(); };

        auto Case1 = [&] {
            return prev->GetDegree() != current->GetDegree();
        };

        auto Case2 = [&] {
            return current->GetDegree() && IsExists(next) && current->GetDegree() == next->GetDegree();
        };

        auto Case3 = [&] {
            return prev->GetDegree() == current->GetDegree()                                &&
                   (!IsExists(next) || current->GetDegree() != next->GetDegree())           &&
                   m_Comparator(prev->GetRoot()->GetValue(), current->GetRoot()->GetValue());
        };

        auto Case4 = [&] {
            return prev->GetDegree() == current->GetDegree()                                 &&
                   (!IsExists(next) || current->GetDegree() != next->GetDegree())            &&
                   !m_Comparator(prev->GetRoot()->GetValue(), current->GetRoot()->GetValue());
        };

        while (current != m_Trees.end()) {
            if (Case1() || Case2()) {
                prev = current;
                current = next;
            } else if (Case3()) {
                BinomialTree<T>::Link(*prev, *current);
                m_Trees.erase(current);

                current = next;
            } else if (Case4()) {
                BinomialTree<T>::Link(*current, *prev);
                m_Trees.erase(prev);

                prev    = current;
                current = next;
            }

            next = std::next(next);
        }
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::SwimToRoot(Node* node) {
        while (node && node->m_Parent) {
            std::swap(node->m_Value, node->m_Parent->m_Value);
            node = node->m_Parent;
        }
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Increase(Node* node) {
        while (node && !node->m_Children.empty()) {
            auto minNode = *std::min_element(
                node->m_Children.begin(),
                node->m_Children.end(),
                [this](const auto& lhs, const auto& rhs) {
                    return m_Comparator(lhs->GetValue(), rhs->GetValue());
                }
            );

            std::swap(node->m_Value, minNode->m_Value);
            node = minNode;
        }
    }

    template <typename T, typename Comparator>
    void BinomialHeap<T, Comparator>::Decrease(Node* node) {
        while (node && node->m_Parent) {
            if (node->m_Value > node->m_Parent->m_Value)
                break;

            std::swap(node->m_Value, node->m_Parent->m_Value);
            node = node->m_Parent;
        }
    }

} // namespace Ng