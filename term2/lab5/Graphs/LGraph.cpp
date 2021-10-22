#include "LGraph.hpp"

#include <iostream>
#include <functional>
#include <algorithm>
#include <queue>
#include <stack>
#include <map>

#include "MGraph.hpp"
#include "../Random/rand.hpp"

/* -------------------------------------------- struct Edge -------------------------------------------- */

// constructor
ng::LGraph::Edge::Edge(int toNode, int weight) : toNode(toNode), weight(weight) {}

// operators
bool ng::LGraph::Edge::operator<(const Edge& other) const {

	return this->weight < other.weight;

}

bool ng::LGraph::Edge::operator>(const Edge& other) const {

	return this->weight > other.weight;

}

/* -------------------------------------------- class LGraph -------------------------------------------- */

// private methods
void ng::LGraph::_rfill() {

	static std::function<bool(const std::vector<Edge>&, const int&)> exist = [](
		const std::vector<Edge>& edges, const int& value) {

		for (const auto& edge : edges)
			if (edge.toNode == value)
				return true;

		return false;

	};

	int fnode;                                            // from node
	int tnode;                                            // to node
	int edges = 0;

	if (this->_directed && this->_edges > this->_nodes * (this->_nodes - 1))
		this->_edges = this->_nodes * (this->_nodes - 1);
	else if (!this->_directed && this->_edges > (this->_nodes * (this->_nodes - 1)) / 2)
		this->_edges = (this->_nodes * (this->_nodes - 1)) / 2;

	while (edges != this->_edges) {

		fnode = ng::rand::irand(0, this->_nodes - 1);
		tnode = ng::rand::irand(0, this->_nodes - 1);

		if (!exist(this->_list[fnode], tnode)) {

			this->_addEdge(fnode, tnode, this->_weighed ? ng::rand::irand(1, 100) : 1, false);
			edges++;

		}

	}

}

void ng::LGraph::_addEdge(const int& fnode, const int& tnode, const int& weight, const bool& increment) {

	if (increment)
		this->_edges++;

	this->_list[fnode].emplace_back(tnode, weight);

	if (!this->_directed && fnode != tnode)
		this->_list[tnode].emplace_back(fnode, weight);

}

void ng::LGraph::_dfs(int snode, bool* visited, std::vector<int>* path) const {

	visited[snode] = true;
	if (path) path->emplace_back(snode);

	for (const auto& edge : this->_list[snode])
		if (!visited[edge.toNode])
			this->_dfs(edge.toNode, visited, path);

}

// constructor / destructor
ng::LGraph::LGraph() : _type("ng::LGraph"), _nodes(0), _edges(0), _directed(false), _weighed(false) {}

ng::LGraph::LGraph(const ng::MGraph& mgraph)
	: _type("ng::LGraph"), _nodes(mgraph._nodes), _edges(mgraph._edges), _directed(mgraph._directed), _weighed(mgraph._weighed) {

	this->_list.resize(this->_nodes);

	for (int i = 0; i < this->_nodes; i++)
		for (int j = 0; j < this->_nodes; j++)
			if (mgraph._matrix[i][j])
				this->_list[i].emplace_back(j, *mgraph._matrix[i][j]);

}

ng::LGraph::LGraph(int nodes, int edges, bool directed, bool weighed)
	: _type("ng::LGraph"), _nodes(nodes), _edges(edges), _directed(directed), _weighed(weighed) {

	this->_list.resize(this->_nodes);

	if (edges != 0)
		this->_rfill();

}

// accessors
const char* ng::LGraph::type() const {

	return this->_type;

}

const int& ng::LGraph::nodes() const {

	return this->_nodes;

}

const int& ng::LGraph::edges() const {

	return this->_edges;

}

const bool& ng::LGraph::directed() const {

	return this->_directed;

}

const bool& ng::LGraph::weighed() const {

	return this->_weighed;

}

bool ng::LGraph::empty() const {

	return this->_nodes == 0;

}

bool ng::LGraph::connected() const {

	bool connected = true;
	auto* visited = new bool[this->_nodes];

	this->dfs(0, visited);

	for (int i = 0; connected && i < this->_nodes; i++)
		if (!visited[i])
			connected = false;

	delete [] visited;
	return connected;

}

