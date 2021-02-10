//////////////////////////////////////////////////////////////////////////////
/// class OrderStatisticsTree::Node
//////////////////////////////////////////////////////////////////////////////
template <typename T>
OrderStatisticsTree<T>::Node::Node() :
    m_Value(T()),
    m_Color(Color::Black),
    m_Parent(nullptr),
    m_Left(nullptr),
    m_Right(nullptr),
    m_Children(1) { }

template <typename T>
OrderStatisticsTree<T>::Node::Node(const T& value, Node* parent, Node* left, Node* right) :
    m_Value(value),
    m_Color(Node::Color::Red),
    m_Parent(parent),
    m_Left(left),
    m_Right(right),
    m_Children(1) { }

template <typename T>
OrderStatisticsTree<T>::Node::~Node() {
    delete m_Left;
    delete m_Right;
}

template <typename T>
void OrderStatisticsTree<T>::Node::Print() const {
    std::cout << m_Value << " {C: ";

    std::cout << (m_Color == Node::Color::Black ? "Black"                          : "Red" ) << ", L: ";
    std::cout << (m_Left                        ? std::to_string(m_Left->m_Value)  : "Null") << ", R: ";
    std::cout << (m_Right                       ? std::to_string(m_Right->m_Value) : "Null") << "}";
}

//////////////////////////////////////////////////////////////////////////////
/// class OrderStatisticsTree
//////////////////////////////////////////////////////////////////////////////
template <typename T>
OrderStatisticsTree<T>::OrderStatisticsTree(const T& value) :
    m_Root(new Node(value)),
    m_Nodes(1) {}

template <typename T>
OrderStatisticsTree<T>::OrderStatisticsTree(Node* root) :
    m_Root(root),
    m_Nodes(root ? 1 : 0) { }

template <typename T>
OrderStatisticsTree<T>::~OrderStatisticsTree() {
    delete m_Root;
}

template <typename T>
int OrderStatisticsTree<T>::GetHeight() const {
    return GetHeight(m_Root);
}

template <typename T>
bool OrderStatisticsTree<T>::IsExists(const T& value) const {
    Node* node = m_Root;

    while (node && value != node->m_Value)
        node = node->m_Value < value ? node->m_Left : node->m_Right;

    return node;
}

template <typename T>
std::optional<T> OrderStatisticsTree<T>::GetMin() const {
    return GetMin(m_Root);
}

template <typename T>
std::optional<T> OrderStatisticsTree<T>::GetMax() const {
    return GetMax(m_Root);
}

template <typename T>
typename OrderStatisticsTree<T>::Node* OrderStatisticsTree<T>::Push(const T& value) {
    if (!m_Root) {
        m_Root = new Node(value);
        m_Root->m_Color = Node::Color::Black;

        return m_Root;
    }

    Node* node   = m_Root;
    Node* parent = nullptr;


    while (node) {
        ++node->m_Children;

        parent = node;

        if (node->m_Value > value)
            node = node->m_Left;
        else
            node = node->m_Right;
    }

    node = new Node(value, parent);

    if (parent->m_Value > value)
        parent->m_Left = node;
    else
        parent->m_Right = node;

    PushFix(node);

    return node;
}

template <typename T>
void OrderStatisticsTree<T>::Pop(const T& value) {
    Node* node = m_Root;

    while (node && node->m_Value != value)
        node = node->m_Value > value ? node->m_Left : node->m_Right;

    if (!node) {
        std::cout << "Fuck!" << std::endl;
        return;
    }

    Node* parent = node->m_Parent;
    Node* child  = nullptr;

    if (node->m_Left && node->m_Right) {
        Node* successor = GetSuccessor(node);

        node->m_Value = successor->m_Value;
        parent        = successor->m_Parent;
        node          = successor;

        if (successor->m_Right)
            child = successor->m_Right;
    } else if (!node->m_Left && node->m_Right) {
        child = node->m_Right;
    } else if (node->m_Left && !node->m_Right) {
        child = node->m_Left;
    }

    if (parent) {
        if (parent->m_Left == node)
            parent->m_Left = child;
        else
            parent->m_Right = child;
    } else {
        m_Root = child;

        if (m_Root)
            m_Root->m_Parent = nullptr;
    }

    if (node->m_Color == Node::Color::Black)
        PopFix(child, parent);
}

template <typename T>
void OrderStatisticsTree<T>::Print() const {
    Print(m_Root, 1, "Root");
}

template <typename T>
int OrderStatisticsTree<T>::GetHeight(Node* node) const {
    return node ? std::max(GetHeight(node->m_Left), GetHeight(node->m_Right)) + 1 : 0;
}

