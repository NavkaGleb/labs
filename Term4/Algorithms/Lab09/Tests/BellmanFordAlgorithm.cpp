#include <string>

#include <doctest/doctest.h>

#include "GraphAlgorithm.hpp"

TEST_CASE("BellmanFordAlgorithm") {
    using namespace std::string_literals;

    Ng::ListGraph<std::string, int> graph(true, true);

    graph.PushNode("A");
    graph.PushNode("B");
    graph.PushNode("C");
    graph.PushNode("D");
    graph.PushNode("E");

    graph.PushEdge("A", "B", -1);
    graph.PushEdge("A", "C", 4);
    graph.PushEdge("B", "C", 3);
    graph.PushEdge("B", "D", 2);
    graph.PushEdge("B", "E", 2);
    graph.PushEdge("D", "B", 1);
    graph.PushEdge("D", "C", 5);
    graph.PushEdge("E", "D", -3);

    std::map<std::string, std::optional<int>> expected = {
        { "A",  0 },
        { "B", -1 },
        { "C",  2 },
        { "D", -2 },
        { "E",  1 }
    };

    std::map<std::string, std::optional<int>> actual = Ng::GraphAlgorithm::BellmanFordAlgorithm(graph, "A"s);

    CHECK(expected == actual);
}