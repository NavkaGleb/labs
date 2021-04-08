namespace Ng {

    template <typename Key, typename Value>
    BPlusLeafNode<Key, Value>::BPlusLeafNode()
        : BPlusNode(BPlusNodeType::Leaf) {}

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::PopKey(const Key& key) {
        std::size_t index = 0;

        while (index < this->m_Keys.size() && key > this->m_Keys[index])
            ++index;

        this->m_Keys.erase(this->m_Keys.begin() + index);
//        this->m_Data.erase(index);
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::Split(typename BPlusNode::KeyIterator separator) {
        auto* sibling = new BPlusLeafNode<Key, Value>;

        sibling->m_Parent = this->m_Parent;

        sibling->m_Keys.insert(sibling->m_Keys.begin(), separator, this->m_Keys.end());
        this->m_Keys.erase(separator, this->m_Keys.end());

        return sibling;
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::BorrowLeft() {
        auto* leftSibling = static_cast<decltype(this)>(this->m_LeftSibling);

        this->m_Keys.insert(this->m_Keys.begin(), leftSibling->GetMaxKey());
        leftSibling->m_Keys.pop_back();

        this->m_Parent->UpdateKeys();
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::BorrowRight() {
        auto* rightSibling = static_cast<decltype(this)>(this->m_RightSibling);

        this->m_Keys.push_back(rightSibling->GetMinKey());
        rightSibling->m_Keys.erase(rightSibling->m_Keys.begin());

        this->m_Parent->UpdateKeys();
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::MergeLeft() {
        auto* leftSibling = static_cast<decltype(this)>(this->m_LeftSibling);

        this->m_Keys.insert(
            this->m_Keys.begin(),
            leftSibling->m_Keys.begin(),
            leftSibling->m_Keys.end()
        );

        if (leftSibling->m_LeftSibling)
            leftSibling->m_LeftSibling->m_RightSibling = this;

        this->m_LeftSibling = leftSibling->m_LeftSibling;
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::MergeRight() {
        auto* rightSibling = static_cast<decltype(this)>(this->m_RightSibling);

        this->m_Keys.insert(
            this->m_Keys.end(),
            rightSibling->m_Keys.begin(),
            rightSibling->m_Keys.end()
        );

        if (rightSibling->m_RightSibling)
            rightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = rightSibling->m_RightSibling;
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMinNode() {
        return this;
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMinNode() const {
        return this;
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMaxNode() {
        return this;
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMaxNode() const {
        return this;
    }

} // namespace Ng