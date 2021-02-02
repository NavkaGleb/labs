#pragma once

#include <iostream>
#include <vector>
#include <initializer_list>
#include <algorithm>

namespace ng {

	template <typename T>
	class OrderedList {
	public:
		// constructor / destructor
		OrderedList() = default;
		virtual ~OrderedList() = default;

		// accessors
		[[nodiscard]] virtual const char* type() const = 0;
		[[nodiscard]] virtual const unsigned& size() const = 0;
		[[nodiscard]] virtual const T& front() const = 0;
		[[nodiscard]] virtual const T& back() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;

		// public methods
		virtual void clear() = 0;

		virtual void push(const T& value) = 0;
		virtual void pop(const T& value) = 0;

		virtual bool exist(const T& value) const = 0;
		virtual std::vector<T> search(const T& left, const T& right) const = 0;

		virtual void print() const = 0;
		virtual void print(std::ostream& ostream) const = 0;

	}; // class OrderedList

} // namespace ng