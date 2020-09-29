#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>

namespace ng {

    template <typename K, typename T, typename C = std::less<>>
    class GeneralMapTree {
    public:
        // inner class
        class Node {
        private:
            // variables
            T _value;
            K _key;
            Node* _parent;
            std::map<K, Node*, C> _children;

        public:
            // friends
            friend GeneralMapTree;

            // constructor
            explicit Node(const T& value, const K& key = K(), Node* parent = nullptr);
            virtual ~Node();

            // accessors
            const T& value() const;
            const K& key() const;
            Node* parent() const;
            const std::map<K, Node*, C>& children() const;

            // operators
            friend std::ostream& operator<<(std::ostream& stream, const Node& node) {
                return stream << node._value;
            }
            Node& operator=(const Node& other);
            bool operator==(const Node& other);

        };

        // constructor / destructor
        GeneralMapTree();
        explicit GeneralMapTree(const T& value);
        explicit GeneralMapTree(Node* root);
        ~GeneralMapTree();

        // accessors
        [[nodiscard]] bool empty() const;
        [[nodiscard]] Node* root() const;
        [[nodiscard]] const std::size_t& nodes() const;
        [[nodiscard]] std::size_t nodes(const Node* node) const;
        std::size_t height(const Node* node) const;
        const Node* path(const std::vector<K>& path) const;
        std::vector<K> path(const Node* node) const;

        // public methods
        Node* push(const K& key, const T& value, const std::vector<K>& path = {});
        Node* push(const K& key, const T& value, Node* parent);
        void pop(const K& key, Node* node);
        void pop(const std::vector<K>& path);

        void tprint(const Node* root) const;
        void pprint() const;

    private:
        // variables
        Node* _root;
        std::size_t _nodes;

        // private methods
        int _height(const Node* node, const int& height) const;

        int _pop(Node* node);

        void _print(const Node* node, const int& level, const K& key) const;
        void _print(const Node* node, const int& level) const;

    };

    ///* ------------------------------------------ class Node ------------------------------------------ *///
    // constructor / destructor
    template <typename K, typename T, typename C>
    GeneralMapTree<K, T, C>::Node::Node(const T& value, const K& key, Node* parent)
        : _value(value), _key(key), _parent(parent) {}

    template <typename K, typename T, typename C>
    GeneralMapTree<K, T, C>::Node::~Node() {

        for (auto& [key, child] : this->_children)
            delete child;

    }

    // accessors
    template <typename K, typename T, typename C>
    const T& GeneralMapTree<K, T, C>::Node::value() const { return this->_value; }

    template <typename K, typename T, typename C>
    typename GeneralMapTree<K, T, C>::Node* GeneralMapTree<K, T, C>::Node::parent() const { return this->_parent; }

    template <typename K, typename T, typename C>
    const std::map<K, typename GeneralMapTree<K, T, C>::Node*, C>& GeneralMapTree<K, T, C>::Node::children() const { return this->_children; }

    // operators
    template <typename K, typename T, typename C>
    typename GeneralMapTree<K, T, C>::Node& GeneralMapTree<K, T, C>::Node::operator=(const Node& other) {

        if (this == &other)
            return *this;

        this->value = other._value;
        this->parent = other._parent;
        this->children = other._children;

    }

    template <typename K, typename T, typename C>
    bool GeneralMapTree<K, T, C>::Node::operator==(const Node& other) {

        return this->_value == other._value && this->_parent == other._parent && this->_children == other._children;

    }

    ///* ------------------------------------------ class GeneralMapTree ------------------------------------------ *///
    // private methods
    template <typename K, typename T, typename C>
    int GeneralMapTree<K, T, C>::_height(const Node* node, const int& height) const {

        int mheight = height;										// max height
        int cheight;												// current height

        for (const auto& child : node->_children) {

            cheight = this->_height(child, height + 1);

            if (mheight < cheight)
                mheight = cheight;

        }

        return mheight;

    }

    template <typename K, typename T, typename C>
    int GeneralMapTree<K, T, C>::_pop(Node* node) {

        if (!node)
            return 0;

        if (!node->_children.empty())
            return 1;

        int nodes = node->_children.size();

        for (const auto& [key, child] : node->_children)
            nodes += this->_pop(child);

        delete [] node;
        return nodes;

    }

    template <typename K, typename T, typename C>
    void GeneralMapTree<K, T, C>::_print(const Node* node, const int& level, const K& key) const {

        if (!node)
            return;

        std::cout << key << " -> " << *node;

        if (node->_children.size() > 0) {

            std::cout << " (" << std::endl;

            for (const auto& [key, child] : node->_children) {

                for (int j = 0; j < level; j++)
                    std::cout << "  ";

                this->_print(child, level + 1, key);

            }

            for (int i = 0; i < level - 1; i++)
                std::cout << "  ";

            std::cout << ")";

        }

        std::cout << std::endl;

    }

