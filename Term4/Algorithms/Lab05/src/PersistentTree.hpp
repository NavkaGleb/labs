#pragma once

#include <vector>
#include <optional>
#include <memory>

#include "PersistentNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class PersistentTree {
    public:
        using Node    = PersistentNode<Key, Value>;
        using NodePtr = typename Node::SmartPointer;

    public:
        PersistentTree() = default;
        explicit PersistentTree(NodePtr root);
        ~PersistentTree() = default;

        PersistentTree<Key, Value> Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        void Print() const;

    private:
        NodePtr m_Root;

    }; // class PersistentTree

} // namespace Ng

#include "PersistentTree.inl"