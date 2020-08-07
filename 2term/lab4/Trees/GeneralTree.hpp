#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <initializer_list>

#include "../Rand/rand.hpp"
#include "Tree.hpp"

typedef unsigned uint;

namespace ng {

	template <typename T>
	class BinarySearchTree;

	template <typename T>
	class GeneralTree : public Tree {
		// friends
		friend BinarySearchTree<T>;

	private:
		// inner class
		class Node {
			// friends
			friend GeneralTree;

		private:
			// variables
			T _value;
			Node* _parent;
			std::vector<Node*> _children;

		public:
			// constructor
			explicit Node(T _value, Node* _parent = nullptr);
			virtual ~Node();

			// accessors
			const T& value() const;
			const Node* parent() const;
			const std::vector<Node*>& children() const;

			// operators
			friend std::ostream& operator<<(std::ostream& ostream, const Node& node) {
				return ostream << &node << " -> " << node._value;
			}
			Node& operator=(const Node& other);
			bool operator==(const Node& other);

		};

		// variables
		Node* _root;                                                              // root of the tree
		std::string _type;                                                        // type of the tree

		// private methods
		const Node* _add(Node* parent, const T& value, const float& probability); // add node to parent by probability
		int _height(const Node* node, const int& height) const;                   // get height of tree whose root = 'node'

		int _erase(Node* node, const T& value);                                   // delete sub tree whose root._value = 'value'

		void _print(const Node* node, const int& level, const int& index) const;  // tab print
		void _print(const Node* node, const int& level) const;                    // path print

	public:
		// constructor / destructor
		GeneralTree();
		explicit GeneralTree(T value);
		explicit GeneralTree(Node* root);
		~GeneralTree() override;

		// accessors
		[[nodiscard]] const std::string& type() const override;                   // get type of the tree
		[[nodiscard]] bool empty() const override;                                // return tree is empty

		// public methods
		int nodes(const Node* node) const;                                        // get amount of nodes of tree whose root = 'node
		int height(const Node* node) const;                                       // get height of tree whose root = 'node'

		std::vector<int> path(const Node* node) const;                            // get path by node

		template <typename Container>
		Node* path(const Container& path) const;                                  // get node by path
		Node* path(std::initializer_list<int> path);                              // get node by path

		template <typename Container>
		const Node* add(const T& value, const Container& path);                   // add new node by path
		const Node* add(const T& value, std::initializer_list<int> path);         // add new node by path
		const Node* add(const T& value, const float& probability);                // add new node by probability

		int erase(const Node* node);                                              // delete sub tree whose root = 'node'
		int erase(std::initializer_list<int> path);                               // delete sub tree with free memory

		template <typename Container>
		GeneralTree* eraseSubTree(const Container& path);                          // delete sub tree from main tree
		GeneralTree* eraseSubTree(std::initializer_list<int> path);                // delete sub tree from main tree
		int eraseSubTree(const T& value);                                         // delete all sub trees whose root._value =  'value'

		void tprint() const;                                                      // tab print
		void pprint() const;                                                      // path print

		// operators

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
	const T& GeneralTree<T>::Node::value() const {

		return this->_value;

	}

	template <typename T>
	bool GeneralTree<T>::empty() const {

		return this->_root == nullptr;

	}

	template <typename T>
	const typename GeneralTree<T>::Node* GeneralTree<T>::Node::parent() const {

		return this->_parent;

	}

	template <typename T>
	const std::vector<typename GeneralTree<T>::Node*>& GeneralTree<T>::Node::children() const {

		return this->_children;

	}

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
	const typename GeneralTree<T>::Node* GeneralTree<T>::_add(Node* parent, const T& value, const float& probability) {

		if (!this->_root) {

			return this->_root = new Node(value);

		} else if (ng::rand::frand(0.f, 1.f) < probability || parent->_children.size() == 0) {

			Node* newNode = new Node(value, parent);
			parent->_children.emplace_back(newNode);
			return newNode;

		} else {

			return this->_add(parent->_children[ng::rand::irand(0, parent->_children.size() - 1)], value, 1.f - probability);

		}

	}

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
	GeneralTree<T>::GeneralTree() : _root(nullptr), _type("ng::GeneralTree") {}

	template <typename T>
	GeneralTree<T>::GeneralTree(T value) : _root(new Node(value)), _type("ng::GeneralTree") {}

