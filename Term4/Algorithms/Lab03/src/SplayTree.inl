namespace Ng {

    ///////////////////////////////////////////////////////////////////////////////
    /// class SplayTree::Node
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value>
    SplayTree<Key, Value>::Node::Node()
        : m_Pair(Key(), Value())
        , m_Parent(nullptr)
        , m_Left(nullptr)
        , m_Right(nullptr) {}

    template <typename Key, typename Value>
    SplayTree<Key, Value>::Node::Node(const Key& key, const Value& value, Node* parent, Node* left, Node* right)
        : m_Pair(key, value)
        , m_Parent(parent)
        , m_Left(left)
        , m_Right(right) {}

    template <typename Key, typename Value>
    SplayTree<Key, Value>::Node::~Node() {
        delete m_Left;
        delete m_Right;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Node::Print(std::ostream& ostream) const {
        ostream << "Key: " << m_Pair.first << ", Value " << m_Pair.second << " {L: ";

        ostream << (m_Left  ? std::to_string(m_Left->m_Pair.first)  : "Null") << ", R: ";
        ostream << (m_Right ? std::to_string(m_Right->m_Pair.first) : "Null") << "}";
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// class SplayTree::Iterator
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value>
    SplayTree<Key, Value>::Iterator::Iterator(Node* node)
        : m_Node(node) {}

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Iterator& SplayTree<Key, Value>::Iterator::operator ++() {
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

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Iterator& SplayTree<Key, Value>::Iterator::operator +=(int n) {
        for (int i = 0; i < n; i++)
            (*this)++;

        return *this;
    }

    template <typename Key, typename Value>
    bool SplayTree<Key, Value>::Iterator::operator !=(const Iterator& other) const {
        return m_Node != other.m_Node;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// class SplayTree::ConstIterator
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value>
    SplayTree<Key, Value>::ConstIterator::ConstIterator(Node* node)
        : m_Node(node) {}

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::ConstIterator& SplayTree<Key, Value>::ConstIterator::operator ++() {
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

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::ConstIterator& SplayTree<Key, Value>::ConstIterator::operator +=(int n) {
        for (int i = 0; i < n; i++)
            (*this)++;

        return *this;
    }

    template <typename Key, typename Value>
    bool SplayTree<Key, Value>::ConstIterator::operator !=(const ConstIterator& other) const {
        return m_Node != other.m_Node;
    }
        
    ///////////////////////////////////////////////////////////////////////////////
    /// class SplayTree
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value>
    SplayTree<Key, Value>::SplayTree(Node* root)
        : m_Root(root)
        , m_Size(root ? 1 : 0) { }

    template <typename Key, typename Value>
    SplayTree<Key, Value>::~SplayTree() {
        Clear();
    }

    template <typename Key, typename Value>
    bool SplayTree<Key, Value>::IsExists(const Key& key) const {
        Node* node = m_Root;

        while (node && key != node->m_Pair.first)
            node = node->m_Pair.first > key ? node->m_Left : node->m_Right;

        return node;
    }

    template <typename Key, typename Value>
    int SplayTree<Key, Value>::GetHeight() const {
        return GetHeight(m_Root);
    }

    template <typename Key, typename Value>
    const Value& SplayTree<Key, Value>::GetMin() const {
        if (!m_Root)
            throw std::out_of_range("Ng::SplayTree::GetMin: m_Root is nullptr!");

        return GetMin(m_Root);
    }

    template <typename Key, typename Value>
    const Value& SplayTree<Key, Value>::GetMax() const {
        if (!m_Root)
            throw std::out_of_range("Ng::SplayTree::GetMax: m_Root is nullptr!");

        return GetMax(m_Root);
    }

    template <typename Key, typename Value>
    Value& SplayTree<Key, Value>::Get(const Key& key) {
        Node* node = m_Root;

        while (node && key != node->m_Pair.first)
            node = node->m_Pair.first > key ? node->m_Left : node->m_Right;

        if (!node)
            throw std::out_of_range("Ng::SplayTree::Get: key is not exists!");

        Splay(node);

        return node->m_Pair.second;
    }

    template <typename Key, typename Value>
    const Value& SplayTree<Key, Value>::Get(const Key& key) const {
        Node* node = m_Root;

        while (node && key != node->m_Pair.first)
            node = node->m_Pair.first > key ? node->m_Left : node->m_Right;

        if (!node)
            throw std::out_of_range("Ng::SplayTree::Get: key is not exists!");

        return node->m_Pair.second;
    }
    
    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Clear() {
        delete m_Root;
        m_Root = nullptr;
    }

    template <typename Key, typename Value>
    Value& SplayTree<Key, Value>::Push(const Key& key, const Value& value) {
        ++m_Size;

        if (!m_Root)
            return (m_Root = new Node(key, value))->m_Pair.second;

        Node* node   = m_Root;
        Node* parent = nullptr;

        while (node) {
            if (node->m_Pair.first == key) {
                --m_Size;
                Splay(node);
                return node->m_Pair.second;
            }

            parent = node;
            node   = node->m_Pair.first > key ? node->m_Left : node->m_Right;
        }

        node = new Node(key, value, parent);

        if (parent->m_Pair.first > key)
            parent->m_Left = node;
        else
            parent->m_Right = node;

        Splay(node);

        return node->m_Pair.second;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Pop(const Key& key) {
        Node* node = GetNode(key);

        if (!node)
            return;

        --m_Size;

        Node* left  = node->m_Left;
        Node* right = node->m_Right;

        if (!left && !right)
            m_Root = nullptr;
        else if (!left)
            Transplant(node, right);
        else if (!right)
            Transplant(node, left);
        else
            Merge(node->m_Left, node->m_Right);

        node->m_Left  = nullptr;
        node->m_Right = nullptr;

        delete node;
    }

    template <typename Key, typename Value>
    Value& SplayTree<Key, Value>::operator [](const Key& key) {
        return Push(key, Value());
    }

    template <typename Key, typename Value>
    int SplayTree<Key, Value>::GetHeight(Node* node) const {
        return node ? std::max(GetHeight(node->m_Left), GetHeight(node->m_Right)) + 1 : 0;
    }

    template <typename Key, typename Value>
    const Value& SplayTree<Key, Value>::GetMin(Node* node) const {
        while (node && node->m_Left)
            node = node->m_Left;

        return node->m_Pair.second;
    }

    template <typename Key, typename Value>
    const Value& SplayTree<Key, Value>::GetMax(Node* node) const {
        while (node && node->m_Right)
            node = node->m_Right;

        return node->m_Pair.second;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::GetMinNode(Node* node) const {
        while (node && node->m_Left)
            node = node->m_Left;

        return node;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::GetMaxNode(Node* node) const {
        while (node && node->m_Right)
            node = node->m_Right;

        return node;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::GetSuccessor(Node* node) const {
        if (node->m_Right)
            return GetMinNode(node->m_Right);

        Node* successor = node->m_Parent;

        while (successor && successor->m_Right == node) {
            node      = successor;
            successor = successor->m_Parent;
        }

        return successor;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::GetPredecessor(Node* node) const {
        if (node->m_Left)
            return GetMaxNode(node->m_Left);

        Node* predecessor = node->m_Parent;

        while (predecessor && predecessor->m_Left == node) {
            node        = predecessor;
            predecessor = predecessor->m_Parent;
        }

        return predecessor;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::GetNode(const Key& key) {
        Node* node = m_Root;

        while (node && key != node->m_Pair.first)
            node = node->m_Pair.first > key ? node->m_Left : node->m_Right;

        if (node)
            Splay(node);

        return node;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Transplant(Node* parent, Node* child) {
        if (!parent->m_Parent)
            m_Root = child;
        else if (parent == parent->m_Parent->m_Left)
            parent->m_Parent->m_Left = child;
        else if (parent == parent->m_Parent->m_Right)
            parent->m_Parent->m_Right = child;

        if (child)
            child->m_Parent = parent->m_Parent;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::RotateLeft(Node* node) {
        //   p      =>      x
        //  / \            / \
        // 1   x    =>    p   3
        //    / \        / \
        //   2   3  =>  1   2

        Node* right     = node->m_Right;
        Node* rightLeft = right->m_Left;

        right->m_Parent   = node->m_Parent;

        if (!node->m_Parent)
            m_Root = right;
        else if (node == node->m_Parent->m_Left)
            node->m_Parent->m_Left = right;
        else
            node->m_Parent->m_Right = right;

        node->m_Parent = right;
        right->m_Left = node;
        node->m_Right = rightLeft;

        if (rightLeft)
            rightLeft->m_Parent = node;

        return right;
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::RotateRight(Node* node) {
        //     p    =>    x
        //    / \        / \
        //   x   3  =>  1   p
        //  / \            / \
        // 1   2    =>    2   3

        Node* left      = node->m_Left;
        Node* leftRight = left->m_Right;

        left->m_Parent   = node->m_Parent;

        if (!node->m_Parent)
            m_Root = left;
        else if (node == node->m_Parent->m_Left)
            node->m_Parent->m_Left = left;
        else
            node->m_Parent->m_Right = left;

        node->m_Parent = left;
        left->m_Right  = node;
        node->m_Left   = leftRight;

        if (leftRight)
            leftRight->m_Parent = node;

        return left;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Zig(Node* node) {
        // Root.Left == node
        //     p    =>    x
        //    / \        / \
        //   x   3  =>  1   p
        //  / \            / \
        // 1   2    =>    2   3
        //
        // Root.Right == node
        //   p      =>      x
        //  / \            / \
        // 1   x    =>    p   3
        //    / \        / \
        //   2   3  =>  1   2
        m_Root = m_Root->m_Left == node ? RotateRight(m_Root) : RotateLeft(m_Root);
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::ZigZig(Node* node) {
        Node* parent      = node->m_Parent;
        Node* grandParent = parent->m_Parent;

        //       g          x
        //      / \   =>   / \
        //     p   4      1   p
        //    / \     =>     / \
        //   x   3          2   g
        //  / \       =>       / \
        // 1   2              3   4
        if (parent->m_Left == node && grandParent->m_Left == parent) {
            if (grandParent == m_Root) {
                m_Root = RotateRight(m_Root);
                m_Root = RotateRight(m_Root);
                return;
            }

            node->m_Parent = RotateRight(grandParent);
            node           = RotateRight(node->m_Parent);

            return Splay(node);
        }

        //   g                  x
        //  / \       =>       / \
        // 1   p              p   4
        //    / \     =>     / \
        //   2   x          g   3
        //      / \   =>   / \
        //     3   4      1   2
        if (parent->m_Right == node && grandParent->m_Right == parent) {
            if (grandParent == m_Root) {
                m_Root = RotateLeft(m_Root);
                m_Root = RotateLeft(m_Root);
                return;
            }

            node->m_Parent = RotateLeft(grandParent);
            node           = RotateLeft(node->m_Parent);

            return Splay(node);
        }
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::ZigZag(Node* node) {
        Node* parent      = node->m_Parent;
        Node* grandParent = parent->m_Parent;

        //     g             x
        //    / \   =>      / \
        //   p   4         /   \
        //  / \     =>    p     g
        // 1   x         / \   / \
        //    / \   =>  1   2 3   4
        //   2   3
        if (parent->m_Right == node && grandParent->m_Left == parent) {
            node   = RotateLeft(parent);
            parent = node->m_Parent;

            if (parent == m_Root) {
                m_Root = RotateRight(m_Root);
                return;
            }

            node = RotateRight(parent);

            return Splay(node);
        }

        //     g             x
        //    / \    =>     / \
        //   1   p         /   \
        //      / \  =>   g     p
        //     x   4     / \   / \
        //    / \    => 1   2 3   4
        //   2   3
        if (parent->m_Left == node && grandParent->m_Right == parent) {
            node   = RotateRight(parent);
            parent = node->m_Parent;

            if (parent == m_Root){
                m_Root = RotateLeft(m_Root);
                return;
            }

            node = RotateLeft(parent);

            return Splay(node);
        }
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Splay(Node* node) {
        if (node == m_Root || !node)
            return;

        Node* parent      = node->m_Parent;
        Node* grandParent = parent->m_Parent;

        if (parent == m_Root)
            return Zig(node);

        if (parent->m_Left  == node && grandParent->m_Left  == parent ||
            parent->m_Right == node && grandParent->m_Right == parent)
            return ZigZig(node);

        if (parent->m_Right == node && grandParent->m_Left  == parent ||
            parent->m_Left  == node && grandParent->m_Right == parent)
            return ZigZag(node);
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Merge(Node* left, Node* right) {
        Node* leftMax = GetMaxNode(left);

        Splay(leftMax);

        leftMax->m_Right = right;
        m_Root           = leftMax;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Print(const Node* node, int level, const char* caption, std::ostream& ostream) const {
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

    template <typename Key, typename Value>
    std::ostream& operator <<(std::ostream& ostream, const SplayTree<Key, Value>& tree) {
        tree.Print(tree.m_Root, 1, "Root", ostream);

        return ostream;
    }
    
} // namespace Ng