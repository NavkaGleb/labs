namespace Ng {

    template <typename Node, typename Edge>
    ListGraph<Node, Edge>::ListGraph(bool isDirected, bool isWeighted)
        : m_IsDirected(isDirected)
        , m_IsWeighted(isWeighted)
        , m_EdgeCount(0) {}

    template <typename Node, typename Edge>
    bool ListGraph<Node, Edge>::IsNodeExists(const Node& node) const {
        return m_AdjacencyList.contains(node);
    }

    template <typename Node, typename Edge>
    bool ListGraph<Node, Edge>::IsEdgeExists(const Node& from, const Node& to) const {
        if (!IsNodeExists(from) || !IsNodeExists(to))
            return false;

        const auto& edges = m_AdjacencyList.at(from);

        return edges.end() != std::find_if(edges.begin(), edges.end(), [&](const InnerEdge& edge) {
            return edge.ToNode == to;
        });
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

        m_AdjacencyList[from].emplace_back(to, edge);
        ++m_EdgeCount;

        if (!m_IsDirected)
            PushEdge(to, from, edge);
    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopEdge(const Node& from, const Node& to) {
        if (!IsEdgeExists(from, to))
            return;

        auto& edges = m_AdjacencyList[from];

        edges.erase(std::remove_if(edges.begin(), edges.end(), [&](const InnerEdge& edge) {
            return edge.ToNode == to;
        }));

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
        for (const auto& [node, edges] : m_AdjacencyList) {
            std::cout << node << ": ";

            for (const auto& edge : edges)
                std::cout << "(" << edge.ToNode << ", " << edge.Value << ") ";
            std::cout << std::endl;
        }
    }

} // namespace Ng