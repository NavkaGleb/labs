#pragma once

#include <iostream>
#include <vector>
#include <string>

// abstract class
namespace ng {

	template <typename T>
	class Tree {
	public:
		class Node {
		public:
			// constructor / destructor
			Node() = default;
			virtual ~Node() = default;

			// public methods
			[[nodiscard]] virtual const T& value() const = 0;

		}; // class Node

		// constructor / destructor
		Tree() = default;
		virtual ~Tree() = default;

		// accessors
		[[nodiscard]] virtual const char* type() const = 0;
		[[nodiscard]] virtual const unsigned& size() const = 0;
		[[nodiscard]] virtual const unsigned& height() const = 0;
		[[nodiscard]] virtual const Node* root() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;

		// public methods
		virtual void clear() = 0;

		virtual Node* push(const T& value) = 0;
		virtual void pop(const T& value) = 0;

		virtual Node* nearestLess(const T& value) const = 0;
		virtual Node* nearestGreater(const T& value) const = 0;
		virtual Node* exist(const T& value) const = 0;

	};

} // namespace ng