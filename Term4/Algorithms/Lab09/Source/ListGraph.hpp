#pragma once

#include "IGraph.hpp"

namespace Ng {

    template <typename Node, typename Edge>
    class ListGraph : public IGraph<Node, Edge> {
    public:
        explicit ListGraph(bool isDirected = false, bool isWeighted = false);
        ~ListGraph() override;

    public:
        [[nodiscard]] inline bool IsEmpty() const override { return m_NodeCount == 0; }
        [[nodiscard]] inline std::size_t GetNodeCount() const override { return m_NodeCount; }
        [[nodiscard]] inline std::size_t GetEdgeCount() const override { return m_EdgeCount; }
        [[nodiscard]] inline bool IsDirected() const override { return m_IsDirected; }
        [[nodiscard]] inline bool IsWeighted() const override { return m_IsWeighted; }

        void SetDirected(bool isDirected) override;
        void SetWeighted(bool isWeighted) override;

        void Clear() override;

        void PushNode(const Node& node) override;
        void PopNode(const Node& node) override;

        void PushEdge(const Node& from, const Node& to, const Edge& edge) override;
        void PushEdge(const Node& from, const Node& to) override;
        void PopEdge(const Node& from, const Node& to) override;
        void PopEdges() override;

        void Print() const override;

    private:
        bool        m_IsDirected;
        bool        m_IsWeighted;
        std::size_t m_NodeCount;
        std::size_t m_EdgeCount;

    }; // class ListGraph

} // namespace Ng

#include "ListGraph.inl"