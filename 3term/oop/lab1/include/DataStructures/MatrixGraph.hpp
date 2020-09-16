#pragma once

#include <vector>
#include <map>
#include <initializer_list>
#include <functional>
#include <stack>
#include <iostream>
#include <queue>
#include <functional>

#include "Graph.hpp"

namespace ng {

	template <typename N, typename E = int>
	class MatrixGraph : public Graph<N, E> {
	public:
		// constructor / destructor
		MatrixGraph();
		MatrixGraph(bool directed, bool weighed);
		~MatrixGraph() override;

		// accessors
		[[nodiscard]] bool empty() const override;
		[[nodiscard]] std::size_t nodes() const override;
		[[nodiscard]] const int& edges() const override;
		[[nodiscard]] const bool& directed() const override;
		[[nodiscard]] const bool& weighed() const override;

		[[nodiscard]] bool connected() const override;
		[[nodiscard]] std::vector<std::vector<N>> components() const override;

		// modifiers
		void directed(const bool& directed) override;
		void weighed(const bool& weighed) override;

		// public methods
		void pushNode(const N& value) override;
		void popNode(const N& value) override;
		void pushEdge(const N& from, const N& to, const E& weight) override;
		void pushEdge(const N& from, const N& to) override;
		void popEdge(const N& from, const N& to) override;
		void popEdges() override;
		void clear() override;
		void print() const override;

        void dfs(const N& node, bool* visited) const override;
        void dfs(const N& node, std::vector<N>& path) const override;
        std::vector<N> dfs(const N& node) const override;

		template <typename T = E>
		void bfs(N node, std::map<N, T>& distance, std::function<T(const E&)> f = [](const E& e) { return e; }) const;
		template <typename T = E>
		std::map<N, T> bfs(N node, std::function<T(const E&)> f = [](const E& e) { return e; }) const;

		template <typename T = E>
		std::map<N, T*> dijkstra(N node, std::function<T(const E&)> f = [](const E& e) { return e; }) const;
//		[[nodiscard]] std::vector<std::vector<int>> floyd() const override;
//
//		[[nodiscard]] std::vector<int> kahn() const override;
//		[[nodiscard]] std::vector<int> topologicalSort() const override;
//
//		[[nodiscard]] SpanningTree* spanningTree(int snode) const override;
//		[[nodiscard]] std::vector<ng::SpanningTree*> spanningForest() const override;
//		[[nodiscard]] std::vector<ng::SpanningTree*> kruskal() const override;

	private:
		// variables
		std::vector<std::vector<E*>> _matrix;
		std::map<N, int> _nodes;
		int _edges;
		bool _directed;
		bool _weighed;

		// private methods
		void _dfs(const N& node, bool* visited, std::vector<N>* path) const;
		void _dfs(const int& nodeIndex, bool* visited) const;

	}; // class MatrixGraph

	// constructor / destructor
	template <typename N, typename E>
	MatrixGraph<N, E>::MatrixGraph() : _edges(0), _directed(false), _weighed(false) {}

	template <typename N, typename E>
	MatrixGraph<N, E>::MatrixGraph(bool directed, bool weighed) : _edges(0), _directed(directed), _weighed(weighed) {}

	template <typename N, typename E>
	MatrixGraph<N, E>::~MatrixGraph() {

		for (std::size_t i = 0; i < this->_nodes.size(); ++i)
			for (std::size_t j = 0; j < this->_nodes.size(); ++j)
				delete this->_matrix[i][j];

	}

	// accessors
	template <typename N, typename E>
	bool MatrixGraph<N, E>::empty() const { return this->_nodes.empty(); }

	template <typename N, typename E>
	std::size_t MatrixGraph<N, E>::nodes() const { return this->_nodes.size(); }

	template <typename N, typename E>
	const int& MatrixGraph<N, E>::edges() const { return this->_edges; }

	template <typename N, typename E>
	const bool& MatrixGraph<N, E>::directed() const { return this->_directed; }

	template <typename N, typename E>
	const bool& MatrixGraph<N, E>::weighed() const { return this->_weighed; }

	template <typename N, typename E>
	bool MatrixGraph<N, E>::connected() const {

	    if (this->_nodes.empty())
	        return false;

		auto* visited = new bool[this->_nodes.size()]();

		this->_dfs(0, visited);

		for (int i = 0; i < this->_nodes.size(); ++i) {

            if (!visited[i]) {

                delete [] visited;
                return false;

            }

        }

		delete [] visited;
		return true;

	}

