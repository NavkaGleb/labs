#include <iostream>

#include <fort.hpp>

namespace Ng {

    template <typename Node, typename Edge>
    ListGraph<Node, Edge>::ListGraph(bool isDirected, bool isWeighted)
        : m_IsDirected(isDirected)
        , m_IsWeighted(isWeighted)
        , m_EdgeCount(0) {}

    template <typename Node, typename Edge>
    ListGraph<Node, Edge>::ListGraph(const std::map<Node, std::map<Node, Edge>>& other)
        : m_IsDirected(false)
        , m_IsWeighted(false)
        , m_EdgeCount(0)
        , m_AdjacencyList(other) {

        for (const auto& [from, edges] : m_AdjacencyList) {
            for (const auto& [to, edge] : edges) {
                if (from == to)
                    continue;

                ++m_EdgeCount;

                if (!IsEdgeExists(to, from))
                    m_IsDirected = true;

                if (m_AdjacencyList[from][to] != Edge())
                    m_IsWeighted = true;
            }
        }
    }

    template <typename Node, typename Edge>
    ListGraph<Node, Edge>::ListGraph(const std::map<Node, std::map<Node, std::optional<Edge>>>& other)
        : m_IsDirected(false)
        , m_IsWeighted(false)
        , m_EdgeCount(0) {

        for (const auto& [from, edges] : other) {
            for (const auto& [to, edge] : edges) {
                if (from == to)
                    continue;

                if (!edge)
                    continue;

                m_AdjacencyList[from][to] = *edge;

                ++m_EdgeCount;

                if (!IsEdgeExists(to, from))
                    m_IsDirected = true;

                if (m_AdjacencyList[from][to] != Edge())
                    m_IsWeighted = true;
            }
        }
    }

    template <typename Node, typename Edge>
    bool ListGraph<Node, Edge>::IsNodeExists(const Node& node) const {
        return m_AdjacencyList.contains(node);
    }

    template <typename Node, typename Edge>
    bool ListGraph<Node, Edge>::IsEdgeExists(const Node& from, const Node& to) const {
        if (!IsNodeExists(from) || !IsNodeExists(to))
            return false;

        return m_AdjacencyList.at(from).contains(to);
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::SetDirected(bool isDirected) {
        m_IsDirected = isDirected;
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::SetWeighted(bool isWeighted) {
        m_IsWeighted = isWeighted;
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::Clear() {

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PushNode(const Node& node) {
        if (IsNodeExists(node))
            return;

        m_AdjacencyList[node] = {};
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopNode(const Node& node) {
        if (!IsNodeExists(node))
            return;

        for (auto& [innerNode, edges] : m_AdjacencyList)
            PopEdge(innerNode, node);

        m_AdjacencyList.erase(node);
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PushEdge(const Node& from, const Node& to) {
        PushEdge(from, to, Edge());
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PushEdge(const Node& from, const Node& to, const Edge& edge) {
        if (IsEdgeExists(from, to))
            return;

        m_AdjacencyList[from][to] = m_IsWeighted ? edge : Edge();
        ++m_EdgeCount;

        if (!m_IsDirected)
            PushEdge(to, from, edge);
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopEdge(const Node& from, const Node& to) {
        if (!IsEdgeExists(from, to))
            return;

        m_AdjacencyList[from].erase(to);
        --m_EdgeCount;

        if (!m_IsDirected)
            PopEdge(to, from);
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopEdges() {
        for (auto& [node, edges] : m_AdjacencyList)
            edges.clear();
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::Print() const {
        fort::char_table table;

        table.set_border_style(FT_BASIC2_STYLE);

        table << "Node";

        for (const auto& [from, edges] : m_AdjacencyList)
            table << from;

        table[table.cur_row()].set_cell_text_align(fort::text_align::center);
        table << fort::endr;

        for (const auto& [from, edges] : m_AdjacencyList) {
            table[table.cur_row()].set_cell_text_align(fort::text_align::center);
            table << from;

            for (const auto& [to, edges] : m_AdjacencyList) {
                if (IsEdgeExists(from, to))
                    table << m_AdjacencyList.at(from).at(to);
                else
                    table << "null";
            }

            table << fort::endr;
        }

        std::cout << table.to_string() << std::endl;
    }

} // namespace Ng