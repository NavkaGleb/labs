#pragma once

#include <memory>

namespace Ng {

    template <typename Key, typename Value>
    class PersistentTree;

    template <typename Key, typename Value>
    class PersistentNode;

    template <typename Key, typename Value>
    using PersistentNodePtr = std::shared_ptr<const PersistentNode<Key, Value>>;

    template <typename Key, typename Value>
    class PersistentNode : public std::enable_shared_from_this<const PersistentNode<Key, Value>> {
    public:
        friend class PersistentTree<Key, Value>;

    public:
        enum class Color { Red = 0, Black };

    public:
        using KeyType      = Key;
        using ValueType    = Value;
        using Pair         = std::pair<const Key, Value>;
        using SmartPointer = PersistentNodePtr<Key, Value>;

    public:
        explicit PersistentNode(
            const Pair&         pair,
            const SmartPointer& left  = nullptr,
            const SmartPointer& right = nullptr,
            Color               color = Color::Red
        );
        ~PersistentNode() = default;

        [[nodiscard]] inline bool IsRed() const { return m_Color == Color::Red; }
        [[nodiscard]] inline bool IsBlack() const { return m_Color == Color::Black; }

        [[nodiscard]] SmartPointer CloneWithPair(const Pair& pair) const;
        [[nodiscard]] SmartPointer CloneWithLeft(const SmartPointer& left) const;
        [[nodiscard]] SmartPointer CloneWithRight(const SmartPointer& right) const;
        [[nodiscard]] SmartPointer CloneAsRed() const;
        [[nodiscard]] SmartPointer CloneAsBlack() const;

        SmartPointer Balance() const;

        void Print(const std::string& indent = std::string()) const;

    public:
        static std::pair<SmartPointer, bool> Push(const SmartPointer& node, const Key& key, const Value& value);
        static std::pair<SmartPointer, bool> Pop(const SmartPointer& node, const Key& key);

        static std::pair<SmartPointer, bool> PopLeft(const SmartPointer& node, const Key& key);
        static std::pair<SmartPointer, bool> PopRight(const SmartPointer& node, const Key& key);

        static SmartPointer Fuse(const SmartPointer& left, const SmartPointer& right);
        static SmartPointer Balance(const SmartPointer& node);
        static SmartPointer BalanceLeft(const SmartPointer& node);
        static SmartPointer BalanceRight(const SmartPointer& node);

    private:
        Pair         m_Pair;
        SmartPointer m_Left;
        SmartPointer m_Right;
        Color        m_Color;

    }; // class PersistentNode

} // namespace Ng

#include "PersistentNode.inl"