	template <typename T>
	GeneralTree<T>::GeneralTree(Node* root) : _root(root), _type("ng::GeneralTree") {}

	template <typename T>
	GeneralTree<T>::~GeneralTree() {

		delete this->_root;

	}

	// accessors
	template <typename T>
	const std::string& GeneralTree<T>::type() const {

		return this->_type;

	}

	// public methods
	template <typename T>
	int GeneralTree<T>::nodes(const Node* node) const {

		/* return amount of nodes of sub tree */

		int amount = 0;

		for (const auto& child : node->_children)
			amount += this->nodes(child);

		return amount + node->_children.size();

	}

	template <typename T>
	int GeneralTree<T>::height(const Node* node) const {

		/* return height of sub tree */

		return this->_height(node, 1);

	}

	template <typename T>
	std::vector<int> GeneralTree<T>::path(const Node* node) const {

		/* return path to 'node' */

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

	template <typename T>
	template <typename Container>
	typename GeneralTree<T>::Node* GeneralTree<T>::path(const Container& path) const {

		/* return node by path */

		if (!this->_root)
			return nullptr;

		Node* currentNode = this->_root;

		for (const auto& index : path) {

			if (currentNode->_children.size() == 0 || currentNode->_children.size() <= index)
				return nullptr;

			currentNode = currentNode->_children[index];

		}

		return currentNode;

	}

	template <typename T>
	typename GeneralTree<T>::Node* GeneralTree<T>::path(std::initializer_list<int> path) {

		/* return node by path */

		if (!this->_root)
			return nullptr;

		Node* currentNode = this->_root;

		for (const auto& index : path) {

			if (currentNode->_children.size() == 0)
				return nullptr;

			currentNode = currentNode->_children[index];

		}

		return currentNode;

	}

	template <typename T>
	template <typename Container>
	const typename GeneralTree<T>::Node* GeneralTree<T>::add(const T& value, const Container& path) {

		/* return new added node */

		if (!this->_root && !path.empty())
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
	const typename GeneralTree<T>::Node* GeneralTree<T>::add(const T& value, std::initializer_list<int> path) {

		/* return new added node */

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
	const typename GeneralTree<T>::Node* GeneralTree<T>::add(const T& value, const float& probability) {

		/* return new added node */

		return this->_add(this->_root, value, probability);

	}

	template <typename T>
	int GeneralTree<T>::erase(const Node* node) {

		/* return amount of deleted nodes */

		int nodes = this->nodes(node) + 1;

		if (node == this->_root) {

			delete this->_root;
			this->_root = nullptr;
			return nodes;

		}

		Node* parent = node->_parent;

		for (int i = 0; i < parent->_children.size(); i++) {

			if (parent->_children[i] == node) {

				parent->_children.erase(parent->_children.begin() + i);
				delete node;
				break;

			}

		}

		return nodes;

	}

	template <typename T>
	int GeneralTree<T>::erase(std::initializer_list<int> path) {

		/* return amount of deleted nodes */

		return this->erase(this->path(path));

	}

	template <typename T>
	int GeneralTree<T>::eraseSubTree(const T& value) {

		/* return amount of deleted nodes */

		return this->_erase(this->_root, value);

	}

	template <typename T>
	template <typename Container>
	GeneralTree<T>* GeneralTree<T>::eraseSubTree(const Container& path) {

		/* return sub tree whose root = this->path(node) */

		Node* node = this->path(path);

		if (!node) return nullptr;

		Node* parent = node->_parent;

		for (int i = 0; i < parent->_children.size(); i++) {

			if (parent->_children[i] == node) {

				parent->_children.erase(parent->_children.begin() + i);
				break;

			}

		}

		return new GeneralTree(node);

	}

	template <typename T>
	GeneralTree<T>* GeneralTree<T>::eraseSubTree(std::initializer_list<int> path) {

		/* return sub tree whose root = this->path(node) */

		Node* node = this->path(path);

		if (!node)
			return nullptr;

		Node* parent = node->_parent;

		for (int i = 0; i < parent->_children.size(); i++) {

			if (parent->_children[i] == node) {

				parent->_children.erase(parent->_children.begin() + i);
				break;

			}

		}

		return new GeneralTree(node);

	}

	template <typename T>
	void GeneralTree<T>::tprint() const {

		this->_print(this->_root, 1, 0);

	}

	template <typename T>
	void GeneralTree<T>::pprint() const {

		this->_print(this->_root, 1);

	}

} // namespace ng