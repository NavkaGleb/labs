#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char** argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}


//
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <set>
//
//#include "DataStructures/Array.hpp"
//#include "DataStructures/Graphs/MatrixGraph.hpp"
//#include "DataStructures/Graphs/ListGraph.hpp"
//#include "DataStructures/Trees/GeneralTree.hpp"
//#include "FileSystem/FileSystem.hpp"
//#include "DataStructures/Trees/ExpressionTree.hpp"

//struct Vertex {
//
//	int x;
//	int y;
//
//	bool operator<(const Vertex& other) const {
//	    return x * y < other.x * other.y;
//	}
//
//	bool operator==(const Vertex& other) const {
//	    return this->x == other.x && this->y == other.y;
//	}
//
//};
//
//struct Edge {
//    float value;
//    std::string name;
//
//    friend std::ostream& operator<<(std::ostream& ostream, const Edge& edge) {
//        return ostream << edge.name << " -> " << edge.value;
//    }
//};
//
//int main() {
//
//    ng::FileSystem fileSystem;
//
//    std::function<bool(const ng::FileSystemObject&)> f = [](const ng::FileSystemObject& object) {
//
//        return object.filename().find(".txt") != std::string::npos;
//
//    };
//
////    fileSystem.pushDirectory("root");
////    fileSystem.pushDirectory("fuck");
////    fileSystem.pushDirectory("alina");
////    fileSystem.pushFile("la.txt");
////    fileSystem.move("alina");
////    fileSystem.pushFile("gleb.txt");
////    fileSystem.pushFile("paper.txt");
////    fileSystem.pushDirectory("images");
//
////    fileSystem.import(".");
////    fileSystem.printTree(fileSystem.root());
////
////    fileSystem.search(".", f);
//
////    fileSystem.import("fas");
////    fileSystem.printTree(fileSystem.root());
//
////    ng::ExpressionTree tree;
////    tree.expression("sin(cos(2*ln(x)))");
////    std::cout << tree << std::endl;
////
////    std::map<std::string, double> vars = { { "x", 21 } };
////
////    std::cout << tree.calc(vars) << std::endl;
//
////	ng::MatrixGraph<Vertex> graph(false, true);
////
////	Vertex a = { 0, 1 };
////	Vertex b = { 2, 3 };
////	Vertex c = { 4, 5 };
////	Vertex d = { 6, 7 };
////
////	graph.pushNode(a);
////	graph.pushNode(b);
////	graph.pushNode(c);
////	graph.pushNode(d);
////
////	graph.pushEdge(a, b, 3);
////	graph.pushEdge(a, c, 7);
////	graph.pushEdge(b, d, 8);
////
////	std::function<float(const int&)> f = [](const int& e) { return e; };
////
////	auto distance = graph.dijkstra(a);
////
////	for (const auto& p : distance)
////	    std::cout << "{ " << p.first.x << ", " << p.first.y << " } -> " << *p.second << std::endl;
//
//}