#include <iostream>

namespace Ng {

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

} // namespace Ng