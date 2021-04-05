namespace Ng {

    template <typename Key, typename Value>
    BPlusNode<Key, Value>::BPlusNode(BPlusNodeType type)
        : m_Parent(nullptr)
        , m_LeftSibling(nullptr)
        , m_RightSibling(nullptr)
        , m_Type(type) {}

    template <typename Key, typename Value>
    typename BPlusNode<Key, Value>::KeyIterator BPlusNode<Key, Value>::GetMedian() const {
        auto median = m_Keys.begin();
        std::advance(median, m_Keys.size() / 2);

        return median;
    }

    template <typename Key, typename Value>
    typename BPlusNode<Key, Value>::KeyIterator BPlusNode<Key, Value>::PushKey(const Key& key) {
        return m_Keys.insert(key).first;
    }

    template <typename Key, typename Value>
    BPlusInternalNode<Key, Value>::BPlusInternalNode()
        : BPlusNode<Key, Value>(BPlusNodeType::Internal) {}

//    template <typename Key, typename Value>
//    void BPlusInternalNode<Key, Value>::ReserveChildren(std::size_t count) {
//        m_Children.reserve(count);
//    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetChild(const Key& key) const {
        if (this->m_Type == BPlusNodeType::Leaf)
            return nullptr;

        auto it    = --this->m_Keys.upper_bound(key);
        auto index = key < *this->m_Keys.begin() ? 0 : std::distance(this->m_Keys.begin(), it) + 1;

        return m_Children[index];
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PushChild(BPlusNode<Key, Value>* child) {
        child->m_Parent = this;

        if (*child->m_Keys.begin() < *this->m_Keys.begin())
            return PushChild(m_Children.begin(), child);

        for (auto [it, i] = std::make_pair(this->m_Keys.begin(), 1); it != this->m_Keys.end(); ++it, ++i)
            if (*it <= *child->m_Keys.begin())
                return PushChild(m_Children.begin() + i, child);
    }

    template <typename Key, typename Value>
    std::pair<
        BPlusNode<Key, Value>*,
        BPlusNode<Key, Value>*
    > BPlusInternalNode<Key, Value>::Split(typename BPlusNode<Key, Value>::KeyIterator separator) {
        auto* sibling = new BPlusInternalNode<Key, Value>;

        sibling->m_Parent = this->m_Parent;

        sibling->m_Keys.insert(std::next(separator), this->m_Keys.end());
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
    std::pair<
        BPlusNode<Key, Value>*,
        BPlusNode<Key, Value>*
    > BPlusLeafNode<Key, Value>::Split(typename BPlusNode<Key, Value>::KeyIterator separator) {
        auto* sibling = new BPlusLeafNode<Key, Value>;

        sibling->m_Parent = this->m_Parent;

        sibling->m_Keys.insert(separator, this->m_Keys.end());
        this->m_Keys.erase(separator, this->m_Keys.end());

        return { this, sibling };
    }

} // namespace Ng