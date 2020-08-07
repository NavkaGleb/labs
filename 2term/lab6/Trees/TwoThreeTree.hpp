#pragma once

#include "Tree.hpp"

namespace ng {

	template <typename T>
	class TwoThreeTree : public Tree<T> {
	public:
		// inner classes
		class Node : public Tree<T>::Node {
		private:
			// variables
			T _value;

		public:
			// constructor / destructor
			Node();
			~Node();

			// accessors

		};

		// constructor / destructor

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] const unsigned& height() const override;
		[[nodiscard]] const Node* root() const override;
		[[nodiscard]] bool empty() const override;

		// public methods
		void clear() override;

		Node* push(const T& value) override;
		void pop(const T& value) override;

		Node* nearestLess(const T& value) const override;
		Node* nearestGreater(const T& value) const override;

	private:
		// variables

		// private methods
	};

} // namespace ng