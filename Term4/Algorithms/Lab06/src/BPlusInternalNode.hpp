#pragma once

#include "BPlusNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class BPlusInternalNode : public BPlusNode<Key, Value> {
    public:
        using ChildContainer     = std::vector<BPlusNode<Key, Value>*>;
        using ChildIterator      = typename ChildContainer::iterator;
        using ChildConstIterator = typename ChildContainer::const_iterator;

    private:
        using BPlusNode = BPlusNode<Key, Value>;

    public:
        BPlusInternalNode();
        ~BPlusInternalNode() override = default;

        [[nodiscard]] const ChildContainer& GetChildren() const { return m_Children; }

        [[nodiscard]] inline BPlusNode* GetMinChild() const { return m_Children.front(); }
        [[nodiscard]] inline BPlusNode* GetMaxChild() const { return m_Children.back(); }

        [[nodiscard]] BPlusNode* GetChild(const Key& key) const;

        void UpdateKeys();

        void PushChild(BPlusNode* child);
        void PopChild(BPlusNode* child);

        void PopKey(const Key& key) override;

        BPlusNode* Split(typename BPlusNode::KeyIterator separator) override;

        void MergeLeft() override;
        void MergeRight() override;

        void BorrowLeft() override;
        void BorrowRight() override;

        BPlusNode* GetMinNode() override;
        const BPlusNode* GetMinNode() const override;

        BPlusNode* GetMaxNode() override;
        const BPlusNode* GetMaxNode() const override;

    private:
        [[nodiscard]] BPlusNode* GetLeftSibling(ChildIterator it) const;
        [[nodiscard]] BPlusNode* GetRightSibling(ChildIterator it) const;

        void PushChild(ChildIterator it, BPlusNode* child);

    private:
        ChildContainer m_Children;

    }; // class BPlusInternalNode

} // namespace Ng

#include "BPlusInternalNode.inl"