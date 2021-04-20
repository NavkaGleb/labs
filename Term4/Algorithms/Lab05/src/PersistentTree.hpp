#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <stack>

#include "PersistentNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class PersistentTree {
    public:
        using Node    = PersistentNode<Key, Value>;
        using NodePtr = typename Node::SmartPointer;

    public:
        class Iterator {
        public:
            explicit Iterator(NodePtr node, std::stack<NodePtr>&& stack = std::stack<NodePtr>());

            inline const typename Node::Pair& operator *() const { return m_Node->m_Pair; }
//            inline typename Node::Pair* operator ->() { return &m_Node->m_Pair; }

            Iterator& operator ++();

            inline bool operator !=(const Iterator& other) const { return m_Node != other.m_Node; }
            inline bool operator ==(const Iterator& other) const { return m_Node == other.m_Node; }

        private:
            NodePtr             m_Node;
            std::stack<NodePtr> m_Stack;

        }; // class Iterator

    public:
        PersistentTree();
        explicit PersistentTree(NodePtr root, std::size_t count);
        ~PersistentTree() = default;

        [[nodiscard]] inline std::size_t GetCount() const { return m_Count; }

        [[nodiscard]] bool IsContains(const Key& key);

        std::optional<std::reference_wrapper<const Value>> At(const Key& key) const;

        PersistentTree<Key, Value> Push(const Key& key, const Value& value);
        PersistentTree<Key, Value> Pop(const Key& key);

        void Print() const;

        [[nodiscard]] Iterator Begin();
        [[nodiscard]] Iterator End();

    private:
        NodePtr     m_Root;
        std::size_t m_Count;

    }; // class PersistentTree

} // namespace Ng

#include "PersistentTree.inl"