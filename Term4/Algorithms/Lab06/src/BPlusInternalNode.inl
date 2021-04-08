namespace Ng {

    template <typename Key, typename Value>
    BPlusInternalNode<Key, Value>::BPlusInternalNode()
        : BPlusNode(BPlusNodeType::Internal) {}

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetChildByKey(const Key& key) const {
        BPlusNode* result = nullptr;

        for (int i = 0; i <= this->m_Keys.size(); ++i) {
            if (i == this->m_Keys.size() || key < this->m_Keys[i]) {
                result = m_Children[i];
                break;
            }
        }

        return result;
    }

    template <typename Key, typename Value>
    std::size_t BPlusInternalNode<Key, Value>::GetChildIndex(const Key& key) const {
        std::size_t index;

        for (int i = 0; i <= this->m_Keys.size(); ++i) {
            if (i == this->m_Keys.size() || key < this->m_Keys[i]) {
                index = i;
                break;
            }
        }

        return index;
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusInternalNode<Key, Value>::GetChildByIndex(std::size_t index) const {
        return m_Children[index];
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::UpdateKeys() {
        auto* node = this;

        while (node) {
//            std::cout << "---------------- Current node in update = ";
//            for (const auto& k : node->GetKeys())
//                std::cout << k << " ";
//            std::cout << std::endl;

            for (int i = 0; i < node->m_Keys.size(); ++i)
                node->m_Keys[i] = node->m_Children[i + 1]->GetMinNode()->GetMinKey();

//            std::cout << "---------------- after update Current node in update = ";
//            for (const auto& k : node->GetKeys())
//                std::cout << k << " ";
//            std::cout << std::endl;

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
//        auto childIterator = std::find(m_Children.begin(), m_Children.end(), child);
//
//        if (childIterator == m_Children.end())
//            return;
//
//        auto index = std::distance(m_Children.begin(), childIterator);
//
//        m_Children.erase(m_Children.begin() + index);
//
////        if (index == 0) {
////            auto result = this->m_Keys.front();
////            this->m_Keys.erase(this->m_Keys.begin());
////            return;
////        }
//
//        auto result = this->m_Keys[index - 1];
////        this->m_Keys.erase(this->m_Keys.begin() + index - 1);

        m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child));
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::PopKey(const Key& key) {
//        std::cout << "----- Pop key in Internal Node = " << key << ", size = " << this->m_Keys.size() << ", index = ";

        std::size_t index = 0;

        while (index < this->m_Keys.size() - 1 && key > this->m_Keys[index])
            ++index;

//        std::cout << index << std::endl;

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
    void BPlusInternalNode<Key, Value>::BorrowLeft() {
        auto* leftSibling = static_cast<decltype(this)>(this->m_LeftSibling);

        this->m_Keys.insert(this->m_Keys.begin(), leftSibling->GetMaxKey());
        leftSibling->m_Keys.pop_back();

        (*this->m_Children.insert(this->m_Children.begin(), leftSibling->GetMaxChild()))->m_Parent = this;
        leftSibling->m_Children.pop_back();

        this->UpdateKeys();
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::BorrowRight() {
        auto* rightSibling = static_cast<decltype(this)>(this->m_RightSibling);

        this->m_Keys.emplace_back(rightSibling->GetMinKey());
        rightSibling->m_Keys.erase(rightSibling->m_Keys.begin());

        this->m_Children.emplace_back(rightSibling->GetMinChild())->m_Parent = this;
        rightSibling->m_Children.erase(rightSibling->m_Children.begin());

        this->UpdateKeys();
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::MergeLeft() {
        auto* leftSibling = static_cast<decltype(this)>(this->m_LeftSibling);

//        std::cout << "Before merging: " << this->m_Keys.size() << ", " << this->m_Children.size() << std::endl;

        this->m_Keys.insert(this->m_Keys.begin(), leftSibling->GetMinKey());

        this->m_Keys.insert(
            this->m_Keys.begin(),
            leftSibling->m_Keys.begin(),
            leftSibling->m_Keys.end()
        );

        this->m_Children.insert(
            this->m_Children.begin(),
            leftSibling->m_Children.begin(),
            leftSibling->m_Children.end()
        );

//        std::cout << "After merging: " << this->m_Keys.size() << ", " << this->m_Children.size() << std::endl;

        for (auto& child : this->m_Children)
            child->m_Parent = this;

        if (leftSibling->m_LeftSibling)
            leftSibling->m_LeftSibling->m_RightSibling = this;

        this->m_LeftSibling = leftSibling->m_LeftSibling;

        this->UpdateKeys();

//        leftSibling->m_Parent->PopKey(leftSibling->GetMinKey());
    }

    template <typename Key, typename Value>
    void BPlusInternalNode<Key, Value>::MergeRight() {
        auto* rightSibling = static_cast<decltype(this)>(this->m_RightSibling);

//        std::cout << "Before merging: " << this->m_Keys.size() << ", " << this->m_Children.size() << std::endl;

        this->m_Keys.push_back(rightSibling->GetMinKey());

        this->m_Keys.insert(
            this->m_Keys.end(),
            rightSibling->m_Keys.begin(),
            rightSibling->m_Keys.end()
        );

        this->m_Children.insert(
            this->m_Children.end(),
            rightSibling->m_Children.begin(),
            rightSibling->m_Children.end()
        );

//        std::cout << "After merging: " << this->m_Keys.size() << ", " << this->m_Children.size() << std::endl;

        for (auto& child : this->m_Children)
            child->m_Parent = this;

        if (rightSibling->m_RightSibling)
            rightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = rightSibling->m_RightSibling;

        this->UpdateKeys();

//        rightSibling->m_Parent->PopKey(rightSibling->GetMinKey());
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