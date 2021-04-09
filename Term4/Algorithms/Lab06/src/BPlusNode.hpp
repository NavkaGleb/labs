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
        using KeyContainer      = std::vector<Key>;
        using KeyIterator       = typename KeyContainer::iterator;

    protected:
        using BPlusInternalNode = BPlusInternalNode<Key, Value>;
        using BPlusLeafNode     = BPlusLeafNode<Key, Value>;

    public:
        virtual ~BPlusNode() = default;

        [[nodiscard]] inline const KeyContainer& GetKeys() const { return m_Keys; }
        [[nodiscard]] inline BPlusInternalNode*& GetParent() { return m_Parent; }
        [[nodiscard]] inline BPlusInternalNode* GetParent() const { return m_Parent; }
        [[nodiscard]] inline BPlusNode* GetLeftSibling() const { return m_LeftSibling; }
        [[nodiscard]] inline BPlusNode* GetRightSibling() const { return m_RightSibling; }

        [[nodiscard]] inline bool IsInternal() const { return m_Type == BPlusNodeType::Internal; }
        [[nodiscard]] inline bool IsLeaf() const { return m_Type == BPlusNodeType::Leaf; }

        [[nodiscard]] inline const Key& GetKey(std::size_t index) const { return m_Keys[index]; }
        [[nodiscard]] inline std::size_t GetKeyCount() const { return m_Keys.size(); }
        [[nodiscard]] inline const Key& GetMinKey() const { return m_Keys.front(); }
        [[nodiscard]] inline const Key& GetMaxKey() const { return m_Keys.back(); }

        [[nodiscard]] bool IsContainsKey(const Key& key) const;

        [[nodiscard]] inline const Key& GetMedian() const;

        KeyIterator PushKey(const Key& key);
        void PopKey(const Key& key);

        virtual BPlusNode* Split() = 0;

        virtual void BorrowLeft() = 0;
        virtual void BorrowRight() = 0;
        virtual void MergeLeft() = 0;
        virtual void MergeRight() = 0;

        virtual BPlusNode* GetMinNode() = 0;
        virtual BPlusNode* GetMaxNode() = 0;

        virtual const BPlusNode* GetMinNode() const = 0;
        virtual const BPlusNode* GetMaxNode() const = 0;

    protected:
        explicit BPlusNode(BPlusNodeType type);

        [[nodiscard]] KeyIterator GetKeyMedianIterator();

    protected:
        KeyContainer       m_Keys;
        BPlusInternalNode* m_Parent;
        BPlusNode*         m_LeftSibling;
        BPlusNode*         m_RightSibling;
        BPlusNodeType      m_Type;

    }; // class BPlusNode
    
} // namespace Ng

#include "BPlusNode.inl"