#include <cmath>
#include <numeric>
#include <utility>

namespace Ng {

    /////////////////////////////////////////////////////////////////////////////
    /// OptimalBinarySearchTree::Node
    /////////////////////////////////////////////////////////////////////////////
    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::Node(const Key& key,
                                                    const Value& value,
                                                    Node* parent,
                                                    Node* left,
                                                    Node* right)
        : m_Pair({ key, value })
        , m_Parent(parent)
        , m_Left(parent)
        , m_Right(parent) {}

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::Node(const Pair& pair,
                                                    Node* parent,
                                                    Node* left,
                                                    Node* right)
        : m_Pair(pair)
        , m_Parent(parent)
        , m_Left(parent)
        , m_Right(parent) {}

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::Node::~Node() {
        delete m_Left;
        delete m_Right;
    }

    /////////////////////////////////////////////////////////////////////////////
    /// OptimalBinarySearchTree
    /////////////////////////////////////////////////////////////////////////////
    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::OptimalBinarySearchTree(std::vector<float> probabilities)
        : m_Probabilities(std::move(probabilities)) {

        InitTable();
        BuildTree(m_Root, 0, m_Table.size() - 1);
    }

    template <Comparable Key, typename Value>
    OptimalBinarySearchTree<Key, Value>::~OptimalBinarySearchTree() {
        delete m_Root;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::InitTable() {
        m_Table.resize(m_Probabilities.size() + 1);

        for (std::size_t i = 0; i < m_Table.size(); ++i) {
            m_Table[i].resize(m_Table.size());
            m_Table[i][i].Cost = 0.0f;
        }

        for (std::size_t i = 0 ; i < m_Table.size(); ++i) {
            for (std::size_t j = 0; j + 1 < m_Table.size() - i; ++j) {
                std::size_t k = i + j + 1;

                float& currentCell = m_Table[j][k].Cost;

                for (std::size_t l = j; l < k; ++l)
                    currentCell = std::min(currentCell, m_Table[j][l].Cost + m_Table[l + 1][k].Cost);

                currentCell += std::accumulate(m_Probabilities.begin() + j, m_Probabilities.begin() + k, 0.0f);
            }
        }
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::BuildTree(Node* node, int left, int top) {
        if (left > top)
            return;

        node = new Node(m_Table[left][top].Pair);

        BuildTree(node->m_Left,  left,     top - 1);
        BuildTree(node->m_Right, left + 1, top    );


        if (node->m_Left)
            node->m_Left->m_Parent = node;

        if (node->m_Right)
            node->m_Right->m_Parent = node;
    }

    template <Comparable Key, typename Value>
    void OptimalBinarySearchTree<Key, Value>::Print() const {
        std::cout << "\t";
        for (int i = 0; i < m_Table.size(); ++i)
            std::cout << i << "\t";
        std::cout << std::endl;

        for (int i = 0; i < m_Table.size(); ++i) {
            std::cout << i << "\t";
            for (int j = 0; j < m_Table.size(); ++j) {
                std::cout << m_Table[i][j].Cost << "\t";
            }

            std::cout << std::endl;
        }
    }

} // namespace Ng