#pragma once

#include <iostream>
#include <vector>

namespace ng {

	class SpanningTree {
	public:
		// inner class
		class Node {
		private:
			// variables
			int _index;
			int _weight;
			Node* _parent;
			std::vector<Node*> _children;

			// friends
			friend SpanningTree;

		public:
			// constructor / destructor
			explicit Node(int index, int weight = 0, Node* parent = nullptr);
			virtual ~Node();

			// accessors
			[[nodiscard]] const int& index() const;
			[[nodiscard]] const int& weight() const;
			[[nodiscard]] const std::vector<Node*>& children() const;

			// public methods
			void print() const;

		};

		// constructor / destructor
		explicit SpanningTree(Node* root = nullptr);
		virtual ~SpanningTree();

		// accessors
		[[nodiscard]] const Node* root() const;
		[[nodiscard]] int weight() const;

		// public methods
		void link(Node* from, Node* to, const int& weight);
		void print() const;

	private:
		// variables
		Node* _root;

		// private methods
		int _weight(Node* node) const;
		void _print(Node* node, const int& level, const std::string& caption) const;

	}; // class SpanningTree

} // namespace ng