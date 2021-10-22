#pragma once

#include <vector>

#include "Graph.hpp"
#include "../Trees/SpanningTree.hpp"

namespace ng {

	class MGraph;

	class LGraph : public Graph {
	private:
		// friend
		friend MGraph;

		// inner structs
		struct Edge {
			// variables
			int toNode;
			int weight;

			// constructor
			Edge() = default;
			explicit Edge(int toNode, int weight = 1);

			// operators
			bool operator<(const Edge& other) const;
			bool operator>(const Edge& other) const;

		};

		// variables
		std::vector<std::vector<Edge>> _list;
		const char* _type;
		int _nodes;
		int _edges;
		bool _directed;
		bool _weighed;

		// private methods
		void _rfill();
		void _addEdge(const int& fnode, const int& tnode, const int& weight, const bool& increment = true);
		void _dfs(int snode, bool* visited, std::vector<int>* path = nullptr) const;

	public:
		// constructor / destructor
		LGraph();
		explicit LGraph(const MGraph& mgraph);
		explicit LGraph(int nodes, int edges = 0, bool directed = false, bool weighed = false);
		~LGraph() override = default;

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const int& nodes() const override;
		[[nodiscard]] const int& edges() const override;
		[[nodiscard]] const bool& directed() const override;
		[[nodiscard]] const bool& weighed() const override;
		[[nodiscard]] bool empty() const override;
		[[nodiscard]] bool connected() const override;
		[[nodiscard]] bool cyclic() const override;
		[[nodiscard]] std::vector<std::vector<int>> components() const override;

		// modifiers
		void directed(const bool& directed) override;
		void weighed(const bool& weighed) override;

		// public methods
		void clear() override;
		void build(const MGraph& mgraph);
		void addNode() override;
		void addEdge(const int& fnode, const int& tnode, const int& weight) override;
		void addEdge(const int& fnode, const int& tnode) override;
		void eraseEdge(const int& fnode, const int& tnode) override;
		void eraseEdges() override;
		void print() const override;

		void bfs(int snode, bool* visited) const override;
		void bfs(int snode, std::vector<int>& distance) const override;
		[[nodiscard]] std::vector<int> bfs(int snode) const override;

		void dfs(int snode, bool* visited) const override;
		void dfs(int snode, std::vector<int>& path) const override;
		[[nodiscard]] std::vector<int> dfs(int snode) const override;

		[[nodiscard]] std::vector<int> dijkstra(int snode) const override;
		[[nodiscard]] std::vector<std::vector<int>> floyd() const override;

		[[nodiscard]] std::vector<int> kahn() const override;
		[[nodiscard]] std::vector<int> topologicalSort() const override;

		[[nodiscard]] SpanningTree* spanningTree(int snode) const override;
		[[nodiscard]] std::vector<SpanningTree*> spanningForest() const override;
		[[nodiscard]] std::vector<SpanningTree*> kruskal() const override;

		int edgesAmount() const;

		std::vector<int> nodes(int currentNode) const;

	}; // class LGraph

} // namespace ng