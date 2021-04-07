#pragma once

#include <set>
#include <vector>

#include "BPlusInternalNode.hpp"
#include "BPlusLeafNode.hpp"

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

    public:
        class Iterator {
        public:
            using Pair = std::pair<Key, Value>;

        public:
            explicit Iterator(Node* node = nullptr);
            ~Iterator() = default;

            Iterator& operator ++();

            const Key& operator *();

            bool operator !=(const Iterator& other) const;
            bool operator ==(const Iterator& other) const;

        private:
            Node*       m_Node;
            std::size_t m_Index;

        }; // class Iterator

    public:

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

        inline Iterator Begin() { return Iterator(GetMinNode()); }
        inline Iterator End() { return Iterator(); }

    private:
        [[nodiscard]] Node* GetLeafNode(const Key& key);
        [[nodiscard]] Node* GetMinNode();
        [[nodiscard]] Node* GetMaxNode();

        void Split(Node* node);
        void Pop(Node* node, const Key& key);

        void Print(const Node* node, int level = 0) const;

    private:
        Node* m_Root;
        int   m_Count;

    }; // class BPlusTree

    // Free functions for range based for!
    template <typename Key, typename Value, int Degree>
    auto begin(Ng::BPlusTree<Key, Value, Degree>& tree) {
        return tree.Begin();
    }

    template <typename Key, typename Value, int Degree>
    auto end(Ng::BPlusTree<Key, Value, Degree>& tree) {
        return tree.End();
    }

} // namespace Ng

#include "BPlusTree.inl"