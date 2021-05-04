#include <doctest/doctest.h>

#include "GraphAlgorithm.hpp"

TEST_CASE("JohnsonAlgorithm") {
    Ng::ListGraph<int, int> graph(true, true);

    graph.PushNode(1);
    graph.PushNode(2);
    graph.PushNode(3);
    graph.PushNode(4);
    graph.PushNode(5);

    graph.PushEdge(1, 2, -4);
    graph.PushEdge(1, 4,  1);
    graph.PushEdge(1, 5, -2);
    graph.PushEdge(2, 3,  5);
    graph.PushEdge(3, 1,  2);
    graph.PushEdge(4, 5,  3);

    std::map<int, std::map<int, std::optional<int>>> expected = {
        { 1, std::map<int, std::optional<int>>{
            { 1,  0            },
            { 2, -4            },
            { 3,  1            },
            { 4,  1            },
            { 5, -2            },
        }},
        { 2, std::map<int, std::optional<int>>{
            { 1,  7            },
            { 2,  0            },
            { 3,  5            },
            { 4,  8            },
            { 5,  5            },
        }},
        { 3, std::map<int, std::optional<int>>{
            { 1,  2            },
            { 2, -2            },
            { 3,  0            },
            { 4,  3            },
            { 5,  0            },
        }},
        { 4, std::map<int, std::optional<int>>{
            { 1,  std::nullopt },
            { 2,  std::nullopt },
            { 3,  std::nullopt },
            { 4,  0            },
            { 5,  3            },
        }},
        { 5, std::map<int, std::optional<int>>{
            { 1,  std::nullopt },
            { 2,  std::nullopt },
            { 3,  std::nullopt },
            { 4,  std::nullopt },
            { 5,  0            },
        }},
    };

    auto actual = Ng::GraphAlgorithm::Johnson(graph);

    CHECK(expected == actual);
}