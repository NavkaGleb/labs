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
        friend BPlusInternalNode<Key, Value>;
        friend BPlusLeafNode<Key, Value>;

    public:
        using KeyContainer     = std::vector<Key>;
        using KeyIterator      = typename KeyContainer::iterator;
        using KeyConstIterator = typename KeyContainer::const_iterator;

    public:
        virtual ~BPlusNode() = default;

        [[nodiscard]] inline const KeyContainer& GetKeys() const { return m_Keys; }
        [[nodiscard]] inline BPlusNode*& GetParent() { return m_Parent; }
        [[nodiscard]] inline BPlusNode* GetParent() const { return m_Parent; }
        [[nodiscard]] inline BPlusNode*& GetLeftSibling() { return m_LeftSibling; }
        [[nodiscard]] inline BPlusNode* GetLeftSibling() const { return m_LeftSibling; }
        [[nodiscard]] inline BPlusNode*& GetRightSibling() { return m_RightSibling; }
        [[nodiscard]] inline BPlusNode* GetRightSibling() const { return m_RightSibling; }

        [[nodiscard]] inline bool IsInternal() const { return m_Type == BPlusNodeType::Internal; }
        [[nodiscard]] inline bool IsLeaf() const { return m_Type == BPlusNodeType::Leaf; }

        [[nodiscard]] inline std::size_t GetKeyCount() const { return m_Keys.size(); }
        [[nodiscard]] inline const Key& GetKey(std::size_t index) const { return m_Keys[index]; }
        [[nodiscard]] inline const Key& GetMinKey() const { return m_Keys.front(); }
        [[nodiscard]] inline const Key& GetMaxKey() const { return m_Keys.back(); }

        [[nodiscard]] inline std::size_t GetKeyIndex(const Key& key) const;
        [[nodiscard]] inline bool IsContainsKey(const Key& key) const;

        [[nodiscard]] KeyIterator GetMedian();

        KeyIterator PushKey(const Key& key);
        virtual void PopKey(const Key& key) = 0;

        virtual std::pair<BPlusNode*, BPlusNode*> Split(KeyIterator separator) = 0;
        virtual void MergeLeft() = 0;
        virtual void MergeRight() = 0;

        virtual const BPlusNode* GetMinNode() const = 0;
        virtual const BPlusNode* GetMaxNode() const = 0;

    protected:
        explicit BPlusNode(BPlusNodeType type);

    protected:
        KeyContainer  m_Keys;
        BPlusNode*    m_Parent;
        BPlusNode*    m_LeftSibling;
        BPlusNode*    m_RightSibling;
        BPlusNodeType m_Type;

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

        [[nodiscard]] inline BPlusNode<Key, Value>* GetMinChild() const { return m_Children.front(); }
        [[nodiscard]] inline BPlusNode<Key, Value>* GetMaxChild() const { return m_Children.back(); }

        [[nodiscard]] BPlusNode<Key, Value>* GetChild(const Key& key) const;

        void UpdateKeys();

        void PushChild(BPlusNode<Key, Value>* child);
        Key PopChild(BPlusNode<Key, Value>* child);

        void PopKey(const Key& key) override;

        std::pair<
            BPlusNode<Key, Value>*,
            BPlusNode<Key, Value>*
        > Split(typename BPlusNode<Key, Value>::KeyIterator separator) override;
        void MergeLeft() override;
        void MergeRight() override;

        const BPlusNode<Key, Value>* GetMinNode() const override;
        const BPlusNode<Key, Value>* GetMaxNode() const override;

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

        void PopKey(const Key& key) override;

        std::pair<
            BPlusNode<Key, Value>*,
            BPlusNode<Key, Value>*
        > Split(typename BPlusNode<Key, Value>::KeyIterator separator) override;
        void MergeLeft() override;
        void MergeRight() override;

        const BPlusNode<Key, Value>* GetMinNode() const override;
        const BPlusNode<Key, Value>* GetMaxNode() const override;

    private:
        DataContainer m_Data;

    }; // class BPlusLeafNode
    
} // namespace Ng

#include "BPlusNode.inl"