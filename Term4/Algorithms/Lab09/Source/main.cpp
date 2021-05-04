#include "ListGraph.hpp"
#include "FibonacciHeap.hpp"

#include "GraphAlgorithm.hpp"

int main() {
    Ng::ListGraph<int, int> graph(true, true);
    Ng::FibonacciHeap<int> heap;

    graph.PushNode(1);
    graph.PushNode(2);
    graph.PushNode(3);
    graph.PushNode(4);

    graph.PushEdge(1, 2, -2);
    graph.PushEdge(1, 3,  7);
    graph.PushEdge(1, 4,  5);
    graph.PushEdge(2, 3,  8);
    graph.PushEdge(2, 4,  6);
    graph.PushEdge(3, 2,  3);
    graph.PushEdge(3, 4, -4);
    graph.PushEdge(4, 1, -1);

    graph.Print();

    std::cout << "NodeCount: " << graph.GetNodeCount() << std::endl;
    std::cout << "EdgeCount: " << graph.GetEdgeCount() << std::endl;

    auto newGraph = Ng::ListGraph<int, int>(Ng::GraphAlgorithm::JohnsonAlgorithm(graph));

    newGraph.Print();

    std::cout << "NodeCount: " << newGraph.GetNodeCount() << std::endl;
    std::cout << "EdgeCount: " << newGraph.GetEdgeCount() << std::endl;
}
