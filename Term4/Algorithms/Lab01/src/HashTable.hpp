#pragma once

#include <array>

#include "UniversalHash.hpp"

namespace Ng {

    ///////////////////////////////////////////////////////////////////////
    /// Header
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, std::size_t Size, typename Hash = UniversalHash<Key>>
    class StaticHashTable {
    private:
        struct Node;

    public:
        using NodeContainer     = std::array<Node, Size>;
        using NodeIterator      = typename NodeContainer::iterator;
        using NodeConstIterator = typename NodeContainer::const_iterator;

        class Iterator {
        public:
            explicit Iterator(NodeContainer& nodes, std::size_t index);

            Iterator& operator ++() noexcept;
            Key& operator *() noexcept;

            bool operator ==(const Iterator& other) const noexcept;
            bool operator !=(const Iterator& other) const noexcept;

            friend class StaticHashTable;

        private:
            NodeContainer& m_Nodes;
            NodeIterator   m_NodeIterator;

        }; // class Iterator

        StaticHashTable();
        virtual ~StaticHashTable() = default;

        [[nodiscard]] inline std::size_t GetMaxSize() const { return Size; }
        [[nodiscard]] inline std::size_t GetSize() const { return m_Size; }

        Iterator Push(const Key& key);
        void Pop(const Key& key);

        Iterator begin();
        Iterator end();

    private:
        struct Node {
            std::optional<Key>         Value;
            std::optional<std::size_t> Next;
            int                        A     = 0;
            int                        B     = 0;
            int                        P     = 0;
            int                        M     = 0;
            
        }; // struct Node

        NodeContainer m_Nodes;
        Hash          m_Hash;
        std::size_t   m_Size;

    }; // class HashTable

    ///////////////////////////////////////////////////////////////////////
    /// Source
    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable::Iterator
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, std::size_t Size, typename Hash>
    StaticHashTable<Key, Size, Hash>::Iterator::Iterator(NodeContainer& nodes, std::size_t index) :
        m_Nodes(nodes),
        m_NodeIterator(nodes.begin() + index) { }

    template <typename Key, std::size_t Size, typename Hash>
    typename StaticHashTable<Key, Size, Hash>::Iterator& StaticHashTable<Key, Size, Hash>::Iterator::operator ++() noexcept {
        do {
            ++m_NodeIterator;
        } while (m_NodeIterator->Value && m_NodeIterator != m_Nodes.end());

        return *this;
    }

    template <typename Key, std::size_t Size, typename Hash>
    Key& StaticHashTable<Key, Size, Hash>::Iterator::operator *() noexcept {
        return *m_NodeIterator->Value;
    }

    template <typename Key, std::size_t Size, typename Hash>
    bool StaticHashTable<Key, Size, Hash>::Iterator::operator ==(const Iterator& other) const noexcept {
        return &m_Nodes == &other.m_Nodes && m_NodeIterator == other.m_nodeIterator;
    }

    template <typename Key, std::size_t Size, typename Hash>
    bool StaticHashTable<Key, Size, Hash>::Iterator::operator !=(const Iterator& other) const noexcept {
        return !(&m_Nodes == &other.m_Nodes && m_NodeIterator == other.m_nodeIterator);
    }

    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, std::size_t Size, typename Hash>
    StaticHashTable<Key, Size, Hash>::StaticHashTable() :
        m_Size(0) {}

    template <typename Key, std::size_t Size, typename Hash>
    typename StaticHashTable<Key, Size, Hash>::Iterator StaticHashTable<Key, Size, Hash>::Push(const Key& key) {
        auto position = m_Hash(key, 4, 43, 101, Size);
        bool inserted = false;

        if (m_Nodes[position].Value) {
            while (m_Nodes[position].Value) {
                if (*m_Nodes[position].Value == key)
                    return Iterator(m_Nodes, position);

                if (m_Nodes[position].Next)
                    position = *m_Nodes[position].Next;
                else
                    break;
            }

            for (std::size_t i = Size; i >= 1; --i) {
                if (!m_Nodes[i].Value) {
                    m_Nodes[position].Next = i;
                    m_Nodes[i].Value = key;
                    position = i;
                    inserted = true;

                    break;
                }
            }
        } else {
            m_Nodes[position].Value = key;
            inserted = true;
        }

        ++m_Size;

        return Iterator(m_Nodes, inserted ? position : m_Nodes.size());
    }
    
    template <typename Key, std::size_t Size, typename Hash>
    typename StaticHashTable<Key, Size, Hash>::Iterator StaticHashTable<Key, Size, Hash>::begin() {
        return { m_Nodes, 0 };
    }

    template <typename Key, std::size_t Size, typename Hash>
    typename StaticHashTable<Key, Size, Hash>::Iterator StaticHashTable<Key, Size, Hash>::end() {
        return { m_Nodes, m_Nodes.size() };
    }

} // namespace Ng