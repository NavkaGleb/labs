#include <utility>
#include <stdexcept>

namespace Ng {

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

        Pop(node, key);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Print() const {
        return Print(m_Root);
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node* BPlusTree<Key, Value, Degree>::GetLeafNode(const Key& key) {
        Node* node = m_Root;

        while (!node->IsLeaf())
            node = INTERNAL_NODE(node)->GetChild(key);

        return node;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Split(Node* node) {
        auto parent                      = INTERNAL_NODE(node->GetParent());
        auto keyMedian                   = node->GetMedian();
        auto [leftSibling, rightSibling] = node->Split(keyMedian);

        if (leftSibling == m_Root) {
            auto* root = new InternalNode;

            root->PushKey(*keyMedian);
            root->PushChild(leftSibling);
            root->PushChild(rightSibling);

            m_Root = root;

            return;
        }

        parent->PushKey(*keyMedian);
        parent->PushChild(rightSibling);

        if (parent->GetKeyCount() == Degree * 2)
            Split(parent);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Pop(Node* node, const Key& key) {
        std::cout << "Node: " << node->GetMinKey() << ", key: " << key << std::endl;

        auto* parent = INTERNAL_NODE(node->GetParent());

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
        } else if (node->GetKeyCount() < Degree - 1) {
            auto  index  = parent->GetKeyIndex(node->GetMinKey());

            if (node->IsInternal()) {
                auto* internalNode = INTERNAL_NODE(node);
                auto* leftSibling  = INTERNAL_NODE(node->GetLeftSibling());
                auto* rightSibling = INTERNAL_NODE(node->GetRightSibling());

                if (leftSibling && leftSibling->GetKeyCount() > Degree - 1) {
                    std::cout << "Is Internal left sibling borrow" << std::endl;

                    internalNode->PushKey(internalNode->GetMinChild()->GetMinKey());
                    internalNode->PushChild(leftSibling->GetMaxChild());
                    internalNode->GetMinChild()->GetParent() = internalNode;

                    leftSibling->PopChild(leftSibling->GetMaxChild());

                    parent->UpdateKeys();
                } else if (rightSibling && rightSibling->GetKeyCount() > Degree - 1) {
                    std::cout << "Is Internal right sibling borrow" << std::endl;

                    internalNode->PushKey(rightSibling->GetMinChild()->GetMinKey());
                    internalNode->PushChild(rightSibling->GetMinChild());
                    internalNode->GetMaxChild()->GetParent() = internalNode;

                    rightSibling->PopChild(rightSibling->GetMinChild());

                    parent->UpdateKeys();
                } else if (leftSibling) {
                    std::cout << "Is Internal merge with left sibling" << std::endl;

                    leftSibling->PushKey(parent->GetKey(index - 1));
                    internalNode->MergeLeft();
                    Pop(parent, parent->GetKey(index - 1));
                } else if (rightSibling) {
                    std::cout << "Is Internal merge with right sibling" << std::endl;

                    rightSibling->PushKey(parent->GetKey(index));
                    internalNode->MergeRight();
                    Pop(parent, parent->GetKey(index));
                }
            } else if (node->IsLeaf()) {
                auto* leafNode     = LEAF_NODE(node);
                auto* leftSibling  = LEAF_NODE(node->GetLeftSibling());
                auto* rightSibling = LEAF_NODE(node->GetRightSibling());

                if (leftSibling && leftSibling->GetKeyCount() > Degree - 1) {
                    std::cout << "Is leaf left sibling borrow" << std::endl;

                    auto  borrowValue = leftSibling->GetMaxKey();
                    auto* leftSiblingParent = INTERNAL_NODE(leftSibling->GetParent());

                    leafNode->PushKey(borrowValue);
                    leftSibling->PopKey(borrowValue);

                    leftSiblingParent->UpdateKeys();

                    if (parent != leftSiblingParent)
                        parent->UpdateKeys();
                } else if (rightSibling && rightSibling->GetKeyCount() > Degree - 1) {
                    std::cout << "Is leaf right sibling borrow" << std::endl;

                    auto  borrowValue        = rightSibling->GetMinKey();
                    auto* rightSiblingParent = INTERNAL_NODE(rightSibling->GetParent());

                    std::cout << "rightSiblingParent: ";
                    for (const auto& k : rightSiblingParent->GetKeys())
                        std::cout << k << " ";
                    std::cout << std::endl;

                    leafNode->PushKey(borrowValue);
                    rightSibling->PopKey(borrowValue);

                    parent->UpdateKeys();

                    std::cout << "rightSiblingParent: ";
                    for (const auto& k : rightSiblingParent->GetKeys())
                        std::cout << k << " ";
                    std::cout << std::endl;

                    std::cout << "parent: ";
                    for (const auto& k : parent->GetKeys())
                        std::cout << k << " ";
                    std::cout << std::endl;

                    if (parent != rightSiblingParent)
                        rightSiblingParent->UpdateKeys();
                } else if (leftSibling) {
                    std::cout << "Is leaf merge with left sibling" << std::endl;

                    leafNode->MergeLeft();
                    Pop(parent, leftSibling->GetParent() == parent ? parent->GetKey(index - 1) : parent->GetMinKey());
                } else if (rightSibling) {
                    std::cout << "Is leaf merge with right sibling" << std::endl;

                    auto* rightSiblingParent = INTERNAL_NODE(rightSibling->GetParent());

                    leafNode->MergeRight();

                    if (parent == rightSiblingParent)
                        Pop(parent, parent->GetKey(index));
                    else
                        Pop(rightSiblingParent, rightSiblingParent->GetMinKey());
                }
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
        std::cout << "Leaf: " << node->IsLeaf() << ", P: ";

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