bool ng::LGraph::cyclic() const {

	bool cyclic = false;
	std::vector<int> visited(this->_nodes, 0);

	std::function<bool(int)> _dfs = [&](int snode) {

		visited[snode] = 1;

		for (const auto& edge : this->_list[snode]) {

			if (visited[edge.toNode] == 0) {

				if (_dfs(edge.toNode))
					return true;

			} else if (visited[edge.toNode] == 1) {

				cyclic = true;
				return true;

			}

		}

		visited[snode] = 2;
		return false;

	};

	for (int i = 0; i < this->_nodes; i++)
		if (_dfs(i))
			return true;

	return cyclic;

}

std::vector<std::vector<int>> ng::LGraph::components() const {

	std::vector<std::vector<int>> components;

	if (!this->_directed) {

		std::vector<int> component;
		bool* visited = new bool[this->_nodes];
		std::fill_n(visited, this->_nodes, false);

		for (int i = 0; i < this->_nodes; i++) {

			if (!visited[i]) {

				component.clear();
				this->_dfs(i, visited, &component);
				components.emplace_back(component);

			}

		}

		delete [] visited;

	} else {

		// Tarjan's Strongly Connected Component Algorithm
		int id = 0;
		bool* onStack = new bool[this->_nodes]();
		int* ids = new int[this->_nodes]();
		int* low = new int[this->_nodes]();
		std::stack<int> stack;

		std::fill_n(ids, this->_nodes, -1);

		std::function<void(int)> _dfs = [&](int snode) {

			stack.emplace(snode);
			onStack[snode] = true;
			ids[snode] = id;
			low[snode] = id++;

			for (const auto& edge : this->_list[snode]) {

				if (ids[edge.toNode] == -1)
					_dfs(edge.toNode);

				if (onStack[edge.toNode])
					low[snode] = std::min(low[snode], low[edge.toNode]);

			}

			if (ids[snode] == low[snode]) {

				components.emplace_back(std::vector<int>(0));

				for (int node = stack.top(); ; node = stack.top()) {

					stack.pop();
					onStack[node] = false;
					components.back().emplace_back(node);

					if (node == snode) break;

				}

				std::reverse(components.back().begin(), components.back().end());

			}

		};

		for (int i = 0; i < this->_nodes; i++)
			if (ids[i] == -1)
				_dfs(i);

		delete [] onStack;
		delete [] ids;
		delete [] low;

	}

	return components;
}

// modifiers
void ng::LGraph::directed(const bool& directed) {

	this->_directed = directed;

}

void ng::LGraph::weighed(const bool& weighed) {

	this->_weighed = weighed;

}

// public methods
void ng::LGraph::clear() {

	this->_nodes = 0;
	this->_edges = 0;
	this->_directed = false;
	this->_weighed = false;

	for (auto& row : this->_list)
		row.clear(), row.shrink_to_fit();

	this->_list.clear();
	this->_list.shrink_to_fit();

}

void ng::LGraph::build(const ng::MGraph& mgraph) {

	this->_nodes = mgraph._nodes;
	this->_edges = mgraph._edges;
	this->_directed = mgraph._directed;
	this->_weighed = mgraph._weighed;

	this->_list.resize(this->_nodes);

	for (int i = 0; i < this->_nodes; i++)
		for (int j = 0; j < this->_nodes; j++)
			if (mgraph._matrix[i][j])
				this->_list[i].emplace_back(j, *mgraph._matrix[i][j]);

}

void ng::LGraph::addNode() {

	this->_nodes++;
	this->_list.emplace_back(std::vector<Edge>(0));

}

void ng::LGraph::addEdge(const int& fnode, const int& tnode, const int& weight) {

	this->_addEdge(fnode, tnode, weight);

}

void ng::LGraph::addEdge(const int& fnode, const int& tnode) {

	this->_addEdge(fnode, tnode, 1);

}

void ng::LGraph::eraseEdge(const int& fnode, const int& tnode) {

	this->_edges--;

	for (int i = 0; i < this->_list[fnode].size(); i++) {

		if (this->_list[fnode][i].toNode == tnode) {

			this->_list[fnode].erase(this->_list[fnode].begin() + i);
			break;

		}

	}

	if (!this->_directed) {

		for (int i = 0; i < this->_list[tnode].size(); i++) {

			if (this->_list[tnode][i].toNode == fnode) {

				this->_list[tnode].erase(this->_list[tnode].begin() + i);
				break;

			}

		}

	}

}

void ng::LGraph::eraseEdges() {

	this->_edges = 0;

	for (int i = 0; i < this->_nodes; i++)
		this->_list[i].clear();

}

