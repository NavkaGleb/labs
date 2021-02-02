#include "MGraph.hpp"

#include <iostream>
#include <algorithm>
#include <queue>
#include <functional>
#include <stack>
#include <map>

#include "LGraph.hpp"
#include "../Random/rand.hpp"

// private methods
void ng::MGraph::_rfill() {

	int fnode;                                                                 // from node
	int tnode;                                                                 // to node
	int edges = 0;

	if (this->_directed && this->_edges > this->_nodes * (this->_nodes - 1))
		this->_edges = this->_nodes * (this->_nodes - 1);
	else if (!this->_directed && this->_edges > (this->_nodes * (this->_nodes - 1)) / 2)
		this->_edges = (this->_nodes * (this->_nodes - 1)) / 2;

	while (edges != this->_edges) {

		fnode = ng::rand::irand(0, this->_nodes - 1);
		tnode = ng::rand::irand(0, this->_nodes - 1);

		if (!this->_matrix[fnode][tnode]) {

			this->_addEdge(fnode, tnode, this->_weighed ? ng::rand::irand(1, 100) : 1, false);
			edges++;

		}

	}

}

void ng::MGraph::_addEdge(const int& fnode, const int& tnode, const int& weight, const bool& increment) {

	if (increment)
		this->_edges++;

	this->_matrix[fnode][tnode] = new int(weight);

	if (!this->_directed)
		this->_matrix[tnode][fnode] = new int(weight);

}

void ng::MGraph::_dfs(int snode, bool* visited, std::vector<int>* path) const {

	visited[snode] = true;
	if (path) path->emplace_back(snode);

	for (int i = 0; i < this->_nodes; i++)
		if (this->_matrix[snode][i] && !visited[i])
			this->_dfs(i, visited, path);

}

// constructor / destructor
ng::MGraph::MGraph() : _type("ng::MGraph"), _nodes(0), _edges(0), _directed(false), _weighed(false) {}

ng::MGraph::MGraph(const ng::LGraph& lgraph)
	: _type("ng::MGraph"), _nodes(lgraph._nodes), _edges(lgraph._edges), _directed(lgraph._directed), _weighed(lgraph._weighed) {

	this->_matrix.resize(this->_nodes);

	for (auto& row : this->_matrix)
		row.resize(this->_nodes, nullptr);

	for (int i = 0; i < this->_nodes; i++)
		for (const auto& edge : lgraph._list[i])
			this->_matrix[i][edge.toNode] = new int(edge.weight);

}

ng::MGraph::MGraph(int nodes, int edges, bool directed, bool weighed)
	: _type("ng::MGraph"), _nodes(nodes), _edges(edges), _directed(directed), _weighed(weighed) {

	this->_matrix.resize(this->_nodes);

	for (auto& a : this->_matrix)
		a.resize(this->_nodes, nullptr);

	if (this->_edges != 0)
		this->_rfill();

}

ng::MGraph::~MGraph() {

	for (auto& row : this->_matrix)
		for (auto& e : row)
			delete e;

}

// accessors
const char* ng::MGraph::type() const {

	return this->_type;

}

const int& ng::MGraph::nodes() const {

	return this->_nodes;

}

const int& ng::MGraph::edges() const {

	return this->_edges;

}

const bool& ng::MGraph::directed() const {

	return this->_directed;

}

const bool& ng::MGraph::weighed() const {

	return this->_weighed;

}

bool ng::MGraph::empty() const {

	return this->_nodes == 0;

}

bool ng::MGraph::connected() const {

	bool connected = true;
	auto* visited = new bool[this->_nodes];

	this->bfs(0, visited);

	for (int i = 0; connected && i < this->_nodes; i++)
		if (!visited[i])
			connected = false;

	delete [] visited;
	return connected;

}

