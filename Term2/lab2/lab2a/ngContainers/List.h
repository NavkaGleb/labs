#ifndef LIST_H
#define LIST_H

#include <iostream>

#include "Container.h"

namespace ng {

	/* ---------------------------------------------- header ---------------------------------------------- */

	template <class T>
	class List : public Container<T> {
	private:
		// inner class
		class Node {
		public:
			// variables
			T value;
			Node* next;

			// constructor
			explicit Node(T value, Node* next = nullptr)
				: value(value), next(next) {}

		};

		// variables
		Node* left;
		Node* right;
		unsigned sz;
		const char* _typename_;

	public:
		// constructor / destructor
		List();
		explicit List(unsigned size, T value);
		List(const std::initializer_list<T>& values);

		~List() override;

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

	/* ---------------------------------------------- source ----------------------------------------------  */

	// constructor / destructor

	template <class T>
	List<T>::List()
		: sz(0), left(nullptr), right(nullptr), _typename_("ng::List") {}

	template <class T>
	List<T>::List(unsigned size, T value)
		: sz(size), _typename_("ng::List") {

		this->left = new Node(value);
		Node* prevNode = this->left;

		for (unsigned i = 1; i < this->sz; i++) {

			auto* newNode = new Node(value);

			prevNode->next = newNode;
			prevNode = prevNode->next;

		}

		this->right = prevNode;

	}

	template <class T>
	List<T>::List(const std::initializer_list<T>& values)
		: sz(values.size()), _typename_("ng::List") {

		this->left = new Node(*values.begin());
		Node* prevNode = this->left;

		for (unsigned i = 1; i < this->sz; i++) {

			auto* newNode = new Node(*(values.begin() + i));

			prevNode->next = newNode;
			prevNode = prevNode->next;

		}

		this->right = prevNode;

	}

	template <class T>
	List<T>::~List() {

		this->clear();

	}

	// accessors

	template <class T>
	const char* List<T>::_get_typename_() const {

		return this->_typename_;

	}

	template <class T>
	const unsigned& List<T>::size() const {

		return this->sz;

	}

	template <class T>
	T& List<T>::front() {

		return this->left->value;

	}

	template <class T>
	T& List<T>::back() {

		return this->right->value;

	}

	template <class T>
	const T& List<T>::front() const {

		return this->left->value;

	}

	template <class T>
	const T& List<T>::back() const {

		return this->right->value;

	}

	// methods

	template <class T>
	bool List<T>::empty() const {

		return this->sz == 0;

	}

	template <class T>
	void List<T>::clear() {

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
	void List<T>::insert(const int index, const T& value) {

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
		Node* currentNode = this->left;

		for (unsigned i = 1; i < index; i++)
			currentNode = currentNode->next;

		Node* nextNode = currentNode->next;

		currentNode->next = newNode;
		newNode->next = nextNode;
		this->sz++;

	}

	template <class T>
	void List<T>::pushFront(const T& value) {

		auto* newNode = new Node(value, this->left);

		this->left = newNode;

		if (!this->right) {

			this->left->next = newNode;
			this->right = newNode;

		}

		this->sz++;

	}

	template <class T>
	void List<T>::pushBack(const T& value) {

		auto* newNode = new Node(value);

		if (!this->left) {

			this->left = newNode;
			this->left->next = newNode;
			this->right = newNode;
			this->sz++;
			return;

		}

		this->right->next = newNode;
		this->right = newNode;
		this->sz++;

	}

	template <class T>
	void List<T>::erase(const int index) {

		if (index < 0 || index > this->sz - 1)
			return;

		Node* currentNode = this->left;

		if (index == 0) {

			currentNode = currentNode->next;
			delete this->left;
			this->left = currentNode;
			this->sz--;
			return;

		}

		Node* prevNode = this->left;
		Node* nextNode;

		for (unsigned i = 0; i < index; i++) {

			prevNode = currentNode;
			currentNode = currentNode->next;

		}

		nextNode = currentNode->next;
		delete currentNode;
		prevNode->next = nextNode;
		this->sz--;

	}

	template <class T>
	void List<T>::popFront() {

		if (this->sz == 0)
			return;

		if (this->sz == 1) {

			delete this->left;
			this->left = nullptr;
			this->sz--;
			return;

		}

		Node* nextNode = this->left->next;

		delete this->left;
		this->left = nextNode;
		this->sz--;

	}

	template <class T>
	void List<T>::popBack() {

		if (this->sz == 0)
			return;

		if (this->sz == 1) {

			delete this->left;
			this->left = nullptr;
			this->right = nullptr;
			this->sz--;
			return;

		}

		Node* prevNode = this->left;
		Node* currentNode = this->left;

		for (unsigned i = 0; i < this->sz - 1; i++) {

			prevNode = currentNode;
			currentNode = currentNode->next;

		}

		prevNode->next = nullptr;
		this->right = prevNode;
		delete currentNode;
		this->sz--;

	}

	template <class T>
	void List<T>::print() const {

		if (this->sz == 0) {

			std::cout << "list is empty (" << std::endl;
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

#endif // LIST_H