void ng::LGraph::print() const {

	for (int i = 0; i < this->_list.size(); i++) {

		std::cout << i << ": ";

		for (const auto& edge : this->_list[i])
			std::cout << "(" << edge.toNode << ", " << edge.weight << ") ";
		std::cout << std::endl;

	}

}

void ng::LGraph::bfs(int snode, bool* visited) const {

	std::fill_n(visited, this->_nodes, false);

	std::queue<int> queue;

	visited[snode] = true;
	queue.push(snode);

	while (!queue.empty()) {

		snode = queue.front();
		queue.pop();

		for (const auto& edge : this->_list[snode])
			if (!visited[edge.toNode])
				visited[edge.toNode] = true, queue.push(edge.toNode);

	}

}

void ng::LGraph::bfs(int snode, std::vector<int>& distance) const {

	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);
	distance.resize(this->_nodes);

	std::queue<int> queue;

	visited[snode] = true;
	queue.push(snode);

	while (!queue.empty()) {

		snode = queue.front();
		queue.pop();

		for (const auto &edge : this->_list[snode]) {

			if (!visited[edge.toNode]) {

				visited[edge.toNode] = true;
				queue.push(edge.toNode);
				distance[edge.toNode] = distance[snode] + edge.weight;

			}

		}

	}

	delete [] visited;

}

std::vector<int> ng::LGraph::bfs(int snode) const {

	std::vector<int> distance;

	this->bfs(snode, distance);

	return distance;

}

void ng::LGraph::dfs(int snode, bool* visited) const {

	this->_dfs(snode, visited);

}

void ng::LGraph::dfs(int snode, std::vector<int>& path) const {

	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);

	this->_dfs(snode, visited, &path);
	delete [] visited;

}

std::vector<int> ng::LGraph::dfs(int snode) const {

	std::vector<int> path;
	this->dfs(snode, path);

	return path;

}

std::vector<int> ng::LGraph::dijkstra(int snode) const {

	std::vector<int> distance(this->_nodes, -1);
	std::priority_queue<std::pair<int, int>> pqueue;

	distance[snode] = 0;
	pqueue.emplace(0, snode);

	while (!pqueue.empty()) {

		snode = pqueue.top().second;
		pqueue.pop();

		for (const auto& edge : this->_list[snode]) {

			if (distance[edge.toNode] == -1 || distance[edge.toNode] > distance[snode] + edge.weight) {

				distance[edge.toNode] = distance[snode] + edge.weight;
				pqueue.emplace(-distance[edge.toNode], edge.toNode);

			}

		}

	}

	return distance;

}

std::vector<std::vector<int>> ng::LGraph::floyd() const {

	ng::MGraph mgraph(*this);

	return mgraph.floyd();

}

std::vector<int> ng::LGraph::kahn() const {

	if (!this->_directed || this->cyclic()) return {};

	int nodes = 0;
	int snode;
	std::vector<int> torder;
	std::vector<int> degree(this->_nodes, 0);
	std::queue<int> queue;

	// init degree
	for (int i = 0; i < this->_nodes; i++)
		for (const auto& edge : this->_list[i])
			degree[edge.toNode]++;

	// init queue
	for (int i = 0; i < this->_nodes; i++)
		if (degree[i] == 0)
			queue.emplace(i);

	while (!queue.empty()) {

		snode = queue.front();
		queue.pop();

		torder.emplace_back(snode);

		for (const auto& edge : this->_list[snode])
			if (--degree[edge.toNode] == 0)
				queue.emplace(edge.toNode);

		nodes++;

	}

	return (nodes == this->_nodes) ? torder : std::vector<int>(0);

}

std::vector<int> ng::LGraph::topologicalSort() const {

	if (!this->_directed || this->cyclic()) return {};

	std::vector<int> torder;
	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);

	std::function<void(int)> _dfs = [&](int snode) {

		visited[snode] = true;

		for (const auto& edge : this->_list[snode])
			if (!visited[edge.toNode])
				_dfs(edge.toNode);

		torder.emplace_back(snode);

	};

	for (int i = 0; i < this->_nodes; i++)
		if (!visited[i])
			_dfs(i);

	delete [] visited;

	std::reverse(torder.begin(), torder.end());
	return torder;

}

