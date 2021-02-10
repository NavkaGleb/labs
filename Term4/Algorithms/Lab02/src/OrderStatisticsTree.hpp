#pragma once

#include <optional>

namespace Ng {

    template <typename T>
    class OrderStatisticsTree {
    public:
        class Node {
        public:
            enum class Color : int { Red = 0, Black };

            Node();
            explicit Node(const T& value,
                          Node* parent = nullptr,
                          Node* left = nullptr,
                          Node* right = nullptr);
            virtual ~Node();

            [[nodiscard]] inline const T& GetValue() const { return m_Value; }
            [[nodiscard]] inline const Color& GetColor() const { return m_Color; }
            [[nodiscard]] inline const Node* GetParent() const { return m_Parent; }
            [[nodiscard]] inline const Node* GetLeft() const { return m_Left; }
            [[nodiscard]] inline const Node* GetRight() const { return m_Right; }
            [[nodiscard]] inline int GetChildren() const { return m_Children; }

            friend class OrderStatisticsTree;

        private:
            void Print() const;

        private:
            T     m_Value;
            Color m_Color;
            Node* m_Parent;
            Node* m_Left;
            Node* m_Right;
            int   m_Children;

        }; // class Node

        explicit OrderStatisticsTree(const T& value);
        explicit OrderStatisticsTree(Node* root = nullptr);
        virtual ~OrderStatisticsTree();

        [[nodiscard]] inline const Node* GetRoot() const { return m_Root; }
        [[nodiscard]] inline int GetNodes() const { return m_Nodes; }

        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] bool IsExists(const T& value) const;
        [[nodiscard]] std::optional<T> GetMin() const;
        [[nodiscard]] std::optional<T> GetMax() const;

        Node* Push(const T& value);
        void Pop(const T& value);

        void Print() const;

    private:
        [[nodiscard]] int GetHeight(Node* node) const;

        [[nodiscard]] std::optional<T> GetMin(Node* node) const;
        [[nodiscard]] std::optional<T> GetMax(Node* node) const;

        [[nodiscard]] Node* GetSuccessor(Node* node) const;

        void RotateLeft(Node* node);
        void RotateRight(Node* node);

        void PushFix(Node* node);
        void PopFix(Node* child, Node* parent);
        void ChildrenFix(Node* node);

        void Print(const Node* node, const int& level, const char* caption) const;

    private:
        Node* m_Root;
        int   m_Nodes;

    }; // class OrderStatisticsTree

#include "OrderStatisticsTree.inl"

} // namespace Ng