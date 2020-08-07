#pragma once

#include "Tree.hpp"
#include <stack>
#include <iostream>

namespace ng {

	/* ------------------------------------------ header ------------------------------------------ */
	template <typename T>
	class BST : public Tree<T> {
	public:
		// inner classes
		class Node : public Tree<T>::Node {
		private:
			// variables
			T _value;
			Node* _parent;
			Node* _left;
			Node* _right;
			int _amount;

			// friends
			friend BST;

		public:
			// constructor / destructor
			explicit Node(T value, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr, int amount = 1);
			~Node() override;

			// accessors
			[[nodiscard]] const T& value() const override;
			[[nodiscard]] const Node* parent() const;
			[[nodiscard]] const Node* left() const;
			[[nodiscard]] const Node* right() const;
			[[nodiscard]] const int& amount() const;

			// operators
			friend std::ostream& operator<<(std::ostream& ostream, const Node& node) {

				ostream << node._value << " { amount: " << node._amount << ", l: ";

				if (node._left) ostream << node._left->_value << ", r: ";
				else ostream << "null, r: ";

				if (node._right) ostream << node._right->_value << " }";
				else ostream << "null }";

				return ostream;

			}

		}; // class Node

		class Iterator {
		private:
			// variables
			Node* _node;
			int _index;

		public:
			// constructor / destructor
			explicit Iterator(Node* node = nullptr);
			virtual ~Iterator() = default;

			// public methods
			T& operator*();
			T* operator->();

			Iterator& next();
			Iterator& operator++();
			Iterator& operator+=(int n);

			bool operator!=(const Iterator& other) const;

		}; // class Iterator

		class ConstIterator {
		private:
			// variables
			Node* _node;
			int _index;

		public:
			// constructor / destructor
			explicit ConstIterator(Node* node = nullptr);
			virtual ~ConstIterator() = default;

			// public methods
			const T& operator*();
			const T* operator->();

			ConstIterator& next();
			ConstIterator& operator++();
			ConstIterator& operator+=(int n);

			bool operator!=(const ConstIterator& other) const;

		}; // class ConstIterator

		// constructor / destructor
		BST();
		explicit BST(T value);
		~BST() override;

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] const unsigned& height() const override;
		[[nodiscard]] const Node* root() const;
		[[nodiscard]] bool empty() const override;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;

		// public methods
		void clear() override;

		Node* push(const T& value) override;
		void pop(const T& value) override;

		Node* nearestLess(const T& value) const override;
		Node* nearestGreater(const T& value) const override;
		Node* exist(const T& value) const override;

		void print() const;

		void postOrder() const;

		int leavesAmount(T value) const;
		int leavesAmount(T value, Node* node) const;

	private:
		// variables
		Node* _root;
		const char* _type;
		unsigned _size;
		mutable unsigned _height;

		// private methods
		unsigned _depth(Node* node) const;

		Node* _push(const T& value, Node*& node, Node* parent);
		Node* _pop(const T& value, Node* node);

		Node* _nearestLess(const T& value, Node* node, Node* parent = nullptr, int interval = -1) const;
		Node* _nearestGreater(const T& value, Node* node, Node* parent = nullptr, int interval = -1) const;
		Node* _exist(const T& value, Node* node) const;