ng::SpanningTree* ng::LGraph::spanningTree(int snode) const {

	std::map<int, ng::SpanningTree::Node*> nodes = { { snode, new ng::SpanningTree::Node(snode) } };
	auto* tree = new ng::SpanningTree(nodes[snode]);
	bool* visited = new bool[this->_nodes]();

	std::function<void(int)> _dfs = [&](int snode) {

		visited[snode] = true;

		for (const auto& edge : this->_list[snode]) {

			if (!visited[edge.toNode]) {

				if (nodes.find(edge.toNode) == nodes.end())
					nodes[edge.toNode] = new ng::SpanningTree::Node(edge.toNode);

				tree->link(nodes[snode], nodes[edge.toNode], edge.weight);
				_dfs(edge.toNode);

			}

		}

	};

	_dfs(snode);

	return tree;

}

std::vector<ng::SpanningTree*> ng::LGraph::spanningForest() const {

	std::vector<ng::SpanningTree*> trees;
	std::vector<ng::SpanningTree::Node*> nodes(this->_nodes, nullptr);
	std::vector<std::vector<int>> components = this->components();

	for (int i = 0; i < this->_nodes; i++)
		nodes[i] = new ng::SpanningTree::Node(i);

	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);

	std::function<void(int, const bool&)> _dfs = [&](int snode, const bool& root) {

		visited[snode] = true;

		if (root)
			trees.emplace_back(new ng::SpanningTree(nodes[snode]));

		for (const auto& edge : this->_list[snode]) {

			if (!visited[edge.toNode]) {

				trees.back()->link(nodes[snode], nodes[edge.toNode], edge.weight);
				_dfs(edge.toNode, false);

			}

		}

	};

	for (const auto& component : components)
		for (const auto& node : component)
			if (!visited[node])
				_dfs(node, true);

	delete [] visited;

	return trees;

}

std::vector<ng::SpanningTree*> ng::LGraph::kruskal() const {

	if (this->_directed || !this->_weighed) return {};

	struct SpanEdge {
		// variables
		int weight;
		int fromNode;
		int toNode;

		// constructor
		SpanEdge() = default;
		SpanEdge(int weight, int fromNode, int toNode) : weight(weight), fromNode(fromNode), toNode(toNode) {}

	};

	std::vector<ng::SpanningTree*> trees;                                         // minimal spanning trees

	std::vector<std::vector<int>> components = this->components();                // components of graph
	std::vector<std::vector<SpanEdge>> cedges;                                    // component edges
	auto* tgraph = new LGraph(this->_nodes, 0, this->_directed, this->_weighed);  // temp graph
	int* parent = new int[this->_nodes];                                          // parent of node

	// parent
	for (int i = 0; i < this->_nodes; i++)
		parent[i] = i;

	// init cedges
	for (const auto& component : components) {

		cedges.emplace_back(std::vector<SpanEdge>(0));

		for (const auto& node : component)
			for (const auto& edge : this->_list[node])
				cedges.back().emplace_back(edge.weight, node, edge.toNode);

		std::sort(cedges.back().begin(), cedges.back().end(), [](const SpanEdge& sedge1, const SpanEdge& sedge2) {

			return sedge1.weight < sedge2.weight;

		});

	}

	std::function<int(int)> _findSet = [&](int snode) { while (parent[snode] != snode) snode = parent[snode]; return snode; };
	std::function<void(int, int)> _unionSet = [&](int u, int v) { parent[u] = parent[v]; };

	for (int i = 0; i < cedges.size(); i++) {

		tgraph->eraseEdges();

		for (const auto& edge : cedges[i]) {

			int pfrom = _findSet(edge.fromNode);                                  // parent from
			int pto = _findSet(edge.toNode);                                      // parent to

			if (pfrom != pto) {

				tgraph->addEdge(edge.fromNode, edge.toNode, edge.weight);
				_unionSet(pfrom, pto);

			}

		}

		trees.emplace_back(tgraph->spanningTree(components[i].front()));

	}

	delete tgraph;
	delete [] parent;

	return trees;

}

int ng::LGraph::edgesAmount() const {

	int nodes = 0;

	for (int i = 0; i < this->_nodes; ++i) {

		nodes += this->_list[i].size();

		for (const auto& edge : this->_list[i])
			if (edge.toNode == i)
				++nodes;

	}

	return nodes / 2;

}

std::vector<int> ng::LGraph::nodes(int currentNode) const {

	std::vector<int> result;
	bool* visited = new bool[this->_nodes]();

	std::function<void(int)> dfs = [&](int snode) {

		visited[snode] = true;

		std::cout << "snode = " << snode << std::endl;

		if (snode != currentNode)
			result.emplace_back(snode);

		for (const auto& edge : this->_list[snode]) {

			if (!visited[edge.toNode])
				dfs(edge.toNode);

		}

	};

	dfs(currentNode);

	delete [] visited;
	return result;

}