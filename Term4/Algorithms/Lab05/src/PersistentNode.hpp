#pragma once

namespace Ng {

    template <typename Key, typename Value>
    class PersistentTree;

    template <typename Key, typename Value>
    class PersistentNode {
    public:
        friend class PersistentTree<Key, Value>;

    public:
        enum class Color { Red = 0, Black };

    public:
        using Pair = std::pair<const Key, Value>;

    public:
        static PersistentNode* Clone(PersistentNode* prototype, PersistentNode* parent = nullptr);

    public:
        explicit PersistentNode(const Pair& pair);
        explicit PersistentNode(Pair&& pair);
        ~PersistentNode();

        [[nodiscard]] inline std::size_t GetCount() const { return m_Count; }

        [[nodiscard]] inline bool IsRed() const { return m_Color == Color::Red; }
        [[nodiscard]] inline bool IsBlack() const { return m_Color == Color::Black; }

        void Print(const std::string& indent = std::string()) const;

    private:
        Pair m_Pair;
        PersistentNode* m_Parent;
        PersistentNode* m_Left;
        PersistentNode* m_Right;
        Color           m_Color;
        std::size_t     m_Count;

    }; // class PersistentNode

} // namespace Ng

#include "PersistentNode.inl"