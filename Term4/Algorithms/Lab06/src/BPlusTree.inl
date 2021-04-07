#include <utility>
#include <stdexcept>
#include <iostream>

namespace Ng {

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::Iterator::Iterator(Node* node)
        : m_Node(node)
        , m_Index(0) {}

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Iterator& BPlusTree<Key, Value, Degree>::Iterator::operator ++() {
        if (m_Index < m_Node->GetKeyCount() - 1) {
            ++m_Index;
        } else {
            m_Node  = m_Node->GetRightSibling();
            m_Index = 0;
        }

        return *this;
    }

    template <typename Key, typename Value, int Degree>
    const Key& BPlusTree<Key, Value, Degree>::Iterator::operator *() {
        return m_Node->GetKey(m_Index);
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::Iterator::operator !=(const Iterator& other) const {
        return m_Node != other.m_Node;
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::Iterator::operator ==(const Iterator& other) const {
        return m_Node == other.m_Node;
    }

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::BPlusTree()
        : m_Root(nullptr)
        , m_Count(0) {

        static_assert(Degree > 1, "Given unacceptable tree Degree!");
    }

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::BPlusTree(BPlusTree<Key, Value, Degree>&& other) noexcept
        : m_Root(std::exchange(other.m_Root, nullptr))
        , m_Count(0) {}

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::~BPlusTree() {
        delete m_Root;
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::IsExists(const Key& key) const {
        if (!m_Root)
            return false;

        Node* node = m_Root;

        while (!node->IsLeaf())
            node = INTERNAL_NODE(node)->GetChild(key);

        return node->IsContainsKey(key);
    }

    template <typename Key, typename Value, int Degree>
    int BPlusTree<Key, Value, Degree>::GetHeight() const {
        if (!m_Root)
            return 0;

        Node* node   = m_Root;
        int   height = 0;

        while (!node->IsLeaf()) {
//            node = INTERNAL_NODE(node)->GetChildren.front();
            ++height;
        }

        return height;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Clear() {
        delete m_Root;
        m_Root = nullptr;
    }

    template <typename Key, typename Value, int Degree>
    Value& BPlusTree<Key, Value, Degree>::Push(const Key& key, const Value& value) {
        if (!m_Root)
            m_Root = new LeafNode;

        Node* node = GetLeafNode(key);

        if (node->IsContainsKey(key))
            return const_cast<Value&>(value);

        node->PushKey(key);

        if (node->GetKeyCount() == Degree * 2)
            Split(node);

        ++m_Count;

        return const_cast<Value&>(value);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Pop(const Key& key) {
        if (!m_Root)
            return;

        auto* node = GetLeafNode(key);

        if (!node->IsContainsKey(key))
            return;

        --m_Count;

        Pop(node, key);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Print() const {
        return Print(m_Root);
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node* BPlusTree<Key, Value, Degree>::GetLeafNode(const Key& key) {
        Node* node = m_Root;

        while (node && !node->IsLeaf())
            node = INTERNAL_NODE(node)->GetChild(key);

        return node;
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node* BPlusTree<Key, Value, Degree>::GetMinNode() {
        Node* node = m_Root;

        while (node && !node->IsLeaf())
            node = node->GetMinNode();

        return node;
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node* BPlusTree<Key, Value, Degree>::GetMaxNode() {
        Node* node = m_Root;

        while (node && !node->IsLeaf())
            node = node->GetMaxNode();

        return node;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Split(Node* node) {
        auto* parent    = node->GetParent();
        auto keyMedian  = node->GetMedian();
        auto* sibling   = node->Split(keyMedian);

        if (node == m_Root) {
            auto* root = new InternalNode;

            root->PushKey(*keyMedian);
            root->PushChild(node);
            root->PushChild(sibling);

            m_Root = root;

            return;
        }

        parent->PushKey(*keyMedian);
        parent->PushChild(sibling);

        if (parent->GetKeyCount() == Degree * 2)
            Split(parent);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Pop(Node* node, const Key& key) {
        auto* parent = node->GetParent();

        node->PopKey(key);

        if (node == m_Root) {
            if (node->GetKeyCount() == 0) {
                if (node->IsLeaf()) {
                    m_Root = nullptr;
                } else {
                    m_Root = INTERNAL_NODE(node)->GetMinChild();
                    m_Root->GetParent() = nullptr;
                }
            }

            return;
        }

        if (node->GetKeyCount() >= Degree - 1)
            return;

        auto  index        = parent->GetKeyIndex(node->GetMinKey());
        auto* leftSibling  = node->GetLeftSibling();
        auto* rightSibling = node->GetRightSibling();

        std::cout << "------- Index = " << index << ", Parent keys = ";
        for (const auto& k : parent->GetKeys())
            std::cout << k << " ";
        std::cout << std::endl;

        if (node->IsInternal()) {
            if (leftSibling && leftSibling->GetKeyCount() > Degree - 1) {
                node->BorrowLeft();
            } else if (rightSibling && rightSibling->GetKeyCount() > Degree - 1) {
                node->BorrowRight();
            } else if (leftSibling) {
                leftSibling->PushKey(parent->GetKey(index - 1));
                node->MergeLeft();

//                std::cout << "Index = " << index << "Parent keys = ";
//                for (const auto& k : parent->GetKeys())
//                    std::cout << k << " ";
//                std::cout << std::endl;

                Pop(parent, parent->GetKey(index - 1));
            } else if (rightSibling) {
                node->PushKey(parent->GetKey(index));
                node->MergeRight();

//                std::cout << "Index = " << index << "Parent keys = ";
//                for (const auto& k : parent->GetKeys())
//                    std::cout << k << " ";
//                std::cout << std::endl;

                Pop(parent, parent->GetKey(index));
            }
        } else if (node->IsLeaf()) {
            if (leftSibling && leftSibling->GetKeyCount() > Degree - 1) {
                node->BorrowLeft();
            } else if (rightSibling && rightSibling->GetKeyCount() > Degree - 1) {
                node->BorrowRight();
            } else if (leftSibling) {
                node->MergeLeft();

                if (parent == leftSibling->GetParent())
                    Pop(parent, parent->GetKey(index - 1));
                else
                    Pop(parent, parent->GetMinKey());
            } else if (rightSibling) {
                node->MergeRight();

                if (parent == rightSibling->GetParent())
                    Pop(parent, parent->GetKey(index));
                else
                    Pop(rightSibling->GetParent(), rightSibling->GetParent()->GetMinKey());
            }
        }
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Print(const Node* node, int level) const {
        if (!node)
            return;

        for (int i = 0; i < level; ++i)
            std::cout << "  ";

        for (const auto& key : node->GetKeys())
            std::cout << key << " ";
        std::cout << " (" << node->IsLeaf() << ") |  P: ";

        if (node->GetParent())
            std::cout << *node->GetParent()->GetKeys().begin();
        else
            std::cout << "Null";
        std::cout << ", ";

//        std::cout << node->GetParent() << ", ";

        std::cout << "LS: ";
        if (node->GetLeftSibling())
            std::cout << *node->GetLeftSibling()->GetKeys().begin();
        else
            std::cout << "Null";
        std::cout << ", ";

        std::cout << "RS: ";
        if (node->GetRightSibling())
            std::cout << *node->GetRightSibling()->GetKeys().begin() << std::endl;
        else
            std::cout << "Null" << std::endl;

        if (node->IsInternal())
            for (const auto& child : CONST_INTERNAL_NODE(node)->GetChildren())
                Print(child, level + 1);
    }

} // namespace Ng