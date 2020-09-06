#include <iostream>

#include <Array.hpp>
#include <MatrixGraph.hpp>

struct Vertex {

	int x;
	int y;

};

int main() {

	ng::Array<int> array = { 1, 5, 36, 94 };

	for (const auto& e : array)
		std::cout << e << " ";
	std::cout << std::endl;

	ng::MatrixGraph<Vertex> graph(false, true);

	graph.pushNode(0);
	graph.pushNode(1);
	graph.pushNode(2);

	graph.pushEdge(0, 1, 12);
	graph.pushEdge(1, 2, 43);

	graph.print();


}