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
        class IIterator {
        protected:
            explicit IIterator(NodePtr node, std::stack<NodePtr>&& stack = std::stack<NodePtr>());

        public:
            IIterator& operator ++();

            inline bool operator !=(const IIterator& other) const { return m_Node != other.m_Node; }
            inline bool operator ==(const IIterator& other) const { return m_Node == other.m_Node; }

        protected:
            NodePtr             m_Node;
            std::stack<NodePtr> m_Stack;

        }; // class IIterator

        class Iterator : public IIterator {
        public:
            explicit Iterator(NodePtr node, std::stack<NodePtr>&& stack = std::stack<NodePtr>());

            inline typename Node::Pair& operator *() { return this->m_Node->m_Pair; }
            inline typename Node::Pair* operator ->() { return &this->m_Node->m_Pair; }

        }; // class Iterator

        class ConstIterator {
        public:
            explicit ConstIterator(NodePtr node, std::stack<NodePtr>&& stack = std::stack<NodePtr>());

            inline const typename Node::Pair& operator *() const { return this->m_Node->m_Pair; }
            inline const typename Node::Pair* operator ->() const { return &this->m_Node->m_Pair; }

        }; // class ConstIterator

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

        [[nodiscard]] ConstIterator Begin() const;
        [[nodiscard]] ConstIterator End() const;

    private:
        NodePtr     m_Root;
        std::size_t m_Count;

    }; // class PersistentTree

    // For range based for!
    template <typename Key, typename Value>
    inline auto begin(PersistentTree<Key, Value>& tree) { return tree.Begin(); }

    template <typename Key, typename Value>
    inline auto end(PersistentTree<Key, Value>& tree) { return tree.End(); }

    template <typename Key, typename Value>
    inline auto begin(const PersistentTree<Key, Value>& tree) { return tree.Begin(); }

    template <typename Key, typename Value>
    inline auto end(const PersistentTree<Key, Value>& tree) { return tree.End(); }

} // namespace Ng

#include "PersistentTree.inl"