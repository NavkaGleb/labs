namespace Ng {

    template <typename Key, typename Value>
    PersistentNode<Key, Value>::PersistentNode(
        const Pair&         pair,
        const SmartPointer& left,
        const SmartPointer& right,
        Color               color
    )
        : m_Pair(pair)
        , m_Left(left)
        , m_Right(right)
        , m_Color(color) {}

    template <typename Key, typename Value>
    PersistentNodePtr<Key, Value> PersistentNode<Key, Value>::CloneWithPair(const Pair& pair) const {
        return std::make_shared<const PersistentNode>(pair, m_Left, m_Right, m_Color);
    }

    template <typename Key, typename Value>
    PersistentNodePtr<Key, Value> PersistentNode<Key, Value>::CloneWithLeft(const SmartPointer& left) const {
        return std::make_shared<const PersistentNode>(m_Pair, left, m_Right, m_Color);
    }

    template <typename Key, typename Value>
    PersistentNodePtr<Key, Value> PersistentNode<Key, Value>::CloneWithRight(const SmartPointer& right) const {
        return std::make_shared<const PersistentNode>(m_Pair, m_Left, right, m_Color);
    }

    template <typename Key, typename Value>
    PersistentNodePtr<Key, Value> PersistentNode<Key, Value>::CloneAsRed() const {
        return std::make_shared<const PersistentNode>(m_Pair, m_Left, m_Right, Color::Red);
    }

    template <typename Key, typename Value>
    PersistentNodePtr<Key, Value> PersistentNode<Key, Value>::CloneAsBlack() const {
        return std::make_shared<const PersistentNode>(m_Pair, m_Left, m_Right, Color::Black);
    }

    template <typename Key, typename Value>
    typename PersistentNode<Key, Value>::SmartPointer PersistentNode<Key, Value>::Balance() const {
        if (IsBlack()) {
            // match: (color_l, color_l_l, color_l_r, color_r, color_r_l, color_r_r)
            if (m_Left && m_Left->IsRed()) {
                // case: (Some(R), Some(R), ..)
                if (m_Left->m_Left && m_Left->m_Left->IsRed()) {
                    const auto newLeft = std::make_shared<PersistentNode>(
                        m_Left->m_Left->m_Pair,
                        m_Left->m_Left->m_Left,
                        m_Left->m_Left->m_Right,
                        Color::Black
                    );

                    const auto newRight = std::make_shared<PersistentNode>(
                        m_Pair,
                        m_Left->m_Right,
                        m_Right,
                        Color::Black
                    );

                    return std::make_shared<const PersistentNode>(m_Left->m_Pair, newLeft, newRight);

                    // case: (Some(R), _, Some(R), ..)
                } else if (m_Left->m_Right && m_Left->m_Right->IsRed()) {
                    const auto newLeft = std::make_shared<PersistentNode>(
                        m_Left->m_Pair,
                        m_Left->m_Left,
                        m_Left->m_Right->m_Left,
                        Color::Black
                    );

                    const auto newRight = std::make_shared<PersistentNode>(
                        m_Pair,
                        m_Left->m_Right->m_Right,
                        m_Right,
                        Color::Black
                    );

                    return std::make_shared<const PersistentNode>(m_Left->m_Right->m_Pair, newLeft, newRight);
                }
            }

            // case: (.., Some(R), Some(R), _)
            if (m_Right && m_Right->IsRed()) {
                if (m_Right->m_Left && m_Right->m_Left->IsRed()) {
                    const auto newLeft = std::make_shared<PersistentNode>(
                        m_Pair,
                        m_Left,
                        m_Right->m_Left->m_Left,
                        Color::Black
                    );

                    const auto newRight = std::make_shared<PersistentNode>(
                        m_Right->m_Pair,
                        m_Right->m_Left->m_Right,
                        m_Right->m_Right,
                        Color::Black
                    );

                    return std::make_shared<const PersistentNode>(m_Right->m_Left->m_Pair, newLeft, newRight);

                    // case: (.., Some(R), _, Some(R))
                } else if (m_Right->m_Right && m_Right->m_Right->IsRed()) {
                    const auto newLeft = std::make_shared<PersistentNode>(
                        m_Pair,
                        m_Left,
                        m_Right->m_Left,
                        Color::Black
                    );

                    const auto newRight = std::make_shared<PersistentNode>(
                        m_Right->m_Right->m_Pair,
                        m_Right->m_Right->m_Left,
                        m_Right->m_Right->m_Right,
                        Color::Black
                    );

                    return std::make_shared<const PersistentNode>(m_Right->m_Pair, newLeft, newRight);
                }
            }
        }

        // red, or no matching case above

        return this->shared_from_this();
    }

    template <typename Key, typename Value>
    void PersistentNode<Key, Value>::Print(const std::string& indent) const {
        std::cout << indent << "+- " << m_Pair.first << ":" << (m_Color == Color::Red ? "Red" : "Black") << std::endl;

        if (m_Left)
            m_Left->Print(indent + "   ");

        if (m_Right)
            m_Right->Print(indent + "   ");
    }

    template <typename Key, typename Value>
    std::pair<PersistentNodePtr<Key, Value>, bool> PersistentNode<Key, Value>::Push(
        const SmartPointer& node,
        const Key&          key,
        const Value&        value
    ) {
        if (node) {
            if (key < node->m_Pair.first) {
                const auto [newLeft, response] = Push(node->m_Left, key, value);
                const auto newNode             = node->CloneWithLeft(newLeft);

                return std::make_pair(
                    response ? newNode->Balance() : newNode,
                    response
                );
            } else if (key > node->m_Pair.first) {
                const auto [newRight, response] = Push(node->m_Right, key, value);
                const auto newNode              = node->CloneWithRight(newRight);

                return std::make_pair(
                    response ? newNode->Balance() : newNode,
                    response
                );
            } else {
                const auto newNode = node->CloneWithPair({ key, value });
                return std::make_pair(newNode, false);
            }
        } else {
            const auto newNode = std::make_shared<const PersistentNode>(std::make_pair(key, value));
            return std::make_pair(newNode, true);
        }
    }

} // namespace Ng