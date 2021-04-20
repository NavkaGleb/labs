#include <cassert>

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
    std::pair<typename PersistentNode<Key, Value>::SmartPointer, bool> PersistentNode<Key, Value>::Push(
        const SmartPointer& node,
        const Key&          key,
        const Value&        value
    ) {
        if (node) {
            if (key < node->m_Pair.first) {
                const auto [newLeft, isPushed] = Push(node->m_Left, key, value);
                const auto newNode             = node->CloneWithLeft(newLeft);

                return std::make_pair(
                    isPushed ? newNode->Balance() : newNode,
                    isPushed
                );
            } else if (key > node->m_Pair.first) {
                const auto [newRight, isPushed] = Push(node->m_Right, key, value);
                const auto newNode              = node->CloneWithRight(newRight);

                return std::make_pair(
                    isPushed ? newNode->Balance() : newNode,
                    isPushed
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

    template <typename Key, typename Value>
    std::pair<typename PersistentNode<Key, Value>::SmartPointer, bool> PersistentNode<Key, Value>::Pop(
        const SmartPointer& node,
        const Key&          key
    ) {
        if (!node)
            return std::make_pair(nullptr, false);

        if (key < node->m_Pair.first)
            return PopLeft(node, key);

        if (key > node->m_Pair.first)
            return PopRight(node, key);

        const auto newNode = Fuse(node->m_Left, node->m_Right);
        return std::make_pair(newNode, true);
    }

    template <typename Key, typename Value>
    std::pair<typename PersistentNode<Key, Value>::SmartPointer, bool> PersistentNode<Key, Value>::PopLeft(
        const SmartPointer& node,
        const Key&          key
    ) {
        const auto [newLeft, isPopped] = Pop(node->m_Left, key);
        const auto newNode             = std::make_shared<const PersistentNode>(
            node->m_Pair,
            newLeft,
            node->m_Right
        );

        const auto balancedNewNode = node->m_Left && node->m_Left->IsBlack() ? BalanceLeft(newNode) : newNode;

        return std::make_pair(balancedNewNode, isPopped);
    }

    template <typename Key, typename Value>
    std::pair<typename PersistentNode<Key, Value>::SmartPointer, bool> PersistentNode<Key, Value>::PopRight(
        const SmartPointer& node,
        const Key&          key
    ) {
        const auto [newRight, isPopped] = Pop(node->m_Right, key);
        const auto newNode              = std::make_shared<const PersistentNode>(
            node->m_Pair,
            node->m_Left,
            newRight
        );

        const auto bal_new_node = node->m_Right && node->m_Right->IsBlack() ? BalanceRight(newNode) : newNode;

        return std::make_pair(bal_new_node, isPopped);
    }

    template <typename Key, typename Value>
    typename PersistentNode<Key, Value>::SmartPointer PersistentNode<Key, Value>::Fuse(
        const SmartPointer& left,
        const SmartPointer& right
    ) {
        // match: (left, right)

        // case: (None, r)
        if (!left)
            return right;

        // case: (l, None)
        if (!right)
            return left;

        // case: (Some(l), Some(r))
        // fall through
        assert(left && right);

        // match: (left.color, right.color)
        // case: (B, R)
        if (!left->IsRed() && right->IsRed()) {
            return std::make_shared<const PersistentNode>(
                right->m_Pair,
                Fuse(left, right->m_Left),
                right->m_Right
            );

            // case: (R, B)
        } else if (left->IsRed() && right->IsBlack()) {
            return std::make_shared<const PersistentNode>(
                left->m_Pair,
                left->m_Left,
                Fuse(left->m_Right, right)
            );

            // case: (R, R)
        } else if (left->IsRed() && right->IsRed()) {
            const auto fused = Fuse(left->m_Right, right->m_Left);

            if (fused && fused->IsRed()) {
                const auto newLeft = std::make_shared<const PersistentNode>(
                    left->m_Pair,
                    left->m_Left,
                    fused->m_Left
                );

                const auto newRight = std::make_shared<const PersistentNode>(
                    right->m_Pair,
                    fused->m_Right,
                    right->m_Right
                );

                return std::make_shared<const PersistentNode>(fused->m_Pair, newLeft, newRight);
            }

            const auto newRight = std::make_shared<const PersistentNode>(
                right->m_Pair,
                fused,
                right->m_Right
            );

            return std::make_shared<const PersistentNode>(left->m_Pair, left->m_Left, newRight);

            // case: (B, B)
        } else if (left->IsBlack() && right->IsBlack()) {
            const auto fused = Fuse(left->m_Right, right->m_Left);

            if (fused && fused->IsRed()) {
                const auto newLeft = std::make_shared<const PersistentNode>(
                    left->m_Pair,
                    left->m_Left,
                    fused->m_Left,
                    Color::Black
                );

                const auto newRight = std::make_shared<const PersistentNode>(
                    right->m_Pair,
                    fused->m_Right,
                    right->m_Right,
                    Color::Black
                );

                return std::make_shared<const PersistentNode>(fused->m_Pair, newLeft, newRight);
            }

            const auto newRight = std::make_shared<const PersistentNode>(
                right->m_Pair,
                fused,
                right->m_Right,
                Color::Black
            );

            const auto newNode = std::make_shared<const PersistentNode>(
                left->m_Pair,
                left->m_Left,
                newRight
            );

            return BalanceLeft(newNode);
        }

        assert(0); // LCOV_EXCL_LINE
    }

    template <typename Key, typename Value>
    typename PersistentNode<Key, Value>::SmartPointer PersistentNode<Key, Value>::Balance(const SmartPointer& node) {
        if (node->m_Left && node->m_Left->IsRed() && node->m_Right && node->m_Right->IsRed()) {
            const auto newLeft  = node->m_Left  ? node->m_Left->CloneAsBlack()  : node->m_Left;
            const auto newRight = node->m_Right ? node->m_Right->CloneAsBlack() : node->m_Right;

            return std::make_shared<const PersistentNode>(node->m_Pair, newLeft, newRight);
        }

        assert(node->IsBlack());
        return node->Balance();
    }

    template <typename Key, typename Value>
    typename PersistentNode<Key, Value>::SmartPointer PersistentNode<Key, Value>::BalanceLeft(
        const SmartPointer& node
    ) {
        // match: (color_l, color_r, color_r_l)
        // case: (Some(R), ..)
        if (node->m_Left && node->m_Left->IsRed()) {
            const auto newLeft = std::make_shared<const PersistentNode>(
                node->m_Left->m_Pair,
                node->m_Left->m_Left,
                node->m_Left->m_Right,
                Color::Black
            );

            return std::make_shared<const PersistentNode>(node->m_Pair, newLeft, node->m_Right);

            // case: (_, Some(B), _)
        } else if (node->m_Right && node->m_Right->IsBlack()) {
            const auto newRight = std::make_shared<const PersistentNode>(
                node->m_Right->m_Pair,
                node->m_Right->m_Left,
                node->m_Right->m_Right
            );

            const auto newNode = std::make_shared<const PersistentNode>(
                node->m_Pair,
                node->m_Left,
                newRight,
                Color::Black
            );

            return Balance(newNode);

            // case: (_, Some(R), Some(B))
        } else if (node->m_Right         && node->m_Right->IsRed() &&
                   node->m_Right->m_Left && node->m_Right->m_Left->IsBlack()) {

            const auto newRight = Balance(std::make_shared<const PersistentNode>(
                node->m_Right->m_Pair,
                node->m_Right->m_Left->m_Right,
                node->m_Right->m_Right->CloneAsRed(),
                Color::Black
            ));

            const auto newLeft = std::make_shared<const PersistentNode>(
                node->m_Pair,
                node->m_Left,
                node->m_Right->m_Left->m_Left,
                Color::Black
            );

            return std::make_shared<const PersistentNode>(node->m_Right->m_Left->m_Pair, newLeft, newRight);
        }

        assert(0);
    }

    template <typename Key, typename Value>
    typename PersistentNode<Key, Value>::SmartPointer PersistentNode<Key, Value>::BalanceRight(
        const SmartPointer& node
    ) {
        // match: (color_l, color_l_r, color_r)
        // case: (.., Some(R))
        if (node->m_Right && node->m_Right->IsRed()) {
            const auto newRight = std::make_shared<const PersistentNode>(
                node->m_Right->m_Pair,
                node->m_Right->m_Left,
                node->m_Right->m_Right,
                Color::Black
            );

            return std::make_shared<const PersistentNode>(node->m_Pair, node->m_Left, newRight);

            // case: (Some(B), ..)
        } else if (node->m_Left && !node->m_Left->IsRed()) {
            const auto newLeft = std::make_shared<const PersistentNode>(
                node->m_Left->m_Pair,
                node->m_Left->m_Left,
                node->m_Left->m_Right
            );

            const auto newNode = std::make_shared<const PersistentNode>(
                node->m_Pair,
                newLeft,
                node->m_Right,
                Color::Black
            );

            return Balance(newNode);

            // case: (Some(R), Some(B), _)
        } else if (node->m_Left          && node->m_Left->IsRed() &&
                   node->m_Left->m_Right && !node->m_Left->m_Right->IsRed()) {

            const auto newLeft = Balance(std::make_shared<const PersistentNode>(
                node->m_Left->m_Pair,
                node->m_Left->m_Left->CloneAsRed(),
                node->m_Left->m_Right->m_Left,
                Color::Black
            ));

            const auto newRight = std::make_shared<const PersistentNode>(
                node->m_Pair,
                node->m_Left->m_Right->m_Right,
                node->m_Right,
                Color::Black
            );

            return std::make_shared<const PersistentNode>(node->m_Left->m_Right->m_Pair, newLeft, newRight);
        }

        assert(0);
    }

} // namespace Ng