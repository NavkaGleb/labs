#include <iostream>

namespace Ng {

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::PersistentTree(NodePtr root)
        : m_Root(root) {}

    template <typename Key, typename Value>
    PersistentTree<Key, Value> PersistentTree<Key, Value>::Push(const Key& key, const Value& value) {
        const auto [maybeBlackNewRoot, response] = Node::Push(m_Root, key, value);
        const auto newRoot                       = maybeBlackNewRoot->CloneAsBlack();

        return PersistentTree(newRoot);
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::Pop(const Key& key) {

    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::Print() const {
        if (m_Root)
            m_Root->Print();
    }

} // namespace Ng