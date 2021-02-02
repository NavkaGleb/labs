#pragma once

#include <vector>

namespace ng {

	class SpanningTree;

	// abstract class (interface for graph)
	class Graph {
	public:
		// constructor / destructor
		Graph() = default;
		virtual ~Graph() = default;

		// accessors
		[[nodiscard]] virtual const char* type() const = 0;
		[[nodiscard]] virtual const int& nodes() const = 0;
		[[nodiscard]] virtual const int& edges() const = 0;
		[[nodiscard]] virtual const bool& directed() const = 0;
		[[nodiscard]] virtual const bool& weighed() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;
		[[nodiscard]] virtual bool connected() const = 0;
		[[nodiscard]] virtual bool cyclic() const = 0;
		[[nodiscard]] virtual std::vector<std::vector<int>> components() const = 0;

		// modifiers
		virtual void directed(const bool& directed) = 0;
		virtual void weighed(const bool& weighed) = 0;

		// public methods
		virtual void clear() = 0;
		virtual void addNode() = 0;
		virtual void addEdge(const int& fnode, const int& tnode, const int& weight) = 0;
		virtual void addEdge(const int& fnode, const int& tnode) = 0;
		virtual void eraseEdge(const int& fnode, const int& tnode) = 0;
		virtual void eraseEdges() = 0;
		virtual void print() const = 0;

		virtual void bfs(int snode, bool* visited) const = 0;
		virtual void bfs(int snode, std::vector<int>& distance) const = 0;
		[[nodiscard]] virtual std::vector<int> bfs(int snode) const = 0;

		virtual void dfs(int snode, bool* visited) const = 0;
		virtual void dfs(int snode, std::vector<int>& path) const = 0;
		[[nodiscard]] virtual std::vector<int> dfs(int snode) const = 0;

		[[nodiscard]] virtual std::vector<int> dijkstra(int snode) const = 0;
		[[nodiscard]] virtual std::vector<std::vector<int>> floyd() const = 0;

		[[nodiscard]] virtual std::vector<int> kahn() const = 0;
		[[nodiscard]] virtual std::vector<int> topologicalSort() const = 0;

		[[nodiscard]] virtual SpanningTree* spanningTree(int snode) const = 0;
		[[nodiscard]] virtual std::vector<SpanningTree*> spanningForest() const = 0;
		[[nodiscard]] virtual std::vector<SpanningTree*> kruskal() const = 0;

	}; // class Graph

} // namespace ng