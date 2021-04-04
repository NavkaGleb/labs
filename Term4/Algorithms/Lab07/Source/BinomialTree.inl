#include <utility>

namespace Ng {

    ////////////////////////////////////////////////////////////////////////////
    /// BinomialTree::Node ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    template <typename T>
    BinomialTree<T>::Node::Node(const T& value)
        : m_Value(value)
        , m_Parent(nullptr) {}

    template <typename T>
    BinomialTree<T>::Node::~Node() noexcept {
        for (auto& child : m_Children)
            delete child;
    }

    template <typename T>
    typename BinomialTree<T>::Node* BinomialTree<T>::Node::Get(std::size_t index) {
        if (index == 0)
            return this;

        for (auto it = m_Children.begin(); it != m_Children.end(); ++it) {
            auto count = (*it)->GetCount();

            if ((index - 1) / count == 0)
                return (*it)->Get(index - 1);

            index -= count;
        }

        return nullptr;
    }

    template <typename T>
    typename BinomialTree<T>::Node::ChildrenContainer BinomialTree<T>::Node::GiveAwayChildren() {
        return std::move(m_Children);
    }

    template <typename T>
    bool BinomialTree<T>::Node::operator !=(const Node& other) const {
        if (m_Value == other.m_Value)
            return false;

        return m_Children != other.m_Children;
    }

    template <typename T>
    bool BinomialTree<T>::Node::operator ==(const Node& other) const {
        if (m_Value != other.m_Value)
            return false;

        return m_Children == other.m_Children;
    }

    ////////////////////////////////////////////////////////////////////////////
    /// BinomialTree ///////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void BinomialTree<T>::Link(BinomialTree& lhs, BinomialTree& rhs) {
        if (rhs.GetDegree() != rhs.GetDegree())
            throw std::invalid_argument("Ng::BinomialTree::Link: Trees are different sizes");

        // Move ctor similarity
        lhs.m_Root->m_Children.emplace_front(rhs.m_Root)->m_Parent = lhs.m_Root;
        rhs.m_Root = nullptr;
    }

    template <typename T>
    BinomialTree<T>::BinomialTree(Node* root)
        : m_Root(root) {}

    template <typename T>
    BinomialTree<T>::BinomialTree(const BinomialTree<T>& other)
        : m_Root(new Node(*other.m_Root)) {
    }

    template <typename T>
    BinomialTree<T>::BinomialTree(BinomialTree<T>&& other) noexcept
        : m_Root(std::exchange(other.m_Root, nullptr)) {}

    template <typename T>
    BinomialTree<T>::~BinomialTree() noexcept {
        delete m_Root;
    }

    template <typename T>
    typename BinomialTree<T>::Node* BinomialTree<T>::Get(std::size_t index) {
        return m_Root->Get(index);
    }

    template <typename T>
    void BinomialTree<T>::Clear() {
        delete m_Root;
        m_Root = nullptr;
    }

    template <typename T>
    void BinomialTree<T>::Print() const {
        return Print(m_Root, 1, 0);
    }

    template <typename T>
    bool BinomialTree<T>::operator !=(const BinomialTree& other) const {
        return m_Root != other.m_Root;
    }

    template <typename T>
    bool BinomialTree<T>::operator ==(const BinomialTree& other) const {
        return m_Root == other.m_Root;
    }

    template <typename T>
    void BinomialTree<T>::Print(const Node* node, int level, int index) const {
        if (!node)
            return;

        std::cout << index << ". { " << node->m_Value << ", " << node->m_Children.size() << " }";

        if (!node->m_Children.empty()) {
            std::cout << " (" << std::endl;

            for (auto it = node->m_Children.begin(); it != node->m_Children.end(); ++it) {
                for (int j = 0; j < level; j++)
                    std::cout << "  ";

                Print(*it, level + 1, std::distance(node->m_Children.begin(), it));
            }

            for (int i = 0; i < level - 1; i++)
                std::cout << "  ";
            std::cout << ")";
        }

        std::cout << std::endl;
    }

} // namespace Ng