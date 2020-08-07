#pragma once

#include <iostream>
#include <vector>
#include <stack>

#include "Tree.hpp"

namespace ng {

	template <typename T>
	class BinarySearchTree : public Tree {
	private:
		// inner class
		class Node {
			// friends
			friend BinarySearchTree;

		private:
			T _value;
			Node* _left;
			Node* _right;
			bool _lthread;                                                                    // left thread
			bool _rthread;                                                                    // right thread

		public:
			// constructor / destructor
			explicit Node(T value, Node* left = nullptr, Node* right = nullptr);
			virtual ~Node();

			// accessors
			const T& value() const;
			const Node* left() const;
			const Node* right() const;

			// operators
			friend std::ostream& operator<<(std::ostream& ostream, const Node& node) {

				ostream << node._value << " {l: ";

				if (node._left) ostream << node._left->_value << ", r: ";
				else ostream << "null, r: ";

				if (node._right) ostream << node._right->_value << ", ";
				else ostream << "null, ";

				ostream << "lt: " << node._lthread << ", ";
				ostream << "rt: " << node._rthread << "}";

				return ostream;

			}

		};

		// variables
		Node* _root;                                                                          // root of the tree
		std::string _type;                                                                    // type of the tree
		bool _threaded;                                                                       // tracks threaded tree or not

		// private methods
		Node* _add(const T& value);                                                           // thread add new node
		Node* _add(Node*& node, const T& value);                                              // simple add new node

		bool _exist(Node* node, const T& value) const;                                        // check if value exist in tree

		void _sorder(Node* node, std::vector<Node*>* result = nullptr) const;                 // symmetric order (recursion)
		void _toThreaded(Node* node);                                                         // create threaded tree
		void _removeThreads(Node* node);                                                      // remove all threads

		void _oprint(Node* node, const char* separator) const;                                // symmetric order
		Node* _nextThread(Node* node) const;                                                  // get next thread node

		void _print(const Node* node, const int& level,
		    const char* caption, bool threaded = false) const;                                // tab print

	public:
		// constructor / destructor
		BinarySearchTree();
		explicit BinarySearchTree(T value);
		~BinarySearchTree() override;

		// accessors
		[[nodiscard]] const std::string& type() const override;                               // get type of the tree
		[[nodiscard]] bool empty() const override;                                            // return tree is empty
		[[nodiscard]] bool exist(const T& value) const;                                       // check if value exist in tree

		// public methods
		void toThreaded();                                                                    // from btree to thread btree
		void removeThreads();                                                                 // remove all threads (to simple btree)
		Node* add(const T& value);                                                            // add value to tree

		void tprint() const;                                                                  // tab print
		void oprint(const char* separator) const;                                             // order print

		void traversal() const;

	};

	///* ------------------------------------------ class Node ------------------------------------------ *///
	// constructor / destructor
	template <typename T>
	BinarySearchTree<T>::Node::Node(T value, Node* left, Node* right)
		: _value(value), _left(left), _right(right), _lthread(false), _rthread(false) {}

	template <typename T>
	BinarySearchTree<T>::Node::~Node() {

		if (this->_left && !this->_lthread)
			delete this->_left;
		if (this->_right && !this->_rthread)
			delete this->_right;

	}

	// accessors
	template <typename T>
	const T& BinarySearchTree<T>::Node::value() const {

		return this->_value;

	}

	template <typename T>
	bool BinarySearchTree<T>::empty() const {

		return this->_root == nullptr;

	}

	template <typename T>
	const typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Node::left() const {

		return _left;

	}

	template <typename T>
	const typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Node::right() const {

		return _right;

	}

	///* ------------------------------------------ class BinarySearchTree ------------------------------------------ *///
	// private methods
	template <typename T>
	typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_add(const T& value) {

		if (!this->_root)
			return this->_root = new Node(value);

		Node* node = this->_root;
		Node* parent = this->_root;

		while (node) {

			if (node->_value == value)
				return this->_root;

			parent = node;

			if (node->_value > value) {

				if (!node->_lthread) node = node->_left;
				else break;

			} else if (node->_value < value) {

				if (!node->_rthread) node = node->_right;
				else break;

			}

		}

		if (!node) {

			node = new Node(value);

			if (parent->_value > node->_value) {

				parent->_left = node;
				node->_right = parent;
				node->_rthread = true;

			} else {

				parent->_right = node;
				node->_left = parent;
				node->_lthread = true;

			}

			return node;

		}

		Node* newNode = new Node(value);

		if (node->_value > newNode->_value) {

			if (node->_left) {

				newNode->_left = node->_left;
				newNode->_lthread = true;

			}

			newNode->_right = node;
			newNode->_rthread = true;
			node->_left = newNode;
			node->_lthread = false;

		} else {

			if (node->_right) {

				newNode->_right = node->_right;
				newNode->_rthread = true;

			}

			newNode->_left = node;
			newNode->_lthread = true;
			node->_right = newNode;
			node->_rthread = false;

		}

		return newNode;

	}

	template <typename T>
	typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_add(Node*& node, const T& value) {

		if (!node)
			return node = new Node(value);
		if (node->_value > value)
			return this->_add(node->_left, value);
		if (node->_value < value)
			return this->_add(node->_right, value);
		return nullptr;

	}

