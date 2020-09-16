//#include <string>
//#include <gtest/gtest.h>
//
//#include "MatrixGraph.hpp"
//
//class MatrixGraphFixture : public ::testing::Test {
//protected:
//    // variables
//    ng::MatrixGraph<std::string, int>* _graph;
//
//    // protected methods
//    void SetUp() override { this->_graph = new ng::MatrixGraph<std::string, int>(false, true); }
//
//    void TearDown() override { delete this->_graph; }
//
//    void InitGraph() {
//
//        this->_graph->pushNode("a");
//        this->_graph->pushNode("b");
//        this->_graph->pushNode("c");
//        this->_graph->pushNode("d");
//        this->_graph->pushNode("e");
//
//        this->_graph->pushEdge("a", "b", 1);
//        this->_graph->pushEdge("a", "c", 2);
//        this->_graph->pushEdge("d", "e", 4);
//
//    }
//
//    bool Connected() {
//
//        this->InitGraph();
//        return this->_graph->connected();
//
//    }
//
//    std::vector<std::vector<std::string>> Components() {
//
//        this->InitGraph();
//        return this->_graph->components();
//
//    }
//
//};
//
//TEST_F(MatrixGraphFixture, InitGraph) {
//
//    this->InitGraph();
//
//    this->_graph->print();
//
//    EXPECT_EQ(false, this->_graph->empty());
//    EXPECT_EQ(5, this->_graph->nodes());
//    EXPECT_EQ(3, this->_graph->edges());
//
//    this->_graph->popNode("c");
//
//    std::cout << std::endl;
//    this->_graph->print();
//
//}
//
//TEST_F(MatrixGraphFixture, Connected) {
//
//    EXPECT_EQ(false, this->Connected());
//
//}
//
//TEST_F(MatrixGraphFixture, Components) {
//
//    std::vector<std::vector<std::string>> expected = { { "a", "b", "c" }, { "d", "e" } };
//    std::vector<std::vector<std::string>> actual = this->Components();
//
//    ASSERT_EQ(expected.size(), actual.size());
//
//    for (int i = 0; i < actual.size(); ++i) {
//
//        ASSERT_EQ(expected[i].size(), actual[i].size());
//        for (int j = 0; j < actual[i].size(); ++j)
//            EXPECT_EQ(expected[i][j], actual[i][j]);
//
//    }
//
//}