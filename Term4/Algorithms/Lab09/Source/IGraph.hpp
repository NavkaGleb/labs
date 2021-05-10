#pragma once

#include <vector>

namespace Ng {

    template <typename Node, typename Edge>
    class IGraph {
    public:
        IGraph() = default;
        virtual ~IGraph() = default;

    public:
        [[nodiscard]] virtual bool IsEmpty() const = 0;
        [[nodiscard]] virtual std::size_t GetNodeCount() const = 0;
        [[nodiscard]] virtual std::size_t GetEdgeCount() const = 0;
        [[nodiscard]] virtual bool IsDirected() const = 0;
        [[nodiscard]] virtual bool IsWeighted() const = 0;

        [[nodiscard]] virtual bool IsNodeExists(const Node& node) const = 0;
        [[nodiscard]] virtual bool IsEdgeExists(const Node& from, const Node& to) const = 0;

        virtual void SetDirected(bool isDirected) = 0;
        virtual void SetWeighted(bool isWeighted) = 0;

        virtual std::vector<Node> GetNodes() const = 0;

        virtual void Clear() = 0;

        virtual void PushNode(const Node& node) = 0;
        virtual void PopNode(const Node& node) = 0;

        virtual void PushEdge(const Node& from, const Node& to) = 0;
        virtual void PushEdge(const Node& from, const Node& to, const Edge& edge) = 0;
        virtual void PopEdge(const Node& from, const Node& to) = 0;
        virtual void PopEdges() = 0;

    }; // class IGraph

} // namespace Ng