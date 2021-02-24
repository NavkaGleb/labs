namespace Ng {

    template <typename Key, typename Value>
    SplayTree<Key, Value>::Node::~Node() {
        delete m_Left;
        delete m_Right;
    }

    template <typename Key, typename Value>
    SplayTree<Key, Value>::SplayTree(Node* root)
        : m_Root(root)
        , m_Size(root ? 1 : 0) { }

    template <typename Key, typename Value>
    SplayTree<Key, Value>::~SplayTree() {
        delete m_Root;
    }

    template <typename Key, typename Value>
    bool SplayTree<Key, Value>::IsExists(const Key& key) const {
        Node* node = m_Root;

        while (node && key != node->m_Key)
            node = node->m_Key > key ? node->m_Left : node->m_Right;

        return node;
    }

    template <typename Key, typename Value>
    int SplayTree<Key, Value>::GetHeight() const {
        return GetHeight(m_Root);
    }

    template <typename Key, typename Value>
    Value& SplayTree<Key, Value>::Get(const Key& key) {
        Node* node = m_Root;

        while (node && key != node->m_Key)
            node = node->m_Key > key ? node->m_Left : node->m_Right;

        if (!node)
            throw std::out_of_range("Ng::SplayTree::Get: key is not exists!");

        Splay(node);

        return node->m_Value;
    }

    template <typename Key, typename Value>
    const Value& SplayTree<Key, Value>::Get(const Key& key) const {
        return Get(key);
    }

    template <typename Key, typename Value>
    typename SplayTree<Key, Value>::Node* SplayTree<Key, Value>::GetNode(const Key& key) {
        Node* node = m_Root;

        while (node && key != node->m_Key)
            node = node->m_Key > key ? node->m_Left : node->m_Right;

        Splay(node);

        return node;
    }

    template <typename Key, typename Value>
    Value& SplayTree<Key, Value>::Push(const Key& key, const Value& value) {
        ++m_Size;

        if (!m_Root)
            return (m_Root = new Node(key, value))->m_Value;

        Node* node   = m_Root;
        Node* parent = nullptr;

        while (node) {
            if (node->m_Key == key) {
                --m_Size;
                Splay(node);
                return node->m_Value;
            }

            parent = node;
            node   = node->m_Key > key ? node->m_Left : node->m_Right;
        }

        node = new Node(key, value, parent);

        if (parent->m_Key > key)
            parent->m_Left = node;
        else
            parent->m_Right = node;

        Splay(node);

        return node->m_Value;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Pop(const Key& key) {
//        Node* node = GetNode(key);
//
//        if (!node)
//            return;
//
//        --m_Size;
//
//        Node* parent = node->m_Parent;
//        Node* child  = nullptr;
//
//        if (node->m_Left && node->m_Right) {
//            Node* successor = GetSuccessor(node);
//
//            node->m_Value = successor->m_Value;
//            parent        = successor->m_Parent;
//            node          = successor;
//
//            if (successor->m_Right)
//                child = successor->m_Right;
//        } else if (!node->m_Left && node->m_Right) {
//            child = node->m_Right;
//        } else if (node->m_Left && !node->m_Right) {
//            child = node->m_Left;
//        }
//
//        if (parent) {
//            if (parent->m_Left == node)
//                parent->m_Left = child;
//            else
//                parent->m_Right = child;
//
//            if (child)
//                child->m_Parent = parent;
//        } else {
//            m_Root = child;
//
//            if (m_Root)
//                m_Root->m_Parent = nullptr;
//        }
//
//        Splay(node);
//
//
//        node->m_Left  = nullptr;
//        node->m_Right = nullptr;
//
//        delete node;
    }

    template <typename Key, typename Value>
    void SplayTree<Key, Value>::Print() const {
        Print(m_Root, 1, "Root");
    }

    template <typename Key, typename Value>
    int SplayTree<Key, Value>::GetHeight(Node* node) const {
        return (node ? std::max(GetHeight(node->m_Left), GetHeight(node->m_Right)) : 0) + 1;
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
    void SplayTree<Key, Value>::Print(const Node* node, const int& level, const char* caption) const {
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