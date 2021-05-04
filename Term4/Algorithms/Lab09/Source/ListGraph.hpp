#pragma once

#include <map>

#include "IGraph.hpp"

namespace Ng {

    template <typename Node, typename Edge>
    class ListGraph : public IGraph<Node, Edge> {
    public:
        friend class GraphAlgorithm;

    public:
        using NodeType = Node;
        using EdgeType = Edge;

    public:
        explicit ListGraph(bool isDirected = false, bool isWeighted = false);
        explicit ListGraph(const std::map<Node, std::map<Node, Edge>>& other);
        explicit ListGraph(const std::map<Node, std::map<Node, std::optional<Edge>>>& other);
        ~ListGraph() override = default;

    public:
        [[nodiscard]] inline bool IsEmpty() const override { return m_AdjacencyList.empty(); }
        [[nodiscard]] inline std::size_t GetNodeCount() const override { return m_AdjacencyList.size(); }
        [[nodiscard]] inline std::size_t GetEdgeCount() const override { return m_EdgeCount / (m_IsDirected ? 1 : 2); }
        [[nodiscard]] inline bool IsDirected() const override { return m_IsDirected; }
        [[nodiscard]] inline bool IsWeighted() const override { return m_IsWeighted; }

        [[nodiscard]] bool IsNodeExists(const Node& node) const override;
        [[nodiscard]] bool IsEdgeExists(const Node& from, const Node& to) const override;

        void SetDirected(bool isDirected) override;
        void SetWeighted(bool isWeighted) override;

        void Clear() override;

        void PushNode(const Node& node) override;
        void PopNode(const Node& node) override;

        void PushEdge(const Node& from, const Node& to) override;
        void PushEdge(const Node& from, const Node& to, const Edge& edge) override;
        void PopEdge(const Node& from, const Node& to) override;
        void PopEdges() override;

        void Print() const override;

    private:
        bool                                 m_IsDirected;
        bool                                 m_IsWeighted;
        std::size_t                          m_EdgeCount;
        std::map<Node, std::map<Node, Edge>> m_AdjacencyList;

    }; // class ListGraph

} // namespace Ng

#include "ListGraph.inl"