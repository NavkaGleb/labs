#pragma once

#include "Tree.hpp"

namespace ng {

	/* ------------------------------------------ header ------------------------------------------ */
	template <typename T>
	class AVLTree : public Tree<T> {
	public:
		// inner classes
		class Node : public Tree<T>::Node {
		private:
			// variables
			T _value;
			Node* _parent;
			Node* _left;
			Node* _right;
			int _height;
			int _balance;
			int _amount;

			// friends
			friend AVLTree;

		public:
			// constructor / destructor
			explicit Node(T value, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr, int height = 1, int amount = 1);
			~Node();

			// accessors
			[[nodiscard]] const T& value() const override;
			[[nodiscard]] const Node* left() const;
			[[nodiscard]] const Node* right() const;
			[[nodiscard]] const int& height() const;
			[[nodiscard]] const int& balance() const;
			[[nodiscard]] const int& amount() const;

			// operators
			friend std::ostream& operator<<(std::ostream& ostream, const Node& node) {

				ostream << node._value << " { amount: " << node._amount << ", h: " << node._height << ", l: ";

				if (node._left) ostream << node._left->_value << ", r: ";
				else ostream << "null, r: ";

				if (node._right) ostream << node._right->_value << " }";
				else ostream << "null }";

				return ostream;

			}

		}; // class Node

		class Iterator {
		private:
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
		AVLTree();
		~AVLTree();

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] const unsigned& height() const;
		[[nodiscard]] const Node* root() const override;
		[[nodiscard]] bool empty() const override;

		// public methods
		void clear() override;

		Node* push(const T& value) override;
		void pop(const T& value) override;

		Node* nearestLess(const T& value) const override;
		Node* nearestGreater(const T& value) const override;
		Node* exist(const T& value) const override;

		void print() const;

	private:
		Node* _root;
		const char* _type;
		unsigned _size;
		mutable unsigned _height;

		// private methods
		Node* _lrotation(Node*& node);
		Node* _rrotation(Node*& node);
		Node* _llcase(Node*& node);
		Node* _lrcase(Node*& node);
		Node* _rlcase(Node*& node);
		Node* _rrcase(Node*& node);

		void _update(Node*& node);
		Node* _balance(Node*& node);

		Node* _push(const T& value, Node*& node, Node* parent = nullptr);
		Node* _pop(const T& value, Node* node);

		Node* _nearestLess(const T& value, Node* node, Node* parent = nullptr, int interval = -1) const;
		Node* _nearestGreater(const T& value, Node* node, Node* parent = nullptr, int interval = -1) const;
		Node* _exist(const T& value, Node* node) const;

		void _print(const Node* node, const int& level, const char* caption) const;

	}; // class AVLTree

	/* ------------------------------------------ source ------------------------------------------ */
	//* ------------------------------------------ class Node ------------------------------------------ *//
	// constructor / destructor
	template <typename T>
	AVLTree<T>::Node::Node(T value, Node* parent, Node* left, Node* right, int height, int amount)
		: _value(value), _parent(parent), _left(left), _right(right), _height(height), _balance(0), _amount(amount) {}

	template <typename T>
	AVLTree<T>::Node::~Node() {

		delete this->_left;
		delete this->_right;

	}

	// accessors
	template <typename T>
	const T& AVLTree<T>::Node::value() const { return this->_value; }

	template <typename T>
	const typename AVLTree<T>::Node* AVLTree<T>::Node::left() const { return this->_left; }

	template <typename T>
	const typename AVLTree<T>::Node* AVLTree<T>::Node::right() const { return this->_right; }

	template <typename T>
	const int& AVLTree<T>::Node::height() const { return this->_height; }

	template <typename T>
	const int& AVLTree<T>::Node::balance() const { return this->_balance; }

	template <typename T>
	const int& AVLTree<T>::Node::amount() const { return this->_amount; }

	//* ------------------------------------------ class Iterator ------------------------------------------ *//
	// constructor
	template <typename T>
	AVLTree<T>::Iterator::Iterator(Node* node) : _node(node), _index(1) {}

	// public methods
	template <typename T>
	T& AVLTree<T>::Iterator::operator*() { return this->_node->_value; }

	template <typename T>
	T* AVLTree<T>::Iterator::operator->() { return &this->_node->_value; }

