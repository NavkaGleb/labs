#include <utility>

namespace Ng {

    template <typename T>
    FibonacciNode<T>::FibonacciNode(const T& value)
        : m_Value(value)
        , m_Parent(nullptr)
        , m_LeftSibling(this)
        , m_RightSibling(this)
        , m_Child(nullptr)
        , m_Degree(0)
        , m_IsMarked(false) {}

    template <typename T>
    FibonacciNode<T>::~FibonacciNode() {

    }

    template <typename T>
    void FibonacciNode<T>::StayAlone() {
        m_LeftSibling->m_RightSibling = m_RightSibling;
        m_RightSibling->m_LeftSibling = m_LeftSibling;

        m_LeftSibling                 = this;
        m_RightSibling                = this;
    }

    template <typename T>
    void FibonacciNode<T>::PushSibling(FibonacciNode* sibling) {
        if (IsAlone()) {
            m_LeftSibling = sibling;
            m_RightSibling = sibling;

            sibling->m_RightSibling = this;
            sibling->m_LeftSibling  = this;

            return;
        }

        auto* leftSibling = m_LeftSibling;

        leftSibling->m_RightSibling = sibling;
        m_LeftSibling               = sibling;

        sibling->m_RightSibling     = this;
        sibling->m_LeftSibling      = leftSibling;
    }

    template <typename T>
    void FibonacciNode<T>::PushChild(FibonacciNode* child) {        
        child->StayAlone();

        child->m_Parent   = this;
        child->m_IsMarked = false;

        ++m_Degree;

        if (!m_Child)
            m_Child = child;
        else
            m_Child->PushSibling(child);
    }

    template <typename T>
    void FibonacciNode<T>::Print(int level) const {
        std::cout << m_Value << ", P: ";

        if (HasParent()) {
            std::cout << m_Parent->m_Value;
        } else {
            std::cout << "Null";
        }

        std::cout << ", L: " << m_LeftSibling->m_Value << ", R: "
        << m_RightSibling->m_Value << std::endl;

        if (m_Degree == 0)
            return;

        auto child = m_Child;

        do {
            for (int j = 0; j < level; ++j)
                std::cout << "  ";

            child->Print(level + 1);

            child = child->m_RightSibling;
        } while (child != m_Child);
    }

} // namespace Ng