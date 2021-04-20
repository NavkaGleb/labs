namespace Ng {

    template <typename Key, typename Value>
    PersistentNode<Key, Value>* PersistentNode<Key, Value>::Clone(PersistentNode* prototype, PersistentNode* parent) {
        auto* node = new PersistentNode(prototype->m_Pair);

        node->m_Parent = parent;
        node->m_Left   = prototype->m_Left;
        node->m_Right  = prototype->m_Right;
        node->m_Color  = prototype->m_Color;

        if (prototype->m_Left)
            prototype->m_Left->m_Parent = node;

        if (prototype->m_Right)
            prototype->m_Right->m_Parent = node;

        if (parent) {
            if (parent->m_Left == prototype)
                parent->m_Left = node;

            if (parent->m_Right == prototype)
                parent->m_Right = node;
        }

        return node;
    }

    template <typename Key, typename Value>
    PersistentNode<Key, Value>::PersistentNode(const Pair& pair)
        : m_Pair(pair)
        , m_Parent(nullptr)
        , m_Left(nullptr)
        , m_Right(nullptr)
        , m_Color(Color::Black)
        , m_Count(0) {}

    template <typename Key, typename Value>
    PersistentNode<Key, Value>::PersistentNode(Pair&& pair)
        : m_Pair(std::move(pair))
        , m_Parent(nullptr)
        , m_Left(nullptr)
        , m_Right(nullptr)
        , m_Color(Color::Black)
        , m_Count(0) {}

    template <typename Key, typename Value>
    PersistentNode<Key, Value>::~PersistentNode() {
        delete m_Left;
        delete m_Right;
    }

    template <typename Key, typename Value>
    void PersistentNode<Key, Value>::Print(const std::string& indent) const {
        std::cout << indent << "+- " << m_Pair.first << " " << this << std::endl;

        if (m_Left)
            m_Left->Print(indent + (!m_Right ? "   " : "|  "));

        if (m_Right)
            m_Right->Print(indent + (m_Right ? "   " : "|  "));
    }

} // namespace Ng