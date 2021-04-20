#include <utility>
#include <cmath>
#include <charconv>

namespace Ng {

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Merge(FibonacciHeap& lhs, FibonacciHeap& rhs) {
        if (!rhs.m_Peak)
            return;

        if (!lhs.m_Peak && rhs.m_Peak) {
            lhs = std::move(rhs);
            return;
        }

        if (Comparator()(rhs.m_Peak->GetValue(), lhs.m_Peak->GetValue()))
            std::swap(lhs, rhs);

        auto* rightSiblingFirst = lhs.m_Peak->GetRightSibling();
        auto* leftSiblingSecond = rhs.m_Peak->GetLeftSibling();

        rhs.m_Peak->SetLeftSibling(lhs.m_Peak);
        lhs.m_Peak->SetRightSibling(rhs.m_Peak);
        rightSiblingFirst->SetLeftSibling(leftSiblingSecond);
        leftSiblingSecond->SetRightSibling(rightSiblingFirst);

        lhs.m_Count += rhs.m_Peak;
    }

    template <typename T, typename Comparator>
    FibonacciHeap<T, Comparator>::FibonacciHeap()
        : m_Peak(nullptr)
        , m_Count(0)
        , m_Comparator(Comparator()){}

    template <typename T, typename Comparator>
    FibonacciHeap<T, Comparator>::FibonacciHeap(FibonacciHeap&& other) noexcept
        : m_Peak(std::exchange(other.m_Peak, nullptr))
        , m_Count(std::exchange(other.m_Count, 0)) {}

    template <typename T, typename Comparator>
    const T& FibonacciHeap<T, Comparator>::GetPeak() const {
        if (!m_Peak)
            throw std::out_of_range("Ng::FibonacciHeap::GetPeak: Heap is emtpy!");

        return m_Peak->GetValue();
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Clear() {
        delete m_Peak;

        m_Peak  = nullptr;
        m_Count = 0;
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Push(const T& value) {
        auto* node = new Node(value);

        ++m_Count;

        Push(node);

        if (m_Comparator(node->GetValue(), m_Peak->GetValue()))
            m_Peak = node;
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Exchange(const T& oldValue, const T& newValue) {
        if (m_Comparator(oldValue, newValue))
            return;

        auto* handle = GetHandle(oldValue);

        if (!handle)
            return;

        auto* parent = handle->GetParent();

        handle->SetValue(newValue);

        if (parent && m_Comparator(handle->GetValue(), parent->GetValue())) {
            Cut(handle);
            CascadingCut(parent);
        }

        if (m_Comparator(handle->GetValue(), m_Peak->GetValue()))
            m_Peak = handle;
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::PopPeak() {
        Pop(m_Peak);
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Pop(const T& value) {
        auto* handle = GetHandle(value);

        if (!handle)
            return;

        if (handle->HasParent()) {
            auto* parent = handle->GetParent();

            Cut(handle);
            CascadingCut(parent);
        }

        Pop(handle);
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Print() const {
        if (!m_Peak)
            return;

        m_Peak->Print();
    }

    template <typename T, typename Comparator>
    FibonacciHeap<T, Comparator>& FibonacciHeap<T, Comparator>::operator =(FibonacciHeap&& other) noexcept {
        m_Peak  = std::exchange(other.m_Peak, nullptr);
        m_Count = std::exchange(other.m_Count, 0);
    }

    template <typename T, typename Comparator>
    typename FibonacciHeap<T, Comparator>::Node* FibonacciHeap<T, Comparator>::GetHandle(const T& value) {
        return m_Peak->GetHandle(value);
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Push(Node* node) {
        node->SetParent(nullptr);
        node->IsMarked(false);

        if (!m_Peak)
            m_Peak = node;
        else
            m_Peak->PushSibling(node);
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Pop(Node* node) {
        if (!node)
            return;

        --m_Count;

        if (node->HasChild()) {
            auto* child = node->GetChild();

            do {
                auto* next = child->GetRightSibling();

                Push(child);
                child = next;
            } while (child != node->GetChild());

            node->SetChild(nullptr);
        }

        if (node->IsAlone()) {
            Clear();
        } else {
            m_Peak = node->GetRightSibling();

            node->StayAlone();
            delete node;

            Consolidate();
        }
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Consolidate() {
        std::vector<Node*> nodes(static_cast<std::size_t>(std::log2(m_Count)) + 2, nullptr);

        for (Node* node = m_Peak; true;) {
            std::size_t degree = node->GetDegree();

            while (nodes[degree] && nodes[degree] != node) {
                if (m_Comparator(nodes[degree]->GetValue(), node->GetValue()))
                    std::swap(nodes[degree], node);

                node->PushChild(nodes[degree]);

                nodes[degree++] = nullptr;
            }

            nodes[degree] = node;

            while (m_Peak->HasParent())
                m_Peak = m_Peak->GetParent();

            node = node->GetRightSibling();

            if (node == m_Peak)
                break;
        }

        m_Peak = *std::min_element(
            nodes.begin(),
            nodes.end(),
            [this](const auto& lhs, const auto& rhs) {
                return lhs && (!rhs || m_Comparator(lhs->GetValue(), rhs->GetValue()));
            }
        );
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::Cut(Node* node) {
        node->GetParent()->PopChild(node);
        Push(node);
    }

    template <typename T, typename Comparator>
    void FibonacciHeap<T, Comparator>::CascadingCut(Node* node) {
        if (node->HasParent()) {
            if (!node->m_IsMarked)
                return node->IsMarked(true);

            auto* parent = node->GetParent();

            Cut(node);
            CascadingCut(parent);
        }
    }

} // namespace Ng