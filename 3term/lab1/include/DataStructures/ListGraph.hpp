#pragma once

#include <vector>
#include <map>
#include <initializer_list>
#include <functional>
#include <stack>
#include <iostream>
#include <queue>
#include <functional>
#include <unordered_set>

#include "Graph.hpp"

namespace ng {

    template <typename N, typename E = int>
    class ListGraph : public Graph<N, E> {
    public:
        // constructor / destructor
        ListGraph();
        ListGraph(bool directed, bool weighed);
        ~ListGraph() override;

        // accessors
        [[nodiscard]] bool empty() const override;
        [[nodiscard]] std::size_t nodes() const override;
        [[nodiscard]] const int &edges() const override;
        [[nodiscard]] const bool &directed() const override;
        [[nodiscard]] const bool &weighed() const override;

        [[nodiscard]] bool connected() const override;
        [[nodiscard]] std::vector<std::vector<N>> components() const override;

        // modifiers
        void directed(const bool &directed) override;
        void weighed(const bool &weighed) override;

        // public methods
        void pushNode(const N &value) override;
        void popNode(const N &value) override;
        void pushEdge(const N &from, const N &to, const E &weight) override;
        void popEdge(const N &from, const N &to) override;
        void popEdges() override;
        void clear() override;
        void print() const override;

        void dfs(const N &node, bool *visited) const override;
        void dfs(const N &node, std::vector<N> &path) const override;
        [[nodiscard]] std::vector<N> dfs(const N &node) const override;

        template <typename T> void bfs(const N &node, std::map<N, T> &distance) const;
        template <typename T> void bfs(const N &node, std::map<N, T> &distance, std::function<T(const E &)> f) const;
        template <typename T> [[nodiscard]] std::map<N, T> bfs(const N &node) const;
        template <typename T> [[nodiscard]] std::map<N, T> bfs(const N &node, std::function<T(const E &)> f) const;
//
//		[[nodiscard]] std::vector<int> dijkstra(int snode) const override;
//		[[nodiscard]] std::vector<std::vector<int>> floyd() const override;
//
//		[[nodiscard]] std::vector<int> kahn() const override;
//		[[nodiscard]] std::vector<int> topologicalSort() const override;
//
//		[[nodiscard]] SpanningTree* spanningTree(int snode) const override;
//		[[nodiscard]] std::vector<ng::SpanningTree*> spanningForest() const override;
//		[[nodiscard]] std::vector<ng::SpanningTree*> kruskal() const override;

    private:
        // inner structs
        struct Edge {
            // variables
            N toNode;
            E value;

            // constructor
            Edge() = default;
            Edge(N toNode, E value) : toNode(toNode), value(value) {}

        };

        // variables
        std::map<N, std::vector<Edge>> _list;
        std::map<N, int> _nodes;
        int _edges;
        bool _directed;
        bool _weighed;

        // private methods
        void _dfs(const N &node, bool *visited, std::vector<N> *path) const;

    }; // class MatrixGraph

    // constructor / destructor
    template <typename N, typename E>
    ListGraph<N, E>::ListGraph() : _edges(0), _directed(false), _weighed(false) {}

    template <typename N, typename E>
    ListGraph<N, E>::ListGraph(bool directed, bool weighed) : _edges(0), _directed(directed), _weighed(weighed) {}

    template <typename N, typename E>
    ListGraph<N, E>::~ListGraph() {



    }

    // accessors
    template <typename N, typename E>
    bool ListGraph<N, E>::empty() const { return this->_nodes.empty(); }

    template <typename N, typename E>
    std::size_t ListGraph<N, E>::nodes() const { return this->_nodes.size(); }

    template <typename N, typename E>
    const int& ListGraph<N, E>::edges() const { return this->_edges; }

    template <typename N, typename E>
    const bool& ListGraph<N, E>::directed() const { return this->_directed; }

    template <typename N, typename E>
    const bool& ListGraph<N, E>::weighed() const { return this->_weighed; }

