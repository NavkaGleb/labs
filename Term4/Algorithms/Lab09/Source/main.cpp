#include <Random/Random.hpp>

#include "GraphAlgorithm.hpp"
#include "Graphviz.hpp"

template <typename Node, typename Edge>
static void Print(const std::map<Node, std::map<Node, std::optional<Edge>>>& matrix) {
    fort::char_table table;

    table.set_border_style(FT_BASIC2_STYLE);

    table << "N";

    for (const auto& [from, edges] : matrix)
        table << from;

    table[table.cur_row()].set_cell_text_align(fort::text_align::right);
    table << fort::endr;

    for (const auto& [from, edges] : matrix) {
        table[table.cur_row()].set_cell_text_align(fort::text_align::right);
        table << from;

        for (const auto& [to, edge] : edges) {
            if (edge)
                table << *edge;
            else
                table << "-";
        }

        table << fort::endr;
    }

    std::cout << table.to_string() << std::endl;
}

int main() {
    Ng::ListGraph<int, int> graph(true, true);

    for (int i = 0; i < 10; ++i)
        graph.PushNode(i);

    for (int i = 0; i < 10; ++i)
        graph.PushEdge(Ng::Random::Get(0, 9), Ng::Random::Get(0, 9), Ng::Random::Get(-5, 10));

    std::cout << "NodeCount: " << graph.GetNodeCount() << std::endl;
    std::cout << "EdgeCount: " << graph.GetEdgeCount() << std::endl;

    std::cout << Ng::Graphviz::ToString(graph) << std::endl;
    std::cout << "http://www.webgraphviz.com/" << std::endl;

    if (auto distance = Ng::GraphAlgorithm::Johnson(graph, 10); !distance.empty())
        Print(distance);
}
