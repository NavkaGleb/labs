#pragma once

#include "BPlusNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class BPlusLeafNode final : public BPlusNode<Key> {
    public:
        using DataContainer = std::vector<Value>;
        using DataIterator  = typename DataContainer::iterator;

        using Pair          = std::pair<const Key&, Value&>;
        using ConstPair     = std::pair<const Key&, const Value&>;

    private:
        using BPlusNode         = BPlusNode<Key>;
        using BPlusInternalNode = BPlusInternalNode<Key>;

    public:
        explicit BPlusLeafNode(BPlusInternalNode* parent = nullptr);
        ~BPlusLeafNode() override = default;

        Value& GetData(const Key& key);
        const Value& GetData(const Key& key) const;

        Pair GetDataByIndex(std::size_t index);
        ConstPair GetDataByIndex(std::size_t index) const;

        void PushData(const Key& key, const Value& value);

        BPlusNode* Split() override;

        void BorrowLeft() override;
        void BorrowRight() override;
        void MergeLeft() override;
        void MergeRight() override;

        BPlusNode* GetMinNode() override;
        BPlusNode* GetMaxNode() override;

        const BPlusNode* GetMinNode() const override;
        const BPlusNode* GetMaxNode() const override;

    private:
        [[nodiscard]] DataIterator GetDataMedianIterator();

    private:
        DataContainer m_Data;

    }; // class BPlusLeafNode

} // namespace Ng

#include "BPlusLeafNode.inl"