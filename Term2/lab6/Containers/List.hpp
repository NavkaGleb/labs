#pragma once

#include "Container.hpp"

namespace ng {

	/* ---------------------------------------------- header ---------------------------------------------- */

	template <typename T>
	class List : public Container<T> {
	public:
		// inner classes
		class Node;
		class Iterator;
		class ConstIterator;
		
		class Node {
		private:
			// variables
			T _value;
			Node* _next;

			// friends
			friend List;
			friend Iterator;
			friend ConstIterator;

		public:
			// constructor
			explicit Node(T value, Node* next = nullptr);
			virtual ~Node() = default;

			// accessors
			const T& value() const;
			const Node* next() const;

		};

		class Iterator {
		private:
			// variables
			Node* _node;

		public:
			// constructor / destructor
			explicit Iterator(Node* node = nullptr);
			virtual ~Iterator() = default;

			// public methods
			T& operator*();
			T* operator->();

			Iterator& operator++();
			Iterator& operator+=(int n);

			bool operator!=(const Iterator& other);

		}; // class Iterator

		class ConstIterator {
		private:
			// variables
			Node* _node;

		public:
			// constructor / destructor
			explicit ConstIterator(Node* node = nullptr);
			virtual ~ConstIterator() = default;

			// public methods
			const T& operator*() const;
			const T* operator->() const;

			ConstIterator& operator++();
			ConstIterator& operator+=(int n);

			bool operator!=(const ConstIterator& other);

		}; // class Iterator

		// constructor / destructor
		List();
		explicit List(unsigned size, T value);
		List(const std::initializer_list<T>& values);

		~List() override;

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] T& front() override;
		[[nodiscard]] T& back() override;
		[[nodiscard]] const T& front() const override;
		[[nodiscard]] const T& back() const override;
		[[nodiscard]] bool empty() const override;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;

		// methods
		void clear() override;

		void push(int index, const T& value) override;
		void push(Iterator it, const T& value);
		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void pop(int index) override;
		void popFront() override;
		void popBack() override;

