#include <cmath>
#include <numeric>
#include <utility>

#include <fort.hpp>

namespace Ng {

    /////////////////////////////////////////////////////////////////////////////
    /// OptimalBinarySearchTree::Node
    /////////////////////////////////////////////////////////////////////////////
    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::Node(const Pair& pair, Node* parent, Node* left, Node* right)
        : m_Pair(pair)
        , m_Parent(parent)
        , m_Left(left)
        , m_Right(right) {}

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::~Node() {
        delete m_Left;
        delete m_Right;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Node::Print() const {
        std::cout << "<Key: " << m_Pair.first << ", Value: " << m_Pair.second << "> {L: ";

        if (m_Left) std::cout << m_Left->m_Pair.first;
        else        std::cout << "Null";
        std::cout << ", R: ";

        if (m_Right) std::cout << m_Right->m_Pair.first;
        else         std::cout << "Null";
        std::cout << "}";
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
    OptimalBinarySearchTree<Key, Value>::OptimalBinarySearchTree()
        : m_Root(nullptr)
        , m_Size(0) {

    }

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::~OptimalBinarySearchTree() {
        delete m_Root;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::SetData(const std::vector<DataCell>& data) {
        m_Size = data.size();

        InitTable(data);
        Build(m_Root, 0, m_Size, data);
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::PrintTable() const {
        fort::char_table table;

        table << fort::header << "N";

        for (int i = 0 ; i < m_Table.size(); ++i) {
            table.cur_cell().set_cell_text_align(fort::text_align::right);
            table << i;
        }

        table << fort::endr;

        for (int i = 0; i < m_Table.size(); ++i) {
            table << i;

            for (int j = 0; j < m_Table.size(); ++j)
                table << std::to_string(m_Table[i][j].Cost) + ": " + std::to_string(m_Table[i][j].Index);

            table << fort::endr;
        }

        std::cout << table.to_string() << std::endl;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Print() const {
        return Print(m_Root, 1, "Root");
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::InitTable(const std::vector<DataCell>& data) {
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
                        currentCell.Index = l;
                }

                currentCell.Cost += std::accumulate(
                    data.begin() + j,
                    data.begin() + k,
                    0.0f,
                    [](float sum, const auto& cell) { return sum + cell.Probability; }
                );
            }
        }
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Build(
        Node*&                       node,
        std::size_t                  row,
        std::size_t                  column,
        const std::vector<DataCell>& data
    ) {
        if (row >= column)
            return;

        std::size_t index = m_Table[row][column].Index;

        node = new Node({ data[index].Key, data[index].Value });

        auto*& left  = node->m_Left;
        auto*& right = node->m_Right;

        Build(left,  row,       index,  data);
        Build(right, index + 1, column, data);

        if (left)
            left->m_Parent = node;

        if (right)
            right->m_Parent = node;
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
    void OptimalBinarySearchTree<Key, Value>::Print(const Node* node, int level, const std::string& caption) const {
        if (!node) {
            std::cout << caption << ": Null" << std::endl;
            return;
        }

        std::cout << caption << ": ";
        node->Print();

        if (node->m_Left || node->m_Right) {
            std::cout << " (" << std::endl;

            for (int i = 0; i < level; i++)
                std::cout << "| ";
            Print(node->m_Left, level + 1, "Left");

            for (int i = 0; i < level; i++)
                std::cout << "| ";
            Print(node->m_Right, level + 1, "Right");

            for (int i = 0; i < level - 1; i++)
                std::cout << "| ";
            std::cout << ")";
        }

        std::cout << std::endl;
    }

} // namespace Ng