#pragma once

#include "ListGraph.hpp"

namespace Ng {

    class GraphAlgorithm {
    public:
        template <typename Node, typename Edge>
        static std::vector<std::vector<std::optional<Edge>>> DijkstraAlgorithm(const ListGraph<Node, Edge>& graph);

        template <typename Node, typename Edge>
        static std::vector<std::optional<Edge>> BellmanFordAlgorithm(const ListGraph<Node, Edge>& graph);

        template <typename Node, typename Edge>
        static std::vector<std::vector<std::optional<Edge>>> JohnsonAlgorithm(const ListGraph<Node, Edge>& graph);

    }; // class GraphAlgorithm

} // namespace Ng