#pragma once

#include "BPlusNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class BPlusLeafNode : public BPlusNode<Key, Value> {
    public:
        using DataContainer = std::vector<Value>;

    private:
        using BPlusNode = BPlusNode<Key, Value>;

    public:
        BPlusLeafNode();
        ~BPlusLeafNode() override = default;

        void PopKey(const Key& key) override;

        BPlusNode* Split(typename BPlusNode::KeyIterator separator) override;

        void BorrowLeft() override;
        void BorrowRight() override;
        void MergeLeft() override;
        void MergeRight() override;

        BPlusNode* GetMinNode() override;
        const BPlusNode* GetMinNode() const override;

        BPlusNode* GetMaxNode() override;
        const BPlusNode* GetMaxNode() const override;

    private:
        DataContainer m_Data;

    }; // class BPlusLeafNode

} // namespace Ng

#include "BPlusLeafNode.inl"