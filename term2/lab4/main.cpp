#include "Modes/Core.hpp"

#include "Trees/BinarySearchTree.hpp"
#include "Rand/rand.hpp"

int main() {

	// 1, 2, 3, 4, 8, 11, 13, 15, 18, 20, 23

//	Core core;
//	core.run();

	ng::BinarySearchTree<int> bst;

	for (int i = 0; i < 10; ++i)
		bst.add(ng::rand::irand(-10, 10));

	bst.tprint();
	std::cout << "traversal:" << std::endl;
	bst.traversal();

	ng::ExpressionTree tree;

	tree.input();
	tree.print();

}