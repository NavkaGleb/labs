#include <string>

#include <doctest/doctest.h>

#include "GraphAlgorithm.hpp"

TEST_CASE("DijkstraAlgorithm") {
    using namespace std::string_literals;

    Ng::ListGraph<std::string, float> graph(false, true);

    for (int i = 0; i < 5; ++i)
        graph.PushNode("Node" + std::to_string(i));

    graph.PushEdge("Node0", "Node1", 8.0f);
    graph.PushEdge("Node1", "Node4", 5.0f);
    graph.PushEdge("Node4", "Node2", 10.0f);
    graph.PushEdge("Node0", "Node2", 1.0f);

    std::map<std::string, std::optional<float>> expected = {
        { "Node0", 8.0f         },
        { "Node1", 0.0f         },
        { "Node2", 9.0f         },
        { "Node3", std::nullopt },
        { "Node4", 5.0f         }
    };

    std::map<std::string, std::optional<float>> actual = Ng::GraphAlgorithm::DijkstraAlgorithm(graph, "Node1"s);

    CHECK(expected == actual);
}