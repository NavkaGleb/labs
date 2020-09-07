#include <iostream>
#include <algorithm>

#include <Array.hpp>
#include <MatrixGraph.hpp>

struct Vertex {

	int x;
	int y;

	bool operator<(const Vertex& other) const {
	    return x * y < other.x * other.y;
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

	ng::MatrixGraph<Vertex, float> graph(false, true);

	Vertex a = { 0, 1 };
	Vertex b = { 2, 3 };
	Vertex c = { 4, 5 };
	Vertex d = { 6, 7 };

	graph.pushNode(a);
	graph.pushNode(b);
	graph.pushNode(c);
	graph.pushNode(d);

	graph.pushEdge(a, b, 3.541f);
	graph.pushEdge(a, c, 7.542f);
	graph.pushEdge(b, d, 8.5f);

	graph.print();

	std::function<float(const Edge&)> func = [](const Edge& edge) { return edge.value; };

    std::map<Vertex, float> distance = graph.bfs<float>(a);

	std::cout << "distance\n";
	for (const auto& p : distance)
        std::cout << "{ " << p.first.x << ", " << p.first.y << " } -> " << p.second << std::endl;


    std::cout << "-----" << std::endl;
	std::vector<Vertex> path;
	graph.dfs(a, path);

	for (const auto& v : path)
	    std::cout << "{ " << v.x << ", " << v.y << " }" << " ";

}