#pragma once

#include <string>

#include "ListGraph.hpp"

namespace Ng {

    class Graphviz {
    public:
        template <typename Node, typename Edge>
        static std::string ToString(const ListGraph<Node, Edge>& graph);

    private:
        static void Begin(std::stringstream& ss);
        static void End(std::stringstream& ss);

    }; // class Graphviz

} // namespace Ng

#include "Graphviz.inl"