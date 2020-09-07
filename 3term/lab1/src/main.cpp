#include <iostream>
#include <algorithm>

#include <Array.hpp>
#include <MatrixGraph.hpp>

struct Vertex {

	int x;
	int y;

	bool operator<(const Vertex& other) const {
	    return x * y < other.x * other.y;
	}

};

int main() {

	ng::Array<int> array = { 1, 5, 36, 94 };

	for (const auto& e : array)
		std::cout << e << " ";
	std::cout << std::endl;

	ng::MatrixGraph<Vertex> graph(false, true);

	Vertex a = { 0, 1 };
	Vertex b = { 2, 3 };
	Vertex c = { 4, 5 };
	Vertex d = { 6, 7 };

	graph.pushNode(a);
	graph.pushNode(b);
	graph.pushNode(c);
	graph.pushNode(d);

	graph.pushEdge(a, b, 3);
	graph.pushEdge(a, c, 7);
	graph.pushEdge(b, d, 8);

	graph.print();

//	graph.popEdge(a, b);
//
//	std::cout << "---" << std::endl;
//	graph.print();

	std::vector<Vertex> path;
	graph.dfs(a, path);

	for (const auto& v : path)
	    std::cout << "{ " << v.x << ", " << v.y << " }" << " ";

}