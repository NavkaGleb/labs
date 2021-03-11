#pragma once

#include <type_traits>
#include <vector>

#include "IteratorRange.hpp"

namespace Ng {

    template <typename T>
    concept Comparable = requires(T lhs, T rhs) {
        { lhs < rhs } -> std::same_as<bool>;
        { lhs > rhs } -> std::same_as<bool>;
    };

    template <Comparable KeyType, typename ValueType>
    class OptimalBinarySearchTree {
    public:
        using Key   = KeyType;
        using Value = ValueType;
        using Pair = std::pair<const Key, Value>;

        struct DataCell {
            Key   Key         = KeyType();
            Value Value       = ValueType();
            float Probability = 0.0f;
        };

        class Node {
        public:
            explicit Node(const Pair& pair, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr);
            virtual ~Node();

            [[nodiscard]] inline const Key& GetKey() const { return m_Pair.first; }
            [[nodiscard]] inline const Value& GetValue() const { return m_Pair.second; }
            [[nodiscard]] inline const Node* GetParent() const { return m_Parent; }
            [[nodiscard]] inline const Node* GetLeft() const { return m_Left; }
            [[nodiscard]] inline const Node* GetRight() const { return m_Right; }

            friend class OptimalBinarySearchTree;

        private:
            void Print() const;

        private:
            Pair  m_Pair;
            Node* m_Parent;
            Node* m_Left;
            Node* m_Right;

        }; // class Node

        class IteratorBase {
        public:
            explicit IteratorBase(Node* node);
            virtual ~IteratorBase() = default;

            IteratorBase& operator ++();
            IteratorBase& operator +=(int n);

            bool operator !=(const IteratorBase& other) const;

        protected:
            [[nodiscard]] Node* GetNode() { return m_Node; }
            [[nodiscard]] const Node* GetNode() const { return m_Node; }

        protected:
            Node* m_Node;

        }; // class IteratorBase

        class Iterator : public IteratorBase {
        public:
            explicit Iterator(Node* node = nullptr)
                : IteratorBase(node) {}
            ~Iterator() override = default;

            [[nodiscard]] inline Pair& operator *() { return this->GetNode()->m_Pair; }
            [[nodiscard]] inline Pair* operator ->() { return &this->GetNode()->m_Pair; }

        }; // class Iterator

        class ConstIterator : public IteratorBase {
        public:
            explicit ConstIterator(Node* node = nullptr)
                : IteratorBase(node) {}
            virtual ~ConstIterator() = default;

            [[nodiscard]] inline const Pair& operator *() const { return this->GetNode()->m_Pair; }
            [[nodiscard]] inline const Pair* operator ->() const { return &this->GetNode()->m_Pair; }

        }; // class ConstIterator

        class KeyIterator : public IteratorBase {
        public:
            explicit KeyIterator(Node* node = nullptr)
                : IteratorBase(node) {}
            virtual ~KeyIterator() = default;

            [[nodiscard]] inline const Key& operator *() const { return this->GetNode()->m_Pair.first; }
            [[nodiscard]] inline const Key* operator ->() const { return &this->GetNode()->m_Pair.first; }

        }; // class KeyIterator

        class ValueIterator : public IteratorBase {
        public:
            explicit ValueIterator(Node* node = nullptr)
                : IteratorBase(node) {}
            virtual ~ValueIterator() = default;

            [[nodiscard]] inline Value& operator *() { return this->GetNode()->m_Pair.second; }
            [[nodiscard]] inline Value* operator ->() { return &this->GetNode()->m_Pair.second; }

        }; // class ValueIterator

         OptimalBinarySearchTree();
        ~OptimalBinarySearchTree();

        [[nodiscard]] inline bool IsEmpty() const { return m_Size == 0; };
        [[nodiscard]] inline int GetSize() const { return m_Size; }

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] int GetHeight() const;

        [[nodiscard]] const Value& GetMin() const;
        [[nodiscard]] const Value& GetMax() const;

        [[nodiscard]] Value& Get(const Key& key);
        [[nodiscard]] const Value& Get(const Key& key) const;

        [[nodiscard]] IteratorRange<KeyIterator> GetKeys() const;
        [[nodiscard]] IteratorRange<ValueIterator> GetValues() const;

        void Clear();

        void SetData(const std::vector<DataCell>& data);

        void PrintTable() const;
        void Print() const;

        [[nodiscard]] Iterator begin() { return Iterator(GetMinNode(m_Root)); }
        [[nodiscard]] Iterator end() { return Iterator(); }

        [[nodiscard]] ConstIterator begin() const { return ConstIterator(GetMinNode(m_Root)); }
        [[nodiscard]] ConstIterator end() const { return ConstIterator(); }

        [[nodiscard]] ConstIterator cbegin() const { return ConstIterator(GetMinNode(m_Root)); }
        [[nodiscard]] ConstIterator cend() const { return ConstIterator(); }

    private:
        struct TablePair {
            float       Cost  = std::numeric_limits<float>::infinity();
            std::size_t Index = 0;
        };

        void InitTable(const std::vector<DataCell>& data);
        void Build(Node*& node, std::size_t row, std::size_t column, const std::vector<DataCell>& data);

        [[nodiscard]] int GetHeight(Node* node) const;

        [[nodiscard]] const Value& GetMin(Node* node) const;
        [[nodiscard]] const Value& GetMax(Node* node) const;

        [[nodiscard]] Node* GetMinNode(Node* node) const;
        [[nodiscard]] Node* GetMaxNode(Node* node) const;

        void Print(const Node* node, int level, const std::string& caption) const;

    private:
        Node*                               m_Root;
        int                                 m_Size;
        std::vector<std::vector<TablePair>> m_Table;

    }; // class OptimalBinarySearchTree

} // namespace Ng

#include "OptimalBinarySearchTree.inl"