#pragma once

#include <array>
#include <vector>

#include "UniversalHash.hpp"

namespace Ng {

    ///////////////////////////////////////////////////////////////////////
    /// Header
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, std::size_t Size, typename HashFunc = UniversalHash<Key>>
    class StaticHashTable {
    private:
        struct Node;
        struct UniversalHashParams;

    public:
        using NodeContainer     = std::array<Node, Size>;
        using NodeIterator      = typename NodeContainer::iterator;
        using NodeConstIterator = typename NodeContainer::const_iterator;

        using KeyContainer      = std::vector<Key*>;
        using KeyIterator       = typename KeyContainer::iterator;
        using KeyConstIterator  = typename KeyContainer::const_iterator;

        class Iterator {
        public:
            Iterator(NodeContainer& nodes, std::size_t nodeIndex, std::size_t keyIndex);
            virtual ~Iterator() = default;

            Iterator& operator ++() noexcept;
            Key& operator *() noexcept;

            bool operator ==(const Iterator& other) const noexcept;
            bool operator !=(const Iterator& other) const noexcept;

            friend class StaticHashTable;

        private:
            NodeContainer& m_Nodes;
            std::size_t    m_NodeIndex;
            std::size_t    m_KeyIndex;

        }; // class Iterator

        StaticHashTable();
        virtual ~StaticHashTable();

        [[nodiscard]] inline std::size_t GetMaxSize() const { return Size; }
        [[nodiscard]] inline std::size_t GetSize() const { return m_Size; }

        [[nodiscard]] std::optional<Key> Find(const Key& key) const;
        Iterator Push(const Key& key);
        void Pop(const Key& key);

        void Print() const;

        Iterator begin();
        Iterator end();

    private:
        struct UniversalHashParams {
            int Prime = 2153;
            int A     = Ng::Random::Get(0, Prime);
            int B     = Ng::Random::Get(1, Prime);

        }; // struct HashParams

        class Node {
        public:
            Node() = default;
            virtual ~Node() = default;

            [[nodiscard]] inline bool IsEmpty() const { return m_Keys.empty(); }
            [[nodiscard]] inline std::size_t GetSize() const { return std::sqrt(m_Keys.size()); }
            [[nodiscard]] inline std::size_t GetActualSize() const { return m_Keys.size(); }
            [[nodiscard]] inline KeyContainer& GetKeys() { return m_Keys; };
            [[nodiscard]] inline const KeyContainer& GetKeys() const { return m_Keys; };

            std::size_t GetHash(const Key& key) const;

            void PushBack(const Key& key);
            void Update(std::size_t size);

            KeyIterator begin() { return m_Keys.begin(); }
            KeyConstIterator cbegin() const { return m_Keys.cbegin(); }
            KeyIterator end() { return m_Keys.end(); }
            KeyConstIterator cend() const { return m_Keys.cend(); }

            Key*& operator [](const std::size_t index) { return m_Keys[index]; }
            const Key* operator [](const std::size_t index) const { return m_Keys[index]; }

        private:
            KeyContainer        m_Keys;
            HashFunc            m_Hash;
            UniversalHashParams m_HashParams;

        }; // class Node

        [[nodiscard]] std::size_t GetHash(const Key& key) const;

        NodeContainer       m_Nodes;
        HashFunc            m_Hash;
        std::size_t         m_Size;
        UniversalHashParams m_HashParams;

    }; // class HashTable

