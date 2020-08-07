#pragma once

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <iterator>

namespace ng {

	// abstract class (interface for Container)
	template <class T>
	class Container {
	public:
		// constructor / destructor
		Container() = default;
		virtual ~Container() = default;

		// accessors
		[[nodiscard]] virtual const char* type() const = 0;
		[[nodiscard]] virtual const unsigned& size() const = 0;
		[[nodiscard]] virtual T& front() = 0;
		[[nodiscard]] virtual T& back() = 0;
		[[nodiscard]] virtual const T& front() const = 0;
		[[nodiscard]] virtual const T& back() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;

		// methods
		virtual void clear() = 0;

		virtual void push(int index, const T& value) = 0;
		virtual void pushFront(const T& value) = 0;
		virtual void pushBack(const T& value) = 0;

		virtual void pop(int index) = 0;
		virtual void popFront() = 0;
		virtual void popBack() = 0;

	}; // class Container

} // namespace ng