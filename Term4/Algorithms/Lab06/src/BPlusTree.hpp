#pragma once

#include <set>
#include <vector>

#include "BPlusNode.hpp"

namespace Ng {

#define INTERNAL_NODE(node)       static_cast<BPlusInternalNode<Key, Value>*>(node)
#define LEAF_NODE(node)           static_cast<BPlusLeafNode<Key, Value>*>(node)

#define CONST_INTERNAL_NODE(node) static_cast<const BPlusInternalNode<Key, Value>*>(node)
#define CONST_LEAF_NODE(node)     static_cast<const BPlusLeafNode<Key, Value>*>(node)

    template <typename Key, typename Value, int Degree>
    class BPlusTree {
    public:
        using KeyType      = Key;
        using ValueType    = Value;
        using Node         = BPlusNode<Key, Value>;
        using InternalNode = BPlusInternalNode<Key, Value>;
        using LeafNode     = BPlusLeafNode<Key, Value>;

        BPlusTree();
        BPlusTree(BPlusTree&& other) noexcept;
        ~BPlusTree();

        [[nodiscard]] inline int GetDegree() const { return Degree; }
        [[nodiscard]] inline int GetCount() const { return m_Count; }
        [[nodiscard]] inline bool IsEmpty() const { return m_Count == 0; }

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] int GetHeight() const;

        void Clear();

        Value& Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        void Print() const;

    private:
        [[nodiscard]] Node* GetLeafNode(const Key& key);

        void Split(Node* node);

        void Print(const Node* node, int level = 0) const;

    private:
        Node* m_Root;
        int   m_Count;

    }; // class BPlusTree

} // namespace Ng

#include "BPlusTree.inl"