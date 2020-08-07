#pragma once

#include "Container.hpp"

namespace ng {

	template <typename T>
	class Array : public Container<T> {
	private:
		// variables
		T* _values;
		unsigned _size;
		unsigned _capacity;
		const char* _type;

		// private methods
		void _resize(const unsigned& capacity);

	public:
		// inner classes
		class Iterator {
		private:
			// variables
			T* _array;
			int _index;
		public:
			// constructor / destructor
			Iterator();
			Iterator(T* array, int index);
			virtual ~Iterator() = default;

			// public methods
			T& operator*();
			T* operator->();

			Iterator& operator++();
			Iterator& operator--();
			Iterator& operator+=(int n);
			Iterator& operator-=(int n);

			bool operator!=(const Iterator& other);

		}; // class Iterator

		class ConstIterator {
		private:
			// variables
			T* _array;
			int _index;
		public:
			// constructor / destructor
			ConstIterator();
			ConstIterator(T* array, int index);
			virtual ~ConstIterator() = default;

			// public methods
			const T& operator*() const;
			const T* operator->() const;

			ConstIterator& operator++();
			ConstIterator& operator--();
			ConstIterator& operator+=(int n);
			ConstIterator& operator-=(int n);

			bool operator!=(const ConstIterator& other);

		}; // class ConstIterator

		// constructor / destructor
		Array();
		explicit Array(unsigned size);
		Array(unsigned size, T value);
		Array(const std::initializer_list<T>& values);
		~Array() override;

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] const unsigned& capacity() const;
		[[nodiscard]] T& front() override;
		[[nodiscard]] T& back() override;
		[[nodiscard]] const T& front() const override;
		[[nodiscard]] const T& back() const override;
		[[nodiscard]] bool empty() const override;

		T& operator[](int index);
		const T& operator[](int index) const;
		T& at(int index);
		const T& at(int index) const;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;
		
		// public methods
		void clear() override;
		void shrinkToFit();

