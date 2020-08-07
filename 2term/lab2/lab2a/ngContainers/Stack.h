#ifndef STACK_H
#define STACK_H

#include "List.h"

namespace ng {

	/* ---------------------------------------------- header ---------------------------------------------- */

	template <class T>
	class Stack {
	private:
		// variables
		List<T> list;

	public:
		// constructor / destructor
		Stack() = default;
		~Stack();

		// accessors
		[[nodiscard]] const unsigned& size() const;
		T& top();

		// methods
		[[nodiscard]] bool empty() const;
		void clear();

		void push(const T& value);
		void pop();

	};

	/* ---------------------------------------------- source ---------------------------------------------- */

	// constructor / destructor

	template <class T>
	Stack<T>::~Stack() {

		this->list.clear();

	}

	// accessors

	template <class T>
	const unsigned& Stack<T>::size() const {

		return this->list.size();

	}

	template <class T>
	T& Stack<T>::top() {

		return this->list.back();

	}

	// methods

	template <class T>
	bool Stack<T>::empty() const {

		return this->list.empty();

	}

	template <class T>
	void Stack<T>::clear() {

		this->list.clear();

	}

	template <class T>
	void Stack<T>::push(const T& value) {

		this->list.pushBack(value);

	}

	template <class T>
	void Stack<T>::pop() {

		this->list.popBack();

	}

}

#endif // STACK_H