template <typename T>
std::optional<T> OrderStatisticsTree<T>::GetMin(Node* node) const {
    if (!node)
        return std::nullopt;

    while (node->m_Left)
        node = node->m_Left;

    return node->m_Value;
}

template <typename T>
std::optional<T> OrderStatisticsTree<T>::GetMax(Node* node) const {
    if (!node)
        return std::nullopt;

    while (node->m_Right)
        node = node->m_Right;

    return node->m_Value;
}

template <typename T>
typename OrderStatisticsTree<T>::Node* OrderStatisticsTree<T>::GetMinNode(Node* node) const {
    while (node && node->m_Left)
        node = node->m_Left;

    return node;
}

template <typename T>
typename OrderStatisticsTree<T>::Node* OrderStatisticsTree<T>::GetMaxNode(Node* node) const {
    while (node && node->m_Right)
        node = node->m_Right;

    return node;
}

template <typename T>
typename OrderStatisticsTree<T>::Node* OrderStatisticsTree<T>::GetSuccessor(Node* node) const {
    Node* result;

    if (node->m_Left) {
        result = node->m_Left;

        while (result->m_Right)
            result = result->m_Right;
    } else {
        result = node->m_Parent;

        while (result && result->m_Left == node) {
            node = result;
            node = result->m_Parent;
        }

        result = result && result->m_Right == node ? result : nullptr;
    }

    return result;
}

template <typename T>
void OrderStatisticsTree<T>::RotateLeft(Node* node) {
    //   c      =>      s
    //  / \            / \
    // u   s    =>    c   r
    //    / \        / \
    //   l   r  =>  u   l

    Node* right     = node->m_Right;
    Node* rightLeft = right->m_Left;

    right->m_Parent = node->m_Parent;

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
}

template <typename T>
void OrderStatisticsTree<T>::RotateRight(Node* node) {
    //     c    =>    u
    //    / \        / \
    //   u   s  =>  l   c
    //  / \            / \
    // l   r    =>    r   s

    Node* left      = node->m_Left;
    Node* leftRight = left->m_Right;

    left->m_Parent = node->m_Parent;

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
}

template <typename T>
void OrderStatisticsTree<T>::PushFix(Node* node) {
    while (node->m_Parent && node->m_Parent->m_Color == Node::Color::Red) {
        Node* parent = node->m_Parent;
        Node* uncle  = node->m_Parent->m_Parent && node->m_Parent->m_Parent->m_Left == node->m_Parent ?
                       node->m_Parent->m_Parent->m_Right :
                       node->m_Parent->m_Parent->m_Left;

        // uncle is leaf => color is black
        if (!uncle || uncle->m_Color == Node::Color::Black) {
            // if uncle is at right, parent at left.
            if (parent->m_Parent->m_Left == parent) {
                // from shape triangle to line
                if (parent->m_Left != node) {
                    RotateLeft(parent);

                    node = parent;
                    parent = node->m_Parent;
                }

                // grand parent rotation from line shape
                if (parent->m_Left == node) {
                    parent->m_Color           = Node::Color::Black;
                    parent->m_Parent->m_Color = Node::Color::Red;

                    RotateRight(parent->m_Parent);
                    // will break because parent color has changed to black.
                }
            } else {
                // from shape triangle to line
                if (parent->m_Right != node) {
                    RotateRight(parent);

                    node = parent;
                    parent = node->m_Parent;
                }

                // grand parent rotation from line shape
                if (parent->m_Right == node) {
                    parent->m_Color           = Node::Color::Black;
                    parent->m_Parent->m_Color = Node::Color::Red;

                    RotateLeft(parent->m_Parent);
                }
            }
        } else if (uncle->m_Color == Node::Color::Red) {
            parent->m_Color = Node::Color::Black;
            uncle->m_Color = Node::Color::Black;
            parent->m_Parent->m_Color = Node::Color::Red;

            node = parent->m_Parent;
        }

    }

    m_Root->m_Color = Node::Color::Black;
}

