#include "ConsoleInterface.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>

#include "../Graphs/MGraph.hpp"
#include "../Graphs/LGraph.hpp"

// private methods
void ConsoleInterface::_initGraphCommands() {

	std::ifstream infile("../Files/graphCommands.txt");
	std::string current;

	if (!infile.is_open())
		throw std::invalid_argument("failed to open the file | ConsoleInterface");

	while (std::getline(infile, current))
		this->_gcommands.emplace_back(current);

	infile.close();

}

void ConsoleInterface::_initDemoCommands() {

	std::ifstream infile("../Files/demoCommands.txt");
	std::string commands;

	if (!infile.is_open())
		throw std::invalid_argument("failed to open the file | ConsoleInterface");

	std::getline(infile, commands);
	infile.close();

	this->_dcommands.str(commands);

}

template <typename T>
void ConsoleInterface::_input(T& value, const bool& nextLine) {

	if (this->_type == TYPE::INTERACTIVE) {

		std::cin >> value;

	} else if (this->_type == TYPE::DEMO) {

		this->_dcommands >> value;

		std::cout << " " << value << std::endl;
		std::cout << "..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	}

	if (nextLine)
		std::cout << std::endl;

}

void ConsoleInterface::_input(std::string& value, const bool& nextLine) {


	if (this->_type == TYPE::INTERACTIVE) {

		std::cin >> value;

	} else if (this->_type == TYPE::DEMO) {

		this->_dcommands >> value;

		std::cout << " ";
		for (const auto& ch : value)
			std::this_thread::sleep_for(std::chrono::milliseconds(300)), std::cout << ch;
		std::cout << "\n..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	}

	if (nextLine)
		std::cout << std::endl;

}

void ConsoleInterface::_selectGraph() {

	if (this->_graphs.empty()) {

		std::cout << "there are no graphs. you must add them first" << std::endl;
		return;

	}

	std::string name;

	std::cout << "select the graph" << std::endl;
	for (const auto& p : this->_graphs)
		std::cout << p.first << " -> " << p.second->type()
		          << " (nodes: " << p.second->nodes()
		          << ", edges: " << p.second->edges()
		          << ", directed: " << p.second->directed()
		          << ", weighed: " << p.second->weighed()
		          << ")" << std::endl;

	std::cout << "name:";
	this->_input(name);

	this->_runGraph(this->_graphs[name]);

}

void ConsoleInterface::_addGraph() {

	std::string name;
	short type;
	int nodes;
	int edges;
	char response;
	bool directed;
	bool weighed;

	std::cout << "enter graph name" << std::endl;
	std::cout << "name:";
	this->_input(name, false);

	std::cout << "select the type" << "\n"
	          << "0 - adjacency matrix graph (ng::MGraph)" << "\n"
	          << "1 - adjacency structure graph (ng::LGraph)" << std::endl;
	std::cout << "type:";
	this->_input(type, false);

	std::cout << "enter amount of nodes" << std::endl;
	std::cout << "nodes:";
	this->_input(nodes, false);

	std::cout << "enter amount of edges" << std::endl;
	std::cout << "edges:";
	this->_input(edges, false);

	std::cout << "directed graph or not" << std::endl;
	std::cout << "[y/n]:";
	this->_input(response, false);
	directed = response == 'y';

	std::cout << "weighed graph of not" << std::endl;
	std::cout << "[y/n]:";
	this->_input(response, false);
	weighed = response == 'y';

	if (type == 0)
		this->_graphs[name] = new ng::MGraph(nodes, edges, directed, weighed);
	else if (type == 1)
		this->_graphs[name] = new ng::LGraph(nodes, edges, directed, weighed);
	else
		std::cout << "you entered not existing type. try again" << std::endl;

	std::cout << "graph " << name << " successfully was added" << std::endl;

}

void ConsoleInterface::_deleteGraph() {

	if (this->_graphs.empty()) {

		std::cout << "there are no graphs" << std::endl;
		return;

	}

	std::string name;

	std::cout << "select the graph" << std::endl;
	for (const auto& p : this->_graphs)
		std::cout << p.first << " -> " << p.second->type()
				  << " (nodes: " << p.second->nodes()
				  << ", edges: " << p.second->edges()
				  << ", directed: " << p.second->directed()
				  << ", weighed: " << p.second->weighed()
				  << ")" << std::endl;

	std::cout << "name:";
	this->_input(name);

	delete this->_graphs[name];
	std::cout << "graph " << name << " was successfully deleted" << std::endl;

}

void ConsoleInterface::_runGraph(ng::Graph* graph) {

	short command;

	while (true) {

		std::cout << "select the command" << std::endl;
		std::for_each(this->_gcommands.begin(), this->_gcommands.end(), [](const std::string& c) { std::cout << c << std::endl; });
		std::cout << "command:";
		this->_input(command);

		if (command == 0) {

			std::cout << "amount of nodes: " << graph->nodes() << std::endl;

		} else if (command == 1) {

			std::cout << "amount of edges: " << graph->edges() << std::endl;

		} else if (command == 2) {

			std::cout << "graph is empty: " << graph->empty() << std::endl;

		} else if (command == 3) {

			std::cout << "graph is connected: " << graph->connected() << std::endl;

		} else if (command == 4) {

			std::cout << "graph is cyclic: " << graph->cyclic() << std::endl;

		} else if (command == 5) {

			std::vector<std::vector<int>> components = graph->components();

			std::cout << "components:" << std::endl;
			for (int i = 0; i < components.size(); i++) {

				std::cout << i + 1 << ". ";
				std::for_each(components[i].begin(), components[i].end(), [](const int& n) { std::cout << n << " "; });
				std::cout << std::endl;

			}

		} else if (command == 6) {

			graph->addNode();
			std::cout << "new node " << graph->nodes() - 1 << " was successfully added to the graph" << std::endl;

		} else if (command == 7) {

			int fnode;
			int tnode;
			int weight = 1;

			std::cout << "node range: ";
			for (int i = 0; i < graph->nodes(); i++) std::cout << i << " ";
			std::cout << std::endl;

			std::cout << "enter node number from which the edge will start" << std::endl;
			std::cout << "fromNode:";
			this->_input(fnode);

			if (fnode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			std::cout << "enter node number to which the edge will come" << std::endl;
			std::cout << "toNode:";
			this->_input(tnode);

			if (tnode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			if (graph->weighed()) {

				std::cout << "enter the edge weight" << std::endl;
				std::cout << "weight:";
				this->_input(weight);

			}

			graph->addEdge(fnode, tnode, weight);
			std::cout << "new edge { " << fnode << ", " << tnode << ", " << weight
			          << " } was successfully added to the graph" << std::endl;

		} else if (command == 8) {

			int fnode;
			int tnode;

			std::cout << "enter node number from which the edge will start" << std::endl;
			std::cout << "fromNode:";
			this->_input(fnode);

			if (fnode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			std::cout << "enter node number to which the edge will come" << std::endl;
			std::cout << "toNode:";
			this->_input(tnode);

			if (tnode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			graph->eraseEdge(fnode, tnode);
			std::cout << "edge { " << fnode << ", " << tnode << " } was successfully erased from the graph" << std::endl;

		} else if (command == 9) {

			graph->eraseEdges();
			std::cout << "all edges was successfully erased from the graph" << std::endl;

		} else if (command == 10) {

			graph->print();

		} else if (command == 11) {

			int snode;
			std::vector<int> distance;

			std::cout << "select the start node of bfs:" << std::endl;
			for (int i = 0; i < graph->nodes(); i++) std::cout << i << " ";
			std::cout << std::endl << "startNode:";
			this->_input(snode);

			if (snode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			graph->bfs(snode, distance);

			for (int i = 0; i < distance.size(); i++)
				std::cout << i << " -> " << distance[i] << "  ";
			std::cout << std::endl;

		} else if (command == 12) {

			int snode;
			std::vector<int> path;

			std::cout << "select the start node of dfs" << std::endl;
			for (int i = 0; i < graph->nodes(); i++) std::cout << i << " ";
			std::cout << std::endl << "startNode:";
			this->_input(snode);

			if (snode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			graph->dfs(snode, path);

			for (const int& node : path)
				std::cout << node << " ";
			std::cout << std::endl;

		} else if (command == 13) {

			int snode;
			std::vector<int> distance;

			std::cout << "select the start node of dijkstra" << std::endl;
			for (int i = 0; i < graph->nodes(); i++) std::cout << i << " ";
			std::cout << std::endl << "startNode:";
			this->_input(snode);

			if (snode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			distance = graph->dijkstra(snode);

			for (int i = 0; i < distance.size(); i++)
				std::cout << i << " -> " << distance[i] << "  ";
			std::cout << std::endl;

		} else if (command == 14) {

			std::vector<std::vector<int>> distance = graph->floyd();

			for (int i = 0; i < distance.size(); i++) {

				std::cout << i << ". ";
				for (const int& d : distance[i]) {

					if (d == INT_MAX) std::cout << -1 << "\t";
					else std::cout << d << "\t";

				}
				std::cout << std::endl;

			}

		} else if (command == 15) {

			std::vector<int> tsort = graph->kahn();

			if (tsort.empty()) {

				std::cout << "graph is not directed or has a cycle" << std::endl;

			} else {

				std::cout << "kahn sort:" << std::endl;
				for (int i = 0; i < tsort.size(); i++) std::cout << i << " -> " << tsort[i] << std::endl;

			}

		} else if (command == 16) {

			std::vector<int> tsort = graph->topologicalSort();

			if (tsort.empty()) {

				std::cout << "graph is not directed or has a cycle" << std::endl;

			} else {

				std::cout << "topological sort:" << std::endl;
				for (int i = 0; i < tsort.size(); i++) std::cout << i << " -> " << tsort[i] << std::endl;

			}

		} else if (command == 17) {

			int snode;
			ng::SpanningTree* tree;

			std::cout << "select the start node of dijkstra" << std::endl;
			for (int i = 0; i < graph->nodes(); i++) std::cout << i << " ";
			std::cout << std::endl << "startNode:";
			this->_input(snode);

			if (snode > graph->nodes() - 1) {

				std::cout << "you are out of node range" << std::endl;
				continue;

			}

			tree = graph->spanningTree(snode);
			std::cout << "spanning dfs tree from " << snode << " node:" << std::endl;
			tree->print();

			delete tree;

		} else if (command == 18) {

			std::vector<ng::SpanningTree*> forest = graph->spanningForest();

			std::cout << "spanning forest:" << std::endl;
			for (auto& tree : forest) {

				tree->print();
				delete tree;

			}

		} else if (command == 19) {

			std::vector<ng::SpanningTree*> mforest = graph->kruskal();

			if (mforest.empty()) {

				std::cout << "the graph is not weighed" << std::endl;

			} else {

				std::cout << "minimal spanning forest:" << std::endl;
				for (auto& tree : mforest) {

					tree->print();
					delete tree;

				}

			}

		} else if (command == 20) {

			break;

		} else {

			std::cout << "you entered not existing command. try again" << std::endl;

		}

		if (this->_type == TYPE::DEMO)
			std::cout << "......." << std::endl, std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		std::cout << std::endl;

	}

}

// constructor / destructor
ConsoleInterface::ConsoleInterface(short type) : _type(type) {

	this->_initGraphCommands();

	if (this->_type == TYPE::DEMO)
		this->_initDemoCommands();

}

ConsoleInterface::~ConsoleInterface() {

	// delete graphs
	for (auto& p : this->_graphs)
		delete p.second;

}

// public methods
void ConsoleInterface::run() {

	short command;

	while (true) {

		std::cout << "select the command" << "\n"
		          << "0 - select graph" << "\n"
		          << "1 - add new graph" << "\n"
		          << "2 - delete graph" << "\n"
		          << "3 - exit" << std::endl;
		std::cout << "command:";
		this->_input(command);

		if (command == 0)
			this->_selectGraph();
		else if (command == 1)
			this->_addGraph();
		else if (command == 2)
			this->_deleteGraph();
		else if (command == 3)
			break;
		else
			std::cout << "you entered not existing command. try again" << std::endl;

		std::cout << std::endl;

	}

}