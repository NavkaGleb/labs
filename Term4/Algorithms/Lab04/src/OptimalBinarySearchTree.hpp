#pragma once

#include <type_traits>
#include <vector>

namespace Ng {

    template <typename T>
    concept Comparable = requires(T lhs, T rhs) {
        { lhs < rhs } -> std::same_as<bool>;
        { rhs < rhs } -> std::same_as<bool>;
    };

    template <Comparable Key, typename Value>
    class OptimalBinarySearchTree {
    public:
        using Pair = std::pair<const Key, Value>;

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
            void Print(std::ostream& ostream) const;

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

        OptimalBinarySearchTree(
            const std::vector<Key>& keys,
            const std::vector<Value>& values,
            const std::vector<float>& probabilities
        );
        ~OptimalBinarySearchTree();

        [[nodiscard]] inline int GetSize() const { return m_Size; }

        void PrintTable() const;

        [[nodiscard]] Iterator begin() { return Iterator(GetMinNode(m_Root)); }
        [[nodiscard]] Iterator end() { return Iterator(); }

        template <Comparable Key_, typename Value_>
        friend std::ostream& operator <<(std::ostream& ostream, const OptimalBinarySearchTree<Key_, Value_>& tree);

    private:
        struct TablePair {
            float Cost = std::numeric_limits<float>::infinity();
            std::pair<Key, Value>  Pair;
        };

        void InitTable(
            const std::vector<Key>& keys,
            const std::vector<Value>& values,
            const std::vector<float>& probabilities
        );
        void Build(Node*& node, int left, int top);

        [[nodiscard]] Node* GetMinNode(Node* node) const;
        [[nodiscard]] Node* GetMaxNode(Node* node) const;

        void Print(const Node* node, int level, const char* caption, std::ostream& ostream) const;

    private:
        Node*                               m_Root;
        int                                 m_Size;
        std::vector<std::vector<TablePair>> m_Table;

    }; // class OptimalBinarySearchTree

} // namespace Ng

#include "OptimalBinarySearchTree.inl"