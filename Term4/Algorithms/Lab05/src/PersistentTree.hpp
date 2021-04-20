#pragma once

#include <vector>
#include <optional>

#include "PersistentNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class PersistentTree {
    public:
        using Node = PersistentNode<Key, Value>;

    public:
        PersistentTree();
        ~PersistentTree();

        void Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        void Print() const;

    private:
        void RotateLeft(Node* node);
        void RotateRight(Node* node);

        void PushFix(Node* node);
        void PopFix(Node* node);

    private:
        Node*              m_CurrentRoot;
        std::vector<Node*> m_Roots;

    }; // class PersistentTree

} // namespace Ng

#include "PersistentTree.inl"