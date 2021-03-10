#include <cmath>
#include <numeric>
#include <utility>
#include "OptimalBinarySearchTree.hpp"


namespace Ng {

    /////////////////////////////////////////////////////////////////////////////
    /// OptimalBinarySearchTree::Node
    /////////////////////////////////////////////////////////////////////////////
    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::Node(const Pair& pair, Node* parent, Node* left, Node* right)
        : m_Pair(pair)
        , m_Parent(parent)
        , m_Left(parent)
        , m_Right(parent) {}

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::~Node() {
        delete m_Left;
        delete m_Right;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Node::Print(std::ostream& ostream) const {
        ostream << "<Key: " << m_Pair.first << ", Value: " << m_Pair.second << "> {L: ";

        if (m_Left) ostream << m_Left->m_Pair.first;
        else        ostream << "Null";
        ostream << ", R: ";

        if (m_Right) ostream << m_Right->m_Pair.first;
        else         ostream << "Null";
        ostream << "}";
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// class SplayTree::IteratorBase
    ///////////////////////////////////////////////////////////////////////////////
    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::IteratorBase::IteratorBase(Node* node)
        : m_Node(node) {}

    template <Comparable Key, typename Value>
    typename OptimalBinarySearchTree<
        Key,
        Value
    >::IteratorBase& OptimalBinarySearchTree<Key, Value>::IteratorBase::operator ++() {
        if (m_Node->m_Right) {
            m_Node = m_Node->m_Right;

            while (m_Node->m_Left)
                m_Node = m_Node->m_Left;
        } else {
            Node* parent = m_Node->m_Parent;

            while (parent && m_Node == parent->m_Right) {
                m_Node = parent;
                parent = parent->m_Parent;
            }

            m_Node = parent;
        }

        return *this;
    }

    template <Comparable Key, typename Value>
    typename OptimalBinarySearchTree<
        Key,
        Value
    >::IteratorBase& OptimalBinarySearchTree<Key, Value>::IteratorBase::operator +=(int n) {
        for (int i = 0; i < n; i++)
            (*this)++;

        return *this;
    }

    template <Comparable Key, typename Value>
    bool OptimalBinarySearchTree<Key, Value>::IteratorBase::operator !=(const IteratorBase& other) const {
        return m_Node != other.m_Node;
    }

    /////////////////////////////////////////////////////////////////////////////
    /// OptimalBinarySearchTree
    /////////////////////////////////////////////////////////////////////////////
    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::OptimalBinarySearchTree(
        const std::vector<Key>& keys,
        const std::vector<Value>& values,
        const std::vector<float>& probabilities
    )
        : m_Root(nullptr)
        , m_Size(keys.size()) {

        InitTable(keys, values, probabilities);
        Build(m_Root, 0, m_Table.size() - 1);
    }

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::~OptimalBinarySearchTree() {
        delete m_Root;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::PrintTable() const {
        std::cout << "\t";
        for (int i = 0; i < m_Table.size(); ++i)
            std::cout << i << "\t";
        std::cout << std::endl;

        for (int i = 0; i < m_Table.size(); ++i) {
            std::cout << i << "\t";
            for (int j = 0; j < m_Table.size(); ++j) {
                std::cout << m_Table[i][j].Cost << ": " << m_Table[i][j].Pair.first << "\t";
            }

            std::cout << std::endl;
        }
    }

    template <Comparable Key, typename Value>
    std::ostream& operator <<(std::ostream& ostream, const OptimalBinarySearchTree<Key, Value>& tree) {
        tree.Print(tree.m_Root, 1, "Root", ostream);

        return ostream;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::InitTable(
        const std::vector<Key>& keys,
        const std::vector<Value>& values,
        const std::vector<float>& probabilities
    ) {
        m_Table.resize(m_Size + 1);

        for (std::size_t i = 0; i < m_Table.size(); ++i) {
            m_Table[i].resize(m_Table.size());
            m_Table[i][i].Cost = 0.0f;
        }

        for (std::size_t i = 0 ; i < m_Table.size(); ++i) {
            for (std::size_t j = 0; j + 1 < m_Table.size() - i; ++j) {
                std::size_t k = i + j + 1;

                TablePair& currentCell = m_Table[j][k];

                for (std::size_t l = j; l < k; ++l) {
                    currentCell.Cost = std::min(currentCell.Cost, m_Table[j][l].Cost + m_Table[l + 1][k].Cost);

                    if (currentCell.Cost == m_Table[j][l].Cost + m_Table[l + 1][k].Cost)
                        currentCell.Pair = { keys[l], values[l] };
                }

                currentCell.Cost += std::accumulate(probabilities.begin() + j, probabilities.begin() + k, 0.0f);
            }
        }
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Build(Node*& node, int left, int top) {
        if (left >= top)
            return;

        node = new Node(m_Table[left][top].Pair);

        Build(node->m_Left,  left,               node->m_Pair.first - 1);
        Build(node->m_Right, node->m_Pair.first, top                   );

        if (node->m_Left)
            node->m_Left->m_Parent = node;

        if (node->m_Right)
            node->m_Right->m_Parent = node;
    }

    template <Comparable Key, typename Value>
    typename OptimalBinarySearchTree<
        Key,
        Value
    >::Node* OptimalBinarySearchTree<Key, Value>::GetMinNode(Node* node) const {
        while (node && node->m_Left)
            node = node->m_Left;

        return node;
    }

    template <Comparable Key, typename Value>
    typename OptimalBinarySearchTree<
        Key,
        Value
    >::Node* OptimalBinarySearchTree<Key, Value>::GetMaxNode(Node* node) const {
        while (node && node->m_Right)
            node = node->m_Right;

        return node;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Print(
        const Node* node,
        int level,
        const char* caption,
        std::ostream& ostream
    ) const {
        if (!node) {
            ostream << caption << ": Null" << std::endl;
            return;
        }

        ostream << caption << ": ";
        node->Print(ostream);

        if (node->m_Left || node->m_Right) {
            ostream << " (" << std::endl;

            for (int i = 0; i < level; i++)
                ostream << "| ";
            Print(node->m_Left, level + 1, "Left", ostream);

            for (int i = 0; i < level; i++)
                ostream << "| ";
            Print(node->m_Right, level + 1, "Right", ostream);

            for (int i = 0; i < level - 1; i++)
                ostream << "| ";
            ostream << ")";
        }

        ostream << std::endl;
    }

} // namespace Ng