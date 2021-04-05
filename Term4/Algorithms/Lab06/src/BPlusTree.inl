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

        // Borrow from left
        // Merge with left
        // Borrow from right (if no left sibling)
        // Merge with right

//        Node* node   = GetLeafNode(key);
//        Node* parent = node->m_Parent;
//
//        std::cout << "Leaf node: ";
//        for (const auto& k : node->m_Keys)
//            std::cout << k << " ";
//        std::cout << std::endl;
//
//        std::cout << "Begin = " << *node->m_Keys.begin() << std::endl;
//
//        if (node->m_Keys.size() > Degree - 1) {
//            std::cout << "Case 1" << std::endl;
//
//            if (*node->m_Keys.begin() == key) {
//                node->m_Keys.erase(key);
//                parent->m_Keys.erase(key);
//                parent->m_Keys.insert(*node->m_Keys.begin());
//
//                std::cout << "new begin = " << *node->m_Keys.begin() << std::endl;
//            } else {
//                node->m_Keys.erase(key);
//            }
//        } else {
//            Node* leftSibling = node->m_LeftSibling;
//
//            if (leftSibling->m_Parent == node->m_Parent && leftSibling->m_Keys.size() > Degree - 1) {
//                // borrow
//                auto borrowValue = *leftSibling->m_Keys.rbegin();
//                parent->m_Keys.erase(*node->m_Keys.begin());
//                parent->m_Keys.insert(borrowValue);
//                node->m_Keys.erase(key);
//                node->m_Keys.insert(borrowValue);
//                leftSibling->m_Keys.erase(borrowValue);
//                std::cout << "borrow left sibling" << std::endl;
//            } else {
//                // merge
//                std::cout << "1 merging!" << std::endl;
//
//                if (node == *parent->m_Children.begin())
//                    parent->m_Keys.erase(parent->m_Keys.begin());
//                else
//                    parent->m_Keys.erase(node->m_Keys.begin());
//
//                std::cout << "2 merging!" << std::endl;
//
//                std::cout << "Key = " << key << std::endl;
//                for (const auto& k : node->m_Keys)
//                    std::cout << key << " ";
//                std::cout << std::endl;
//
//                node->m_Keys.erase(key);
//
//                std::cout << "-- erasing key from node" << std::endl;
//
//                leftSibling->m_Keys.insert(node->m_Keys.begin(), node->m_Keys.end());
//
//                std::cout << "3m erging!" << std::endl;
//
//                parent->m_Children.erase(
//                    std::remove(
//                        parent->m_Children.begin(),
//                        parent->m_Children.end(),
//                        node
//                    )
//                );
//
//                delete node;
//
//                std::cout << "merge left sibling" << std::endl;
//            }
//        }



//        // Case 1
//        if (node->m_Keys.size() > Degree - 2) {
//            parent->m_Children.erase(
//                std::remove(
//                    parent->m_Children.begin(),
//                    parent->m_Children.end(),
//                    node
//                )
//            );
//
//            delete node;
//        }
//
//        // Case 2
//        if (node->m_Keys.size() == Degree - 2 )
//        if (parent->m_Keys.empty() && !parent->m_Children.empty()) {
//            std::cout << "push key to parent" << std::endl;
//            parent->m_Keys.insert(*parent->m_Children.front()->GetKeyMedian());
//        }
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