    ///////////////////////////////////////////////////////////////////////
    /// Source
    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable::Iterator
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    StaticHashTable<
        Key,
        Value,
        Size,
        HashFunc
    >::Iterator::Iterator(NodeContainer& nodes, std::size_t nodeIndex, std::size_t keyIndex) :
        m_Nodes(nodes),
        m_NodeIndex(nodeIndex),
        m_KeyIndex(keyIndex) { }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<Key, Value, Size, HashFunc>::Iterator& StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator ++() noexcept {
        do {
            ++m_KeyIndex;

            if (m_Nodes[m_NodeIndex].Data.begin() + m_KeyIndex == m_Nodes[m_NodeIndex].Data.end())
                ++m_NodeIndex;
        } while (!m_Nodes[m_NodeIndex].Data[m_KeyIndex]);

        return *this;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    Key& StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator *() noexcept {
        return *m_Nodes[m_NodeIndex][m_KeyIndex];
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    bool StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator ==(const Iterator& other) const noexcept {
        return &m_Nodes == &other.m_Nodes &&
               m_NodeIndex == other.m_NodeIndex &&
               m_KeyIndex == other.m_KeyIndex;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    bool StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator !=(const Iterator& other) const noexcept {
        return !(&m_Nodes == &other.m_Nodes &&
                 m_NodeIndex == other.m_NodeIndex &&
                 m_KeyIndex == other.m_KeyIndex);
    }

    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable::Node
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    std::size_t StaticHashTable<Key, Value, Size, HashFunc>::Node::GetHash(const Key& key) const {
        return m_Hash(key, m_HashParams.A, m_HashParams.B, m_HashParams.Prime, m_Keys.size());
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Node::PushBack(const Key& key) {
        m_Keys.emplace_back(new Key(key));
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Node::Update(std::size_t size) {
        m_Keys.clear();
        m_Keys.resize(size, nullptr);
        m_HashParams = UniversalHashParams();
    }

    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    StaticHashTable<Key, Value, Size, HashFunc>::StaticHashTable() :
        m_Size(0) {}

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    StaticHashTable<Key, Value, Size, HashFunc>::~StaticHashTable() {
        for (auto& node : m_Nodes)
            for (auto& key : node)
                delete key;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    std::optional<Key> StaticHashTable<Key, Value, Size, HashFunc>::Find(const Key& key) const {
        auto& node = m_Nodes[GetHash(key)];

        if (node.GetActualSize() == 1 && *node[0] == key)
            return key;

        for (std::size_t i = 0; i < m_Nodes[hash].GetActualSize(); ++i)
            if (m_Nodes[hash][i] && *m_Nodes[hash][i] == key)
                return { m_Nodes, hash, i };

        return std::nullopt;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<Key, Value, Size, HashFunc>::Iterator StaticHashTable<Key, Value, Size, HashFunc>::Push(const Key& key) {
        auto  nodeHash    = GetHash(key);
        auto& currentNode = m_Nodes[nodeHash];

        if (currentNode.IsEmpty()) {
            currentNode.PushBack(key);
            return { m_Nodes, nodeHash, 0 };
        }

        std::size_t  size     = currentNode.GetSize() + 1;
        KeyContainer temp     = currentNode.GetKeys();
        bool         inserted = false;

        temp.emplace_back(new Key(key));

        while (!inserted) {
            currentNode.Update(size * size);
            inserted = true;

            for (const auto& k : temp) {
                if (!k)
                    continue;

                auto hash = currentNode.GetHash(*k);

                if (currentNode[hash]) {
                    inserted = false;
                    break;
                }

                currentNode[hash] = k;
            }
        }

        return { m_Nodes, 0, 0 };
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Pop(const Key& key) {

    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Print() const {
        std::cout << "Print" << std::endl;

        for (std::size_t i = 0; i < Size; ++i) {
            if (m_Nodes[i].IsEmpty()) {
                std::cout << i << ": empty" << std::endl;
                continue;
            }

            std::cout << i << ": ";

            for (const auto& e : m_Nodes[i].GetKeys()) {
                if (e)
                    std::cout << *e;
                else
                    std::cout << "null";
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
    
    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<Key, Value, Size, HashFunc>::Iterator StaticHashTable<Key, Value, Size, HashFunc>::begin() {
        return { m_Nodes, 0, 0 };
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<Key, Value, Size, HashFunc>::Iterator StaticHashTable<Key, Value, Size, HashFunc>::end() {
        return { m_Nodes, m_Nodes.size(), m_Nodes.back().Data.size() };
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    std::size_t StaticHashTable<Key, Value, Size, HashFunc>::GetHash(const Key& key) const {
        return m_Hash(key, m_HashParams.A, m_HashParams.B, m_HashParams.Prime, Size);
    }

} // namespace Ng