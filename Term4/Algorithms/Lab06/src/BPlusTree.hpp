#pragma once

#include <set>
#include <vector>

namespace Ng {

    template <typename Key, typename Value, int Degree>
    class BPlusTree {
    public:
        using KeyType   = Key;
        using ValueType = Value;

        class Node {
        public:
            using KeyContainer   = std::set<Key>;
            using KeyIterator    = typename KeyContainer::iterator;

            using ChildContainer = std::vector<Node*>;
            using ChildIterator  = typename ChildContainer::iterator;

            explicit Node(bool isLeaf = true);

            [[nodiscard]] inline const KeyContainer& GetKeys() const { return m_Keys; }
            [[nodiscard]] inline const ChildContainer& GetChildren() const { return m_Children; }
            [[nodiscard]] inline Node* GetParent() const { return m_Parent; }
            [[nodiscard]] inline Node* GetLeftSibling() const { return m_LeftSibling; }
            [[nodiscard]] inline Node* GetRightSibling() const { return m_RightSibling; }
            [[nodiscard]] inline bool IsLeaf() const { return m_IsLeaf; }
            [[nodiscard]] inline bool IsContainsKey(const Key& key) const { return m_Keys.contains(key); }
            [[nodiscard]] inline bool IsFull() const { return m_Keys.size() == Degree * 2; }

            [[nodiscard]] inline KeyIterator GetBeginKey() { return m_Keys.begin(); }
            [[nodiscard]] inline KeyIterator GetEndKey() { return m_Keys.end(); }
            [[nodiscard]] inline ChildIterator GetBeginChild() { return m_Children.begin(); }
            [[nodiscard]] inline ChildIterator GetEndChild() { return m_Children.end(); }
            [[nodiscard]] inline bool IsChildrenEmpty() const { return m_Children.empty(); }

            KeyIterator GetKeyMedian();
            ChildIterator GetChildIterator(KeyIterator keyMedian);

            Node* GetChild(const Key& key);

            KeyIterator PushKey(const Key& key);
            void PushKeys(KeyIterator begin, KeyIterator end);
            void PopKeys(KeyIterator begin, KeyIterator end);

            void PushChild(Node* child);
            void PushChild(ChildIterator iterator, Node* child);
            void PushChildren(ChildIterator begin, ChildIterator end);
            void PopChildren(ChildIterator begin, ChildIterator end);

        private:
            std::set<Key>      m_Keys;
            std::vector<Node*> m_Children;
            Node*              m_Parent;
            Node*              m_LeftSibling;
            Node*              m_RightSibling;
            bool               m_IsLeaf;

        }; // class Node

        BPlusTree();
        BPlusTree(BPlusTree&& other) noexcept;
        ~BPlusTree();

        [[nodiscard]] inline int GetDegree() const { return Degree; }

        [[nodiscard]] bool IsExists(const Key& key) const;

        Value Search(const Key& key) const;

        Value& Push(const Key& key, const Value& value);

        void Print() const;

    private:
        [[nodiscard]] Node* GetLeafNode(const Key& key);

        void Split(Node* node);

        Value Search(const Node* node, const Key& key) const;

        void Print(const Node* node, int level = 0) const;

    private:
        Node* m_Root;

    }; // class BPlusTree

} // namespace Ng

#include "BPlusTree.inl"