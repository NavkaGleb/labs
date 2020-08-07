#include <iostream>
#include <list>

#include "Containers/Array.hpp"
#include "Containers/List.hpp"

#include "OrderedLists/LOrderedList.hpp"
#include "OrderedLists/AOrderedList.hpp"
#include "OrderedLists/BSTOrderedList.hpp"
#include "OrderedLists/AVLOrderedList.hpp"
#include "Random/rand.hpp"

int main() {

	ng::List<int> list;
	ng::BST<int> bst;
	ng::AVLTree<int> avl;

	ng::LOrderedList<int> olist;
	ng::AOrderedList<int> alist;
	ng::BSTOrderedList<int> bstlist;

	for (int i = 0; i < 30; ++i)
		bst.push(ng::rand::irand(-10, 10));

	bst.print();

	int rand = ng::rand::irand(-10, 10);
	std::cout << rand << " leaves amount = " << bst.leavesAmount(rand) << std::endl;

}
