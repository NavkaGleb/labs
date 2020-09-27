#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "Random/Random.hpp"

namespace ng {

    template <typename T>
    class GeneralTree {
    public:
        // inner class
        class Node {
        private:
            // variables
            T _value;
            Node* _parent;
            std::vector<Node*> _children;

        public:
            // friends
            friend GeneralTree;

            // constructor
            explicit Node(T _value, Node* _parent = nullptr);
            virtual ~Node();

            // accessors
            const T& value() const;
            const Node* parent() const;
            const std::vector<Node*>& children() const;

            // operators
            friend std::ostream& operator<<(std::ostream& ostream, const Node& node) {
                return ostream << &node << " -> " << *node._value;
            }
            Node& operator=(const Node& other);
            bool operator==(const Node& other);

        };

        // constructor / destructor
        GeneralTree();
        explicit GeneralTree(T value);
        explicit GeneralTree(Node* root);
        ~GeneralTree();

        // accessors
        [[nodiscard]] bool empty() const;
        [[nodiscard]] const std::size_t& nodes() const;
        [[nodiscard]] std::size_t nodes(const Node* node) const;
        std::size_t height(const Node* node) const;
        const Node* path(const std::initializer_list<int>& path) const;
        std::vector<int> path(const Node* node) const;

        // public methods
        const Node* push(const T& value, const std::vector<int>& path = {});
        void pop(const Node* node);
        void pop(const std::initializer_list<int>& path);
        void popChildren(const Node* node);

        void tprint() const;
        void pprint() const;

    private:
        // variables
        Node* _root;
        std::size_t _nodes;

        // private methods
        int _height(const Node* node, const int& height) const;

        int _erase(Node* node, const T& value);

        void _print(const Node* node, const int& level, const int& index) const;
        void _print(const Node* node, const int& level) const;

    };

    ///* ------------------------------------------ class Node ------------------------------------------ *///
    // constructor / destructor
    template <typename T>
    GeneralTree<T>::Node::Node(T _value, Node* _parent) : _value(_value), _parent(_parent) {}

    template <typename T>
    GeneralTree<T>::Node::~Node() {

        for (auto& child : this->_children)
            delete child;

    }

    // accessors
    template <typename T>
    const T& GeneralTree<T>::Node::value() const { return this->_value; }

    template <typename T>
    const typename GeneralTree<T>::Node* GeneralTree<T>::Node::parent() const { return this->_parent; }

    template <typename T>
    const std::vector<typename GeneralTree<T>::Node*>& GeneralTree<T>::Node::children() const { return this->_children; }

    // operators
    template <typename T>
    typename GeneralTree<T>::Node& GeneralTree<T>::Node::operator=(const Node& other) {

        if (this == &other)
            return *this;

        this->value = other._value;
        this->parent = other._parent;
        this->children = other._children;

    }

    template <typename T>
    bool GeneralTree<T>::Node::operator==(const Node& other) {

        return this->_value == other._value && this->_parent == other._parent && this->_children == other._children;

    }

    ///* ------------------------------------------ class GeneralTree ------------------------------------------ *///
    // private methods
    template <typename T>
    int GeneralTree<T>::_height(const Node* node, const int& height) const {

        int mheight = height;										// max height
        int cheight;												// current height

        for (const auto& child : node->_children) {

            cheight = this->_height(child, height + 1);

            if (mheight < cheight)
                mheight = cheight;

        }

        return mheight;

    }

    template <typename T>
    int GeneralTree<T>::_erase(Node* node, const T& value) {

        int nodes = 0;

        if (node->_value == value) {

            return this->erase(node);

        } else if (!node->_children.empty()) {

            for (int i = 0; i < node->_children.size();) {

                int dnodes = this->_erase(node->_children[i], value);                          // deleted nodes
                (dnodes == 0) ? i++ : nodes += dnodes;

            }

        }

        return nodes;

    }

