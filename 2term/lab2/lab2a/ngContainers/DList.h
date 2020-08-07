#ifndef DLIST_H
#define DLIST_H

#include <iostream>
#include <cassert>

#include "Container.h"

namespace ng {

	/* ---------------------------------------------- header ---------------------------------------------- */
	template <class T>
	class DList : public Container<T> {
	private:
		// inner class
		class Node {
		public:
			// variables
			T value;
			Node* prev;
			Node* next;

			// constructor
			explicit Node(T value, Node* prev = nullptr, Node* next = nullptr)
				: value(value), prev(prev), next(next) {}

		};

		// variables
		unsigned sz;
		Node* right;
		Node* left;
		const char* _typename_;

	public:
		// constructor / destructor
		DList();
		DList(unsigned size, T value);
		DList(const std::initializer_list<T>& values);

		~DList() override;

		// accessors
		[[nodiscard]] const char* _get_typename_() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] T& front() override;
		[[nodiscard]] T& back() override;
		[[nodiscard]] const T& front() const override;
		[[nodiscard]] const T& back() const override;

		// methods
		[[nodiscard]] bool empty() const override;
		void clear() override;

		void insert(int index, const T& value) override;
		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void erase(int index) override;
		void popFront() override;
		void popBack() override;

		void print() const;

	};

	/* ---------------------------------------------- source ---------------------------------------------- */
	// constructor / destructor

	template <class T>
	DList<T>::DList()
		: sz(0), left(nullptr), right(nullptr), _typename_("ng::DList") {}

	template <class T>
	DList<T>::DList(unsigned size, T value)
		: sz(size), _typename_("ng::DList") {

		this->left = new Node(value);
		Node* prevNode = this->left;

		for (unsigned i = 1; i < this->sz; i++) {

			auto* newNode = new Node(value, nullptr, prevNode);

			prevNode->next = newNode;
			prevNode = prevNode->next;

		}

		this->right = prevNode;

	}

	template <class T>
	DList<T>::DList(const std::initializer_list<T>& values)
		: sz(values.size()), _typename_("ng::DList") {

		this->left = new Node(*values.begin());
		Node* prevNode = this->left;

		for (unsigned i = 1; i < this->sz; i++) {

			auto* newNode = new Node(*(values.begin() + i), nullptr, prevNode);

			prevNode->next = newNode;
			prevNode = prevNode->next;

		}

		this->right = prevNode;

	}

	template <class T>
	DList<T>::~DList() {

		this->clear();

	}

	// accessors

	template <class T>
	const char* DList<T>::_get_typename_() const {

		return this->_typename_;

	}

	template <class T>
	const unsigned& DList<T>::size() const {

		return this->sz;

	}

	template <class T>
	T& DList<T>::front() {

		assert(this->sz != 0);
		return this->left->value;

	}

	template <class T>
	T& DList<T>::back() {

		assert(this->sz != 0);
		return this->right->value;

	}

	template <class T>
	const T& DList<T>::front() const {

		assert(this->sz != 0);
		return this->left->value;

	}

	template <class T>
	const T& DList<T>::back() const {

		assert(this->sz != 0);
		return this->right->value;

	}

	// methods

	template <class T>
	bool DList<T>::empty() const {

		return this->sz == 0;

	}

	template <class T>
	void DList<T>::clear() {

		Node* currentNode = this->left;
		Node* nextNode;

		for (unsigned i = 0; i < this->sz; i++) {

			nextNode = currentNode->next;
			delete currentNode;
			currentNode = nextNode;

		}

		this->left = nullptr;
		this->right = nullptr;
		this->sz = 0;

	}

	template <class T>
	void DList<T>::insert(const int index, const T& value) {

		if (index < 0 || index > this->sz)
			return;

		if (index == 0) {

			this->pushFront(value);
			return;

		}

		if (index == this->sz) {

			this->pushBack(value);
			return;

		}

		auto* newNode = new Node(value);
		Node* currentNode;

		if (index > this->sz / 2) {

			currentNode = this->right;

			for (unsigned i = this->sz - 1; i > index; i--)
				currentNode = currentNode->prev;

		} else {

			currentNode = this->left;

			for (unsigned i = 0; i < index; i++)
				currentNode = currentNode->next;

		}

		currentNode->prev->next = newNode;
		newNode->prev = currentNode->prev;
		currentNode->prev = newNode;
		newNode->next = currentNode;
		this->sz++;

	}

	template <class T>
	void DList<T>::pushFront(const T& value) {

		Node* newNode = new Node(value);
		this->sz++;

		if (!this->left) {

			this->left = newNode;
			this->right = newNode;

			this->left->next = newNode;
			this->right->prev = newNode;
			return;

		}

		this->left->prev = newNode;
		newNode->next = this->left;
		this->left = newNode;

	}

	template <class T>
	void DList<T>::pushBack(const T& value) {

		Node* newNode = new Node(value);
		this->sz++;

		if (!this->right) {

			this->left = newNode;
			this->right = newNode;

			this->left->next = newNode;
			this->right->prev = newNode;
			return;

		}

		this->right->next = newNode;
		newNode->prev = this->right;
		this->right = newNode;

	}

	template <class T>
	void DList<T>::erase(const int index) {

		if (index < 0 || index > this->sz - 1)
			return;

		if (index == 0) {

			this->popFront();
			return;

		}

		if (index == this->sz - 1) {

			this->popBack();
			return;

		}

		Node* currentNode;

		if (index > this->sz / 2) {

			currentNode = this->right;

			for (unsigned i = this->sz - 1; i > index; i++)
				currentNode = currentNode->prev;

		} else {

			currentNode = this->left;

			for (unsigned i = 0; i < index; i++)
				currentNode = currentNode->next;

		}

		currentNode->prev->next = currentNode->next;
		currentNode->next->prev = currentNode->prev;
		delete currentNode;
		this->sz--;

	}

	template <class T>
	void DList<T>::popFront() {

		if (this->sz == 0)
			return;

		if (this->sz == 1) {

			delete this->left;
			this->left = nullptr;
			this->right = nullptr;
			this->sz--;
			return;

		}

		Node* nextNode = this->left->next;

		delete this->left;
		this->left = nextNode;
		this->left->prev = nullptr;
		this->sz--;

	}

	template <class T>
	void DList<T>::popBack() {

		if (this->sz == 0)
			return;

		if (this->sz == 1) {

			delete this->left;
			this->left = nullptr;
			this->right = nullptr;
			this->sz--;
			return;

		}

		Node* prevNode = this->right->prev;

		delete this->right;
		this->right = prevNode;
		this->right->next = nullptr;
		this->sz--;

	}

	template <class T>
	void DList<T>::print() const {

		if (this->sz == 0) {

			std::cout << "dlist is empty (" << std::endl;
			return;

		}

		Node* currentNode = this->left;

		for (unsigned i = 0; i < this->sz; i++) {

			std::cout << currentNode->value << " ";
			currentNode = currentNode->next;

		}

		std::cout << std::endl;

	}

}

#endif // DLIST_H
