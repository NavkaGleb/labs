#include <iostream>
#include <algorithm>

#include <Array.hpp>
#include <MatrixGraph.hpp>
#include <ListGraph.hpp>

struct Vertex {

	int x;
	int y;

	bool operator<(const Vertex& other) const {
	    return x * y < other.x * other.y;
	}

	bool operator==(const Vertex& other) const {
	    return this->x == other.x && this->y == other.y;
	}

};

struct Edge {
    float value;
    std::string name;

    friend std::ostream& operator<<(std::ostream& ostream, const Edge& edge) {
        return ostream << edge.name << " -> " << edge.value;
    }
};

int main() {

	ng::ListGraph<Vertex> graph(false, true);

	Vertex a = { 0, 1 };
	Vertex b = { 2, 3 };
	Vertex c = { 4, 5 };
	Vertex d = { 6, 7 };

	graph.pushNode(a);
	graph.pushNode(b);
	graph.pushNode(c);
	graph.pushNode(d);

	graph.pushEdge(a, b, 3);
	graph.pushEdge(a, c, 7);
	graph.pushEdge(b, d, 8);

	std::function<int(const int&)> f = [](const int& e) { return e; };

	auto distance = graph.dijkstra(a, f);

	for (const auto& p : distance)
	    std::cout << "{ " << p.first.x << ", " << p.first.y << " } -> " << *p.second << std::endl;

}