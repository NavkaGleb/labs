#include <iostream>

namespace Ng {

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::PersistentTree()
        : m_CurrentRoot(nullptr) {}

    template <typename Key, typename Value>
    PersistentTree<Key, Value>::~PersistentTree() {
        for (Node* root : m_Roots)
            delete root;
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::Push(const Key& key, const Value& value) {
        auto* node = new Node({ key, value });

        if (!m_CurrentRoot) {
            m_CurrentRoot = m_Roots.emplace_back(node);
            return;
        }

        m_CurrentRoot = m_Roots.emplace_back(Node::Clone(m_CurrentRoot));

        Node* pathNode = m_CurrentRoot;
        Node* parent   = nullptr;

        while (pathNode) {
            parent = pathNode;

            if (pathNode->m_Pair.first > node->m_Pair.first)
                pathNode = pathNode->m_Left ? Node::Clone(pathNode->m_Left, pathNode) : nullptr;
            else
                pathNode = pathNode->m_Right ? Node::Clone(pathNode->m_Right, pathNode) : nullptr;
        }

        node->m_Parent = parent;

        if (parent->m_Pair.first > node->m_Pair.first)
            parent->m_Left = node;
        else
            parent->m_Right = node;

        PushFix(node);
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::Pop(const Key& key) {

    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::Print() const {
        if (m_CurrentRoot)
            m_CurrentRoot->Print();
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::RotateLeft(Node* node) {
        Node* rightClone = Node::Clone(node->m_Right, node);
        
        node->m_Right = rightClone->m_Left;

        if (rightClone->m_Left)
            rightClone->m_Left->m_Parent = node;
        rightClone->m_Parent = node->m_Parent;

        if (!node->m_Parent)
            m_CurrentRoot = rightClone;
        else if (node == node->m_Parent->m_Left)
            node->m_Parent->m_Left = rightClone;
        else
            node->m_Parent->m_Right = rightClone;

        rightClone->m_Left = node;
        node->m_Parent     = rightClone;
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::RotateRight(Node* node) {
        Node* leftClone = Node::Clone(node->m_Left, node);
        
        node->m_Left = leftClone->m_Right;

        if (leftClone->m_Right)
            leftClone->m_Right->m_Parent = node;
        leftClone->m_Parent = node->m_Parent;

        if (!node->m_Parent)
            m_CurrentRoot = leftClone;
        else if (node == node->m_Parent->m_Left)
            node->m_Parent->m_Left = leftClone;
        else
            node->m_Parent->m_Right = leftClone;

        leftClone->m_Right = node;
        node->m_Parent     = leftClone;
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::PushFix(Node* node) {
        std::cout << "---- " << node->m_Pair.first << std::endl;
        std::cout << "Push fix" << std::endl;

        std::cout << "node parent = " << node->m_Parent << std::endl;

        while (node->m_Parent && node->m_Parent->m_Color == Node::Color::Red) {
            Node* parent = node->m_Parent;

            std::cout << "-- current node = " << node->m_Pair.first << std::endl;

            if (parent == parent->m_Parent->m_Left) {
                Node* uncle = parent->m_Parent->m_Right            ?
                              Node::Clone(parent->m_Parent->m_Right, parent->m_Parent) :
                              nullptr;

                if (uncle && uncle->m_Color == Node::Color::Red) {
                    parent->m_Color           = Node::Color::Black;
                    uncle->m_Color            = Node::Color::Black;
                    parent->m_Parent->m_Color = Node::Color::Red;

                    node                      = parent->m_Parent;
                } else {
                    if (node == parent->m_Right){
                        node = parent;
                        RotateLeft(node);
                    }

                    parent = node->m_Parent;

                    parent->m_Color           = Node::Color::Black;
                    parent->m_Parent->m_Color = Node::Color::Red;

                    RotateRight(parent->m_Parent);
                }
            } else {
                Node* uncle = parent->m_Parent->m_Left            ?
                              Node::Clone(parent->m_Parent->m_Left, parent->m_Parent) :
                              nullptr;

                if (uncle && uncle->m_Color == Node::Color::Red){
                    node->m_Parent->m_Color           = Node::Color::Black;
                    uncle->m_Color                    = Node::Color::Black;
                    node->m_Parent->m_Parent->m_Color = Node::Color::Red;

                    node                              = parent->m_Parent;
                } else {
                    if (node == parent->m_Left){
                        node = parent;
                        RotateRight(node);
                    }

                    parent                    = node->m_Parent;

                    parent->m_Color           = Node::Color::Black;
                    parent->m_Parent->m_Color = Node::Color::Red;

                    RotateLeft(parent->m_Parent);
                }
            }
        }

        m_CurrentRoot->m_Color = Node::Color::Black;

        std::cout << "PushFix end" << std::endl;
    }

    template <typename Key, typename Value>
    void PersistentTree<Key, Value>::PopFix(Node* node) {

    }

} // namespace Ng