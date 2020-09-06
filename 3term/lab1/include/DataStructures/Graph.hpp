#pragma once

#include <vector>

namespace ng {

	class SpanningTree;

	template <typename N, typename E>
	class Graph {
	public:
		// constructor / destructor
		Graph() = default;
		virtual ~Graph() = default;

		// accessors
		[[nodiscard]] virtual bool empty() const = 0;
		[[nodiscard]] virtual std::size_t nodes() const = 0;
		[[nodiscard]] virtual const int& edges() const = 0;
		[[nodiscard]] virtual const bool& directed() const = 0;
		[[nodiscard]] virtual const bool& weighed() const = 0;

		[[nodiscard]] virtual bool connected() const = 0;
		[[nodiscard]] virtual std::vector<std::vector<N>> components() const = 0;

		// modifiers
		virtual void directed(const bool& directed) = 0;
		virtual void weighed(const bool& weighed) = 0;

		// public methods
		virtual void pushNode(const N& value) = 0;
		virtual void popNode(const N& value) = 0;
		virtual void pushEdge(const N& from, const N& to, const E& weight) = 0;
		virtual void popEdge(const N& from, const N& to) = 0;
		virtual void popEdges() = 0;
		virtual void clear() = 0;
		virtual void print() const = 0;

//		virtual void bfs(int snode, bool* visited) const = 0;
//		virtual void bfs(int snode, std::vector<int>& distance) const = 0;
//		[[nodiscard]] virtual std::vector<int> bfs(int snode) const = 0;
//
//		virtual void dfs(int snode, bool* visited) const = 0;
//		virtual void dfs(int snode, std::vector<int>& path) const = 0;
//		[[nodiscard]] virtual std::vector<int> dfs(int snode) const = 0;
//
//		[[nodiscard]] virtual std::vector<int> dijkstra(int snode) const = 0;
//		[[nodiscard]] virtual std::vector<std::vector<int>> floyd() const = 0;
//
//		[[nodiscard]] virtual std::vector<int> kahn() const = 0;
//		[[nodiscard]] virtual std::vector<int> topologicalSort() const = 0;
//
//		[[nodiscard]] virtual SpanningTree* spanningTree(int snode) const = 0;
//		[[nodiscard]] virtual std::vector<SpanningTree*> spanningForest() const = 0;
//		[[nodiscard]] virtual std::vector<SpanningTree*> kruskal() const = 0;

	}; // class Graph

} // namespace ng