#include <gtest/gtest.h>

#include "TestFunc.hpp"
#include "DataStructures/Graphs/MatrixGraph.hpp"

class MatrixGraphFixture : public ::testing::Test {
protected:
    // variables
    ng::MatrixGraph<std::string, double>* _graph;

    // protected methods
    void TearDown() override { delete this->_graph; }

    void CreateGraph(bool directed, bool weighed) {

        this->_graph = new ng::MatrixGraph<std::string, double>(directed, weighed);

    }

    void InitGraph() {

        this->_graph->pushNode("a");
        this->_graph->pushNode("b");
        this->_graph->pushNode("c");
        this->_graph->pushNode("d");
        this->_graph->pushNode("e");

        this->_graph->pushEdge("a", "b", 1.2);
        this->_graph->pushEdge("a", "c", 2.4);
        this->_graph->pushEdge("d", "e", 4.1);

    }

};

TEST_F(MatrixGraphFixture, Init) {

    this->CreateGraph(false, true);
    this->InitGraph();
    EXPECT_EQ(false, this->_graph->empty());
    EXPECT_EQ(5, this->_graph->nodes());
    EXPECT_EQ(3, this->_graph->edges());

    this->_graph->popEdge("a", "b");
    EXPECT_EQ(5, this->_graph->nodes());
    EXPECT_EQ(2, this->_graph->edges());

    this->_graph->popNode("a");
    EXPECT_EQ(4, this->_graph->nodes());
    EXPECT_EQ(1, this->_graph->edges());

    this->_graph->popEdges();
    EXPECT_EQ(4, this->_graph->nodes());
    EXPECT_EQ(0, this->_graph->edges());

    this->_graph->clear();
    EXPECT_EQ(0, this->_graph->nodes());
    EXPECT_EQ(0, this->_graph->edges());

}

TEST_F(MatrixGraphFixture, Connected) {

    this->CreateGraph(false, true);
    this->InitGraph();
    EXPECT_EQ(false, this->_graph->connected());

}

TEST_F(MatrixGraphFixture, Components) {

    this->CreateGraph(false, true);
    this->InitGraph();

    std::vector<std::vector<std::string>> expected = { { "a", "b", "c" }, { "d", "e" } };
    std::vector<std::vector<std::string>> actual = this->_graph->components();

    for (std::size_t i = 0; i < actual.size(); ++i)
        for (std::size_t j = 0; j < actual[i].size(); ++j)
            EXPECT_EQ(expected[i][j], actual[i][j]);

}

TEST_F(MatrixGraphFixture, DFS) {

    this->CreateGraph(false, true);
    this->InitGraph();

    std::vector<std::string> expected = { "a", "b", "c" };
    std::vector<std::string> actual = this->_graph->dfs("a");

    for (std::size_t i = 0; i < actual.size(); ++i)
        EXPECT_EQ(expected[i], actual[i]);

    expected = { "d", "e" };
    actual = this->_graph->dfs("d");

    for (std::size_t i = 0; i < actual.size(); ++i)
        EXPECT_EQ(expected[i], actual[i]);

}

TEST_F(MatrixGraphFixture, BFS) {

    this->CreateGraph(false, true);
    this->InitGraph();

    std::map<std::string, double> expected = { { "a", 0.0 }, { "b", 1.2 }, { "c", 2.4 }, { "d", 0.0 }, { "e", 0.0 } };
    std::map<std::string, double> actual = this->_graph->bfs("a");

    for (const auto& [key, value] : expected)
        EXPECT_EQ(expected[key], actual[key]);

    expected = { { "a", 0.0 }, { "b", 0.0 }, { "c", 0.0 }, { "d", 0.0 }, { "e", 4.1 } };
    actual = this->_graph->bfs("d");

    for (const auto& [key, value] : expected)
        EXPECT_EQ(expected[key], actual[key]);

}

TEST_F(MatrixGraphFixture, Dijkstra) {

    this->CreateGraph(false, true);
    this->InitGraph();
    this->_graph->pushEdge("c", "d", 2.15);

    std::map<std::string, double*> expected = {
        { "a", new double(0.0) },
        { "b", new double(1.2) },
        { "c", new double(2.4) },
        { "d", new double(4.55) },
        { "e", new double(8.65) }
    };
    std::map<std::string, double*> actual = this->_graph->dijkstra("a");

    for (const auto& [key, value] : expected)
        EXPECT_PRED2(POINTER_EQ<double>, expected[key], value);

    for (auto& [key, value] : actual) {

        delete value;
        delete expected[key];

    }

}