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
            using Pair = typename LeafNode::Pair;

        public:
            explicit Iterator(LeafNode* node = nullptr);
            ~Iterator() = default;

            Iterator& operator ++();

            Pair operator *();

            bool operator !=(const Iterator& other) const;
            bool operator ==(const Iterator& other) const;

        private:
            LeafNode*   m_Node;
            std::size_t m_Index;

        }; // class Iterator

        class ConstIterator {
        public:
            using Pair = typename LeafNode::ConstPair;

        public:
            explicit ConstIterator(const LeafNode* node = nullptr);
            ~ConstIterator() = default;

            ConstIterator& operator ++();

            Pair operator *() const;

            bool operator !=(const ConstIterator& other) const;
            bool operator ==(const ConstIterator& other) const;

        private:
            const LeafNode* m_Node;
            std::size_t     m_Index;

        }; // class ConstIterator

    public:

        BPlusTree();
        BPlusTree(BPlusTree&& other) noexcept;
        ~BPlusTree();

        [[nodiscard]] inline int GetDegree() const { return Degree; }
        [[nodiscard]] inline int GetCount() const { return m_Count; }
        [[nodiscard]] inline bool IsEmpty() const { return m_Count == 0; }

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] int GetHeight() const;

        [[nodiscard]] const Key& GetMinKey() const;
        [[nodiscard]] const Key& GetMaxKey() const;

        void Clear();

        bool Push(const Key& key, const Value& value);
        bool Pop(const Key& key);

        void Print() const;

        inline Iterator Begin() { return Iterator(GetMinNode()); }
        inline Iterator End() { return Iterator(); }

        inline ConstIterator Begin() const { return ConstIterator(GetMinNode()); }
        inline ConstIterator End() const { return ConstIterator(); }

    private:
        [[nodiscard]] LeafNode* GetLeafNode(const Key& key);

        [[nodiscard]] LeafNode* GetMinNode();
        [[nodiscard]] LeafNode* GetMaxNode();

        [[nodiscard]] const LeafNode* GetMinNode() const;
        [[nodiscard]] const LeafNode* GetMaxNode() const;

        void Split(Node* node);
        void Pop(Node* node, const Key& key);

        void Print(const Node* node, int level = 0) const;

    private:
        Node* m_Root;
        int   m_Count;

    }; // class BPlusTree

    // Free functions for range based for!
    template <typename Key, typename Value, int Degree>
    inline auto begin(Ng::BPlusTree<Key, Value, Degree>& tree) { return tree.Begin(); }

    template <typename Key, typename Value, int Degree>
    inline auto end(Ng::BPlusTree<Key, Value, Degree>& tree) { return tree.End(); }

    template <typename Key, typename Value, int Degree>
    inline auto cbegin(Ng::BPlusTree<Key, Value, Degree>& tree) { return tree.ConstBegin(); }

    template <typename Key, typename Value, int Degree>
    inline auto cend(Ng::BPlusTree<Key, Value, Degree>& tree) { return tree.End(); }

} // namespace Ng

#include "BPlusTree.inl"