#include <functional>

namespace Ng {

    template <typename Key, typename Value>
    BPlusLeafNode<Key, Value>::BPlusLeafNode()
        : BPlusNode(BPlusNodeType::Leaf) {}

    template <typename Key, typename Value>
    Value& BPlusLeafNode<Key, Value>::GetData(const Key& key) {
        auto keyIterator = std::find(this->m_Keys.begin(), this->m_Keys.end(), key);

        if (keyIterator == this->m_Keys.end())
            throw std::invalid_argument("Ng::BPlusLeafNode::GetData: Given key is not exist in node!");

        return this->m_Data[std::distance(this->m_Keys.begin(), keyIterator)];
    }

    template <typename Key, typename Value>
    const Value& BPlusLeafNode<Key, Value>::GetData(const Key& key) const {
        auto keyIterator = std::find(this->m_Keys.begin(), this->m_Keys.end(), key);

        if (keyIterator == this->m_Keys.end())
            throw std::invalid_argument("Ng::BPlusLeafNode::GetData: Given key is not exist in node!");

        return this->m_Data[std::distance(this->m_Keys.begin(), keyIterator)];
    }

    template <typename Key, typename Value>
    typename BPlusLeafNode<Key, Value>::Pair BPlusLeafNode<Key, Value>::GetDataByIndex(std::size_t index) {
        if (this->m_Keys.size() < index)
            throw std::out_of_range("Ng::BPlusLeafNode::GetDataByIndex: Index out of range");

        return { this->m_Keys[index], this->m_Data[index] };
    }

    template <typename Key, typename Value>
    typename BPlusLeafNode<Key, Value>::ConstPair BPlusLeafNode<Key, Value>::GetDataByIndex(std::size_t index) const {
        if (this->m_Keys.size() < index)
            throw std::out_of_range("Ng::BPlusLeafNode::GetDataByIndex: Index out of range");

        return { this->m_Keys[index], this->m_Data[index] };
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::PushData(const Key& key, const Value& value) {
        auto keyIterator = std::find(this->m_Keys.begin(), this->m_Keys.end(), key);

        if (keyIterator == this->m_Keys.end())
            throw std::invalid_argument("Ng::BPlusLeafNode::PushData: Given key is not exist in node!");

        this->m_Data.insert(this->m_Data.begin() + std::distance(this->m_Keys.begin(), keyIterator), value);
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::Split() {
        auto* sibling = new BPlusLeafNode<Key, Value>;

        sibling->m_Parent = this->m_Parent;

        sibling->m_Keys.insert(
            sibling->m_Keys.begin(),
            this->GetKeyMedianIterator(),
            this->m_Keys.end()
        );

        this->m_Keys.erase(this->GetKeyMedianIterator(), this->m_Keys.end());

        sibling->m_Data.insert(
            sibling->m_Data.begin(),
            this->GetDataMedianIterator(),
            this->m_Data.end()
        );

        this->m_Data.erase(this->GetDataMedianIterator(), this->m_Data.end());

        return sibling;
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::BorrowLeft() {
        auto* leftSibling = static_cast<decltype(this)>(this->m_LeftSibling);

        this->m_Keys.insert(this->m_Keys.begin(), leftSibling->GetMaxKey());
        leftSibling->m_Keys.pop_back();

        this->m_Data.insert(this->m_Data.begin(), leftSibling->m_Data.back());
        leftSibling->m_Data.pop_back();

        this->m_Parent->UpdateKeys();
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::BorrowRight() {
        auto* rightSibling = static_cast<decltype(this)>(this->m_RightSibling);

        this->m_Keys.push_back(rightSibling->GetMinKey());
        rightSibling->m_Keys.erase(rightSibling->m_Keys.begin());

        this->m_Data.push_back(rightSibling->m_Data.front());
        rightSibling->m_Data.erase(rightSibling->m_Data.begin());

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

        this->m_Data.insert(
            this->m_Data.begin(),
            leftSibling->m_Data.begin(),
            leftSibling->m_Data.end()
        );

        if (leftSibling->m_LeftSibling)
            leftSibling->m_LeftSibling->m_RightSibling = this;

        this->m_LeftSibling = leftSibling->m_LeftSibling;

        this->m_Parent->UpdateKeys();
        leftSibling->m_Parent->UpdateKeys();

        leftSibling->m_Parent->PopChild(leftSibling);
    }

    template <typename Key, typename Value>
    void BPlusLeafNode<Key, Value>::MergeRight() {
        auto* rightSibling = static_cast<decltype(this)>(this->m_RightSibling);

        this->m_Keys.insert(
            this->m_Keys.end(),
            rightSibling->m_Keys.begin(),
            rightSibling->m_Keys.end()
        );

        this->m_Data.insert(
            this->m_Data.end(),
            rightSibling->m_Data.begin(),
            rightSibling->m_Data.end()
        );

        if (rightSibling->m_RightSibling)
            rightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = rightSibling->m_RightSibling;

        this->m_Parent->UpdateKeys();
        rightSibling->m_Parent->UpdateKeys();

        rightSibling->m_Parent->PopChild(rightSibling);
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMinNode() {
        return this;
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMaxNode() {
        return this;
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMinNode() const {
        return this;
    }

    template <typename Key, typename Value>
    const BPlusNode<Key, Value>* BPlusLeafNode<Key, Value>::GetMaxNode() const {
        return this;
    }

    template <typename Key, typename Value>
    typename BPlusLeafNode<Key, Value>::DataIterator BPlusLeafNode<Key, Value>::GetDataMedianIterator() {
        return this->m_Data.begin() + this->m_Data.size() / 2;
    }

} // namespace Ng