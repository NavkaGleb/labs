#pragma once

#include <vector>

namespace Ng {

    template <typename T>
    class BinomialTree {
    public:
        using InnerType = T;

        class Node {
        public:
            using ChildrenContainer = std::vector<Node*>;

            friend class BinomialTree;

        private:
            T                 m_Key;
            int               m_Degree;
            Node*             m_Parent;
            Node*             m_Sibling;
            ChildrenContainer m_Children;

        }; // class Node

        BinomialTree() : m_Root(nullptr) {}
        virtual ~BinomialTree() noexcept = default;

        [[nodiscard]] inline int GetDegree() const { return m_Root->m_Degree; }

    private:
        Node* m_Root;

    }; // class BinomialTree

} // namespace Ng