#pragma once

#include <type_traits>
#include <vector>

#include "ITree.hpp"

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
            Node(const Key& key,
                 const Value& value,
                 Node* parent = nullptr,
                 Node* left = nullptr,
                 Node* right = nullptr
            );
            Node(const Pair& pair, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr);
            virtual ~Node();

            [[nodiscard]] inline const Key& GetKey() const { return m_Pair.first; }
            [[nodiscard]] inline const Value& GetValue() const { return m_Pair.second; }
            [[nodiscard]] inline const Node* GetParent() const { return m_Parent; }
            [[nodiscard]] inline const Node* GetLeft() const { return m_Left; }
            [[nodiscard]] inline const Node* GetRight() const { return m_Right; }

            friend class OptimalBinarySearchTree;

        private:
            Pair  m_Pair;
            Node* m_Parent;
            Node* m_Left;
            Node* m_Right;

        }; // class Node

        OptimalBinarySearchTree(std::vector<float> probabilities);
        ~OptimalBinarySearchTree();

        void Print() const;

    private:
        struct TablePair {
            float Cost = std::numeric_limits<float>::infinity();
            Pair  Pair;
        };

        void InitTable();
        void BuildTree(Node* node, int left, int top);

    private:
        Node* m_Root;

        std::vector<float>                  m_Probabilities;
        std::vector<std::vector<TablePair>> m_Table;

    }; // class OptimalBinarySearchTree

} // namespace Ng

#include "OptimalBinarySearchTree.inl"