    template <typename K, typename T, typename C>
    void GeneralMapTree<K, T, C>::_print(const Node* node, const int& level) const {

        if (!node)
            return;

        std::vector<int> path = this->path(node);

        if (!path.empty()) {

            std::cout << "{ ";
            for (int i = 0; i < path.size() - 1; i++) std::cout << path[i] << "-";
            std::cout << path.back() << " } ";

        }

        std::cout << *node;

        if (node->_children.size() > 0) {

            std::cout << " (" << std::endl;

            for (int i = 0; i < node->_children.size(); i++) {

                for (int j = 0; j < level; j++) std::cout << "  ";
                this->_print(node->_children[i], level + 1);

            }

            for (int i = 0; i < level - 1; i++) std::cout << "  ";
            std::cout << ")";

        }

        std::cout << std::endl;

    }

    // constructor / destructor
    template <typename K, typename T, typename C>
    GeneralMapTree<K, T, C>::GeneralMapTree() : _root(nullptr) {}

    template <typename K, typename T, typename C>
    GeneralMapTree<K, T, C>::GeneralMapTree(const T& value) : _root(new Node(value)) {}

    template <typename K, typename T, typename C>
    GeneralMapTree<K, T, C>::GeneralMapTree(Node* root) : _root(root) {}

    template <typename K, typename T, typename C>
    GeneralMapTree<K, T, C>::~GeneralMapTree() { delete this->_root; }

    // accessors
    template <typename K, typename T, typename C>
    bool GeneralMapTree<K, T, C>::empty() const { return this->_root == nullptr; }

    template <typename K, typename T, typename C>
    typename GeneralMapTree<K, T, C>::Node* GeneralMapTree<K, T, C>::root() const { return this->_root; }

    template <typename K, typename T, typename C>
    const std::size_t& GeneralMapTree<K, T, C>::nodes() const { return this->_nodes; }

    template <typename K, typename T, typename C>
    std::size_t GeneralMapTree<K, T, C>::nodes(const Node* node) const {

        int amount = 0;

        for (const auto& child : node->_children)
            amount += this->nodes(child);

        return amount + node->_children.size();

    }

    template <typename K, typename T, typename C>
    std::size_t GeneralMapTree<K, T, C>::height(const Node* node) const { return this->_height(node, 1); }

    template <typename K, typename T, typename C>
    const typename GeneralMapTree<K, T, C>::Node* GeneralMapTree<K, T, C>::path(const std::vector<K>& path) const {

        if (!this->_root)
            return nullptr;

        Node* currentNode = this->_root;

        for (std::size_t i = 0; i < path.size(); ++i) {

            if (currentNode->_children.size() == 0)
                return nullptr;

            if (currentNode->_children.count(path[i]) == 1)
                currentNode = currentNode->_children[path[i]];
            else if (i != path.size() - 1)
                return nullptr;

        }

        return currentNode;

    }

    template <typename K, typename T, typename C>
    std::vector<K> GeneralMapTree<K, T, C>::path(const Node* node) const {

        if (node == this->_root)
            return {};

        std::vector<K> path;
        Node* current = node->_parent;

        while (current) {

            path.emplace_back(current->_key);
            current = current->_parent;

        }

        std::reverse(path.begin(), path.end());
        return path;

    }

    // public methods
    template <typename K, typename T, typename C>
    typename GeneralMapTree<K, T, C>::Node* GeneralMapTree<K, T, C>::push(
        const K& key, const T& value, const std::vector<K>& path) {

        if (!this->_root && path.size() != 0)
            return nullptr;

        if (!this->_root)
            return this->_root = new Node(value);

        Node* parent = this->_root;

        for (std::size_t i = 0; i < path.size(); ++i) {

            if (parent->_children.size() == 0)
                return nullptr;

            if (parent->_children.count(path[i]) == 1)
                parent = parent->_children[path[i]];
            else if (i != path.size() - 1)
                return nullptr;

        }

        Node* newNode = new Node(value, key, parent);
        parent->_children[key] = newNode;
        ++this->_nodes;
        return newNode;

    }

    template <typename K, typename T, typename C>
    typename GeneralMapTree<K, T, C>::Node* GeneralMapTree<K, T, C>::push(const K& key, const T& value, Node* parent) {

        Node* newNode = new Node(value, key, parent);

        parent->_children[key] = newNode;
        ++this->_nodes;

        return newNode;

    }

    template <typename K, typename T, typename C>
    void GeneralMapTree<K, T, C>::pop(const K& key, Node* node) {

        if (node == this->_root) {

            delete this->_root;
            this->_root = nullptr;
            this->_nodes = 0;
            return;

        }

        Node* parent = node->_parent;
        this->_nodes -= this->_pop(node);
        parent->_children.erase(key);

    }

    template <typename K, typename T, typename C>
    void GeneralMapTree<K, T, C>::pop(const std::vector<K>& path) {

        this->pop(path.back(), this->path(path));

    }

    template <typename K, typename T, typename C>
    void GeneralMapTree<K, T, C>::tprint(const Node* root) const { this->_print(root, 1, root->value()->filename()); }

    template <typename K, typename T, typename C>
    void GeneralMapTree<K, T, C>::pprint() const { this->_print(this->_root, 1); }

} // namespace ng