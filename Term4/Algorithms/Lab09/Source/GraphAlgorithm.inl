#include "FibonacciHeap.hpp"

namespace Ng {

    template <typename Node, typename Edge>
    std::map<Node, std::optional<Edge>> GraphAlgorithm::DijkstraAlgorithm(
        const ListGraph<Node, Edge>& graph,
        const Node&                  source
    ) {
        std::map<Node, std::optional<Edge>> distance;
        FibonacciHeap<std::pair<Edge, Node>, decltype([](const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        })> heap;

        for (const auto& [from, edges] : graph.m_AdjacencyList)
            distance[from] = std::nullopt;

        distance[source] = Edge();
        heap.Push({ Edge(), source });

        while (!heap.IsEmpty()) {
            auto node = heap.GetPeak().second;
            heap.PopPeak();

            for (const auto& [to, edge] : graph.m_AdjacencyList.at(node)) {
                if (!distance[to] || *distance[to] > *distance[node] + edge) {
                    if (!distance[to])
                        distance[to] = Edge();

                    distance[to] = *distance[node] + edge;
                    heap.Push({ *distance[to], to });
                }
            }
        }

        return distance;
    }

    template <typename Node, typename Edge>
    std::map<Node, std::optional<Edge>> GraphAlgorithm::BellmanFordAlgorithm(
        const ListGraph<Node, Edge>& graph,
        const Node&                  source
    ) {
        std::map<Node, std::optional<Edge>> distance;

        for (const auto& [from, edges] : graph.m_AdjacencyList)
            distance[from] = std::nullopt;

        distance[source] = Edge();

        for (int i = 0; i < graph.GetNodeCount() - 1; ++i) {
            for (const auto& [from, edges] : graph.m_AdjacencyList) {
                for (const auto& [to, edge] : edges) {
                    if (!distance[to] || *distance[to] > *distance[from] + edge) {
                        if (!distance[to])
                            distance[to] = Edge();

                        *distance[to] = *distance[from] + edge;
                    }
                }
            }
        }

        // Check for negative cycles
        for (const auto& [from, edges] : graph.m_AdjacencyList)
            for (const auto& [to, edge] : edges)
                if (!distance[to] || *distance[to] > *distance[from] + edge)
                    return {};

        return distance;
    }

    template <typename Node, typename Edge>
    std::map<Node, std::map<Node, std::optional<Edge>>> GraphAlgorithm::JohnsonAlgorithm(const ListGraph<Node, Edge>& graph) {
        if (!graph.IsDirected() || !graph.IsDirected())
            return {};

        std::map<Node, std::map<Node, std::optional<Edge>>> distance;
        ListGraph<Node, Edge> temp(graph);

        // Push temp node
        temp.PushNode(Node());

        for (const auto& [from, edges] : temp.m_AdjacencyList)
            temp.PushEdge(Node(), from, Edge());

        auto bellmanFord = BellmanFordAlgorithm(temp, Node());

        // Check if there ara negative cycles
        if (bellmanFord.empty())
            return {};

        temp.PopNode(Node());

        // Create all edges > 0
        for (auto& [from, edges] : temp.m_AdjacencyList)
            for (auto& [to, edge] : edges)
                edge += *bellmanFord[from] - *bellmanFord[to];

        for (const auto& [from, edges] : temp.m_AdjacencyList)
            distance[from] = DijkstraAlgorithm(temp, from);

        // Get distance for original graph
        for (auto& [from, edges] : distance)
            for (auto& [to, edge] : edges)
                *edge += *bellmanFord[to] - *bellmanFord[from];

        return distance;
    }

} // namespace Ng