#include <utility>
#include <stdexcept>

namespace Ng {

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::Node::Node(bool isLeaf)
        : m_Parent(nullptr)
        , m_LeftSibling(nullptr)
        , m_RightSibling(nullptr)
        , m_IsLeaf(isLeaf) {}

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node::KeyIterator BPlusTree<Key, Value, Degree>::Node::GetKeyMedian() {
        auto median = m_Keys.begin();
        std::advance(median, m_Keys.size() / 2);

        return median;
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<
        Key,
        Value,
        Degree
    >::Node::ChildIterator BPlusTree<Key, Value, Degree>::Node::GetChildIterator(KeyIterator keyMedian) {
        return m_Children.begin() + std::distance(m_Keys.begin(), keyMedian) + 1;
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node* BPlusTree<Key, Value, Degree>::Node::GetChild(const Key& key) {
        if (m_IsLeaf)
            return nullptr;

        auto it    = --m_Keys.upper_bound(key);
        auto index = key < *m_Keys.begin() ? 0 : std::distance(m_Keys.begin(), it) + 1;

        return m_Children[index];
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<
        Key,
        Value,
        Degree
    >::Node::KeyIterator BPlusTree<Key, Value, Degree>::Node::PushKey(const Key& key) {
        return m_Keys.insert(key).first;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Node::PushKeys(KeyIterator begin, KeyIterator end) {
        m_Keys.insert(begin, end);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Node::PushChild(Node* child) {
        PushChild(m_Children.end(), child);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Node::PushChild(ChildIterator iterator, Node* child) {
        iterator = m_Children.insert(iterator, child);

        child->m_Parent = this;

        Node* leftSibling  = iterator            == m_Children.begin() ? nullptr : *std::prev(iterator);
        Node* rightSibling = std::next(iterator) == m_Children.end()   ? nullptr : *std::next(iterator);

        child->m_LeftSibling  = leftSibling;
        child->m_RightSibling = rightSibling;

        if (leftSibling)
            leftSibling->m_RightSibling = child;

        if (rightSibling)
            rightSibling->m_LeftSibling = child;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Node::PushChildren(ChildIterator begin, ChildIterator end) {
        m_Children.reserve(std::distance(begin, end));

        for (auto it = begin; it != end; ++it)
            m_Children.emplace_back(*it)->m_Parent = this;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Node::PopChildren(ChildIterator begin, ChildIterator end) {
        m_Children.erase(begin, end);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Node::PopKeys(KeyIterator begin, KeyIterator end) {
        m_Keys.erase(begin, end);
    }

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::BPlusTree()
        : m_Root(nullptr) {

        static_assert(Degree > 1, "Given unacceptable tree Degree!");
    }

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::BPlusTree(BPlusTree<Key, Value, Degree>&& other) noexcept
        : m_Root(std::exchange(other.m_Root, nullptr)) {}

    template <typename Key, typename Value, int Degree>
    BPlusTree<Key, Value, Degree>::~BPlusTree() {
        delete m_Root;
    }

    template <typename Key, typename Value, int Degree>
    bool BPlusTree<Key, Value, Degree>::IsExists(const Key& key) const {
        Node* node = m_Root;

        while (!node->IsLeaf())
            node = node->GetChild(key);

        return node->IsContainsKey(key);
    }

    template <typename Key, typename Value, int Degree>
    Value BPlusTree<Key, Value, Degree>::Search(const Key& key) const {
        return Search(m_Root, key);
    }

    template <typename Key, typename Value, int Degree>
    Value& BPlusTree<Key, Value, Degree>::Push(const Key& key, const Value& value) {
        if (!m_Root)
            m_Root = new Node;

        Node* node = GetLeafNode(key);

        if (node->IsContainsKey(key))
            return const_cast<Value&>(value);

        node->PushKey(key);

        if (node->IsFull())
            Split(node);

        return const_cast<Value&>(value);
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Print() const {
        return Print(m_Root);
    }

    template <typename Key, typename Value, int Degree>
    typename BPlusTree<Key, Value, Degree>::Node* BPlusTree<Key, Value, Degree>::GetLeafNode(const Key& key) {
        Node* node = m_Root;

        while (!node->IsLeaf())
            node = node->GetChild(key);

        return node;
    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Split(Node* node) {
        auto parent      = node->GetParent();
        auto keyMedian   = node->GetKeyMedian();
        auto childMedian = node->GetChildIterator(keyMedian);

        Node* sibling = new Node(node->IsLeaf());

        sibling->PushKeys(sibling->IsLeaf() ? keyMedian : std::next(keyMedian), node->GetEndKey());
        node->PopKeys(keyMedian, node->GetEndKey());

        if (!node->IsChildrenEmpty()) {
            sibling->PushChildren(childMedian, node->GetEndChild());
            node->PopChildren(childMedian, node->GetEndChild());
        }

        if (node == m_Root) {
            Node* root = new Node(false);

            root->PushKey(*keyMedian);
            root->PushChild(node);
            root->PushChild(sibling);

            m_Root = root;

            return;
        }

        auto keyIterator = parent->PushKey(*keyMedian);

        parent->PushChild(parent->GetChildIterator(keyIterator), sibling);

        if (parent->IsFull())
            Split(parent);
    }

    template <typename Key, typename Value, int Degree>
    Value BPlusTree<Key, Value, Degree>::Search(const Node* node, const Key& key) const {

    }

    template <typename Key, typename Value, int Degree>
    void BPlusTree<Key, Value, Degree>::Print(const Node* node, int level) const {
        for (int i = 0; i < level; ++i)
            std::cout << "  ";

        for (const auto& key : node->GetKeys())
            std::cout << key << " ";
        std::cout << "Left: " << node->IsLeaf() << ", P: ";

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

        for (const auto& child : node->GetChildren())
            Print(child, level + 1);
    }

} // namespace Ng