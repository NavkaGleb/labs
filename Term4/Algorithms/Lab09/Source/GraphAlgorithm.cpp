#include "GraphAlgorithm.hpp"

namespace Ng {

    template <typename Node, typename Edge>
    std::vector<std::vector<std::optional<Edge>>> GraphAlgorithm::DijkstraAlgorithm(const ListGraph<Node, Edge>& graph) {
        std::vector<std::vector<std::optional<Edge>>> result(
            graph.GetNodeCount(),
            std::vector(graph.GetNodeCount(), std::nullopt)
        );

        return result;
    }

    template <typename Node, typename Edge>
    std::vector<std::optional<Edge>> GraphAlgorithm::BellmanFordAlgorithm(const ListGraph<Node, Edge>& graph) {
        std::vector<std::optional<Edge>> result(graph.GetNodeCount(), std::nullopt);

        return result;
    }

    template <typename Node, typename Edge>
    std::vector<std::vector<std::optional<Edge>>> GraphAlgorithm::JohnsonAlgorithm(const ListGraph<Node, Edge>& graph) {
        std::vector<std::vector<std::optional<Edge>>> result(
            graph.GetNodeCount(),
            std::vector(graph.GetNodeCount(), std::nullopt)
        );

        return result;
    }

} // namespace Ng