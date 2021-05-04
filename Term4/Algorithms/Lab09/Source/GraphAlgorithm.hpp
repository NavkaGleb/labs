#pragma once

#include <vector>
#include <optional>

#include "ListGraph.hpp"

namespace Ng {

    class GraphAlgorithm {
    public:
        template <typename Node, typename Edge>
        static std::map<Node, std::optional<Edge>> DijkstraAlgorithm(
            const ListGraph<Node, Edge>& graph,
            const Node&                  source
        );

        template <typename Node, typename Edge>
        static std::map<Node, std::optional<Edge>> BellmanFordAlgorithm(
            const ListGraph<Node, Edge>& graph,
            const Node&                  source
        );

        template <typename Node, typename Edge>
        static std::map<Node, std::map<Node, std::optional<Edge>>> JohnsonAlgorithm(const ListGraph<Node, Edge>& graph);

    }; // class GraphAlgorithm

} // namespace Ng

#include "GraphAlgorithm.inl"