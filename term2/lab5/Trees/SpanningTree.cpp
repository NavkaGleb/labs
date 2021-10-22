#include "SpanningTree.hpp"

/* -------------------------------------------- class Node -------------------------------------------- */

// constructor / destructor
ng::SpanningTree::Node::Node(int index, int weight, Node* parent) : _index(index), _weight(weight), _parent(parent) {}

ng::SpanningTree::Node::~Node() {

	for (auto& child : this->_children)
		delete child;

}

// accessors
const int& ng::SpanningTree::Node::index() const {

	return this->_index;

}

const int& ng::SpanningTree::Node::weight() const {

	return this->_weight;

}

const std::vector<ng::SpanningTree::Node*>& ng::SpanningTree::Node::children() const {

	return this->_children;

}

// public methods
void ng::SpanningTree::Node::print() const {

	std::cout << this->_index << " -> " << this->_weight;

}

/* -------------------------------------------- class SpanningTree -------------------------------------------- */

// private methods
int ng::SpanningTree::_weight(Node* node) const {

	if (!node) return 0;

	int result = node->_weight;

	for (const auto& child : node->_children)
		result += this->_weight(child);

	return result;

}

void ng::SpanningTree::_print(Node* node, const int& level, const std::string& caption) const {

	if (!node) return;

	std::cout << caption << ": ";
	node->print();

	if (!node->_children.empty()) {

		std::cout << " (" << std::endl;

		for (int i = 0; i < node->_children.size(); i++) {

			for (int j = 0; j < level; j++) std::cout << "  ";
			this->_print(node->_children[i], level + 1, std::to_string(i));

		}

		for (int i = 0; i < level - 1; i++) std::cout << "  ";
		std::cout << ")";

	}

	std::cout << std::endl;

}

// constructor / destructor
ng::SpanningTree::SpanningTree(Node* root) : _root(root) {}

ng::SpanningTree::~SpanningTree() {

	delete this->_root;

}

// accessors
const ng::SpanningTree::Node* ng::SpanningTree::root() const {

	return this->_root;

}

int ng::SpanningTree::weight() const {

	return this->_weight(this->_root);

}

// public methods
void ng::SpanningTree::link(Node* from, Node* to, const int& weight) {

	if (!this->_root) return;

	from->_children.emplace_back(to);
	to->_parent = from;
	to->_weight = weight;

}

void ng::SpanningTree::print() const {

	this->_print(this->_root, 1, "root");

}