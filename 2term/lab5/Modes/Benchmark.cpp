#include "Benchmark.hpp"

#include <fstream>

#include "../TextTable/TextTable.hpp"
#include "../Graphs/MGraph.hpp"
#include "../Graphs/LGraph.hpp"

/* -------------------------------------------- class Timer -------------------------------------------- */
// constructor / destructor
Benchmark::Timer::Timer(std::vector<std::pair<std::string, std::vector<double>>>& data, const int& index)
	: data(data), index(index) {

	this->startTimePoint = std::chrono::high_resolution_clock::now();

}

Benchmark::Timer::~Timer() {

	std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(this->startTimePoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimePoint).time_since_epoch().count();

	this->data[this->index].second.emplace_back(static_cast<double>(end - start) * 0.001);

}

// private methods
void Benchmark::_graphRun(ng::Graph* graph, const int& index) {

	if (index % 2 == 0)
		std::cout << "matrix" << std::endl;
	else
		std::cout << "list" << std::endl;

	std::cout << "--nodes = " << graph->nodes() << ", edges = " << graph->edges() << std::endl;
	std::cout << "connected = " << graph->connected() << std::endl;

	{
		Timer timer(this->_data, index);
		std::vector<int> path = graph->dfs(0);
	}

	{
		Timer timer(this->_data, index);
		std::vector<int> distance = graph->bfs(0);
	}

	{
		Timer timer(this->_data, index);
		std::vector<int> distance = graph->dijkstra(0);
	}

	{
		Timer timer(this->_data, index);
		std::vector<std::vector<int>> components = graph->components();
	}

	{
		Timer timer(this->_data, index);
		bool cyclic = graph->cyclic();
	}

	{
		Timer timer(this->_data, index);
		bool connected = graph->connected();
	}

	{
		Timer timer(this->_data, index);
		std::vector<int> order = graph->kahn();
	}

	{
		Timer timer(this->_data, index);
		std::vector<int> order = graph->topologicalSort();
	}

	{
		Timer timer(this->_data, index);
		std::vector<ng::SpanningTree*> forest = graph->spanningForest();
	}

	{
		Timer timer(this->_data, index);
		std::vector<ng::SpanningTree*> forest = graph->kruskal();
	}


}

void Benchmark::_run(const bool& directed, const bool& weighed, const int& edges) {

	this->_graphs["matrix"] = new ng::MGraph(this->_n, edges, directed, weighed);
	this->_graphs["list"] = new ng::LGraph(this->_n, edges, directed, weighed);

	this->_data.emplace_back("matrix " + std::to_string(this->_n) + "/" + std::to_string(edges), std::vector<double>(0));
	this->_data.emplace_back("list " + std::to_string(this->_n) + "/" + std::to_string(edges), std::vector<double>(0));

	this->_graphRun(this->_graphs["matrix"], static_cast<int>(this->_data.size() - 2));
	this->_graphRun(this->_graphs["list"], static_cast<int>(this->_data.size() - 1));

	delete this->_graphs["matrix"];
	delete this->_graphs["list"];

}

void Benchmark::_update() {

	this->_n *= 2;

}

void Benchmark::_reset() {

	this->_n = 64;
	this->_data.clear();

}

void Benchmark::_save(const char* filepath) {

	ng::TextTable ttable;
	std::ofstream outfile(filepath);

	if (!outfile.is_open())
		throw std::invalid_argument("failed to open the file | Benchmark");

	ttable.add("");
	ttable.add("dfs");
	ttable.add("bfs");
	ttable.add("components");
	ttable.add("cyclic");
	ttable.add("connected");
	ttable.add("dijkstra");
	ttable.add("kahn");
	ttable.add("topological sort");
	ttable.add("spanning forest");
	ttable.add("kruskal");
	ttable.add("");
	ttable.endOfRow();

	for (const auto& p : this->_data) {

		ttable.add(p.first);

		for (const auto& measurement : p.second)
			ttable.add(ng::to_string(measurement));

		ttable.add(p.first);
		ttable.endOfRow();

	}

	ttable.print(outfile);
	outfile.close();

}

// constructor / destructor
Benchmark::Benchmark() : _n(64) {}

Benchmark::~Benchmark() {

	// delete graphs
	for (const auto& p : this->_graphs)
		delete p.second;

}

// public methods
void Benchmark::run() {

	std::cout << "benching data..." << std::endl;

	// not directed && weighed graphs
	for (int i = 0; i < 6; i++) {

		this->_run(false, true, this->_n);
		this->_run(false, true, (this->_n * (this->_n - 1)) / 4);
		this->_run(false, true, (this->_n * (this->_n - 1)) / 2);

		this->_update();

	}

	this->_save("../Files/ndirectedWeighedBenchmark.txt");
	this->_reset();

	// directed && weighed graphs
	for (int i = 0; i < 6; i++) {

		this->_run(true, true, this->_n);
		this->_run(true, true, (this->_n * (this->_n - 1)) / 4);
		this->_run(true, true, (this->_n * (this->_n - 1)) / 2);

		this->_update();

	}

	this->_save("../Files/directedWeighedBenchmark.txt");
	this->_reset();

	std::cout << "end of benchmark" << std::endl;

}