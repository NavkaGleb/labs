#include <iostream>

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

} // namespace Ng