	private:
		// variables
		Node* _left;
		Node* _right;
		unsigned _size;
		const char* _type;

	};

	/* ---------------------------------------------- source ----------------------------------------------  */

	/* ---------------------------------------------- class Node ---------------------------------------------- */
	// constructor / destructor
	template <typename T>
	List<T>::Node::Node(T value, Node* next) : _value(value), _next(next) {}
	
	// accessors
	template <typename T>
	const T& List<T>::Node::value() const { return this->_value; }

	template <typename T>
	const typename List<T>::Node* List<T>::Node::next() const { return this->_next; }

	/* ---------------------------------------------- class Iterator ---------------------------------------------- */
	// constructor
	template <typename T>
	List<T>::Iterator::Iterator(Node* node) : _node(node) {}

	// public methods
	template <typename T>
	T& List<T>::Iterator::operator*() { return this->_node->_value; }

	template <typename T>
	T* List<T>::Iterator::operator->() { return &this->_node->_value; }

	template <typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator++() {

		this->_node = this->_node->_next;
		return *this;

	}

	template <typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator+=(int n) {

		for (int i = 0; i < n; i++)
			this->_node = this->_node->_next;

	}

	template <typename T>
	bool List<T>::Iterator::operator!=(const Iterator& other) { return this->_node != other._node; }

	/* ---------------------------------------------- class ConstIterator ---------------------------------------------- */
	// constructor
	template <typename T>
	List<T>::ConstIterator::ConstIterator(Node* node) : _node(node) {}

	// public methods
	template <typename T>
	const T& List<T>::ConstIterator::operator*() const { return this->_node->_value; }

	template <typename T>
	const T* List<T>::ConstIterator::operator->() const { return &this->_node->_value; }

	template <typename T>
	typename List<T>::ConstIterator& List<T>::ConstIterator::operator++() {

		this->_node = this->_node->_next;
		return *this;

	}

	template <typename T>
	typename List<T>::ConstIterator& List<T>::ConstIterator::operator+=(int n) {

		for (int i = 0; i < n; i++)
			this->_node = this->_node->_next;

	}

	template <typename T>
	bool List<T>::ConstIterator::operator!=(const ConstIterator& other) { return this->_node != other._node; }

	/* ---------------------------------------------- class List ---------------------------------------------- */
	// constructor / destructor
	template <typename T>
	List<T>::List() : _size(0), _left(nullptr), _right(nullptr), _type("ng::List") {}

	template <typename T>
	List<T>::List(unsigned size, T value) : _size(size), _type("ng::List") {

		this->_left = new Node(value);
		Node* prevNode = this->_left;

		for (unsigned i = 1; i < this->_size; i++) {

			auto* newNode = new Node(value);

			prevNode->_next = newNode;
			prevNode = prevNode->_next;

		}

		this->_right = prevNode;

	}

	template <typename T>
	List<T>::List(const std::initializer_list<T>& values) : _size(values.size()), _type("ng::List") {

		this->_left = new Node(*values.begin());
		Node* prevNode = this->_left;

		for (unsigned i = 1; i < this->_size; i++) {

			auto* newNode = new Node(*(values.begin() + i));

			prevNode->_next = newNode;
			prevNode = prevNode->_next;

		}

		this->_right = prevNode;

	}

	template <typename T>
	List<T>::~List() {

		Node* currentNode = this->_left;
		Node* nextNode;

		while (currentNode) {

			nextNode = currentNode->_next;
			delete currentNode;
			currentNode = nextNode;

		}

	}

	// accessors
	template <typename T>
	const char* List<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& List<T>::size() const { return this->_size; }

	template <typename T>
	T& List<T>::front() {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_left->_value;

	}

	template <typename T>
	T& List<T>::back() {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_right->_value;

	}

	template <typename T>
	const T& List<T>::front() const {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_left->_value;

	}

	template <typename T>
	const T& List<T>::back() const {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_right->_value;

	}

	template <typename T>
	bool List<T>::empty() const { return this->_size == 0; }

	template <typename T>
	typename List<T>::Iterator List<T>::begin() { return List::Iterator(this->_left); }

	template <typename T>
	typename List<T>::ConstIterator List<T>::begin() const { return List::ConstIterator(this->_left); }

	template <typename T>
	typename List<T>::Iterator List<T>::end() { return List::Iterator(this->_right->_next); }

	template <typename T>
	typename List<T>::ConstIterator List<T>::end() const { return List::ConstIterator(this->_right->_next); }

	// methods
	template <typename T>
	void List<T>::clear() {

		Node* currentNode = this->_left;
		Node* nextNode;

		while (currentNode) {

			nextNode = currentNode->_next;
			delete currentNode;
			currentNode = nextNode;

		}

		this->_left = nullptr;
		this->_right = nullptr;
		this->_size = 0;

	}

	template <typename T>
	void List<T>::push(const int index, const T& value) {

		if (index < 0 || index > this->_size)
			return;

		if (index == 0) {

			this->pushFront(value);
			return;

		}

		if (index == this->_size) {

			this->pushBack(value);
			return;

		}

		auto* newNode = new Node(value);
		Node* currentNode = this->_left;

		for (unsigned i = 1; i < index; i++)
			currentNode = currentNode->_next;

		Node* nextNode = currentNode->_next;

		currentNode->_next = newNode;
		newNode->_next = nextNode;
		this->_size++;

	}

	template <typename T>
	void List<T>::push(typename List<T>::Iterator it, const T& value) {

		auto* newNode = new Node(value);

		if (!it->_node) {

			it->_node = newNode;
			this->_right->_next = newNode;
			this->_right = newNode;
			return;

		}

		it->_node = newNode;
		newNode->_next = this->_left;
		this->_left = newNode;

	}

	template <typename T>
	void List<T>::pushFront(const T& value) {

		auto* newNode = new Node(value, this->_left);

		this->_left = newNode;

		if (!this->_right) {

			this->_left->_next = newNode;
			this->_right = newNode;

		}

		this->_size++;

	}

	template <typename T>
	void List<T>::pushBack(const T& value) {

		auto* newNode = new Node(value);

		if (!this->_left) {

			this->_left = newNode;
			this->_right = newNode;
			this->_size++;
			return;

		}

		this->_right->_next = newNode;
		this->_right = newNode;
		this->_size++;

	}

	template <typename T>
	void List<T>::pop(const int index) {

		if (index < 0 || index > this->_size - 1)
			return;

		Node* currentNode = this->_left;

		if (index == 0) {

			this->popFront();
			return;

		}

		Node* prevNode = this->_left;
		Node* nextNode;

		for (unsigned i = 0; i < index; i++) {

			prevNode = currentNode;
			currentNode = currentNode->_next;

		}

		nextNode = currentNode->_next;
		delete currentNode;
		prevNode->_next = nextNode;
		this->_size--;

	}

	template <typename T>
	void List<T>::popFront() {

		if (this->_size == 0)
			return;

		if (this->_size == 1) {

			delete this->_left;
			this->_left = nullptr;
			this->_size--;
			return;

		}

		Node* nextNode = this->_left->_next;

		delete this->_left;
		this->_left = nextNode;
		this->_size--;

	}

	template <typename T>
	void List<T>::popBack() {

		if (this->_size == 0)
			return;

		if (this->_size == 1) {

			delete this->_left;
			this->_left = nullptr;
			this->_right = nullptr;
			this->_size--;
			return;

		}

		Node* prevNode = this->_left;
		Node* currentNode = this->_left;

		for (unsigned i = 0; i < this->_size - 1; i++) {

			prevNode = currentNode;
			currentNode = currentNode->_next;

		}

		prevNode->_next = nullptr;
		this->_right = prevNode;
		delete currentNode;
		this->_size--;

	}

}