		void _print(const Node* node, const int& level, const char* caption) const;

	}; // class BST

	/* ------------------------------------------ source ------------------------------------------ */

	//* ------------------------------------------ class Node ------------------------------------------ *//
	// constructor / destructor
	template <typename T>
	BST<T>::Node::Node(T value, Node* parent, Node* left, Node* right, int amount)
		: _value(value),  _parent(parent), _left(left), _right(right), _amount(amount) {}

	template <typename T>
	BST<T>::Node::~Node() {

		delete this->_left;
		delete this->_right;

	}

	// accessors
	template <typename T>
	const T& BST<T>::Node::value() const { return this->_value; }

	template <typename T>
	const typename BST<T>::Node* BST<T>::Node::parent() const { return this->_parent; }

	template <typename T>
	const typename BST<T>::Node* BST<T>::Node::left() const { return _left; }

	template <typename T>
	const typename BST<T>::Node* BST<T>::Node::right() const { return _right; }

	template <typename T>
	const int& BST<T>::Node::amount() const { return this->_amount; }

	//* ------------------------------------------ class Iterator ------------------------------------------ *//
	// constructor / destructor
	template <typename T>
	BST<T>::Iterator::Iterator(Node* node) : _node(node), _index(1) {}

	// public methods
	template <typename T>
	T& BST<T>::Iterator::operator*() { return this->_node->_value; }

	template <typename T>
	T* BST<T>::Iterator::operator->() { return &this->_node->_value; }

	template <typename T>
	typename BST<T>::Iterator& BST<T>::Iterator::next() {

		if (this->_node->_right) {

			this->_node = this->_node->_right;

			while (this->_node->_left)
				this->_node = this->_node->_left;

			this->_index = 1;

		} else {

			Node* parent = this->_node->_parent;

			while (parent && this->_node == parent->_right)
				this->_node = parent, parent = parent->_parent;

			this->_node = parent;

			this->_index = 1;

		}

		return *this;

	}

	template <typename T>
	typename BST<T>::Iterator& BST<T>::Iterator::operator++() {

		if (this->_index < this->_node->_amount) {

			++this->_index;

		} else if (this->_node->_right) {

			this->_node = this->_node->_right;

			while (this->_node->_left)
				this->_node = this->_node->_left;

			this->_index = 1;

		} else {

			Node* parent = this->_node->_parent;

			while (parent && this->_node == parent->_right)
				this->_node = parent, parent = parent->_parent;

			this->_node = parent;

			this->_index = 1;

		}

		return *this;

	}

	template <typename T>
	typename BST<T>::Iterator& BST<T>::Iterator::operator+=(int n) {

		for (int i = 0; i < n; i++)
			(*this)++;

	}

	template <typename T>
	bool BST<T>::Iterator::operator!=(const Iterator& other) const { return this->_node != other._node; }

	//* ------------------------------------------ class ConstIterator ------------------------------------------ *//
	// constructor / destructor
	template <typename T>
	BST<T>::ConstIterator::ConstIterator(Node* node) : _node(node), _index(1) {}

	// public methods
	template <typename T>
	const T& BST<T>::ConstIterator::operator*() { return this->_node->_value; }

	template <typename T>
	const T* BST<T>::ConstIterator::operator->() { return &this->_node->_value; }

	template <typename T>
	typename BST<T>::ConstIterator& BST<T>::ConstIterator::next() {

		if (this->_node->_right) {

			this->_node = this->_node->_right;

			while (this->_node->_left)
				this->_node = this->_node->_left;

			this->_index = 1;

		} else {

			Node* parent = this->_node->_parent;

			while (parent && this->_node == parent->_right)
				this->_node = parent, parent = parent->_parent;

			this->_node = parent;

			this->_index = 1;

		}

		return *this;

	}

	template <typename T>
	typename BST<T>::ConstIterator& BST<T>::ConstIterator::operator++() {

		if (this->_index < this->_node->_amount) {

			++this->_index;

		} else if (this->_node->_right) {

			this->_node = this->_node->_right;

			while (this->_node->_left)
				this->_node = this->_node->_left;

			this->_index = 1;

		} else {

			Node* parent = this->_node->_parent;

			while (parent && this->_node == parent->_right)
				this->_node = parent, parent = parent->_parent;

			this->_node = parent;

			this->_index = 1;

		}

		return *this;

	}

	template <typename T>
	typename BST<T>::ConstIterator& BST<T>::ConstIterator::operator+=(int n) {

		for (int i = 0; i < n; i++)
			(*this)++;

	}

	template <typename T>
	bool BST<T>::ConstIterator::operator!=(const ConstIterator& other) const { return this->_node != other._node; }

	//* ------------------------------------------ class BST ------------------------------------------ *//
	// private methods
	template <typename T>
	unsigned BST<T>::_depth(Node* node) const { return !node ? 0 : 1 + std::max(this->_depth(node->_left), this->_depth(node->_right)); }

	template <typename T>
	typename BST<T>::Node* BST<T>::_push(const T& value, Node*& node, Node* parent) {

		if (!node) {

			++this->_size;
			return node = new Node(value, parent);

		}

		if (node->_value == value) {

			++this->_size;
			++node->_amount;
			return node;

		}

		return node->_value > value ? this->_push(value, node->_left, node) : this->_push(value, node->_right, node);

	}

	template <typename T>
	typename BST<T>::Node* BST<T>::_pop(const T& value, Node* node) {

		if (!node) return nullptr;

		if (node->_value > value) {

			node->_left = this->_pop(value, node->_left);

		} else if (node->_value < value) {

			node->_right = this->_pop(value, node->_right);

		} else {

			if (!node->_left && !node->_right) {

				delete node;
				this->_size -= node->_amount;
				return nullptr;

			}

			if (!node->_left) {

				Node* temp = node->_right;
				temp->_parent = node->_parent;

				node->_right = nullptr;
				delete node;
				this->_size -= node->_amount;
				return temp;

			}

			if (!node->_right) {

				Node* temp = node->_left;
				temp->_parent = node->_parent;

				node->_left = nullptr;
				delete node;
				this->_size -= node->_amount;
				return temp;

			}

			Node* temp = node->_right;

			while (temp->_left)
				temp = temp->_left;

			node->_value = temp->_value;
			std::swap(node->_amount, temp->_amount);
			node->_right = this->_pop(temp->_value, node->_right);

		}

		return node;

	}

	template <typename T>
	typename BST<T>::Node* BST<T>::_nearestLess(const T& value, Node* node, Node* parent, int interval) const {

		if (!node) return parent;

		if ((interval > std::abs(node->_value - value) || interval == -1) && node->_value <= value) {

			interval = std::abs(node->_value - value);
			parent = node;

		}

		if (node->_value > value)
			return this->_nearestLess(value, node->_left, parent, interval);

		return this->_nearestLess(value, node->_right, parent, interval);

	}

	template <typename T>
	typename BST<T>::Node* BST<T>::_nearestGreater(const T& value, Node* node, Node* parent, int interval) const {

		if (!node) return parent;

		if ((interval > std::abs(node->_value - value) || interval == -1) && node->_value >= value) {

			interval = std::abs(node->_value - value);
			parent = node;

		}

		if (node->_value > value)
			return this->_nearestGreater(value, node->_left, parent, interval);

		return this->_nearestGreater(value, node->_right, parent, interval);

	}

	template <typename T>
	typename BST<T>::Node* BST<T>::_exist(const T& value, Node* node) const {

		if (!node)
			return nullptr;
		if (node->_value > value)
			return this->_exist(value, node->_left);
		if (node->_value < value)
			return this->_exist(value, node->_right);

		return node;

	}

	template <typename T>
	void BST<T>::_print(const Node* node, const int& level, const char* caption) const {

		if (!node) {

			std::cout << caption << ": null" << std::endl;
			return;

		}

		std::cout << caption << ": " << *node;

		if (node->_left || node->_right) {

			std::cout << " (" << std::endl;

			for (int i = 0; i < level; i++) std::cout << "| ";
			this->_print(node->_left, level + 1, "left");

			for (int i = 0; i < level; i++) std::cout << "| ";
			this->_print(node->_right, level + 1, "right");

			for (int i = 0; i < level - 1; i++) std::cout << "| ";
			std::cout << ")";

		}

		std::cout << std::endl;

	}

	// constructor / destructor
	template <typename T>
	BST<T>::BST() : _root(nullptr), _type("ng::BST"), _size(0), _height(0) {}

	template <typename T>
	BST<T>::BST(T value) : _root(new Node(value)), _type("ng::BST"), _size(0), _height(0) {}

	template <typename T>
	BST<T>::~BST() { delete this->_root; }

	// accessors
	template <typename T>
	const char* BST<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& BST<T>::size() const { return this->_size; }

	template <typename T>
	const unsigned& BST<T>::height() const { return this->_height = this->_depth(this->_root); }

	template <typename T>
	const typename BST<T>::Node* BST<T>::root() const { return this->_root; }

	template <typename T>
	bool BST<T>::empty() const { return this->_root == nullptr; }

	template <typename T>
	typename BST<T>::Iterator BST<T>::begin() {

		Node* node = this->_root;
		while (node->_left) node = node->_left;

		return Iterator(node);

	}

	template <typename T>
	typename BST<T>::ConstIterator BST<T>::begin() const {

		Node* node = this->_root;
		while (node->_left) node = node->_left;

		return ConstIterator(node);

	}

	template <typename T>
	typename BST<T>::Iterator BST<T>::end() { return Iterator(nullptr); }

	template <typename T>
	typename BST<T>::ConstIterator BST<T>::end() const { return ConstIterator(nullptr); }


	// public methods
	template <typename T>
	void BST<T>::clear() {

		delete this->_root;
		this->_root = nullptr;
		this->_size = 0;

	}

	template <typename T>
	typename BST<T>::Node* BST<T>::push(const T& value) { return this->_push(value, this->_root, nullptr); }

	template <typename T>
	void BST<T>::pop(const T& value) { this->_root = this->_pop(value, this->_root); }

	template <typename T>
	typename BST<T>::Node* BST<T>::nearestLess(const T& value) const { return this->_nearestLess(value, this->_root); }

	template <typename T>
	typename BST<T>::Node* BST<T>::nearestGreater(const T& value) const { return this->_nearestGreater(value, this->_root); }

	template <typename T>
	typename BST<T>::Node* BST<T>::exist(const T& value) const { return this->_exist(value, this->_root); }

	template <typename T>
	void BST<T>::print() const {

		this->_print(this->_root, 1, "root");
		std::cout << std::endl;

	}

	template <typename T>
	void BST<T>::postOrder() const {

		std::stack<Node*> stack;
		Node* node = this->_root;

		do {

			while (node) {

				if (node->_right)
					stack.emplace(node->_right);

				stack.emplace(node);
				node = node->_left;

			}

			node = stack.top();
			stack.pop();

			if (!stack.empty() && node->_right && stack.top() == node->_right) {

				stack.pop();
				stack.emplace(node);
				node = node->_right;

			} else {

				std::cout << node->_value << std::endl;
				node = nullptr;

			}

		} while (!stack.empty());

	}

	template <typename T>
	int BST<T>::leavesAmount(T value) const {

		return leavesAmount(value, this->_root);

	}

	template <typename T>
	int BST<T>::leavesAmount(T value, Node* node) const {

		if (!node)
			return 0;

		if (!node->_left && !node->_right && node->_value == value)
			return 1;

		return leavesAmount(value, node->_left) + leavesAmount(value, node->_right);

	}

} // namespace ng