		void push(int index, const T& value) override;
		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void pop(int index) override;
		void popFront() override;
		void popBack() override;

	};

	/* ---------------------------------------------- source ---------------------------------------------- */

	/* ---------------------------------------------- class Iterator ---------------------------------------------- */
	// constructor
	template <typename T>
	Array<T>::Iterator::Iterator() : _array(nullptr), _index(0) {}

	template <typename T>
	Array<T>::Iterator::Iterator(T* array, int index) : _array(array), _index(index) {}

	// public methods
	template <typename T>
	T& Array<T>::Iterator::operator*() { return this->_array[this->_index]; }

	template <typename T>
	T* Array<T>::Iterator::operator->() { return &this->_array[this->_index]; }

	template <typename T>
	typename Array<T>::Iterator& Array<T>::Iterator::operator++() { this->_index++; return *this; }

	template <typename T>
	typename Array<T>::Iterator& Array<T>::Iterator::operator--() { this->_index--; return *this; }

	template <typename T>
	typename Array<T>::Iterator& Array<T>::Iterator::operator+=(int n) { this->_index += n; return *this; }

	template <typename T>
	typename Array<T>::Iterator& Array<T>::Iterator::operator-=(int n) { this->_index -= n; return *this; }

	template <typename T>
	bool Array<T>::Iterator::operator!=(const Array<T>::Iterator& other) { return this->_index != other._index; }

	/* ---------------------------------------------- ConstIterator ---------------------------------------------- */
	// constructor
	template <typename T>
	Array<T>::ConstIterator::ConstIterator() : _array(nullptr), _index(0) {}

	template <typename T>
	Array<T>::ConstIterator::ConstIterator(T* array, int index) : _array(array), _index(index) {}

	// public methods
	template <typename T>
	const T& Array<T>::ConstIterator::operator*() const { return this->_array[this->_index]; }

	template <typename T>
	const T* Array<T>::ConstIterator::operator->() const { return &this->_array[this->_index]; }

	template <typename T>
	typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator++() { this->_index++; return *this; }

	template <typename T>
	typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator--() { this->_index--; return *this; }

	template <typename T>
	typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator+=(int n) { this->_index += n; return *this; }

	template <typename T>
	typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator-=(int n) { this->_index -= n; return *this; }

	template <typename T>
	bool Array<T>::ConstIterator::operator!=(const Array<T>::ConstIterator& other) { return this->_index != other._index; }

	/* ---------------------------------------------- class Array ---------------------------------------------- */
	// private methods
	template <typename T>
	void Array<T>::_resize(const unsigned& capacity) {

		this->_capacity = capacity;
		T* n_values = new T[this->_capacity];

		// copy _values and delete
		for (unsigned i = 0; i < this->_size; i++)
			n_values[i] = this->_values[i];

		delete [] this->_values;
		this->_values = n_values;

	}

	// constructor / destructor
	template <typename T>
	Array<T>::Array() : _values(nullptr), _size(0), _capacity(0), _type("ng::Array") {}

	template <typename T>
	Array<T>::Array(unsigned size) : _size(size), _capacity(size), _type("ng::Array") {

		this->_values = new T[this->_capacity];

	}

	template <typename T>
	Array<T>::Array(unsigned size, T value) : _size(size), _capacity(size), _type("ng::Array") {

		this->_values = new T[this->_capacity];

		for (unsigned i = 0; i < this->_size; i++)
			this->_values[i] = value;

	}

	template <typename T>
	Array<T>::Array(const std::initializer_list<T>& values)
		: _size(values.size()), _capacity(values.size()), _type("ng::Array") {

		this->_values = new T[this->_capacity];

		for (unsigned i = 0; i < values.size(); i++)
			this->_values[i] = *(values.begin() + i);

	}

	template <typename T>
	Array<T>::~Array() {

		std::cout << "ng::Array destructor" << std::endl;
		delete [] this->_values;

	}

	// accessors
	template <typename T>
	const char* Array<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& Array<T>::size() const { return this->_size; }

	template <typename T>
	const unsigned& Array<T>::capacity() const { return this->_capacity; }

	template <typename T>
	T& Array<T>::front() {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_values[0];

	}

	template <typename T>
	T& Array<T>::back() {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_values[this->_size - 1];

	}

	template <typename T>
	const T& Array<T>::front() const {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_values[0];

	}

	template <typename T>
	const T& Array<T>::back() const {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_values[this->_size - 1];

	}

	template <typename T>
	bool Array<T>::empty() const { return this->_size == 0; }

	template <typename T>
	T& Array<T>::operator[](int index) {

		if (index >= this->_size)
			throw std::out_of_range("index out of range");

		return this->_values[index];

	}

	template <typename T>
	const T& Array<T>::operator[](int index) const {

		if (index >= this->_size)
			throw std::out_of_range("index out of range");

		return this->_values[index];

	}

	template <typename T>
	T& Array<T>::at(int index) {

		if (index >= this->_size)
			throw std::out_of_range("index out of range");

		return this->_values[index];

	}

	template <typename T>
	const T& Array<T>::at(int index) const {

		if (index >= this->_size)
			throw std::out_of_range("index out of range");

		return this->_values[index];

	}

	template <typename T>
	typename Array<T>::Iterator Array<T>::begin() { return Array::Iterator(this->_values, 0); }

	template <typename T>
	typename Array<T>::ConstIterator Array<T>::begin() const { return Array::ConstIterator(this->_values, 0); }

	template <typename T>
	typename Array<T>::Iterator Array<T>::end() { return Array::Iterator(this->_values, this->_size); }

	template <typename T>
	typename Array<T>::ConstIterator Array<T>::end() const { return Array::ConstIterator(this->_values, this->_size); }

	// public methods
	template <typename T>
	void Array<T>::clear() {

		delete [] this->_values;

		this->_values = nullptr;
		this->_size = 0;

	}

	template <typename T>
	void Array<T>::shrinkToFit() { this->_capacity = this->_size; }

	template <typename T>
	void Array<T>::push(const int index, const T& value) {

		if (index < 0 || index > this->_size)
			return;

		if (this->_size == 0) {

			this->pushBack(value);
			return;

		}

		if (this->_size == this->_capacity) {

			this->_capacity *= 2;
			auto* values = new T[this->_capacity];

			for (unsigned i = 0; i < index; i++)
				values[i] = this->_values[i];

			for (unsigned i = index; i < this->_size; i++)
				values[i + 1] = this->_values[i];

			values[index] = value;

			delete [] this->_values;
			this->_values = values;
			this->_size++;
			return;

		}

		for (unsigned i = this->_size; i > index; i--)
			this->_values[i] = this->_values[i - 1];

		this->_values[index] = value;
		this->_size++;

	}

	template <typename T>
	void Array<T>::pushFront(const T& value) { this->push(0, value); }

	template <typename T>
	void Array<T>::pushBack(const T& value) {

		if (this->_size == 0 && this->_capacity == 0) {

			this->_size++;
			this->_capacity++;
			this->_values = new T[this->_size];
			this->_values[0] = value;

			return;

		}

		if (this->_size == this->_capacity)
			this->_resize(this->_capacity * 2);

		this->_values[this->_size] = value;
		this->_size++;

	}

	template <typename T>
	void Array<T>::pop(const int index) {

		if (index < 0 || index > this->_size - 1)
			return;

		for (unsigned i = index; i < this->_size - 1; i++)
			this->_values[i] = this->_values[i + 1];

		this->_size--;

	}

	template <typename T>
	void Array<T>::popFront() { this->pop(0); }

	template <typename T>
	void Array<T>::popBack() {

		if (this->_size == 0)
			return;

		this->_size--;

		if (this->_size == this->_capacity / 4)
			this->_resize(this->_capacity / 2);

	}

}