	template <typename N, typename E>
	std::vector<std::vector<N>> MatrixGraph<N, E>::components() const {

		std::vector<std::vector<N>> components;

		if (this->_nodes.empty())
		    return components;

		if (!this->_directed) {

			std::vector<N> component;
			bool* visited = new bool[this->_nodes.size()]();

			for (const auto& p : this->_nodes) {

				if (!visited[p.second]) {

					component.clear();
					this->_dfs(p.first, visited, &component);
					components.emplace_back(component);

				}

			}

			delete [] visited;

		} else {

			// Tarjan's Strongly Connected Component Algorithm
			int id = 0;
			bool* onStack = new bool[this->_nodes.size()]();
			int* ids = new int[this->_nodes.size()]();
			int* low = new int[this->_nodes.size()]();
			std::stack<N> stack;

			std::fill_n(ids, this->_nodes.size(), -1);

			std::function<void(const N&)> _dfs = [&](const N& node) {

				stack.emplace(node);
				onStack[this->_nodes.at(node)] = true;
				ids[this->_nodes.at(node)] = id;
				low[this->_nodes.at(node)] = id++;

				for (const auto& [key, value] : this->_nodes) {

					if (this->_matrix[this->_nodes.at(node)][value] && ids[value] == -1)
						_dfs(key);

					if (this->_matrix[this->_nodes.at(node)][value] && onStack[value])
						low[this->_nodes.at(node)] = std::min(low[this->_nodes.at(node)], low[value]);

				}

				if (ids[this->_nodes.at(node)] == low[this->_nodes.at(node)]) {

					components.emplace_back(std::vector<N>(0));

					for (N n = stack.top(); ; n = stack.top()) {

						stack.pop();
						onStack[this->_nodes.at(n)] = false;
						components.back().emplace_back(n);

						if (n == node)
						    break;

					}

					std::reverse(components.back().begin(), components.back().end());

				}

			};

			for (const auto p : this->_nodes)
				if (ids[p.second] == -1)
					_dfs(p.first);

			delete [] onStack;
			delete [] ids;
			delete [] low;

		}

		return components;

	}

	// modifiers
	template <typename N, typename E>
	void MatrixGraph<N, E>::directed(const bool& directed) { this->_directed = directed; }

	template <typename N, typename E>
	void MatrixGraph<N, E>::weighed(const bool& weighed) { this->_weighed = weighed; }

