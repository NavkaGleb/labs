#include <utility>
#include <iostream>

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
        delete m_Child;

        if (!IsAlone()) {
            delete m_LeftSibling;
            delete m_RightSibling;
        }
    }

    template <typename T>
    FibonacciNode<T>* FibonacciNode<T>::GetHandle(const T& value) {
        FibonacciNode* node   = this;
        FibonacciNode* result = nullptr;

        do {
            if (node->m_Value == value)
                return node;

            if (node->m_Child)
                result = node->m_Child->GetHandle(value);

            if (result)
                return result;

            if (node->m_RightSibling == this)
                return nullptr;

            node = node->m_RightSibling;
        } while (node != this);

        return result;
    }

    template <typename T>
    void FibonacciNode<T>::StayAlone() {
        m_IsMarked                    = false;
        m_Parent                      = nullptr;

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
    void FibonacciNode<T>::PopChild(FibonacciNode* child) {
        if (child->IsAlone())
            m_Child = nullptr;
        else
            m_Child = child->m_RightSibling;

        --m_Degree;

        child->StayAlone();
    }

    template <typename T>
    void FibonacciNode<T>::Print(const std::string& indent) const {
        for (const FibonacciNode* node = this; true;) {
            std::cout << indent << "+- " << node->m_Value << std::endl;

            if (node->m_Child)
                node->m_Child->Print(indent + (node->m_RightSibling == node ? "   " : "|  "));

            node = node->m_RightSibling;

            if (node == this)
                return;
        }
    }

} // namespace Ng