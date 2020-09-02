#pragma once

#include <cstdlib>

namespace ng {

	template <typename T>
	class StaticContainer {
	public:
		// inners classes
		class Iterator;
		class ConstIterator;

		class Iterator {
		public:
			// constructor / destructor
			Iterator() = default;
			virtual ~Iterator() = default;

			// accessors
			virtual T& operator * () = 0;
			virtual T* operator -> () = 0;

			// public methods
			virtual bool operator != (const Iterator& other) = 0;
			virtual bool operator == (const Iterator& other) = 0;

		}; // class Iterator

		class ConstIterator {
		public:
			// constructor / destructor
			ConstIterator() = default;
			virtual ~ConstIterator() = default;

			// accessors
			virtual const T& operator * () const = 0;
			virtual const T* operator -> () const = 0;

			// public methods
			virtual bool operator != (const ConstIterator& other) = 0;
			virtual bool operator == (const ConstIterator& other) = 0;

		}; // class ConstIterator

		// constructor / destructor
		StaticContainer() = default;
		virtual ~StaticContainer() = default;

		// accessors
		[[nodiscard]] virtual bool empty() const = 0;
		[[nodiscard]] virtual const std::size_t& size() const = 0;

		virtual T& front() = 0;
		virtual T& back() = 0;
		virtual T& at(std::size_t index) = 0;

		virtual const T& front() const = 0;
		virtual const T& back() const = 0;
		virtual const T& at(std::size_t index) const = 0;

	}; // class StaticContainer

} // namespace ng