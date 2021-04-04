#pragma once

namespace Ng {

    template <typename T>
    class BPlusTree {
    public:
        using InnerType = T;

        class Node {
        public:

        private:

        }; // class Node

        BPlusTree();
        ~BPlusTree();

    private:
        Node* m_Root;

    }; // class BPlusTree

} // namespace Ng

#include "BPlusTree.inl"