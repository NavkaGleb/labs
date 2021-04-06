namespace Ng {

    template <typename Key, typename Value>
    BPlusNode<Key, Value>::BPlusNode(BPlusNodeType type)
        : m_Parent(nullptr)
        , m_LeftSibling(nullptr)
        , m_RightSibling(nullptr)
        , m_Type(type) {}

    template <typename Key, typename Value>
    std::size_t BPlusNode<Key, Value>::GetKeyIndex(const Key& key) const {
        return std::distance(
            m_Keys.begin(),
            std::upper_bound(
                m_Keys.begin(),
                m_Keys.end(),
                key
            )
        );
    }

    template <typename Key, typename Value>
    bool BPlusNode<Key, Value>::IsContainsKey(const Key& key) const {
        return std::binary_search(m_Keys.begin(), m_Keys.end(), key);
    }

    template <typename Key, typename Value>
    typename BPlusNode<Key, Value>::KeyIterator BPlusNode<Key, Value>::GetMedian() {
        return m_Keys.begin() + m_Keys.size() / 2;
    }

    template <typename Key, typename Value>
    typename BPlusNode<Key, Value>::KeyIterator BPlusNode<Key, Value>::PushKey(const Key& key) {
        if (m_Keys.empty())
            return m_Keys.insert(m_Keys.end(), key);

        return m_Keys.insert(
            std::upper_bound(m_Keys.begin(), m_Keys.end(), key),
            key
        );
    }

    template <typename Key, typename Value>
    void BPlusNode<Key, Value>::PopKey(const Key& key) {
        m_Keys.erase(std::remove(m_Keys.begin(), m_Keys.end(), key));
    }

    template <typename Key, typename Value>
    BPlusInternalNode<Key, Value>::BPlusInternalNode()
        : BPlusNode<Key, Value>(BPlusNodeType::Internal) {}

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetChild(const Key& key) const {
        if (this->m_Type == BPlusNodeType::Leaf)
            return nullptr;

        auto it    = std::prev(std::upper_bound(this->m_Keys.begin(), this->m_Keys.end(), key));
        auto index = key < *this->m_Keys.begin() ? 0 : std::distance(this->m_Keys.begin(), it) + 1;

        return m_Children[index];
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::UpdateKeys() {
        auto* node = static_cast<BPlusNode<Key, Value>*>(this);

        while (node) {
            if (node->IsInternal())
                for (int i = 0; i < this->m_Keys.size(); ++i)
                    this->m_Keys[i] = m_Children[i + 1]->GetMinNode()->GetMinKey();

            node = node->m_Parent;
        }
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PushChild(BPlusNode<Key, Value>* child) {
        child->m_Parent = this;

        if (m_Children.empty() || *child->m_Keys.begin() < *this->m_Keys.begin())
            return PushChild(m_Children.begin(), child);

        for (auto it = this->m_Keys.begin(); it != this->m_Keys.end(); ++it)
            if (*it >= child->m_Keys.front())
                return PushChild(m_Children.begin() + std::distance(this->m_Keys.begin(), it) + 1, child);

        return PushChild(m_Children.end(), child);
    }

    template <typename Key, typename Value>
    Key BPlusInternalNode<Key, Value>::PopChild(BPlusNode<Key, Value>* child) {
        auto index = std::distance(m_Children.begin(), std::find(m_Children.begin(), m_Children.end(), child));

        m_Children.erase(m_Children.begin() + index);

        if (index == 0) {
            auto result = this->m_Keys.front();
            this->m_Keys.erase(this->m_Keys.begin());
            return result;
        }

        auto result = this->m_Keys[index - 1];
        this->m_Keys.erase(this->m_Keys.begin() + index - 1);
        return result;
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PopKey(const Key& key) {
        auto index = std::distance(
            this->m_Keys.begin(),
            std::find(
                this->m_Keys.begin(),
                this->m_Keys.end(),
                key
            )
        );

        this->m_Keys.erase(this->m_Keys.begin() + index);
        this->m_Children.erase(this->m_Children.begin() + index + 1);
    }

    template <typename Key, typename Value>
    std::pair<
        BPlusNode<Key, Value>*,
        BPlusNode<Key, Value>*
    > BPlusInternalNode<Key, Value>::Split(typename BPlusNode<Key, Value>::KeyIterator separator) {
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

        return { this, sibling };
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

//        leftSibling->m_Parent = this->m_Parent;

        if (this->m_RightSibling)
            this->m_RightSibling->m_LeftSibling = this->m_LeftSibling;

        this->m_LeftSibling->m_RightSibling = this->m_RightSibling;

//        PopChild(this);
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

//        this->m_Parent = rightSibling->m_Parent;

        if (rightSibling->m_RightSibling)
            rightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = rightSibling->m_RightSibling;

//        PopChild(rightSibling);
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetMinNode() const {
        return m_Children.front()->IsLeaf() ? m_Children.front() : m_Children.front()->GetMinNode();
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetMaxNode() const {
        return m_Children.front()->IsLeaf() ? m_Children.back() : m_Children.back()->GetMaxNode();
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetLeftSibling(ChildIterator it) const {
        return it == m_Children.begin() ? nullptr : *std::prev(it);
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetRightSibling(ChildIterator it) const {
        return std::next(it) == m_Children.end() ? nullptr : *std::next(it);
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PushChild(ChildIterator it, BPlusNode<Key, Value>* child) {
        it = m_Children.insert(it, child);

        auto* leftSibling  = GetLeftSibling(it);
        auto* rightSibling = GetRightSibling(it);

        child->m_LeftSibling  = leftSibling;
        child->m_RightSibling = rightSibling;

        if (leftSibling)
            leftSibling->m_RightSibling = child;

        if (rightSibling)
            rightSibling->m_LeftSibling = child;
    }

    template <typename Key, typename Value>
    BPlusLeafNode<Key, Value>::BPlusLeafNode()
        : BPlusNode<Key, Value>(BPlusNodeType::Leaf) {}

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::PopKey(const Key& key) {
        this->m_Keys.erase(std::remove(this->m_Keys.begin(), this->m_Keys.end(), key));
    }

    template <typename Key, typename Value>
    std::pair<
        BPlusNode<Key, Value>*,
        BPlusNode<Key, Value>*
    > BPlusLeafNode<Key, Value>::Split(typename BPlusNode<Key, Value>::KeyIterator separator) {
        auto* sibling = new BPlusLeafNode<Key, Value>;

        sibling->m_Parent = this->m_Parent;

        sibling->m_Keys.insert(sibling->m_Keys.begin(), separator, this->m_Keys.end());
        this->m_Keys.erase(separator, this->m_Keys.end());

        return { this, sibling };
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::MergeLeft() {
        auto* leftSibling = static_cast<BPlusLeafNode<Key, Value>*>(this->m_LeftSibling);

        leftSibling->m_Keys.insert(
            leftSibling->m_Keys.end(),
            this->m_Keys.begin(),
            this->m_Keys.end()
        );

//        leftSibling->m_Parent = this->m_Parent;

        if (this->m_RightSibling)
            this->m_RightSibling->m_LeftSibling = this->m_LeftSibling;

        this->m_LeftSibling->m_RightSibling = this->m_RightSibling;
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::MergeRight() {
        auto* rightSibling = static_cast<BPlusLeafNode<Key, Value>*>(this->m_RightSibling);

        this->m_Keys.insert(
            this->m_Keys.end(),
            rightSibling->m_Keys.begin(),
            rightSibling->m_Keys.end()
        );

//        this->m_Parent = rightSibling->m_Parent;

        if (rightSibling->m_RightSibling)
            rightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = rightSibling->m_RightSibling;
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMinNode() const {
        return this;
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMaxNode() const {
        return this;
    }

} // namespace Ng