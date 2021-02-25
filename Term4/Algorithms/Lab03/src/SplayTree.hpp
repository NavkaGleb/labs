#pragma once

#include <stdexcept>

#include "ITree.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class SplayTree : public ITree<Key, Value> {
    public:
        using Pair = std::pair<const Key, Value>;

        class Node {
        public:

            Node();
            Node(const Key& key,
                 const Value& value,
                 Node* parent = nullptr,
                 Node* left   = nullptr,
                 Node* right  = nullptr);
            virtual ~Node();

            [[nodiscard]] inline const Key& GetKey() const { return m_Pair.first; }
            [[nodiscard]] inline const Value& GetValue() const { return m_Pair.second; }
            [[nodiscard]] inline const Node* GetParent() const { return m_Parent; }
            [[nodiscard]] inline const Node* GetLeft() const { return m_Left; }
            [[nodiscard]] inline const Node* GetRight() const { return m_Right; }

            friend class SplayTree;

        private:
            void Print(std::ostream& ostream) const;

        private:
            Pair  m_Pair;
            Node* m_Parent;
            Node* m_Left;
            Node* m_Right;

        }; // class Node

        class Iterator {
        public:
            explicit Iterator(Node* node = nullptr);
            virtual ~Iterator() = default;

            [[nodiscard]] inline Pair& operator *() { return m_Node->m_Pair; }
            [[nodiscard]] inline Pair* operator ->() { return *m_Node->m_Pair; }

            Iterator& operator ++();
            Iterator& operator +=(int n);

            bool operator !=(const Iterator& other) const;

        private:
            Node* m_Node;

        }; // class ConstIterator

        class ConstIterator {
        public:
            explicit ConstIterator(Node* node = nullptr);
            virtual ~ConstIterator() = default;

            [[nodiscard]] inline const Pair& operator *() const { return m_Node->m_Pair; }
            [[nodiscard]] inline const Pair* operator ->() const { return *m_Node->m_Pair; }

            ConstIterator& operator ++();
            ConstIterator& operator +=(int n);

            bool operator !=(const ConstIterator& other) const;

        private:
            Node* m_Node;

        }; // class ConstIterator

        explicit SplayTree(Node* root = nullptr);
        ~SplayTree() override;

        [[nodiscard]] inline bool IsEmpty() const override { return m_Size == 0; };
        [[nodiscard]] inline int GetSize() const override { return m_Size; };
        [[nodiscard]] inline const Node* GetRoot() const { return m_Root; }

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] int GetHeight() const;

        [[nodiscard]] const Value& GetMin() const;
        [[nodiscard]] const Value& GetMax() const;

        [[nodiscard]] Value& Get(const Key& key);
        [[nodiscard]] const Value& Get(const Key& key) const;

        void Clear();

        Value& Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        [[nodiscard]] Iterator begin() { return Iterator(GetMinNode(m_Root)); }
        [[nodiscard]] Iterator end() { return Iterator(); }

        [[nodiscard]] ConstIterator begin() const { return ConstIterator(GetMinNode(m_Root)); }
        [[nodiscard]] ConstIterator end() const { return ConstIterator(); }

        [[nodiscard]] ConstIterator cbegin() const { return ConstIterator(GetMinNode(m_Root)); }
        [[nodiscard]] ConstIterator cend() const { return ConstIterator(); }

        Value& operator [](const Key& value);

        template <typename Key_, typename Value_>
        friend std::ostream& operator <<(std::ostream& ostream, const SplayTree<Key_, Value_>& tree);

    private:
        [[nodiscard]] int GetHeight(Node* node) const;

        [[nodiscard]] const Value& GetMin(Node* node) const;
        [[nodiscard]] const Value& GetMax(Node* node) const;

        [[nodiscard]] Node* GetMinNode(Node* node) const;
        [[nodiscard]] Node* GetMaxNode(Node* node) const;

        [[nodiscard]] Node* GetSuccessor(Node* node) const;
        [[nodiscard]] Node* GetPredecessor(Node* node) const;

        [[nodiscard]] Node* GetNode(const Key& key);

        void Transplant(Node* parent, Node* child);

        Node* RotateLeft(Node* node);
        Node* RotateRight(Node* node);

        void Zig(Node* node);
        void ZigZag(Node* node);
        void ZigZig(Node* node);
        void Splay(Node* node);

        void Merge(Node* left, Node* right);

        void Print(const Node* node, int level, const char* caption, std::ostream& ostream) const;

    private:
        Node* m_Root;
        int   m_Size;

    }; // class SplayTree

} // namespace Ng

#include "SplayTree.inl"