    template <typename N, typename E>
    bool ListGraph<N, E>::connected() const {

        auto* visited = new bool[this->_nodes.size()]();

        if (!this->_list.empty())
            this->_dfs(this->_list.begin()->first, visited, nullptr);

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
    std::vector<std::vector<N>> ListGraph<N, E>::components() const {

        std::vector<std::vector<N>> components;

        if (!this->_directed) {

            std::vector<N> component;
            bool* visited = new bool[this->_nodes.size()]();

            for (const auto p : this->_nodes) {

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

            std::function<void(N)> _dfs = [&](N node) {

                stack.emplace(node);
                onStack[this->_nodes.at(node)] = true;
                ids[this->_nodes.at(node)] = id;
                low[this->_nodes.at(node)] = id++;

                for (const auto& edge : this->_list.at(node)) {

                    if (ids[this->_nodes.at(edge.toNode)] == -1)
                        _dfs(edge.toNode);

                    if (onStack[this->_nodes.at(edge.toNode)])
                        low[this->_nodes.at(node)] = std::min(
                            low[this->_nodes.at(node)],
                            low[this->_nodes.at(edge.toNode)]
                        );

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

            for (const auto& p : this->_nodes)
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
    void ListGraph<N, E>::directed(const bool& directed) { this->_directed = directed; }

    template <typename N, typename E>
    void ListGraph<N, E>::weighed(const bool& weighed) { this->_weighed = weighed; }

    // public methods
    template <typename N, typename E>
    void ListGraph<N, E>::pushNode(const N& value) {

        this->_nodes[value] = this->_nodes.size();
        this->_list[value] = std::vector<Edge>(0);

    }

    template <typename N, typename E>
    void ListGraph<N, E>::popNode(const N& value) {



    }

    template <typename N, typename E>
    void ListGraph<N, E>::pushEdge(const N& from, const N& to, const E& weight) {

        this->_list[from].emplace_back(to, weight);

        if (!this->_directed)
            this->_list[to].emplace_back(from, weight);

        ++this->_edges;

    }

    template <typename N, typename E>
    void ListGraph<N, E>::popEdge(const N& from, const N& to) {


    }

    template <typename N, typename E>
    void ListGraph<N, E>::popEdges() {

        for (auto& p : this->_list)
            p.second.clear();

    }

    template <typename N, typename E>
    void ListGraph<N, E>::clear() {

        for (auto& p : this->_list)
            p.second.clear();

        this->_list.clear();

    }

    template <typename N, typename E>
    void ListGraph<N, E>::print() const {



    }

    template <typename N, typename E>
    void ListGraph<N, E>::dfs(const N& node, bool* visited) const {

        this->_dfs(node, visited, nullptr);

    }

    template <typename N, typename E>
    void ListGraph<N, E>::dfs(const N& node, std::vector<N>& path) const {

        bool* visited = new bool[this->_nodes.size()];

        this->_dfs(node, visited, &path);
        delete [] visited;

    }

    template <typename N, typename E>
    std::vector<N> ListGraph<N, E>::dfs(const N& node) const {

        std::vector<N> path;

        this->dfs(node, path);
        return path;

    }

    template <typename N, typename E>
    template <typename T>
    void ListGraph<N, E>::bfs(const N& node, std::map<N, T>& distance) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::queue<N> queue;
        N cnode = node;

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            cnode = queue.front();
            queue.pop();

            for (const auto& edge : this->_list.at(cnode))

                if (!visited[this->_nodes.at(edge.toNode)]) {

                    visited[this->_nodes.at(edge.toNode)] = true;
                    queue.emplace(edge.toNode);
                    distance[edge.toNode] = distance[cnode] + edge.value;

                }

        }

        delete [] visited;

    }

    template <typename N, typename E>
    template <typename T>
    void ListGraph<N, E>::bfs(const N& node, std::map<N, T>& distance, std::function<T(const E&)> f) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::queue<N> queue;
        N cnode = node;

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            cnode = queue.front();
            queue.pop();

            for (const auto& edge : this->_list.at(cnode))

                if (!visited[this->_nodes.at(edge.toNode)]) {

                    visited[this->_nodes.at(edge.toNode)] = true;
                    queue.emplace(edge.toNode);
                    distance[edge.toNode] = distance[cnode] + f(edge.value);

                }

        }

        delete [] visited;

    }

    template <typename N, typename E>
    template <typename T>
    std::map<N, T> ListGraph<N, E>::bfs(const N& node) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::map<N, T> distance;
        std::queue<N> queue;
        N cnode = node;

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            cnode = queue.front();
            queue.pop();

            for (const auto& edge : this->_list.at(cnode))

                if (!visited[this->_nodes.at(edge.toNode)]) {

                    visited[this->_nodes.at(edge.toNode)] = true;
                    queue.emplace(edge.toNode);
                    distance[edge.toNode] = distance[cnode] + edge.value;

                }

        }

        delete [] visited;
        return distance;

    }

    template <typename N, typename E>
    template <typename T>
    std::map<N, T> ListGraph<N, E>::bfs(const N& node, std::function<T(const E&)> f) const {

        bool* visited = new bool[this->_nodes.size()]();
        std::map<N, T> distance;
        std::queue<N> queue;
        N cnode = node;

        visited[this->_nodes.at(node)] = true;
        queue.emplace(node);

        while (!queue.empty()) {

            cnode = queue.front();
            queue.pop();

            for (const auto& edge : this->_list.at(cnode))

                if (!visited[this->_nodes.at(edge.toNode)]) {

                    visited[this->_nodes.at(edge.toNode)] = true;
                    queue.emplace(edge.toNode);
                    distance[edge.toNode] = distance[cnode] + f(edge.value);

                }

        }

        delete [] visited;
        return distance;

    }

    // private methods
    template <typename N, typename E>
    void ListGraph<N, E>::_dfs(const N& node, bool* visited, std::vector<N>* path) const {

        visited[this->_nodes.at(node)] = true;

        if (path)
            path->emplace_back(node);

        for (const auto& edge : this->_list.at(node))
            if (!visited[this->_nodes.at(edge.toNode)])
                this->_dfs(edge.toNode, visited, path);

    }

} // namespace ng