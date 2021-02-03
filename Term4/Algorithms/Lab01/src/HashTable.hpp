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
        struct UniversalHashParams;
        struct Node;

    public:
        using NodeContainer     = std::array<Node, Size>;
        using NodeIterator      = typename NodeContainer::iterator;
        using NodeConstIterator = typename NodeContainer::const_iterator;

        using Pair              = std::pair<Key, Value>;
        using DataContainer     = std::vector<Pair*>;
        using DataIterator      = typename DataContainer::iterator;
        using DataConstIterator = typename DataContainer::const_iterator;

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

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] std::optional<Value> Find(const Key& key) const;
        Iterator Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        void Print() const;

        Iterator begin();
        Iterator end();

        Value& operator [](const Key& key);

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

            [[nodiscard]] inline bool IsEmpty() const { return m_Data.empty(); }
            [[nodiscard]] inline std::size_t GetSize() const { return std::sqrt(m_Data.size()); }
            [[nodiscard]] inline std::size_t GetActualSize() const { return m_Data.size(); }
            [[nodiscard]] inline DataContainer& GetData() { return m_Data; };
            [[nodiscard]] inline const DataContainer& GetData() const { return m_Data; };

            std::size_t GetHash(const Key& key) const;

            void PushBack(const Key& key, const Value& value);
            void Update(std::size_t size);

            DataIterator begin() { return m_Data.begin(); }
            DataConstIterator cbegin() const { return m_Data.cbegin(); }
            DataIterator end() { return m_Data.end(); }
            DataConstIterator cend() const { return m_Data.cend(); }

            Pair*& operator [](const std::size_t index) { return m_Data[index]; }
            const Pair* operator [](const std::size_t index) const { return m_Data[index]; }

        private:
            DataContainer       m_Data;
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
        return m_Hash(key, m_HashParams.A, m_HashParams.B, m_HashParams.Prime, m_Data.size());
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Node::PushBack(const Key& key, const Value& value) {
        m_Data.emplace_back(new Pair(key, value));
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Node::Update(std::size_t size) {
        m_Data.clear();
        m_Data.resize(size, nullptr);
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
    bool StaticHashTable<Key, Value, Size, HashFunc>::IsExists(const Key& key) const {
        auto& node = m_Nodes[GetHash(key)];

        if (node.GetActualSize() == 1 && node[0]->first == key)
            return true;

        for (std::size_t i = 0; i < node.GetActualSize(); ++i)
            if (node[i] && node[i]->first == key)
                return true;

        return false;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    std::optional<Value> StaticHashTable<Key, Value, Size, HashFunc>::Find(const Key& key) const {
        auto& node = m_Nodes[GetHash(key)];

        if (node.GetActualSize() == 1 && node[0] == key)
            return node[0]->second;

        for (std::size_t i = 0; i < node.GetActualSize(); ++i)
            if (node[i] && node[i]->first == key)
                return node[i]->second;

        return std::nullopt;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<
        Key,
        Value,
        Size,
        HashFunc
    >::Iterator StaticHashTable<Key, Value, Size, HashFunc>::Push(const Key& key, const Value& value) {
        auto  nodeHash    = GetHash(key);
        auto& currentNode = m_Nodes[nodeHash];

        if (currentNode.IsEmpty()) {
            currentNode.PushBack(key, value);
            return { m_Nodes, nodeHash, 0 };
        }

        std::size_t  size     = currentNode.GetSize() + 1;
        DataContainer temp    = currentNode.GetData();
        bool         inserted = false;

        temp.emplace_back(new std::pair<Key, Value>(key, value));

        while (!inserted) {
            currentNode.Update(size * size);
            inserted = true;

            for (const auto& pair : temp) {
                if (!pair)
                    continue;

                auto hash = currentNode.GetHash(pair->first);

                if (currentNode[hash]) {
                    inserted = false;
                    break;
                }

                currentNode[hash] = pair;
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

            for (const auto& pair : m_Nodes[i].GetData()) {
                if (pair)
                    std::cout << pair->first << " -> " << pair->second;
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
    Value& StaticHashTable<Key, Value, Size, HashFunc>::operator [](const Key& key) {
        if (!IsExists(key))
            Push(key, Value());

        return Find(key).value();
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    std::size_t StaticHashTable<Key, Value, Size, HashFunc>::GetHash(const Key& key) const {
        return m_Hash(key, m_HashParams.A, m_HashParams.B, m_HashParams.Prime, Size);
    }

} // namespace Ng