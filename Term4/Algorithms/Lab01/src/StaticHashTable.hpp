#pragma once

#include <array>
#include <vector>

#include <Random/Random.hpp>

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

        using Pair              = std::pair<Key, Value>;
        using DataContainer     = std::vector<Pair*>;
        using DataIterator      = typename DataContainer::iterator;
        using DataConstIterator = typename DataContainer::const_iterator;

        class Iterator {
        public:
            Iterator(NodeContainer& nodes, std::size_t nodeIndex, std::size_t dataIndex);
            virtual ~Iterator() = default;

            Iterator& operator ++() noexcept;
            Pair& operator *() noexcept;

            bool operator ==(const Iterator& other) const noexcept;
            bool operator !=(const Iterator& other) const noexcept;

            friend class StaticHashTable;

        private:
            NodeContainer& m_Nodes;
            std::size_t    m_NodeIndex;
            std::size_t    m_DataIndex;

        }; // class Iterator

        StaticHashTable();
        virtual ~StaticHashTable();

        [[nodiscard]] inline std::size_t GetMaxSize() const { return Size; }
        [[nodiscard]] inline std::size_t GetSize() const { return m_Size; }

        [[nodiscard]] bool IsExists(const Key& key) const;
        [[nodiscard]] Iterator Find(const Key& key);

        Iterator Push(const Key& key, const Value& value);
        void Pop(const Key& key);

        void Print() const;

        Iterator begin();
        Iterator end();

        Value& operator [](const Key& key);

    private:
        struct UniversalHashParams {
            int Prime = 2153;
            int A     = 0;
            int B     = 0;

            void Random() {
                Prime = Ng::Random::Get({ 2153, 4027, 8447, 10687 });
                A     = Ng::Random::Get(0, Prime);
                B     = Ng::Random::Get(1, Prime);
            }

        }; // struct HashParams

        class Node {
        public:
            Node();
            virtual ~Node() = default;

            [[nodiscard]] inline bool IsEmpty() const { return m_Data.empty(); }
            [[nodiscard]] inline std::size_t& GetSize() { return m_Size; }
            [[nodiscard]] inline std::size_t GetSize() const { return m_Size; }
            [[nodiscard]] inline std::size_t GetActualSize() const { return m_Data.size(); }
            [[nodiscard]] inline DataContainer& GetData() { return m_Data; };
            [[nodiscard]] inline const DataContainer& GetData() const { return m_Data; };

            std::size_t GetHash(const Key& key) const;

            void PushBack(const Key& key, const Value& value);
            void Update(std::size_t size);

            DataIterator begin() { return m_Data.begin(); }
            DataConstIterator begin() const { return m_Data.cbegin(); }
            DataConstIterator cbegin() const noexcept { return m_Data.cbegin(); }

            DataIterator end() { return m_Data.end(); }
            DataConstIterator end() const { return m_Data.cend(); }
            DataConstIterator cend() const noexcept { return m_Data.cend(); }

            Pair*& operator [](const std::size_t index) { return m_Data[index]; }
            const Pair* operator [](const std::size_t index) const { return m_Data[index]; }

        private:
            DataContainer       m_Data;
            HashFunc            m_Hash;
            std::size_t         m_Size;
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
    >::Iterator::Iterator(NodeContainer& nodes, std::size_t nodeIndex, std::size_t dataIndex) :
        m_Nodes(nodes),
        m_NodeIndex(nodeIndex),
        m_DataIndex(dataIndex) { }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<
        Key,
        Value,
        Size,
        HashFunc
    >::Iterator& StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator ++() noexcept {

        do {
            ++m_DataIndex;

            while (m_DataIndex >= m_Nodes[m_NodeIndex].GetActualSize()) {
                ++m_NodeIndex;
                m_DataIndex = 0;
            }

            if (m_NodeIndex == Size)
                break;

        } while (!m_Nodes[m_NodeIndex][m_DataIndex]);

        return *this;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<
        Key,
        Value,
        Size,
        HashFunc
    >::Pair& StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator *() noexcept {
        return *m_Nodes[m_NodeIndex][m_DataIndex];
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    bool StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator ==(const Iterator& other) const noexcept {
        return &m_Nodes == &other.m_Nodes &&
               m_NodeIndex == other.m_NodeIndex &&
               m_DataIndex == other.m_DataIndex;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    bool StaticHashTable<Key, Value, Size, HashFunc>::Iterator::operator !=(const Iterator& other) const noexcept {
        return !(&m_Nodes == &other.m_Nodes &&
                 m_NodeIndex == other.m_NodeIndex &&
                 m_DataIndex == other.m_DataIndex);
    }

    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable::Node
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    StaticHashTable<Key, Value, Size, HashFunc>::Node::Node() :
        m_Size(0) { }

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
        m_HashParams.Random();
    }

    ///////////////////////////////////////////////////////////////////////
    /// class StaticHashTable
    ///////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    StaticHashTable<Key, Value, Size, HashFunc>::StaticHashTable() :
        m_Size(0) {

        m_HashParams.Random();
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    StaticHashTable<Key, Value, Size, HashFunc>::~StaticHashTable() {
        for (auto& node : m_Nodes)
            for (auto& pair : node)
                delete pair;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    bool StaticHashTable<Key, Value, Size, HashFunc>::IsExists(const Key& key) const {
        auto& node = m_Nodes[GetHash(key)];

        if (node.IsEmpty())
            return false;

        auto hash = node.GetHash(key);

        return hash < node.GetActualSize() && node[hash] && node[hash]->first == key;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<
        Key,
        Value,
        Size,
        HashFunc
    >::Iterator StaticHashTable<Key, Value, Size, HashFunc>::Find(const Key& key) {
        auto  nodeHash = GetHash(key);
        auto& node     = m_Nodes[nodeHash];

        if (!node.IsEmpty()) {
            auto hash = node.GetHash(key);

            if (hash < node.GetActualSize() && node[hash] && node[hash]->first == key)
                return { m_Nodes, nodeHash, hash };
        }

        return end();
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<
        Key,
        Value,
        Size,
        HashFunc
    >::Iterator StaticHashTable<Key, Value, Size, HashFunc>::Push(const Key& key, const Value& value) {
        if (IsExists(key))
            return Find(key);

        auto  nodeHash = GetHash(key);
        auto& node     = m_Nodes[nodeHash];

        ++m_Size;
        ++node.GetSize();

        if (node.IsEmpty()) {
            node.PushBack(key, value);
            return { m_Nodes, nodeHash, 0 };
        }

        bool          inserted  = false;
        std::size_t   dataIndex = 0;
        DataContainer temp      = node.GetData();

        temp.emplace_back(new Pair(key, value));

        std::cout << "Col" << " " << std::pow(node.GetSize(), 2) << std::endl;

        while (!inserted) {
            node.Update(std::pow(node.GetSize(), 2));
            inserted = true;

            for (const auto& pair : temp) {
                if (!pair)
                    continue;

                auto hash = node.GetHash(pair->first);

                if (pair->first == key)
                    dataIndex = hash;

                if (node[hash]) {
                    inserted = false;
                    break;
                }

                node[hash] = pair;
            }
        }

        return { m_Nodes, nodeHash, dataIndex };
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Pop(const Key& key) {
        if (!IsExists(key))
            return;

        auto  nodeHash    = GetHash(key);
        auto& node        = m_Nodes[nodeHash];
        auto  deletedHash = node.GetHash(key);

        delete node[deletedHash];
        node[deletedHash] = nullptr;

        --m_Size;
        --node.GetSize();

        if (node.GetSize() == 0) {
            node.Update(0);
        } else if (static_cast<int>(std::pow(node.GetSize() + 1, 2)) < node.GetActualSize()) {
            auto temp = node.GetData();
            bool done = false;

            while (!done) {
                node.Update(std::pow(node.GetSize(), 2));
                done = true;

                for (const auto& pair : temp) {
                    if (!pair)
                        continue;

                    auto hash = node.GetHash(pair->first);

                    if (node[hash]) {
                        done = false;
                        break;
                    }

                    node[hash] = pair;
                }
            }
        }
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    void StaticHashTable<Key, Value, Size, HashFunc>::Print() const {
        std::cout << "Print" << std::endl;

        for (std::size_t i = 0; i < Size; ++i) {
            if (m_Nodes[i].IsEmpty()) {
                std::cout << i << ": empty" << std::endl;
                continue;
            }

            std::cout << i << " " << m_Nodes[i].GetSize() << " " << m_Nodes[i].GetActualSize() << ": ";

            for (const auto& pair : m_Nodes[i]) {
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
        for (std::size_t i = 0; i < Size; ++i)
            for (auto& pair : m_Nodes[i])
                if (pair)
                    return { m_Nodes, i, m_Nodes[i].GetHash(pair->first) };

        return end();
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    typename StaticHashTable<Key, Value, Size, HashFunc>::Iterator StaticHashTable<Key, Value, Size, HashFunc>::end() {
        return { m_Nodes, Size, 0 };
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    Value& StaticHashTable<Key, Value, Size, HashFunc>::operator [](const Key& key) {
        if (!IsExists(key))
            Push(key, Value());

        return (*Find(key)).second;
    }

    template <typename Key, typename Value, std::size_t Size, typename HashFunc>
    std::size_t StaticHashTable<Key, Value, Size, HashFunc>::GetHash(const Key& key) const {
        return m_Hash(key, m_HashParams.A, m_HashParams.B, m_HashParams.Prime, Size);
    }

} // namespace Ng