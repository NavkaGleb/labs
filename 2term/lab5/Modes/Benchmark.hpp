#pragma once

#include <map>
#include <string>
#include <vector>
#include <chrono>

#include "Mode.hpp"
#include "../Graphs/Graph.hpp"

class Benchmark : public Mode {
private:
	// inner classes
	class Timer {
	public:
		// variables
		std::vector<std::pair<std::string, std::vector<double>>>& data;
		int index;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;

		// constructor / destructor
		Timer(std::vector<std::pair<std::string, std::vector<double>>>& data, const int& index);
		virtual ~Timer();

	};

	// variables
	int _n;
	std::vector<std::pair<std::string, std::vector<double>>> _data;
	std::map<std::string, ng::Graph*> _graphs;

	// private methods
	void _graphRun(ng::Graph* graph, const int& index);
	void _run(const bool& directed, const bool& weighed, const int& edges);

	void _update();
	void _reset();
	void _save(const char* filepath);

public:
	// constructor / destructor
	Benchmark();
	~Benchmark() override;

	// public methods
	void run() override;

}; // class Benchmark