	// public methods
	template <typename N, typename E>
	void MatrixGraph<N, E>::pushNode(const N& value) {

		this->_nodes[value] = this->_nodes.size();

		for (auto& a : this->_matrix)
			a.emplace_back(nullptr);

		this->_matrix.emplace_back(std::vector<E*>(this->_nodes.size(), nullptr));

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::popNode(const N& value) {

        int index = this->_nodes[value];

        for (int i = 0; i < this->_nodes.size(); ++i)
            this->_matrix[i].erase(this->_matrix[i].begin() + index);

        this->_matrix.erase(this->_matrix.begin() + index);
        this->_nodes.erase(value);

        for (const auto& [key, value] : this->_nodes)
            if (value > index)
                --this->_nodes[key];

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::pushEdge(const N& from, const N& to, const E& weight) {

		this->_matrix[this->_nodes[from]][this->_nodes[to]] = new E(weight);

		if (!this->_directed)
			this->_matrix[this->_nodes[to]][this->_nodes[from]] = new E(weight);

		++this->_edges;

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::pushEdge(const N& from, const N& to) {

        this->_matrix[this->_nodes[from]][this->_nodes[to]] = new E();

        if (!this->_directed)
            this->_matrix[this->_nodes[to]][this->_nodes[from]] = new E();

        ++this->_edges;

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::popEdge(const N& from, const N& to) {

		delete this->_matrix[this->_nodes[from]][this->_nodes[to]];
		this->_matrix[this->_nodes[from]][this->_nodes[to]] = nullptr;

		if (!this->_directed) {

			delete this->_matrix[this->_nodes[to]][this->_nodes[from]];
			this->_matrix[this->_nodes[to]][this->_nodes[from]] = nullptr;

		}

		--this->_edges;

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::popEdges() {

        for (const auto& row : this->_matrix)
            for (const auto& e : row)
                delete e;

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::clear() {

        for (const auto& row : this->_matrix)
            for (const auto& e : row)
                delete e;

        this->_nodes.clear();

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::print() const {

	    std::cout << "  ";
	    for (int i = 0; i < this->_nodes.size(); ++i)
	        std::cout << i << " ";
	    std::cout << std::endl;

		for (int i = 0; i < this->_nodes.size(); ++i) {

			for (int j = 0; j < this->_nodes.size(); ++j) {

			    if (j == 0)
			        std::cout << i << " ";

				if (this->_matrix[i][j])
					std::cout << *this->_matrix[i][j] << " ";
				else
					std::cout << "n" << " ";

			}

			std::cout << std::endl;

		}

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::dfs(const N& node, bool* visited) const {

        this->_dfs(node, visited, nullptr);

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::dfs(const N& node, std::vector<N>& path) const {

	    bool* visited = new bool[this->_nodes.size()];

	    this->_dfs(node, visited, &path);
	    delete [] visited;

	}

	template <typename N, typename E>
	std::vector<N> MatrixGraph<N, E>::dfs(const N& node) const {

	    std::vector<N> path;

	    this->dfs(node, path);
	    return path;

	}

    template <typename N, typename E>
    template <typename T>
    void MatrixGraph<N, E>::bfs(N node, std::map<N, T>& distance, std::function<T(const E&)> f) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::queue<N> queue;

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            node = queue.front();
            queue.pop();

            for (const auto& [key, value] : this->_nodes) {

                if (this->_matrix[this->_nodes.at(node)][value] && !visited[value]) {

                    visited[value] = true;
                    queue.emplace(key);
                    distance[key] = distance[node] + f(*this->_matrix[this->_nodes.at(node)][value]);

                }

            }

        }

        delete [] visited;

    }

    template <typename N, typename E>
    template <typename T>
    std::map<N, T> MatrixGraph<N, E>::bfs(N node, std::function<T(const E&)> f) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::map<N, T> distance;
        std::queue<N> queue;

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            node = queue.front();
            queue.pop();

            for (const auto& [key, value] : this->_nodes) {

                if (this->_matrix[this->_nodes.at(node)][value] && !visited[value]) {

                    visited[value] = true;
                    queue.emplace(key);
                    distance[key] = distance[node] + f(*this->_matrix[this->_nodes.at(node)][value]);

                }

            }

        }

        delete [] visited;
        return distance;

    }

    template <typename N, typename E>
    template <typename T>
    std::map<N, T*> MatrixGraph<N, E>::dijkstra(N node, std::function<T(const E&)> f) const {

        std::map<N, T*> distance;
        std::priority_queue<std::pair<T, N>> pqueue;

        for (const auto& p : this->_nodes)
            distance[p.first] = nullptr;

        distance[node] = new T();
        pqueue.emplace(*distance[node], node);

        while (!pqueue.empty()) {

            node = pqueue.top().second;
            pqueue.pop();

            for (const auto& [key, value] : this->_nodes) {

                if (this->_matrix[this->_nodes.at(node)][value] && (
                    !distance[value] ||
                    *distance[key] > *distance[node] + f(*this->_matrix[this->_nodes.at(node)][value]))) {

                    if (!distance[key])
                        distance[key] = new T();

                    *distance[key] = *distance[node] + f(*this->_matrix[this->_nodes.at(node)][value]);
                    pqueue.emplace(*distance[key] * -1, key);

                }

            }

        }

        return distance;

    }

	// private methods
	template <typename N, typename E>
	void MatrixGraph<N, E>::_dfs(const N& node, bool* visited, std::vector<N>* path) const {

	    visited[this->_nodes.at(node)] = true;

	    if (path)
	        path->emplace_back(node);

	    for (const auto& [key, value] : this->_nodes)
	        if (this->_matrix[this->_nodes.at(node)][value] && !visited[value])
	            this->_dfs(key, visited, path);

	}

	template <typename N, typename E>
	void MatrixGraph<N, E>::_dfs(const int& nodeIndex, bool* visited) const {

	    visited[nodeIndex] = true;

	    for (int i = 0; i < this->_nodes.size(); ++i)
	        if (this->_matrix[nodeIndex][i] && !visited[i])
	            this->_dfs(i, visited);

	}

} // namespace ng