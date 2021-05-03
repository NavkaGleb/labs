#include "ListGraph.hpp"

int main() {
    Ng::ListGraph<int, int> graph;

    graph.PushNode(0);
    graph.PushNode(1);
    graph.PushNode(2);
    graph.PushNode(3);
    graph.PushNode(4);

    graph.PushEdge(0, 1, 8);
    graph.PushEdge(1, 4, 5);

    graph.Print();

    std::cout << "NodeCount: " << graph.GetNodeCount() << std::endl;
    std::cout << "EdgeCount: " << graph.GetEdgeCount() << std::endl;

    graph.PopNode(1);

    graph.Print();
}
