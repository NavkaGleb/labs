#pragma once

namespace Ng {

    template <typename Key, typename Value>
    class ITree {
    public:
        ITree() = default;
        virtual ~ITree() = default;

        [[nodiscard]] virtual inline bool IsEmpty() const = 0;
        [[nodiscard]] virtual inline int GetSize() const = 0;

        [[nodiscard]] virtual bool IsExists(const Key& key) const = 0;
        [[nodiscard]] virtual int GetHeight() const = 0;

        virtual Value& Push(const Key& key, const Value& value) = 0;
        virtual void Pop(const Key& key) = 0;

    }; // class ITree

} // namespace Ng