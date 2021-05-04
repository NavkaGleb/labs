#pragma once

#include <vector>
#include <optional>

#include "ListGraph.hpp"

namespace Ng {

    class GraphAlgorithm {
    public:
        template <typename Node, typename Edge>
        static std::map<Node, std::optional<Edge>> Dijkstra(const ListGraph<Node, Edge>& graph, const Node& source);

        template <typename Node, typename Edge>
        static std::map<Node, std::optional<Edge>> BellmanFord(const ListGraph<Node, Edge>& graph, const Node& source);

        template <typename Node, typename Edge>
        static std::map<Node, std::map<Node, std::optional<Edge>>> Johnson(
            const ListGraph<Node, Edge>& graph,
            const Node&                  tempNode = Node()
        );

    }; // class GraphAlgorithm

} // namespace Ng

#include "GraphAlgorithm.inl"