bool ng::MGraph::cyclic() const {

	bool cyclic = false;
	std::vector<int> visited(this->_nodes, 0);

	std::function<bool(int)> _dfs = [&](int snode) {

		visited[snode] = 1;

		for (int i = 0; i < this->_nodes; i++) {

			if (this->_matrix[snode][i] && visited[i] == 0) {

				if (_dfs(i))
					return true;

			} else if (this->_matrix[snode][i] && visited[i] == 1) {

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

std::vector<std::vector<int>> ng::MGraph::components() const {

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

			for (int i = 0; i < this->_nodes; i++) {

				if (this->_matrix[snode][i] && ids[i] == -1)
					_dfs(i);

				if (this->_matrix[snode][i] && onStack[i])
					low[snode] = std::min(low[snode], low[i]);

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
void ng::MGraph::directed(const bool& directed) {

	this->_directed = directed;

}

void ng::MGraph::weighed(const bool& weighed) {

	this->_weighed = weighed;

}

// public methods
void ng::MGraph::clear() {

	this->_nodes = 0;
	this->_edges = 0;
	this->_directed = false;
	this->_weighed = false;

	for (auto& row : this->_matrix)
		row.clear(), row.shrink_to_fit();

	this->_matrix.clear();
	this->_matrix.shrink_to_fit();

}

void ng::MGraph::build(const ng::LGraph& lgraph) {

	this->_nodes = lgraph._nodes;
	this->_edges = lgraph._edges;
	this->_directed = lgraph._directed;
	this->_weighed = lgraph._weighed;

	this->_matrix.resize(this->_nodes);

	for (auto& row : this->_matrix)
		row.resize(5, nullptr);

	for (int i = 0; i < this->_nodes; i++)
		for (const auto& edge : lgraph._list[i])
			this->_matrix[i][edge.toNode] = new int(edge.weight);

}

void ng::MGraph::addNode() {

	for (auto& a : this->_matrix) a.emplace_back(nullptr);
	this->_matrix.emplace_back(std::vector<int*>(++this->_nodes, nullptr));

}

void ng::MGraph::addEdge(const int& fnode, const int& tnode, const int& weight) {

	this->_addEdge(fnode, tnode, weight);

}

void ng::MGraph::addEdge(const int& fnode, const int& tnode) {

	this->_addEdge(fnode, tnode, 1);

}

void ng::MGraph::eraseEdge(const int& fnode, const int& tnode) {

	this->_edges--;

	delete this->_matrix[fnode][tnode];
	this->_matrix[fnode][tnode] = nullptr;

	if (!this->_directed) {

		delete this->_matrix[tnode][fnode];
		this->_matrix[tnode][fnode] = nullptr;

	}

}

void ng::MGraph::eraseEdges() {

	this->_edges = 0;

	for (int i = 0; i < this->_nodes; i++)
		for (int j = 0; j < this->_nodes; j++)
			delete this->_matrix[i][j], this->_matrix[i][j] = nullptr;

}

void ng::MGraph::print() const {

	std::cout << "\t";
	for (int i = 0; i < this->_nodes; i++) std::cout << i << "\t";
	std::cout << std::endl;

	for (int i = 0; i < this->_nodes; i++) {

		std::cout << i << "\t";

		for (const auto& edge : this->_matrix[i]) {

			if (edge) std::cout << *edge;
			else std::cout << "n";
			std::cout << "\t";

		}

		std::cout << std::endl;

	}

}

void ng::MGraph::bfs(int snode, std::vector<int>& distance) const {

	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);
	distance.resize(this->_nodes);

	std::queue<int> queue;

	visited[snode] = true;
	queue.emplace(snode);

	while (!queue.empty()) {

		snode = queue.front();
		queue.pop();

		for (int i = 0; i < this->_nodes; i++) {

			if (this->_matrix[snode][i] && !visited[i]) {

				visited[i] = true;
				queue.emplace(i);
				distance[i] = distance[snode] + *this->_matrix[snode][i];

			}

		}

	}

	delete [] visited;

}

void ng::MGraph::bfs(int snode, bool* visited) const {

	std::fill_n(visited, this->_nodes, false);

	std::queue<int> queue;

	visited[snode] = true;
	queue.emplace(snode);

	while (!queue.empty()) {

		snode = queue.front();
		queue.pop();

		for (int i = 0; i < this->_nodes; i++)
			if (this->_matrix[snode][i] && !visited[i])
				visited[i] = true, queue.emplace(i);

	}

}

std::vector<int> ng::MGraph::bfs(int snode) const {

	std::vector<int> distance;

	this->bfs(snode, distance);

	return distance;

}

void ng::MGraph::dfs(int snode, std::vector<int>& path) const {

	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);

	this->_dfs(snode, visited, &path);
	delete [] visited;

}

void ng::MGraph::dfs(int snode, bool* visited) const {

	this->_dfs(snode, visited);

}

std::vector<int> ng::MGraph::dfs(int snode) const {

	std::vector<int> path;
	this->dfs(snode, path);

	return path;

}

std::vector<int> ng::MGraph::dijkstra(int snode) const {

	std::vector<int> distance(this->_nodes, -1);
	std::priority_queue<std::pair<int, int>> pqueue;

	distance[snode] = 0;
	pqueue.emplace(0, snode);

	while (!pqueue.empty()) {

		snode = pqueue.top().second;
		pqueue.pop();

		for (int i = 0; i < this->_nodes; i++) {

			if (this->_matrix[snode][i] && (distance[i] == -1 || distance[i] > distance[snode] + *this->_matrix[snode][i])) {

				distance[i] = distance[snode] + *this->_matrix[snode][i];
				pqueue.emplace(-distance[i], i);

			}

		}

	}

	return distance;

}

std::vector<std::vector<int>> ng::MGraph::floyd() const {

	if (!this->_weighed) return {};

	static const int infinity = INT_MAX;

	std::vector<std::vector<int>> distance(this->_nodes, std::vector<int>(this->_nodes, 0));

	// init distance
	for (int i = 0; i < this->_nodes; i++)
		for (int j = 0; j < this->_nodes; j++)
			distance[i][j] = (i == j) ? 0 : this->_matrix[i][j] ? *this->_matrix[i][j] : infinity;

	// algorithm
	for (int k = 0; k < this->_nodes; k++)
		for (int i = 0; i < this->_nodes; i++)
			for (int j = 0; j < this->_nodes; j++)
				if (distance[i][k] != infinity && distance[k][j] != infinity)
					distance[i][j] = std::min(distance[i][j], distance[i][k] + distance[k][j]);
//
//	for (int i = 0; i < this->_nodes; i++)
//		for (int j = 0; j < this->_nodes; j++)
//			if (distance[i][j] == infinity)
//				distance[i][j] = -1;

	return distance;

}

std::vector<int> ng::MGraph::kahn() const {

	if (!this->_directed || this->cyclic()) return {};

	int nodes = 0;
	int snode;
	std::vector<int> torder;
	std::vector<int> degree(this->_nodes, 0);
	std::queue<int> queue;

	// init degree
	for (int i = 0; i < this->_nodes; i++)
		for (int j = 0; j < this->_nodes; j++)
			if (this->_matrix[i][j])
				degree[j]++;

	// init queue
	for (int i = 0; i < this->_nodes; i++)
		if (degree[i] == 0)
			queue.emplace(i);

	// algorithm
	while (!queue.empty()) {

		snode = queue.front();
		queue.pop();

		torder.emplace_back(snode);

		for (int i = 0; i < this->_nodes; i++)
			if (this->_matrix[snode][i])
				if (--degree[i] == 0)
					queue.emplace(i);

		nodes++;

	}

	return (nodes == this->_nodes) ? torder : std::vector<int>(0);

}

std::vector<int> ng::MGraph::topologicalSort() const {

	if (!this->_directed || this->cyclic()) return {};

	std::vector<int> torder;
	bool* visited = new bool[this->_nodes];
	std::fill_n(visited, this->_nodes, false);

	std::function<void(int)> _dfs = [&](int snode) {

		visited[snode] = true;

		for (int i = 0; i < this->_nodes; i++)
			if (this->_matrix[snode][i] && !visited[i])
				_dfs(i);

		torder.emplace_back(snode);

	};

	for (int i = 0; i < this->_nodes; i++)
		if (!visited[i])
			_dfs(i);

	std::reverse(torder.begin(), torder.end());
	return torder;

}

ng::SpanningTree* ng::MGraph::spanningTree(int snode) const {

	std::map<int, ng::SpanningTree::Node*> nodes = { { snode, new ng::SpanningTree::Node(snode) } };
	auto* tree = new ng::SpanningTree(nodes[snode]);
	bool* visited = new bool[this->_nodes]();

	std::function<void(int)> _dfs = [&](int snode) {

		visited[snode] = true;

		for (int i = 0; i < this->_nodes; i++) {

			if (this->_matrix[snode][i] && !visited[i]) {

				if (nodes.find(i) == nodes.end())
					nodes[i] = new ng::SpanningTree::Node(i);

				tree->link(nodes[snode], nodes[i], *this->_matrix[snode][i]);
				_dfs(i);

			}

		}

	};

	_dfs(snode);

	return tree;

}

std::vector<ng::SpanningTree*> ng::MGraph::spanningForest() const {

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

		for (int i = 0; i < this->_nodes; i++) {

			if (this->_matrix[snode][i] && !visited[i]) {

				trees.back()->link(nodes[snode], nodes[i], *this->_matrix[snode][i]);
				_dfs(i, false);

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

std::vector<ng::SpanningTree*> ng::MGraph::kruskal() const {

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
	auto* tgraph = new MGraph(this->_nodes, 0, this->_directed, this->_weighed);  // temp graph
	int* parent = new int[this->_nodes];                                          // parent of node

	// parent
	for (int i = 0; i < this->_nodes; i++)
		parent[i] = i;

	// init cedges
	for (const auto& component : components) {

		cedges.emplace_back(std::vector<SpanEdge>(0));

		for (const auto& node : component)
			for (int i = 0; i < this->_nodes; i++)
				if (this->_matrix[node][i])
					cedges.back().emplace_back(*this->_matrix[node][i], node, i);

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