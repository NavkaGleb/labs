#include <utility>
#include <stdexcept>
#include <iostream>
#include <stack>

namespace Ng {

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::Iterator::Iterator(LeafNode* node)
        : m_Node(node)
        , m_Index(0) {}

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Iterator& BPlusTree<Key, Value, Degree>::Iterator::operator ++() {
        if (m_Index < m_Node->GetKeyCount() - 1) {
            ++m_Index;
        } else {
            m_Node  = LEAF_NODE(m_Node->GetRightSibling());
            m_Index = 0;
        }

        return *this;
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Iterator::Pair BPlusTree<
        Key,
        Value,
        Degree
    >::Iterator::operator *() {
        return m_Node->GetDataByIndex(m_Index);
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
    BPlusTree<Key, Value, Degree>::ConstIterator::ConstIterator(const LeafNode* node)
        : m_Node(node)
        , m_Index(0) {}

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::ConstIterator& BPlusTree<Key, Value, Degree>::ConstIterator::operator ++() {
        if (m_Index < m_Node->GetKeyCount() - 1) {
            ++m_Index;
        } else {
            m_Node  = CONST_LEAF_NODE(m_Node->GetRightSibling());
            m_Index = 0;
        }

        return *this;
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::ConstIterator::Pair BPlusTree<
        Key,
        Value,
        Degree
    >::ConstIterator::operator *() const {
        return m_Node->GetDataByIndex(m_Index);
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::ConstIterator::operator !=(const ConstIterator& other) const {
        return m_Node != other.m_Node;
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::ConstIterator::operator ==(const ConstIterator& other) const {
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
            node = INTERNAL_NODE(node)->GetChildByKey(key);

        return node->IsContainsKey(key);
    }

    template <typename Key, typename Value, int Degree>
    int BPlusTree<Key, Value, Degree>::GetHeight() const {
        if (!m_Root)
            return 0;

        Node* node   = m_Root;
        int   height = 0;

        while (!node->IsLeaf()) {
            node = INTERNAL_NODE(node)->GetMinChild();
            ++height;
        }

        return height;
    }

    template <typename Key, typename Value, int Degree>
    const Key& BPlusTree<Key, Value, Degree>::GetMinKey() const {
        if (!m_Root)
            throw std::out_of_range("Ng::BPlusTree::GetMinKey: Tree is empty!");

        return m_Root->GetMinNode()->GetMinKey();
    }

    template <typename Key, typename Value, int Degree>
    const Key& BPlusTree<Key, Value, Degree>::GetMaxKey() const {
        if (!m_Root)
            throw std::out_of_range("Ng::BPlusTree::GetMaxKey: Tree is empty!");

        return m_Root->GetMaxNode()->GetMaxNode();
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Clear() {
        delete m_Root;
        m_Root = nullptr;
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::Push(const Key& key, const Value& value) {
        if (!m_Root)
            m_Root = new LeafNode;

        LeafNode* node = GetLeafNode(key);

        if (node->IsContainsKey(key))
            return false;

        ++m_Count;

        node->PushKey(key);
        node->PushData(key, value);

        if (node->GetKeyCount() == Degree * 2)
            Split(node);

        return true;
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::Pop(const Key& key) {
        if (!m_Root)
            return false;

        auto* node = GetLeafNode(key);

        if (!node->IsContainsKey(key))
            return false;

        --m_Count;

        Pop(node, key);

        return true;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Print() const {
        return Print(m_Root);
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::LeafNode* BPlusTree<Key, Value, Degree>::GetLeafNode(const Key& key) {
        Node* node = m_Root;

        while (node && !node->IsLeaf())
            node = INTERNAL_NODE(node)->GetChildByKey(key);

        return LEAF_NODE(node);
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::LeafNode* BPlusTree<Key, Value, Degree>::GetMinNode() {
        return !m_Root ? nullptr : LEAF_NODE(m_Root->GetMinNode());
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::LeafNode* BPlusTree<Key, Value, Degree>::GetMaxNode() {
        return !m_Root ? nullptr : LEAF_NODE(m_Root->GetMaxNode());
    }

    template <typename Key, typename Value, int Degree>
    const typename BPlusTree<Key, Value, Degree>::LeafNode* BPlusTree<Key, Value, Degree>::GetMinNode() const {
        return !m_Root ? nullptr : CONST_LEAF_NODE(m_Root->GetMinNode());
    }

    template <typename Key, typename Value, int Degree>
    const typename BPlusTree<Key, Value, Degree>::LeafNode* BPlusTree<Key, Value, Degree>::GetMaxNode() const {
        return !m_Root ? nullptr : CONST_LEAF_NODE(m_Root->GetMaxNode());
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Split(Node* node) {
        auto  keyMedian = node->GetMedian();
        auto* sibling   = node->Split();

        if (node == m_Root) {
            auto* root = new InternalNode;

            root->PushKey(keyMedian);
            root->PushChild(node);
            root->PushChild(sibling);

            m_Root = root;

            return;
        }

        node->GetParent()->PushKey(keyMedian);
        node->GetParent()->PushChild(sibling);

        if (node->GetParent()->GetKeyCount() == Degree * 2)
            Split(node->GetParent());
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Pop(Node* node, const Key& key) {
//        std::cout << "-- Current Node " << node->IsLeaf() << " in deletion = ";
//        for (const auto& k : node->GetKeys())
//            std::cout << k << " ";
//        std::cout << ", Key to delete = " << key << ", Parent: ";
//
//        if (node->GetParent())
//            std::cout << node->GetParent()->GetMinKey() << std::endl;
//        else
//            std::cout << "Null" << std::endl;

        node->PopKey(key);

//        std::cout << "CUrrent node print = " << std::endl;
//        Print(node, 1);

        if (node == m_Root) {
            if (node->GetKeyCount() == 0 && node->IsLeaf()) {
                m_Root = nullptr;
            } else if (node->GetKeyCount() == 0 && node->IsInternal()) {
                m_Root = INTERNAL_NODE(node)->GetMinChild();
                m_Root->GetParent() = nullptr;
            }

            return;
        }

        if (node->GetKeyCount() < Degree - 1) {
            auto* leftSibling = node->GetLeftSibling();
            auto* rightSibling = node->GetRightSibling();

            if (leftSibling && leftSibling->GetKeyCount() > Degree - 1) {

//                std::cout << node->IsLeaf() << " borrow left" << std::endl;

                node->BorrowLeft();

//                std::cout << "after: " << std::endl;
//                Print(node, 1);

            } else if (rightSibling && rightSibling->GetKeyCount() > Degree - 1) {

//                std::cout << node->IsLeaf() << " borrow right" << std::endl;

                node->BorrowRight();

//                std::cout << "after: " << std::endl;
//                Print(node, 1);

            } else if (leftSibling) {

//                std::cout << node->IsLeaf() << " merge left" << std::endl;

                node->MergeLeft();

//                std::cout << "after: " << std::endl;
//                Print(node, 1);

                Pop(leftSibling->GetParent(), node->GetMinNode()->GetMinKey());

            } else if (rightSibling) {

//                std::cout << node->IsLeaf() << " merge right" << std::endl;

                node->MergeRight();

//                std::cout << "after: " << std::endl;
//                Print(node, 1);

                Pop(node->GetParent(), rightSibling->GetMinNode()->GetMinKey());

            }

//            if (m_Root->GetKeyCount() == 0)
//                m_Root = INTERNAL_NODE(m_Root)->GetMaxChild();

        } else {

            node->GetParent()->UpdateKeys();

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