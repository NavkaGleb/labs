#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <cassert>
#include <cstring>

#include "Container.h"

namespace ng {

	/* ---------------------------------------------- header ---------------------------------------------- */

	template <class T>
	class Array : public Container<T> {
	private:
		// variables
		T* values;
		unsigned sz;						// size
		unsigned cpt;						// capacity
		const char* _typename_;

		typedef T* iterator;
		typedef const T* const_iterator;

		// methods
		void resize(const unsigned& capacity);

	public:
		// constructor / destructor
		Array();
		explicit Array(unsigned size);
		Array(unsigned size, T value);
		Array(const std::initializer_list<T>& values);

		~Array() override;

		// accessors
		[[nodiscard]] const char* _get_typename_() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] const unsigned& capacity() const;

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		T& operator [](int index);
		T& at(int index);
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

		void print(char separator, bool showIndex = false) const;

	};

	/* ---------------------------------------------- source ---------------------------------------------- */

	// private methods

	template <class T>
	void Array<T>::resize(const unsigned& capacity) {

		this->cpt = capacity;
		T* nvalues = new T[this->cpt];                         // new values

		// copy values and delete
		for (unsigned i = 0; i < this->sz; i++)
			nvalues[i] = this->values[i];

		delete [] this->values;
		this->values = nvalues;

	}

	// constructor / destructor

	template <class T>
	Array<T>::Array()
		: values(nullptr), sz(0), cpt(0), _typename_("ng::Array") {}

	template <class T>
	Array<T>::Array(unsigned size)
		: sz(size), cpt(size), _typename_("ng::Array") {

		this->values = new T[this->cpt];

	}

	template <class T>
	Array<T>::Array(unsigned size, T value)
		: sz(size), cpt(size), _typename_("ng::Array") {

		this->values = new T[this->cpt];

		for (unsigned i = 0; i < this->sz; i++)
			this->values[i] = value;

	}

	template <class T>
	Array<T>::Array(const std::initializer_list<T>& values)
		: sz(values.size()), cpt(values.size()), _typename_("ng::Array") {

		this->values = new T[this->cpt];

		for (unsigned i = 0; i < values.size(); i++)
			this->values[i] = *(values.begin() + i);

	}

	template <class T>
	Array<T>::~Array() {

		if (this->cpt != 0)
			this->clear();

	}

	// accessors

	template <class T>
	const char* Array<T>::_get_typename_() const {

		return this->_typename_;

	}

	template <class T>
	const unsigned& Array<T>::size() const {

		return this->sz;

	}

	template <class T>
	const unsigned& Array<T>::capacity() const {

		return this->cpt;

	}

	template <class T>
	typename Array<T>::iterator ng::Array<T>::begin() {

		return &this->values[0];

	}

	template <class T>
	typename Array<T>::const_iterator Array<T>::begin() const {

		return &this->values[0];

	}

	template <class T>
	typename Array<T>::iterator Array<T>::end() {

		return &this->values[this->sz];

	}

	template <class T>
	typename Array<T>::const_iterator Array<T>::end() const {

		return &this->values[this->sz];

	}

	template <class T>
	T& Array<T>::operator [](const int index) {

		return this->values[index];

	}

	template <class T>
	T& Array<T>::at(const int index) {

		return this->values[index];

	}

	template <class T>
	T& Array<T>::front() {

		assert(this->sz != 0);
		return this->values[0];

	}

	template <class T>
	T& Array<T>::back() {

		assert(this->sz != 0);
		return this->values[this->sz - 1];

	}

	template <class T>
	const T& Array<T>::front() const {

		assert(this->sz != 0);
		return this->values[0];

	}

	template <class T>
	const T& Array<T>::back() const {

		assert(this->sz != 0);
		return this->values[this->sz - 1];

	}

	// public methods

	template <class T>
	bool Array<T>::empty() const {

		return this->sz == 0;

	}

	template <class T>
	void Array<T>::clear() {

		delete[] this->values;

		this->values = nullptr;
		this->sz = 0;
		this->cpt = 0;

	}

	template <class T>
	void Array<T>::insert(const int index, const T& value) {

		if (index < 0 || index > this->sz)
			return;

		if (this->sz == 0) {

			this->pushBack(value);
			return;

		}

		if (this->sz == this->cpt) {

			this->cpt *= 2;
			auto* nvalues = new T[this->cpt];						// new values

			for (unsigned i = 0; i < index; i++)
				nvalues[i] = this->values[i];

			for (unsigned i = index; i < this->sz; i++)
				nvalues[i + 1] = this->values[i];

			nvalues[index] = value;

			delete [] this->values;
			this->values = nvalues;
			this->sz++;
			return;

		}

		for (unsigned i = this->sz; i > index; i--)
			this->values[i] = this->values[i - 1];

		this->values[index] = value;
		this->sz++;

	}

	template <class T>
	void Array<T>::pushFront(const T& value) {

		this->insert(0, value);

	}

	template <class T>
	void Array<T>::pushBack(const T& value) {

		if (this->sz == 0 && this->cpt == 0) {

			this->sz++;
			this->cpt++;
			this->values = new T[this->sz];
			this->values[0] = value;

			return;

		}

		if (this->sz == this->cpt)
			this->resize(this->cpt * 2);

		this->values[this->sz] = value;
		this->sz++;

	}

	template <class T>
	void Array<T>::erase(const int index) {

		if (index < 0 || index > this->sz - 1)
			return;

		for (unsigned i = index; i < this->sz - 1; i++)
			this->values[i] = this->values[i + 1];

		this->sz--;

	}

	template <class T>
	void Array<T>::popFront() {

		this->erase(0);

	}

	template <class T>
	void Array<T>::popBack() {

		if (this->sz == 0)
			return;

		this->sz--;

		if (this->sz == this->cpt / 4)
			this->resize(this->cpt / 2);

	}

	template <class T>
	void Array<T>::print(const char separator, const bool showIndex) const {

		if (this->sz == 0) {

			std::cout << "array is empty (" << std::endl;
			return;

		}

		for (unsigned i = 0; i < this->sz; i++) {

			if (showIndex)
				std::cout << i << ": ";
			std::cout << this->values[i] << separator;

		}

		std::cout << std::endl;

	}

}

#endif // ARRAY_H
