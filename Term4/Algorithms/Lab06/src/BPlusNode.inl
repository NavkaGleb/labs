#include <iostream>

namespace Ng {

    template <typename Key, typename Value>
    bool BPlusNode<Key, Value>::IsContainsKey(const Key& key) const {
        return std::binary_search(m_Keys.begin(), m_Keys.end(), key);
    }

    template <typename Key, typename Value>
    const Key& BPlusNode<Key, Value>::GetMedian() const {
        return m_Keys[m_Keys.size() / 2];
    }

    template <typename Key, typename Value>
    void BPlusNode<Key, Value>::PushKey(const Key& key) {
        if (m_Keys.empty()) {
            m_Keys.insert(m_Keys.end(), key);
            return;
        }

        m_Keys.insert(
            std::upper_bound(m_Keys.begin(), m_Keys.end(), key),
            key
        );
    }

    template <typename Key, typename Value>
    void BPlusNode<Key, Value>::PopKey(const Key& key) {
        m_Keys.erase(std::find(m_Keys.begin(), m_Keys.end(), key));
    }

    template <typename Key, typename Value>
    BPlusNode<Key, Value>::BPlusNode(BPlusNodeType type, BPlusInternalNode* parent)
        : m_Parent(parent)
        , m_LeftSibling(nullptr)
        , m_RightSibling(nullptr)
        , m_Type(type) {}

    template <typename Key, typename Value>
    typename BPlusNode<Key, Value>::KeyIterator BPlusNode<Key, Value>::GetKeyMedianIterator() {
        return m_Keys.begin() + m_Keys.size() / 2;
    }

    template <typename Key, typename Value>
    void BPlusNode<Key, Value>::PopLeftSibling() {
        if (this->m_LeftSibling->m_LeftSibling)
            this->m_LeftSibling->m_LeftSibling->m_RightSibling = this;

        this->m_LeftSibling = this->m_LeftSibling->m_LeftSibling;
    }

    template <typename Key, typename Value>
    void BPlusNode<Key, Value>::PopRightSibling() {
        if (this->m_RightSibling->m_RightSibling)
            this->m_RightSibling->m_RightSibling->m_LeftSibling = this;

        this->m_RightSibling = this->m_RightSibling->m_RightSibling;
    }

} // namespace Ng