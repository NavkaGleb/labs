#pragma once

#include <vector>
#include <optional>
#include <memory>

#include "PersistentNode.hpp"

namespace Ng {

    template <typename Key, typename Value>
    class PersistentTree {
    public:
        using Node    = PersistentNode<Key, Value>;
        using NodePtr = typename Node::SmartPointer;

    public:
        PersistentTree();
        explicit PersistentTree(NodePtr root, std::size_t count);
        ~PersistentTree() = default;

        [[nodiscard]] inline std::size_t GetCount() const { return m_Count; }

        [[nodiscard]] bool IsContains(const Key& key);

        std::optional<std::reference_wrapper<const Value>> At(const Key& key) const;

        PersistentTree<Key, Value> Push(const Key& key, const Value& value);
        PersistentTree<Key, Value> Pop(const Key& key);

        void Print() const;

    private:
        NodePtr     m_Root;
        std::size_t m_Count;

    }; // class PersistentTree

} // namespace Ng

#include "PersistentTree.inl"