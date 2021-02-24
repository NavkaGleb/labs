#pragma once

#include <stdexcept>

#include "ITree.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class SplayTree : public ITree<Key, Value> {
    public:
        class Node {
        public:
            Node() = default;
            Node(const Key& key, const Value& value, Node* parent = nullptr)
                : m_Key(key)
                , m_Value(value)
                , m_Left(nullptr)
                , m_Right(nullptr)
                , m_Parent(parent) {}
            virtual ~Node();

            [[nodiscard]] inline const Key& GetKey() const { return m_Key; }
            [[nodiscard]] inline const Value& GetValue() const { return m_Value; }
            [[nodiscard]] inline const Node& GetLeft() const { return *m_Left; }
            [[nodiscard]] inline const Node& GetRight() const { return *m_Right; }
            [[nodiscard]] inline const Node& GetParent() const { return *m_Parent; }

            void Print() const {
                std::cout << "Key: " << m_Key << ", Value " << m_Value << " {L: ";

                std::cout << (m_Left  ? std::to_string(m_Left->m_Key)  : "Null") << ", R: ";
                std::cout << (m_Right ? std::to_string(m_Right->m_Key) : "Null") << "}";
            }

            friend class SplayTree;

        private:
            Key   m_Key;
            Value m_Value;
            Node* m_Left;
            Node* m_Right;
            Node* m_Parent;

        }; // class Node

        explicit SplayTree(Node* root = nullptr);
        ~SplayTree() override;

        [[nodiscard]] inline bool IsEmpty() const override { return m_Size == 0; };
        [[nodiscard]] inline int GetSize() const override { return m_Size; };
        [[nodiscard]] inline const Node& GetRoot() const { return *m_Root; }

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] int GetHeight() const;

        [[nodiscard]] Value& Get(const Key& key);
        [[nodiscard]] const Value& Get(const Key& key) const;

        [[nodiscard]] Node* GetNode(const Key& key);

//        void Merge(Node* left, Node* right);

        Value& Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        void Print() const;

    private:
        [[nodiscard]] int GetHeight(Node* node) const;

        Node* RotateLeft(Node* node);
        Node* RotateRight(Node* node);

        void Zig(Node* node);
        void ZigZag(Node* node);
        void ZigZig(Node* node);
        void Splay(Node* node);

        void Print(const Node* node, const int& level, const char* caption) const;

    private:
        Node* m_Root;
        int   m_Size;

    }; // class SplayTree



} // namespace Ng

#include "SplayTree.inl"