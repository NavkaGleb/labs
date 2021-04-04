namespace Ng {

    template <typename T>
    BPlusTree<T>::BPlusTree()
        : m_Root(nullptr) {}

    template <typename T>
    BPlusTree<T>::~BPlusTree() {
        delete m_Root;
    }

} // namespace Ng