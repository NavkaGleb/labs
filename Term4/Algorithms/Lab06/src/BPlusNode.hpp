#pragma once

#include <set>
#include <vector>

namespace Ng {

    enum class BPlusNodeType : int {
        Internal = 0,
        Leaf
    };

    template <typename Key, typename Value> class BPlusInternalNode;
    template <typename Key, typename Value> class BPlusLeafNode;

    template <typename Key, typename Value>
    class BPlusNode {
    public:
        using KeyContainer     = std::set<Key>;
        using KeyIterator      = typename KeyContainer::iterator;
        using KeyConstIterator = typename KeyContainer::const_iterator;

    public:
        virtual ~BPlusNode() = default;

        [[nodiscard]] inline const KeyContainer& GetKeys() const { return m_Keys; }
        [[nodiscard]] inline BPlusNode* GetParent() const { return m_Parent; }
        [[nodiscard]] inline BPlusNode* GetLeftSibling() const { return m_LeftSibling; }
        [[nodiscard]] inline BPlusNode* GetRightSibling() const { return m_RightSibling; }

        [[nodiscard]] inline std::size_t GetKeyCount() const { return m_Keys.size(); }
        [[nodiscard]] inline bool IsInternal() const { return m_Type == BPlusNodeType::Internal; }
        [[nodiscard]] inline bool IsLeaf() const { return m_Type == BPlusNodeType::Leaf; }
        [[nodiscard]] inline bool IsContainsKey(const Key& key) const { return m_Keys.contains(key); }

        [[nodiscard]] KeyIterator GetMedian() const;

        KeyIterator PushKey(const Key& key);

        virtual std::pair<BPlusNode*, BPlusNode*> Split(KeyIterator separator) = 0;

    protected:
        explicit BPlusNode(BPlusNodeType type);

    protected:
        KeyContainer  m_Keys;
        BPlusNode*    m_Parent;
        BPlusNode*    m_LeftSibling;
        BPlusNode*    m_RightSibling;
        BPlusNodeType m_Type;

    private:
//        friend class BPlusTree;
        friend BPlusInternalNode<Key, Value>;
        friend BPlusLeafNode<Key, Value>;

    }; // class BPlusNode

    template <typename Key, typename Value>
    class BPlusInternalNode : public BPlusNode<Key, Value> {
    public:
        using ChildContainer     = std::vector<BPlusNode<Key, Value>*>;
        using ChildIterator      = typename ChildContainer::iterator;
        using ChildConstIterator = typename ChildContainer::const_iterator;

    public:
        BPlusInternalNode();
        ~BPlusInternalNode() override = default;

        [[nodiscard]] const ChildContainer& GetChildren() const { return m_Children; }

        BPlusNode<Key, Value>* GetChild(const Key& key) const;

//        void ReserveChildren(std::size_t count);
        void PushChild(BPlusNode<Key, Value>* child);

        std::pair<
            BPlusNode<Key, Value>*,
            BPlusNode<Key, Value>*
        > Split(typename BPlusNode<Key, Value>::KeyIterator separator) override;

    private:
        [[nodiscard]] BPlusNode<Key, Value>* GetLeftSibling(ChildIterator it) const;
        [[nodiscard]] BPlusNode<Key, Value>* GetRightSibling(ChildIterator it) const;

        void PushChild(ChildIterator it, BPlusNode<Key, Value>* child);

    private:
        ChildContainer m_Children;

    }; // class BPlusInternalNode

    template <typename Key, typename Value>
    class BPlusLeafNode : public BPlusNode<Key, Value> {
    public:
        using DataContainer = std::vector<Value>;

    public:
        BPlusLeafNode();
        ~BPlusLeafNode() override = default;

        std::pair<
            BPlusNode<Key, Value>*,
            BPlusNode<Key, Value>*
        > Split(typename BPlusNode<Key, Value>::KeyIterator separator) override;

    private:
        DataContainer m_Data;

    }; // class BPlusLeafNode
    
} // namespace Ng

#include "BPlusNode.inl"