	template <typename T>
	typename AVLTree<T>::Iterator& AVLTree<T>::Iterator::next() {

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
	typename AVLTree<T>::Iterator& AVLTree<T>::Iterator::operator++() {

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
	typename AVLTree<T>::Iterator& AVLTree<T>::Iterator::operator+=(int n) {

		for (int i = 0; i < n; i++)
			(*this)++;

	}

	template <typename T>
	bool AVLTree<T>::Iterator::operator!=(const Iterator& other) const { return this->_node != other._node; }

	//* ------------------------------------------ class ConstIterator ------------------------------------------ *//
	// constructor
	template <typename T>
	AVLTree<T>::ConstIterator::ConstIterator(Node* node) : _node(node), _index(1) {}

	// public methods
	template <typename T>
	const T& AVLTree<T>::ConstIterator::operator*() { return this->_node->_value; }

	template <typename T>
	const T* AVLTree<T>::ConstIterator::operator->() { return &this->_node->_value; }

	template <typename T>
	typename AVLTree<T>::ConstIterator& AVLTree<T>::ConstIterator::next() {

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
	typename AVLTree<T>::ConstIterator& AVLTree<T>::ConstIterator::operator++() {

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
	typename AVLTree<T>::ConstIterator& AVLTree<T>::ConstIterator::operator+=(int n) {

		for (int i = 0; i < n; i++)
			(*this)++;

	}

	template <typename T>
	bool AVLTree<T>::ConstIterator::operator!=(const ConstIterator& other) const { return this->_node != other._node; }

	//* ------------------------------------------ class AVLTree ------------------------------------------ *//
	// private methods
	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_lrotation(Node*& node) {

		Node* newParent = node->_right;

		node->_right = newParent->_left;
		newParent->_parent = node->_parent;
		newParent->_left = node;
		node->_parent = newParent;

		this->_update(node);
		this->_update(newParent);

		return newParent;

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_rrotation(Node*& node) {

		Node* newParent = node->_left;

		node->_left = newParent->_right;
		newParent->_parent = node->_parent;
		newParent->_right = node;
		node->_parent = newParent;

		this->_update(node);
		this->_update(newParent);

		return newParent;

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_llcase(Node*& node) { return this->_rrotation(node); }

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_lrcase(Node*& node) {

		node->_left = this->_lrotation(node->_left);
		return this->_rrotation(node);

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_rlcase(Node*& node) {

		node->_right = this->_rrotation(node->_right);
		return this->_lrotation(node);

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_rrcase(Node*& node) { return this->_lrotation(node); }

	template <typename T>
	void AVLTree<T>::_update(Node*& node) {

		int lheight = node->_left ? node->_left->_height : -1;
		int rheight = node->_right ? node->_right->_height : -1;

		node->_height = std::max(lheight, rheight) + 1;
		node->_balance = rheight - lheight;

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_balance(Node*& node) {

		if (node->_balance == -2)
			return node->_left->_balance <= 0 ? this->_llcase(node) : this->_lrcase(node);
		else if (node->_balance == 2)
			return node->_right->_balance >= 0 ? this->_rrcase(node) : this->_rlcase(node);

		return node;

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_push(const T& value, Node*& node, Node* parent) {

		if (!node) {

			++this->_size;
			return node = new Node(value, parent);

		}

		if (node->_value == value) {

			++this->_size;
			++node->_amount;
			return node;

		}

		if (node->_value > value)
			node->_left = this->_push(value, node->_left, node);
		else if (node->_value < value)
			node->_right = this->_push(value, node->_right, node);

		this->_update(node);
		return this->_balance(node);

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_pop(const T& value, Node* node) {

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

			if (node->_left->_height > node->_right->_height) {

				Node* max = node->_left;

				while (max->_right)
					max = max->_right;

				node->_value = max->_value;
				std::swap(node->_amount, max->_amount);
				node->_left = this->_pop(max->_value, node->_left);

			} else {

				Node* min = node->_right;

				while (min->_left)
					min = min->_left;

				node->_value = min->_value;
				std::swap(node->_amount, min->_amount);
				node->_right = this->_pop(min->_value, node->_right);

			}

		}

		this->_update(node);
		return this->_balance(node);

	}

	template<typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_nearestLess(const T &value, AVLTree::Node *node, AVLTree::Node *parent, int interval) const {

		if (!node) return parent;

		if ((interval > std::abs(node->_value - value) || interval == -1) && node->_value <= value) {

			interval = std::abs(node->_value - value);
			parent = node;

		}

		if (node->_value > value)
			return this->_nearestLess(value, node->_left, parent, interval);

		return this->_nearestLess(value, node->_right, parent, interval);

	}

	template<typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_nearestGreater(const T &value, AVLTree::Node *node, AVLTree::Node *parent, int interval) const {

		if (!node) return parent;

		if ((interval > std::abs(node->_value - value) || interval == -1) && node->_value >= value) {

			interval = std::abs(node->_value - value);
			parent = node;

		}

		if (node->_value > value)
			return this->_nearestGreater(value, node->_left, parent, interval);

		return this->_nearestGreater(value, node->_right, parent, interval);

	}

	template<typename T>
	typename AVLTree<T>::Node* AVLTree<T>::_exist(const T &value, Node* node) const {

		if (!node)
			return nullptr;
		if (node->_value > value)
			return this->_exist(value, node->_left);
		if (node->_value < value)
			return this->_exist(value, node->_right);

		return node;

	}

	template <typename T>
	void AVLTree<T>::_print(const Node* node, const int& level, const char* caption) const {

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
	AVLTree<T>::AVLTree() : _root(nullptr), _type("ng::AVLTree"), _size(0) {}

	template <typename T>
	AVLTree<T>::~AVLTree() { delete this->_root; }

	// accessors
	template <typename T>
	const char* AVLTree<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& AVLTree<T>::size() const { return this->_size; }

	template <typename T>
	const unsigned& AVLTree<T>::height() const { return this->_height =  this->_root ? this->_root->_height : 0; }

	template <typename T>
	const typename AVLTree<T>::Node* AVLTree<T>::root() const { return this->_root; }

	template <typename T>
	bool AVLTree<T>::empty() const { return this->_root == nullptr; }

	// public methods
	template <typename T>
	void AVLTree<T>::clear() {

		delete this->_root;
		this->_root = nullptr;
		this->_size = 0;

	}

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::push(const T& value) { return this->_root = this->_push(value, this->_root); }

	template <typename T>
	void AVLTree<T>::pop(const T& value) { this->_root = this->_pop(value, this->_root); }

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::nearestLess(const T& value) const { return this->_nearestLess(value, this->_root); }

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::nearestGreater(const T& value) const { return this->_nearestGreater(value, this->_root); }

	template <typename T>
	typename AVLTree<T>::Node* AVLTree<T>::exist(const T& value) const { return this->_exist(value, this->_root); }

	template <typename T>
	void AVLTree<T>::print() const {

		this->_print(this->_root, 1, "root");
		std::cout << std::endl;

	}

} // namespace ng