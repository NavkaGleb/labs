#include <iostream>

namespace Ng {

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::IIterator::IIterator(NodePtr node, std::stack<NodePtr>&& stack)
        : m_Node(node)
        , m_Stack(std::move(stack)) {}

    template <typename Key, typename Value>
    typename PersistentTree<Key, Value>::IIterator& PersistentTree<Key, Value>::IIterator::operator ++() {
        m_Node = m_Node->m_Right;

        while (!m_Stack.empty() || m_Node) {
            if (m_Node) {
                m_Stack.push(m_Node);
                m_Node = m_Node->m_Left;
            } else {
                m_Node = m_Stack.top();
                m_Stack.pop();
                break;
            }
        }

        return *this;
    }

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::Iterator::Iterator(NodePtr node, std::stack<NodePtr>&& stack)
        : PersistentTree<Key, Value>::IIterator(node, std::move(stack)) {}

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::ConstIterator::ConstIterator(NodePtr node, std::stack<NodePtr>&& stack)
        : PersistentTree<Key, Value>::IIterator(node, std::move(stack)) {}

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::PersistentTree()
        : m_Count(0) {}

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::PersistentTree(NodePtr root, std::size_t count)
        : m_Root(root)
        , m_Count(count) {}

    template <typename Key, typename Value>
    bool PersistentTree<Key, Value>::IsContains(const Key& key) {
        auto node = m_Root;

        while (node) {
            if (key < node->m_Pair.first)
                node = node->m_Left;
            else if (key > node->m_Pair.first)
                node = node->m_Right;
            else
                return true;
        }

        return false;
    }

    template <typename Key, typename Value>
    std::optional<std::reference_wrapper<const Value>> PersistentTree<Key, Value>::At(const Key& key) const {
        auto node = m_Root;

        while (node) {
            if (key < node->m_Pair.first)
                node = node->m_Left;
            else if (key > node->m_Pair.first)
                node = node->m_Right;
            else
                return node->m_Pair.second;
        }

        return std::nullopt;
    }

    template <typename Key, typename Value>
    PersistentTree<Key, Value> PersistentTree<Key, Value>::Push(const Key& key, const Value& value) {
        const auto [maybeBlackNewRoot, isPushed] = Node::Push(m_Root, key, value);
        const auto newRoot                       = maybeBlackNewRoot->CloneAsBlack();
        const auto newCount                      = m_Count + (isPushed ? 1 : 0);

        return PersistentTree(newRoot, newCount);
    }

    template <typename Key, typename Value>
    PersistentTree<Key, Value> PersistentTree<Key, Value>::Pop(const Key& key) {
        const auto [maybeBlackNewRoot, isPopped] = Node::Pop(m_Root, key);

        if (isPopped) {
            const auto newRoot = maybeBlackNewRoot ? maybeBlackNewRoot->CloneAsBlack() : maybeBlackNewRoot;
            return PersistentTree(newRoot, m_Count - 1);
        }

        return *this;
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::Print() const {
        if (m_Root)
            m_Root->Print();
    }

    template <typename Key, typename Value>
    typename PersistentTree<Key, Value>::Iterator PersistentTree<Key, Value>::Begin() {
        NodePtr             node = m_Root;
        std::stack<NodePtr> stack;

        while (!stack.empty() || node) {
            if (!node->m_Left)
                break;

            stack.push(node);
            node = node->m_Left;
        }

        return Iterator(node, std::move(stack));
    }

    template <typename Key, typename Value>
    typename PersistentTree<Key, Value>::Iterator PersistentTree<Key, Value>::End() {
        return Iterator(nullptr);
    }

    template <typename Key, typename Value>
    typename PersistentTree<Key, Value>::ConstIterator PersistentTree<Key, Value>::Begin() const {
        NodePtr             node = m_Root;
        std::stack<NodePtr> stack;

        while (!stack.empty() || node) {
            if (!node->m_Left)
                break;

            stack.push(node);
            node = node->m_Left;
        }

        return ConstIterator(node, std::move(stack));
    }

    template <typename Key, typename Value>
    typename PersistentTree<Key, Value>::ConstIterator PersistentTree<Key, Value>::End() const {
        return ConstIterator(nullptr);
    }

} // namespace Ng