template <typename T>
void OrderStatisticsTree<T>::PopFix(Node* child, Node* parent) {
    while (true) {
        if (child == m_Root) {
            child->m_Color = Node::Color::Black;
            break;
        }

        parent = child->m_Parent;

        if (parent->m_Left == child) {
            Node *sibling = parent->m_Right;

            if (sibling->m_Color == Node::Color::Red) {
                parent->m_Color = Node::Color::Red;
                sibling->m_Color = Node::Color::Black;

                RotateLeft(parent);
            }

            sibling = parent->m_Right;

            // parent is black, sibling, and sibling's children are all black
            if (parent->m_Color           == Node::Color::Black &&
                sibling->m_Color          == Node::Color::Black &&
                sibling->m_Left->m_Color  == Node::Color::Black &&
                sibling->m_Right->m_Color == Node::Color::Black) {

                sibling->m_Color = Node::Color::Red;
                child = parent;

                continue;
            }

            // parent is red, sibling, and sibling's children are all black
            if (parent->m_Color           == Node::Color::Red &&
                sibling->m_Color          == Node::Color::Black &&
                sibling->m_Left->m_Color  == Node::Color::Black &&
                sibling->m_Right->m_Color == Node::Color::Black) {

                parent->m_Color = Node::Color::Black;
                sibling->m_Color = Node::Color::Red;

                break;
            }

            // above cases sibling children are all back, now reduce one of sibling child is red cases:
            // if sibling's left is red, sibling's right is black (triangle pattern again)
            if (sibling->m_Color == Node::Color::Black &&
                sibling->m_Left->m_Color == Node::Color::Red &&
                sibling->m_Right->m_Color == Node::Color::Black) {

                sibling->m_Color = Node::Color::Red;
                sibling->m_Left->m_Color = Node::Color::Black;
            }

            sibling = parent->m_Right;

            // sibling's left is black, sibling's right is red, sibling is black
            // since one of sibling child is red cases are are reduce to only this case
            // now compare to current node again.
            if (sibling->m_Color          == Node::Color::Black &&
                sibling->m_Right->m_Color == Node::Color::Red) {

                sibling->m_Color          = parent->m_Color;
                parent->m_Color           = Node::Color::Black;
                sibling->m_Right->m_Color = Node::Color::Black;

                RotateLeft(parent);
                break;
            }
        } else {
            Node* sibling = parent->m_Left;

            if (sibling->m_Color == Node::Color::Red) {
                parent->m_Color = Node::Color::Red;
                sibling->m_Color = Node::Color::Black;

                RotateRight(parent);
            }

            sibling = parent->m_Left;

            // parent is black, sibling, and sibling's children are all black
            if (parent->m_Color           == Node::Color::Black &&
                sibling->m_Color          == Node::Color::Black &&
                sibling->m_Left->m_Color  == Node::Color::Black &&
                sibling->m_Right->m_Color == Node::Color::Black) {

                sibling->m_Color = Node::Color::Red;
                child = parent;

                continue;
            }

            // parent is red, sibling, and sibling's children are all black
            if (parent->m_Color           == Node::Color::Red &&
                sibling->m_Color          == Node::Color::Black &&
                sibling->m_Left->m_Color  == Node::Color::Black &&
                sibling->m_Right->m_Color == Node::Color::Black) {

                parent->m_Color  = Node::Color::Black;
                sibling->m_Color = Node::Color::Red;

                break;
            }

            // above cases sibling children are all back, now reduce one of sibling child is red cases:
            // if sibling's left is red, sibling's right is black (triangle pattern again)
            if (sibling->m_Color          == Node::Color::Black &&
                sibling->m_Left->m_Color  == Node::Color::Black &&
                sibling->m_Right->m_Color == Node::Color::Red) {

                sibling->m_Color          = Node::Color::Red;
                sibling->m_Right->m_Color = Node::Color::Black;
            }

            sibling = parent->m_Left;

            // sibling's left is red, sibling's right is black, sibling is black
            // since one of sibling child is red cases are are reduce to only this case
            // now compare to current node again.
            if (sibling->m_Color         == Node::Color::Black &&
                sibling->m_Left->m_Color == Node::Color::Red) {

                sibling->m_Color         = parent->m_Color;
                parent->m_Color          = Node::Color::Black;
                sibling->m_Left->m_Color = Node::Color::Black;

                RotateRight(parent);
                break;
            }
        }
    }
}

template <typename T>
void OrderStatisticsTree<T>::Transplant(Node* first, Node* second) {
    if (!first->m_Parent)
        m_Root = second;
    else if (first == first->m_Parent->m_Left)
        first->m_Parent->m_Left = second;
    else
        first->m_Parent->m_Right = second;

    second->m_Parent = first->m_Parent;
}

template <typename T>
void OrderStatisticsTree<T>::Print(const Node* node, const int& level, const char* caption) const {
    if (!node) {
        std::cout << caption << ": null" << std::endl;
        return;
    }

    std::cout << caption << ": ";
    node->Print();

    if (node->m_Left || node->m_Right) {
        std::cout << " (" << std::endl;

        for (int i = 0; i < level; i++)
            std::cout << "| ";
        Print(node->m_Left, level + 1, "left");

        for (int i = 0; i < level; i++)
            std::cout << "| ";
        Print(node->m_Right, level + 1, "right");

        for (int i = 0; i < level - 1; i++)
            std::cout << "| ";
        std::cout << ")";
    }

    std::cout << std::endl;
}