#include "GraphAlgorithm.hpp"

template <typename Node, typename Edge>
static void Print(const std::map<Node, std::map<Node, std::optional<Edge>>>& matrix) {
    fort::char_table table;

    table.set_border_style(FT_BASIC2_STYLE);

    table << "Node";

    for (const auto& [from, edges] : matrix)
        table << from;

    table[table.cur_row()].set_cell_text_align(fort::text_align::center);
    table << fort::endr;

    for (const auto& [from, edges] : matrix) {
        table[table.cur_row()].set_cell_text_align(fort::text_align::center);
        table << from;

        for (const auto& [to, edge] : edges) {
            if (edge)
                table << *edge;
            else
                table << "null";
        }

        table << fort::endr;
    }

    std::cout << table.to_string() << std::endl;
}

int main() {
    Ng::ListGraph<int, int> graph(true, true);

    graph.PushNode(1);
    graph.PushNode(2);
    graph.PushNode(3);
    graph.PushNode(4);
    graph.PushNode(5);

    graph.PushEdge(1, 2, -4);
    graph.PushEdge(1, 4,  1);
    graph.PushEdge(1, 5, -2);
    graph.PushEdge(2, 3,  5);
    graph.PushEdge(3, 1,  2);
    graph.PushEdge(4, 5,  3);

    graph.Print();

    std::cout << "NodeCount: " << graph.GetNodeCount() << std::endl;
    std::cout << "EdgeCount: " << graph.GetEdgeCount() << std::endl;

    Print(Ng::GraphAlgorithm::Johnson(graph));
}
