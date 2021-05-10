#include "Graphviz.hpp"

namespace Ng {

    void Graphviz::Begin(std::stringstream& ss) {
        ss << "digraph G {" << std::endl;
        ss << " " << "node [shape = circle];" << std::endl;
    }

    void Graphviz::End(std::stringstream& ss) {
        ss << "}";
    }

} // namespace Ng