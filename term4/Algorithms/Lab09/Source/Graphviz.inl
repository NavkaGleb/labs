namespace Ng {

    template <typename Node, typename Edge>
    std::string Graphviz::ToString(const ListGraph<Node, Edge>& graph) {
        std::stringstream result;

        Begin(result);
        {
            for (const auto& from : graph.GetNodes()) {
                for (const auto& to : graph.GetNodes()) {
                    if (graph.IsEdgeExists(from, to)) {
                        result << "  " << from << " -> " << to << " [ label = " << graph.m_AdjacencyList.at(from).at(to) << " ];" << std::endl;
                    }
                }
            }
        }
        End(result);

        return result.str();
    }

} // namespace Ng