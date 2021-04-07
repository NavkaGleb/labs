namespace Ng {

    template <typename Key, typename Value>
    BPlusInternalNode<Key, Value>::BPlusInternalNode()
        : BPlusNode(BPlusNodeType::Internal) {}

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetChild(const Key& key) const {
        if (this->m_Type == BPlusNodeType::Leaf)
            return nullptr;

//        if (key < this->m_Keys.front())
//            return GetMinChild();
//
//        for (auto it = this->m_Keys.begin(); it != this->m_Keys.end(); ++it) {
//            if (*it >= key) {
//                auto index = std::distance(this->m_Keys.begin(), it) + 1;
//                return m_Children[std::distance(this->m_Keys.begin(), it) + 1];
//            }
//        }

        auto it    = std::upper_bound(this->m_Keys.begin(), this->m_Keys.end(), key);

//        std::cout << "-- Searched key = " << *it << std::endl;

        auto index = key < this->m_Keys.front() ? 0 : std::distance(this->m_Keys.begin(), it);

        return m_Children[index];
//        return GetMaxChild();
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::UpdateKeys() {
        auto* node = static_cast<BPlusNode*>(this);

        while (node) {
            if (node->IsInternal())
                for (int i = 0; i < this->m_Keys.size(); ++i)
                    this->m_Keys[i] = m_Children[i + 1]->GetMinNode()->GetMinKey();

            node = node->m_Parent;
        }
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PushChild(BPlusNode* child) {
        child->m_Parent = this;

        if (m_Children.empty() || child->GetMinKey() < m_Children.front()->GetMinKey())
            return PushChild(m_Children.begin(), child);

        if (m_Children.back()->GetMinKey() < child->GetMinKey())
            return PushChild(m_Children.end(), child);

        for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
            if ((*it)->GetMinKey() > child->GetMinKey())
                return PushChild(it, child);
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PopChild(BPlusNode* child) {
        auto childIterator = std::find(m_Children.begin(), m_Children.end(), child);

        if (childIterator == m_Children.end())
            return;

        auto index = std::distance(m_Children.begin(), childIterator);

        m_Children.erase(m_Children.begin() + index);

        if (index == 0) {
            auto result = this->m_Keys.front();
            this->m_Keys.erase(this->m_Keys.begin());
            return;
        }

        auto result = this->m_Keys[index - 1];
        this->m_Keys.erase(this->m_Keys.begin() + index - 1);
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PopKey(const Key& key) {
        auto keyIterator = std::find(this->m_Keys.begin(), this->m_Keys.end(), key);

        if (keyIterator == this->m_Keys.end())
            return;

        auto index = std::distance(this->m_Keys.begin(), keyIterator);

        this->m_Keys.erase(this->m_Keys.begin() + index);
        this->m_Children.erase(this->m_Children.begin() + index + 1);
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::Split(typename BPlusNode::KeyIterator separator) {
        auto* sibling = new BPlusInternalNode<Key, Value>;

        sibling->m_Parent = this->m_Parent;

        sibling->m_Keys.insert(sibling->m_Keys.begin(), std::next(separator), this->m_Keys.end());
        this->m_Keys.erase(separator, this->m_Keys.end());

        auto* thisInternal    = static_cast<BPlusInternalNode<Key, Value>*>(this);
        auto* siblingInternal = static_cast<BPlusInternalNode<Key, Value>*>(sibling);
        auto  childSeparator  = thisInternal->m_Children.begin() + this->m_Keys.size() + 1;

        siblingInternal->m_Children.reserve(std::distance(childSeparator, thisInternal->m_Children.end()));

        for (auto it = childSeparator; it != thisInternal->m_Children.end(); ++it)
            siblingInternal->m_Children.emplace_back(*it)->m_Parent = siblingInternal;

        thisInternal->m_Children.erase(childSeparator, thisInternal->m_Children.end());

        return sibling;
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::MergeLeft() {
        auto* leftSibling = static_cast<BPlusInternalNode<Key, Value>*>(this->m_LeftSibling);

        leftSibling->m_Keys.insert(
            leftSibling->m_Keys.end(),
            this->m_Keys.begin(),
            this->m_Keys.end()
        );

        leftSibling->m_Children.insert(
            leftSibling->m_Children.end(),
            m_Children.begin(),
            m_Children.end()
        );

        for (auto& child : leftSibling->m_Children)
            child->m_Parent = leftSibling;

        if (this->m_RightSibling)
            this->m_RightSibling->m_LeftSibling = this->m_LeftSibling;

        this->m_LeftSibling->m_RightSibling = this->m_RightSibling;
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::BorrowLeft() {
        auto* borrowChild = GetMaxChild();

        static_cast<decltype(this)>(this->m_LeftSibling)->PopChild(borrowChild);

        borrowChild->m_Parent = this;

        this->PushKey(GetMinChild()->GetMinKey());

        m_Children.insert(m_Children.begin(), borrowChild);
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::BorrowRight() {
        auto* borrowChild = static_cast<decltype(this)>(this->m_RightSibling)->GetMinChild();

        borrowChild->m_Parent = this;

        static_cast<decltype(this)>(this->m_RightSibling)->PopChild(borrowChild);

        this->PushKey(static_cast<decltype(this)>(this->m_RightSibling)->GetMinChild()->GetMinKey());
        m_Children.emplace_back(borrowChild);
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::MergeRight() {
        auto* rightSibling = static_cast<BPlusInternalNode<Key, Value>*>(this->m_RightSibling);

        this->m_Keys.insert(
            this->m_Keys.end(),
            rightSibling->m_Keys.begin(),
            rightSibling->m_Keys.end()
        );

        m_Children.insert(
            m_Children.end(),
            rightSibling->m_Children.begin(),
            rightSibling->m_Children.end()
        );

        for (auto& child : this->m_Children)
            child->m_Parent = this;

        if (rightSibling->m_RightSibling)
            rightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = rightSibling->m_RightSibling;
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetMinNode() {
        return m_Children.front()->IsLeaf() ? m_Children.front() : m_Children.front()->GetMinNode();
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetMinNode() const {
        return m_Children.front()->IsLeaf() ? m_Children.front() : m_Children.front()->GetMinNode();
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetMaxNode() {
        return m_Children.front()->IsLeaf() ? m_Children.back() : m_Children.back()->GetMaxNode();
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetMaxNode() const {
        return m_Children.front()->IsLeaf() ? m_Children.back() : m_Children.back()->GetMaxNode();
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetLeftSibling(ChildIterator it) const {
        return it == m_Children.begin() ? (*std::next(it))->m_LeftSibling : *std::prev(it);
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetRightSibling(ChildIterator it) const {
        return std::next(it) == m_Children.end() ? (*std::prev(it))->m_RightSibling : *std::next(it);
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PushChild(ChildIterator it, BPlusNode* child) {
        it = m_Children.insert(it, child);

        if (m_Children.size() == 1)
            return;

        auto* leftSibling  = GetLeftSibling(it);
        auto* rightSibling = GetRightSibling(it);

        child->m_LeftSibling  = leftSibling;
        child->m_RightSibling = rightSibling;

        if (leftSibling)
            leftSibling->m_RightSibling = child;

        if (rightSibling)
            rightSibling->m_LeftSibling = child;
    }

} // namespace Ng