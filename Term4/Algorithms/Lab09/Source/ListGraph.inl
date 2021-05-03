namespace Ng {

    template <typename Node, typename Edge>
    ListGraph<Node, Edge>::ListGraph(bool isDirected, bool isWeighted)
        : m_IsDirected(isDirected)
        , m_IsWeighted(isWeighted)
        , m_NodeCount(0)
        , m_EdgeCount(0) {}

    template <typename Node, typename Edge>
    ListGraph<Node, Edge>::~ListGraph() {

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

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopNode(const Node& node) {

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PushEdge(const Node& from, const Node& to, const Edge& edge) {

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PushEdge(const Node& from, const Node& to) {

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopEdge(const Node& from, const Node& to) {

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::PopEdges() {

    }

    template <typename Node, typename Edge>
    void ListGraph<Node, Edge>::Print() const {

    }

} // namespace Ng