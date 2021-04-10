#include <utility>
#include <cmath>
#include <charconv>

namespace Ng {

    template <typename T>
    FibonacciHeap<T>::FibonacciHeap()
        : m_Peak(nullptr)
        , m_Count(0) {}

    template <typename T>
    FibonacciHeap<T>::FibonacciHeap(FibonacciHeap&& other) noexcept
        : m_Peak(std::exchange(other.m_Peak, nullptr))
        , m_Count(std::exchange(other.m_Count, 0)) {}

    template <typename T>
    const T& FibonacciHeap<T>::GetPeak() const {
        if (!m_Peak)
            throw std::out_of_range("Ng::FibonacciHeap::GetPeak: Heap is emtpy!");

        return m_Peak->m_Value;
    }

    template <typename T>
    void FibonacciHeap<T>::Clear() {
        delete m_Peak;

        m_Peak  = nullptr;
        m_Count = 0;
    }

    template <typename T>
    void FibonacciHeap<T>::Push(const T& value) {
        auto* node = new Node(value);

        ++m_Count;

        Push(node);

        if (node->m_Value < m_Peak->m_Value)
            m_Peak = node;
    }

    template <typename T>
    void FibonacciHeap<T>::PopPeak() {
        if (!m_Peak)
            return;

        --m_Count;

        if (m_Peak->m_Child) {
            auto* child = m_Peak->m_Child;

            do {
                auto* next = child->m_RightSibling;
                Push(child);

                child = next;
            } while (child != m_Peak->m_Child);
        }

        if (m_Peak->m_RightSibling == m_Peak) {
            m_Peak = nullptr;
        } else {
            auto* newPeak = m_Peak->m_RightSibling;
            m_Peak->StayAlone();

            m_Peak = newPeak;

            Consolidate();
        }

    }

    template <typename T>
    void FibonacciHeap<T>::Print() const {
        if (!m_Peak) {
            std::cout << "Null!" << std::endl;
            return;
        }

        auto* node = m_Peak;

        while (true) {
            node->Print(1);

            node = node->m_RightSibling;

            if (node == m_Peak)
                return;
        }
    }

    template <typename T>
    FibonacciHeap<T>& FibonacciHeap<T>::operator =(FibonacciHeap&& other) noexcept {
        m_Peak = std::exchange(other.m_Peak, nullptr);
        m_Count = std::exchange(other.m_Count, 0);
    }

    template <typename T>
    void FibonacciHeap<T>::Merge(FibonacciHeap<T>& lhs, FibonacciHeap<T>& rhs) {
        if (!rhs.m_Peak)
            return;

        if (!lhs.m_Peak && rhs.m_Peak) {
            lhs = std::move(rhs);
            return;
        }

        if (lhs.m_Peak->m_Value > rhs.m_Peak->m_Value)
            std::swap(lhs, rhs);

        auto* rightSiblingFirst = lhs.m_Peak->m_RightSibling;
        auto* leftSiblingSecond = rhs.m_Peak->m_LeftSibling;

        lhs.m_Peak->m_RightSibling        = rhs.m_Peak;
        rhs.m_Peak->m_LeftSibling         = lhs.m_Peak;
        rightSiblingFirst->m_LeftSibling  = leftSiblingSecond;
        leftSiblingSecond->m_RightSibling = rightSiblingFirst;

        lhs.m_Count += rhs.m_Peak;
    }

    template <typename T>
    void FibonacciHeap<T>::Push(Node* node) {
        node->m_Parent = nullptr;

        if (!m_Peak)
            m_Peak = node;
        else
            m_Peak->PushSibling(node);
    }

    template <typename T>
    void FibonacciHeap<T>::Consolidate() {
        std::vector<Node*> nodes(static_cast<std::size_t>(std::log2(m_Count)) + 1, nullptr);

        for (Node* node = m_Peak; true;) {
            std::size_t degree = node->GetDegree();

            while (nodes[degree] && nodes[degree] != node) {
                if (node->m_Value > nodes[degree]->m_Value)
                    std::swap(node, nodes[degree]);

                node->PushChild(nodes[degree]);

                nodes[degree++] = nullptr;
            }

            nodes[degree] = node;

            while (m_Peak->HasParent())
                m_Peak = m_Peak->m_Parent;

            node = node->m_RightSibling;

            if (node == m_Peak)
                break;
        }

        m_Peak = *std::min_element(
            nodes.begin(),
            nodes.end(),
            [](const auto& lhs, const auto& rhs) {
                return lhs && (!rhs || lhs->m_Value < rhs->m_Value);
            }
        );
    }

} // namespace Ng