namespace Ng {

    template <typename Key, typename Value>
    BPlusLeafNode<Key, Value>::BPlusLeafNode()
        : BPlusNode(BPlusNodeType::Leaf) {}

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::PopKey(const Key& key) {
        this->m_Keys.erase(std::remove(this->m_Keys.begin(), this->m_Keys.end(), key));
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
        auto borrowValue = this->m_LeftSibling->GetMaxKey();

        this->PushKey(borrowValue);
        this->m_LeftSibling->PopKey(borrowValue);
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::BorrowRight() {
        auto  borrowValue        = this->m_RightSibling->GetMinKey();

        this->PushKey(borrowValue);
        this->m_RightSibling->PopKey(borrowValue);
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::MergeLeft() {
        auto* leftSibling = static_cast<BPlusLeafNode<Key, Value>*>(this->m_LeftSibling);

        leftSibling->m_Keys.insert(
            leftSibling->m_Keys.end(),
            this->m_Keys.begin(),
            this->m_Keys.end()
        );

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