    template <typename T>
    void GeneralTree<T>::_print(const Node* node, const int& level, const int& index) const {

        if (!node)
            return;

        std::cout << index << ". " << *node;

        if (node->_children.size() > 0) {

            std::cout << " (" << std::endl;

            for (int i = 0; i < node->_children.size(); i++) {

                for (int j = 0; j < level; j++) std::cout << "  ";
                this->_print(node->_children[i], level + 1, i);

            }

            for (int i = 0; i < level - 1; i++) std::cout << "  ";
            std::cout << ")";

        }

        std::cout << std::endl;

    }

    template <typename T>
    void GeneralTree<T>::_print(const Node* node, const int& level) const {

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
    template <typename T>
    GeneralTree<T>::GeneralTree() : _root(nullptr) { std::cout << "default constructor" << std::endl; }

    template <typename T>
    GeneralTree<T>::GeneralTree(T value) : _root(new Node(value)) {}

    template <typename T>
    GeneralTree<T>::GeneralTree(Node* root) : _root(root) {}

    template <typename T>
    GeneralTree<T>::~GeneralTree() { delete this->_root; }

    // accessors
    template <typename T>
    bool GeneralTree<T>::empty() const { return this->_root == nullptr; }

    template <typename T>
    std::size_t GeneralTree<T>::nodes(const Node* node) const {

        int amount = 0;

        for (const auto& child : node->_children)
            amount += this->nodes(child);

        return amount + node->_children.size();

    }

    template <typename T>
    std::size_t GeneralTree<T>::height(const Node* node) const { return this->_height(node, 1); }

    template <typename T>
    const typename GeneralTree<T>::Node* GeneralTree<T>::path(const std::initializer_list<int>& path) const {

        if (!this->_root)
            return nullptr;

        Node* currentNode = this->_root;

        for (const auto &index : path) {

            if (currentNode->_children.size() == 0)
                return nullptr;

            currentNode = currentNode->_children[index];

        }

        return currentNode;

    }

    template <typename T>
    std::vector<int> GeneralTree<T>::path(const Node* node) const {

        if (node == this->_root)
            return std::vector<int>();

        std::vector<int> path;
        Node* prevNode = const_cast<Node*>(node);
        Node* currentNode = node->_parent;

        typename std::vector<Node*>::iterator it;

        while (currentNode) {

            it = std::find(currentNode->_children.begin(), currentNode->_children.end(), prevNode);

            if (it == currentNode->_children.end())
                return std::vector<int>();

            path.emplace_back(std::distance(currentNode->_children.begin(), it));
            prevNode = currentNode;
            currentNode = currentNode->_parent;

        }

        std::reverse(path.begin(), path.end());
        return path;

    }

    // public methods
    template <typename T>
    const typename GeneralTree<T>::Node* GeneralTree<T>::push(const T& value, const std::vector<int>& path) {

        /* return new added node */

        if (this->_root == nullptr)
            std::cout << "fuck this nullptr" << std::endl;

        if (!this->_root && path.size() != 0)
            return nullptr;

        if (!this->_root)
            return this->_root = new Node(value);

        Node* parentNode = this->_root;

        for (const auto& index : path) {

            if (parentNode->_children.size() == 0)
                return nullptr;

            parentNode = parentNode->_children[index];

        }

        Node* newNode = new Node(value, parentNode);
        parentNode->_children.emplace_back(newNode);
        return newNode;

    }

    template <typename T>
    void GeneralTree<T>::pop(const Node* node) {

        if (node == this->_root) {

            delete this->_root;
            this->_root = nullptr;
            return;

        }

        Node* parent = node->_parent;

        for (int i = 0; i < parent->_children.size(); i++) {

            if (parent->_children[i] == node) {

                parent->_children.erase(parent->_children.begin() + i);
                delete node;
                break;

            }

        }

    }

    template <typename T>
    void GeneralTree<T>::pop(const std::initializer_list<int>& path) {

         this->erase(this->path(path));

    }

    //    template <typename T>
    //    void GeneralTree<T>::eraseSubTree(const T& value) {
    //
    //        return this->_erase(this->_root, value);
    //
    //    }

    template <typename T>
    void GeneralTree<T>::tprint() const { this->_print(this->_root, 1, 0); }

    template <typename T>
    void GeneralTree<T>::pprint() const { this->_print(this->_root, 1); }

} // namespace ng