	template <typename T>
	bool BinarySearchTree<T>::_exist(Node* node, const T& value) const {

		if (!node)
			return false;
		if (node->_value == value)
			return true;
		if (node->_value > value && !node->_lthread)
			return this->_exist(node->_left, value);
		if (node->_value < value && !node->_rthread)
			return this->_exist(node->_right, value);

		return false;

	}

	template <typename T>
	void BinarySearchTree<T>::_sorder(Node* node, std::vector<Node*>* result) const {

		if (node) {

			this->_sorder(node->_left, result);

			if (result)
				result->emplace_back(node);
			else
				std::cout << node->_value << " ";

			this->_sorder(node->_right, result);

		}

	}

	template <typename T>
	void BinarySearchTree<T>::_toThreaded(Node* node) {

		if (this->_threaded)
			return;

		std::vector<Node*> sorder;
		this->_sorder(node, &sorder);

		for (int i = 0; i < sorder.size(); i++) {

			if (!sorder[i]->_left && i != 0) {

				sorder[i]->_lthread = true;
				sorder[i]->_left = sorder[i - 1];

			}

			if (!sorder[i]->_right && i != sorder.size() - 1) {

				sorder[i]->_rthread = true;
				sorder[i]->_right = sorder[i + 1];

			}

		}

	}

	template <typename T>
	void BinarySearchTree<T>::_removeThreads(Node* node) {

		if (!this->_threaded)
			return;

		if (!node)
			return;

		if (!node->_lthread)
			this->_removeThreads(node->_left);

		if (node->_lthread) {

			node->_lthread = false;
			node->_left = nullptr;

		}

		if (node->_rthread) {

			node->_rthread = false;
			node->_right = nullptr;

		}

		if (!node->_rthread)
			this->_removeThreads(node->_right);

	}

	template <typename T>
	void BinarySearchTree<T>::_oprint(Node* node, const char* separator) const {

		if (!this->_threaded) {

			this->_sorder(this->_root);

		} else {

			while (!node->_lthread && node->_left)
				node = node->_left;

			while (node) {

				std::cout << node->_value << separator;
				node = this->_nextThread(node);

			}

		}

	}

	template <typename T>
	typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_nextThread(Node* node) const {

		Node* nextNode = node->_right;

		if (!node->_rthread)
			while (nextNode && !nextNode->_lthread)
				nextNode = nextNode->_left;

		return nextNode;

	}

	template <typename T>
	void BinarySearchTree<T>::_print(const Node* node, const int& level, const char* caption, bool threaded) const {

		if (!node) {

			std::cout << caption << ": null" << std::endl;
			return;

		}

		if (threaded) {

			std::cout << "thread to" << ": " << node->_value << std::endl;
			return;

		}

		std::cout << caption << ": " << *node;

		if (node->_left || node->_right) {

			std::cout << " (" << std::endl;

			for (int i = 0; i < level; i++) std::cout << "| ";
			this->_print(node->_left, level + 1, "left", node->_lthread);

			for (int i = 0; i < level; i++) std::cout << "| ";
			this->_print(node->_right, level + 1, "right", node->_rthread);

			for (int i = 0; i < level - 1; i++) std::cout << "| ";
			std::cout << ")";

		}

		std::cout << std::endl;

	}

	// constructor / destructor
	template <typename T>
	BinarySearchTree<T>::BinarySearchTree() : _root(nullptr), _type("ng::BinarySearchTree"), _threaded(false) {}

	template <typename T>
	BinarySearchTree<T>::BinarySearchTree(T value) : _root(new Node(value)), _type("ng::BinarySearchTree"), _threaded(false) {}

	template <typename T>
	BinarySearchTree<T>::~BinarySearchTree() {

		delete this->_root;

	}

	// accessors
	template <typename T>
	const std::string& BinarySearchTree<T>::type() const {

		return this->_type;

	}

	template <typename T>
	bool BinarySearchTree<T>::exist(const T& value) const {

		return this->_exist(this->_root, value);

	}

	// public methods
	template <typename T>
	void BinarySearchTree<T>::toThreaded() {

		this->_toThreaded(this->_root);
		this->_threaded = true;

	}

	template <typename T>
	void BinarySearchTree<T>::removeThreads() {

		this->_removeThreads(this->_root);
		this->_threaded = false;

	}

	template <typename T>
	typename BinarySearchTree<T>::Node* BinarySearchTree<T>::add(const T& value) {

		if (!this->_threaded) return this->_add(this->_root, value);
		return this->_add(value);

	}

	template <typename T>
	void BinarySearchTree<T>::tprint() const {

		this->_print(this->_root, 1, "root");
		std::cout << std::endl;

	}

	template <typename T>
	void BinarySearchTree<T>::oprint(const char* separator) const {

		this->_oprint(this->_root, separator);
		std::cout << std::endl;

	}

	template <typename T>
	void BinarySearchTree<T>::traversal() const {

		std::stack<Node*> stack;

		Node* node = this->_root;

		while (node || !stack.empty()) {

			while (node) {

				stack.emplace(node);
				node = node->_right;

			}

			node = stack.top();
			stack.pop();

			std::cout << node->_value << std::endl;

			node = node->_left;

		}

	}

} // namespace ng