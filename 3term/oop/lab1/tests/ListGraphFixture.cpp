#include <gtest/gtest.h>

#include "ListGraph.hpp"

class ListGraphFixture : public ::testing::Test {
protected:
    // variables
    ng::ListGraph<std::string, int>* _graph;

    // protected methods
    void SetUp() override { this->_graph = new ng::ListGraph<std::string, int>(false, true); }

    void TearDown() override { delete this->_graph; }

    void InitGraph() {

        this->_graph->pushNode("a");
        this->_graph->pushNode("b");
        this->_graph->pushNode("c");
        this->_graph->pushNode("d");
        this->_graph->pushNode("e");

        this->_graph->pushEdge("a", "b", 1);
        this->_graph->pushEdge("a", "c", 2);
        this->_graph->pushEdge("d", "e", 4);

    }

};

TEST_F(ListGraphFixture, initListGraph) {

    this->InitGraph();

    this->_graph->print();

    EXPECT_EQ(false, this->_graph->empty());
    EXPECT_EQ(5, this->_graph->nodes());
    EXPECT_EQ(3, this->_graph->edges());

    std::cout << std::endl;
    this->_graph->popEdge("a", "b");

    this->_graph->print();

}

TEST_F(ListGraphFixture, connected) {

    EXPECT_EQ(false, this->_graph->connected());

}

TEST_F(ListGraphFixture, components) {

    std::vector<std::vector<std::string>> expected = { { "a", "b", "c" }, { "d", "e" } };
    std::vector<std::vector<std::string>> result = this->_graph->components();

    for (int i = 0; i < result.size(); ++i)
        for (int j = 0; j < result[i].size(); ++j)
            EXPECT_EQ(expected[i][j], result[i][j]);

}