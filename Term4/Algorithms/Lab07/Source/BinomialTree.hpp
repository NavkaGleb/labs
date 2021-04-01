#pragma once

#include <vector>
#include <cmath>

namespace Ng {

    template <typename T, typename Comparator>
    class BinomialHeap;

    template <typename T>
    class BinomialTree {
    public:
        using InnerType = T;

        class Node {
        public:
            using ChildrenContainer = std::list<Node*>;

            explicit Node(const T& value = T());
            Node(const Node& other) = default;
            ~Node() noexcept;

            [[nodiscard]] inline const T& GetValue() const { return m_Value; }
            [[nodiscard]] inline std::size_t GetDegree() const { return m_Children.size(); }
            [[nodiscard]] inline std::size_t GetCount() const { return std::pow(2, GetDegree()); }
            [[nodiscard]] inline ChildrenContainer& GetChildren() { return m_Children; }
            [[nodiscard]] inline const ChildrenContainer& GetChildren() const { return m_Children; }

            Node* Get(std::size_t index);
            ChildrenContainer GiveAwayChildren();

            bool operator !=(const Node& other) const;
            bool operator ==(const Node& other) const;

            friend class BinomialTree;

            template <typename U, typename Comparator>
            friend class BinomialHeap;

        private:
            T                 m_Value;
            Node*             m_Parent;
            ChildrenContainer m_Children;

        }; // class Node

        static void Link(BinomialTree& lhs, BinomialTree& rhs);

        explicit BinomialTree(Node* root = nullptr);
        BinomialTree(const BinomialTree& other);
        BinomialTree(BinomialTree&& other) noexcept;
        ~BinomialTree() noexcept;

        [[nodiscard]] inline std::size_t GetDegree() const { return m_Root->GetDegree(); }
        [[nodiscard]] inline std::size_t GetCount() const { return m_Root->GetCount(); }
        [[nodiscard]] inline Node* GetRoot() const { return m_Root; }

        Node* Get(std::size_t index);

        void Clear();
        void Print() const;

        bool operator !=(const BinomialTree& other) const;
        bool operator ==(const BinomialTree& other) const;

    private:
        void Print(const Node* node, int level, int index) const;

    private:
        Node* m_Root;

    }; // class BinomialTree

} // namespace Ng

#include "BinomialTree.inl"