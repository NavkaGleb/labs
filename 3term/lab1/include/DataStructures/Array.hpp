#pragma once

#include <initializer_list>

#include "StaticContainer.hpp"

namespace ng {

	template <typename T, std::size_t sz = 0>
	class Array : public StaticContainer<T> {
	public:
		// inner classes
		class Iterator : public StaticContainer<T>::Iterator {
		public:
			// constructor / destructor
			Iterator(T* data, std::size_t index);
			~Iterator() override = default;

			// accessors
			T& operator*() override;
			T* operator->() override;

			// public methods
			Iterator& operator++();
			Iterator& operator--();
			Iterator& operator+=(int n);
			Iterator& operator-=(int n);

			bool operator!=(const Iterator& other);

		private:
			// variables
			T* _data;
			std::size_t _index;

		}; // class Iterator

		class ConstIterator : public StaticContainer<T>::ConstIterator {
		public:
			// constructor / destructor
			ConstIterator(T* data, std::size_t index);
			~ConstIterator() override = default;

			// accessors
			const T& operator*() const override;
			const T* operator->() const override;

			// public methods
			ConstIterator& operator++();
			ConstIterator& operator--();
			ConstIterator& operator+=(int n);
			ConstIterator& operator-=(int n);

			bool operator!=(const ConstIterator& other);

		private:
			// variables
			T* _data;
			std::size_t _index;

		}; // class ConstIterator

		// constructor / destructor
		Array();
		Array(const std::initializer_list<T>& list);
		~Array() override;

		// accessors
		[[nodiscard]] bool empty() const override;
		[[nodiscard]] const std::size_t& size() const override;

		T& front() override;
		T& back() override;
		T& at(std::size_t index) override;
		T& operator[](std::size_t index) override;

		const T& front() const override;
		const T& back() const override;
		const T& at(std::size_t index) const override;
		const T& operator[](std::size_t index) const override;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;
		
	private:
		// variables
		T* _data;
		std::size_t _size;
		
	}; // class Array

	// ----------------------------------- class Iterator -----------------------------------
	// constructor / destructor
	template <typename T, std::size_t sz>
	Array<T, sz>::Iterator::Iterator(T* data, std::size_t index) : _data(data), _index(index) {}

	// accessors
	template <typename T, std::size_t sz>
	T& Array<T, sz>::Iterator::operator*() { return this->_data[this->_index]; }

	template <typename T, std::size_t sz>
	T* Array<T, sz>::Iterator::operator->() { return &this->_data[this->_index]; }

	// public methods
	template <typename T, std::size_t sz>
	typename Array<T, sz>::Iterator& Array<T, sz>::Iterator::operator++() {

		++this->_index;
		return *this;

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::Iterator& Array<T, sz>::Iterator::operator--() {

		--this->_index;
		return *this;

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::Iterator& Array<T, sz>::Iterator::operator+=(int n) {

		this->_index += n;
		return *this;

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::Iterator& Array<T, sz>::Iterator::operator-=(int n) {

		this->_index -= n;
		return *this;

	}

	template <typename T, std::size_t sz>
	bool Array<T, sz>::Iterator::operator!=(const Array<T, sz>::Iterator& other) { return this->_index != other._index; }

	// ----------------------------------- class ConstIterator -----------------------------------
	// constructor / destructor
	template <typename T, std::size_t sz>
	Array<T, sz>::ConstIterator::ConstIterator(T* data, std::size_t index) : _data(data), _index(index) {}

	// accessors
	template <typename T, std::size_t sz>
	const T& Array<T, sz>::ConstIterator::operator*() const { return this->_data[this->_index]; }

	template <typename T, std::size_t sz>
	const T* Array<T, sz>::ConstIterator::operator->() const { return &this->_data[this->_index]; }

	// public methods
	template <typename T, std::size_t sz>
	typename Array<T, sz>::ConstIterator& Array<T, sz>::ConstIterator::operator++() {

		++this->_index;
		return *this;

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::ConstIterator& Array<T, sz>::ConstIterator::operator--() {

		--this->_index;
		return *this;

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::ConstIterator& Array<T, sz>::ConstIterator::operator+=(int n) {

		this->_index += n;
		return *this;

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::ConstIterator& Array<T, sz>::ConstIterator::operator-=(int n) {

		this->_index -= n;
		return *this;

	}

	template <typename T, std::size_t sz>
	bool Array<T, sz>::ConstIterator::operator!=(const Array<T, sz>::ConstIterator& other) { return this->_index != other._index; }

	// ----------------------------------- class Array -----------------------------------
	// constructor / destructor
	template <typename T, std::size_t sz>
	Array<T, sz>::Array() : _size(sz) { this->_data = new T[this->_size]; }

	template <typename T, std::size_t sz>
	Array<T, sz>::Array(const std::initializer_list<T>& list) : _size(list.size()) {

		this->_data = new T[this->_size];

		for (std::size_t i = 0; i < this->_size; ++i)
			this->_data[i] = *(list.begin() + i);

	}

	template <typename T, std::size_t sz>
	Array<T, sz>::~Array() { delete this->_data; }

	// accessors
	template <typename T, std::size_t sz>
	bool Array<T, sz>::empty() const { return this->_size == 0; }

	template <typename T, std::size_t sz>
	const std::size_t& Array<T, sz>::size() const { return this->_size; }

	template <typename T, std::size_t sz>
	T& Array<T, sz>::front() {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_data[0];

	}

	template <typename T, std::size_t sz>
	T& Array<T, sz>::back() {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_data[this->_size - 1];

	}

	template <typename T, std::size_t sz>
	T& Array<T, sz>::at(std::size_t index) {

		if (this->_size == 0 || index < 0 || index > this->_size - 1)
			throw std::out_of_range("index out of range");

		return this->_data[index];

	}

	template <typename T, std::size_t sz>
	T& Array<T, sz>::operator[](std::size_t index) {

		if (this->_size == 0 || index < 0 || index > this->_size - 1)
			throw std::out_of_range("index out of range");

		return this->_data[index];

	}

	template <typename T, std::size_t sz>
	const T& Array<T, sz>::front() const {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_data[0];

	}

	template <typename T, std::size_t sz>
	const T& Array<T, sz>::back() const {

		if (this->_size == 0)
			throw std::out_of_range("index out of range");

		return this->_data[this->_size - 1];

	}

	template <typename T, std::size_t sz>
	const T& Array<T, sz>::at(std::size_t index) const {

		if (this->_size == 0 || index < 0 || index > this->_size - 1)
			throw std::out_of_range("index out of range");

		return this->_data[index];

	}

	template <typename T, std::size_t sz>
	const T& Array<T, sz>::operator[](std::size_t index) const {

		if (this->_size == 0 || index < 0 || index > this->_size - 1)
			throw std::out_of_range("index out of range");

		return this->_data[index];

	}

	template <typename T, std::size_t sz>
	typename Array<T, sz>::Iterator Array<T, sz>::begin() { return Iterator(this->_data, 0); }

	template <typename T, std::size_t sz>
	typename Array<T, sz>::ConstIterator Array<T, sz>::begin() const { return ConstIterator(this->_data, 0); }

	template <typename T, std::size_t sz>
	typename Array<T, sz>::Iterator Array<T, sz>::end() { return Iterator(this->_data, this->_size); }

	template <typename T, std::size_t sz>
	typename Array<T, sz>::ConstIterator Array<T, sz>::end() const { return ConstIterator(this->_data, this->_size); }

} // namespace ng