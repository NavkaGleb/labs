#pragma once

#include <iterator>

#include "StaticContainer.hpp"

namespace ng {

	template <typename T, std::size_t sz>
	class Array : public StaticContainer<T> {
	public:
		// inner classes
		class Iterator : public std::iterator<std::forward_iterator_tag, T>  {
		public:
			// constructor / destructor
			Iterator(T* data, std::size_t index);
			~Iterator() override = default;

			// accessors
			T& operator * () override;
			T* operator -> () override;

			// public methods
			Iterator& operator ++ ();
			Iterator& operator -- ();
			Iterator& operator += (int n);
			Iterator& operator -= (int n);

			bool operator != (const Iterator& other) override;
			bool operator == (const Iterator& other) override;

		private:
			// variables
			T* _data;
			std::size_t _index;

		};

		// constructor / destructor
		Array();
		~Array() override;

		// accessors
		[[nodiscard]] bool empty() const override;
		[[nodiscard]] const std::size_t& size() const override;

		T& front() override;
		T& back() override;
		T& at(std::size_t index) override;

		const T& front() const override;
		const T& back() const override;
		const T& at(std::size_t index) const override;

		Iterator begin();
		
	private:
		// variables
		T* _data;
		std::size_t _size;
		
	}; // class Array

	// ----------------------------------- class Iterator -----------------------------------
	// constructor / destructor
	template <typename T, std::size_t s>
	Array<T, s>::Iterator::Iterator(T* data, std::size_t index) : _data(data), _index(index) {}

	// accessors
	template <typename T, std::size_t s>
	T& Array<T, s>::Iterator::Iterator::operator * () { return this->_data[this->_index]; }

	template <typename T, std::size_t s>
	T* Array<T, s>::Iterator::Iterator::operator -> () { return &this->_data[this->_index]; }

	// public methods
	template <typename T, std::size_t s>
	typename Array<T, s>::Iterator& Array<T, s>::Iterator::operator ++ () {

		++this->_index;
		return *this;

	}

	template <typename T, std::size_t s>
	typename Array<T, s>::Iterator& Array<T, s>::Iterator::operator -- () {

		--this->_index;
		return *this;

	}

	template <typename T, std::size_t s>
	typename Array<T, s>::Iterator& Array<T, s>::Iterator::operator += (int n) {

		this->_index += n;
		return *this;

	}

	template <typename T, std::size_t s>
	typename Array<T, s>::Iterator& Array<T, s>::Iterator::operator -= (int n) {

		this->_index -= n;
		return *this;

	}

	template <typename T, std::size_t s>
	bool Array<T, s>::Iterator::operator != (const Array<T, s>::Iterator& other) { return this->_index != other._index; }

	template <typename T, std::size_t s>
	bool Array<T, s>::Iterator::operator == (const Array<T, s>::Iterator& other) { return this->_index == other._index; }

	// ----------------------------------- class Array -----------------------------------
	// constructor / destructor
	template <typename T, std::size_t s>
	Array<T, s>::Array() : _size(s) { this->_data = new T[this->_size]; }

	template <typename T, std::size_t s>
	Array<T, s>::~Array() { delete this->_data; }

	// accessors
	template <typename T, std::size_t s>
	bool Array<T, s>::empty() const { return this->_size == 0; }

	template <typename T, std::size_t s>
	const std::size_t& Array<T, s>::size() const { return this->_size; }

	template <typename T, std::size_t s>
	T& Array<T, s>::front() { return this->_data[0]; }

	template <typename T, std::size_t s>
	T& Array<T, s>::back() { return this->_data[this->_size - 1]; }

	template <typename T, std::size_t s>
	T& Array<T, s>::at(std::size_t index) { return this->_data[index]; }

	template <typename T, std::size_t s>
	const T& Array<T, s>::front() const { return this->_data[0]; }

	template <typename T, std::size_t s>
	const T& Array<T, s>::back() const { return this->_data[this->_size - 1]; }

	template <typename T, std::size_t s>
	const T& Array<T, s>::at(std::size_t index) const { return this->_data[index]; }

	template <typename T, std::size_t s>
	typename Array<T, s>::Iterator Array<T, s>::begin() { return Iterator(this->_data, 0); }


} // namespace ng