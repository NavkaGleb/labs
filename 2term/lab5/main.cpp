#include "Modes/Core.hpp"

#include "Random/rand.hpp"
#include "Graphs/LGraph.hpp"

int main() {

	// 1, 2, 5, 6, 7 11, 13, 14, 15, 18

	/*
	 * 1  - adjacency matrix
	 * 2  - adjacency structure
	 * 5  - check if graph is connected
	 * 6  - search for graph components
	 * 7  - check if graph is cyclic
	 * 11 - dfs (recursive)
	 * 13 - bfs
	 * 14 - dijkstra's algorithm
	 * 15 - floyd's algorithm
	 * 17 - kahn's algorithm
	 * 18 - topological sort
	 * 19 - algorithm for building a skeleton tree on the basis search in depth
	 * 21 - kruskal's algorithm
	 *
	 * */

//	Core core;
//	core.run();

	ng::LGraph graph(6, 0, true, false);

	graph.addEdge(0, 3);
	graph.addEdge(0, 5);
	graph.addEdge(0, 1);
	graph.addEdge(1, 4);
	graph.addEdge(2, 0);
	graph.addEdge(3, 2);
	graph.addEdge(3, 4);
	graph.addEdge(4, 4);
	graph.addEdge(4, 1);
	graph.addEdge(4, 2);
	graph.addEdge(5, 1);
	graph.addEdge(5, 5);


	graph.print();
	auto nodes = graph.nodes(0);
	for (const auto& e : nodes)
		std::cout << e << " ";
	std::cout << std::endl;

}
