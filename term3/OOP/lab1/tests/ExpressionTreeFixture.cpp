#include <gtest/gtest.h>

#include "TestFunc.hpp"
#include "DataStructures/Trees/ExpressionTree.hpp"

class ExpressionTreeFixture : public ::testing::Test {
protected:
    // variables
    ng::ExpressionTree* _tree;

    // protected methods
    void TearDown() override { delete this->_tree; }

    void CreateTree(std::string expression) { this->_tree = new ng::ExpressionTree(expression); }

    void Differentiate(const std::string& variable) { this->_tree->differentiate(variable); }

    void Simplify() { this->_tree->simplify(); }

};

bool DOUBLE_EQ(const double& a, const double& b) {

    static const double eps = 0.000001;

    return std::abs(a) - std::abs(b) <= eps;

}

TEST_F(ExpressionTreeFixture, SimpleExression) {

    std::map<std::string, double> variables = { { "x", 1.2 }, { "y", -2.32 } };
    std::string expectedExpression = "(x+y)";
    std::string expectedSimplifiedExpression("(x+y)");
    double expectedResult = -1.12;
    std::string expectedDerivative = "(1+0)";
    std::string expectedSimplifiedDerivative = "1";
    double expectedDerivativeResult = 1.0;

    this->CreateTree("x+y");
    EXPECT_EQ(expectedExpression, this->_tree->fullParenthesisExpression());

    this->Simplify();
    EXPECT_EQ(expectedSimplifiedExpression, this->_tree->fullParenthesisExpression());
    EXPECT_PRED2(DOUBLE_EQ, expectedResult, this->_tree->calc(variables));

    this->Differentiate("x");
    EXPECT_EQ(expectedDerivative, this->_tree->fullParenthesisExpression());

    this->Simplify();
    EXPECT_EQ(expectedSimplifiedDerivative, this->_tree->fullParenthesisExpression());
    EXPECT_PRED2(DOUBLE_EQ, expectedDerivativeResult, this->_tree->calc());

}

TEST_F(ExpressionTreeFixture, TrigonometricExression) {

    std::map<std::string, double> variables = { { "x", 1.2 }, { "y", -2.32 } };
    std::string expectedExpression = "(sin<cos<x>>+cos<y>)";
    std::string expectedSimplifiedExpression("(sin<cos<x>>+cos<y>)");
    double expectedResult = -0.326576;
    std::string expectedDerivative = "((cos<cos<x>>*((-1*sin<x>)*1))+((-1*sin<y>)*0))";
    std::string expectedSimplifiedDerivative = "(cos<cos<x>>*(-1*sin<x>))";
    double expectedDerivativeResult = -0.871516;

    this->CreateTree("sin(cos(x))+cos(y)");
    EXPECT_EQ(expectedExpression, this->_tree->fullParenthesisExpression());

    this->Simplify();
    EXPECT_EQ(expectedSimplifiedExpression, this->_tree->fullParenthesisExpression());
    EXPECT_PRED2(DOUBLE_EQ, expectedResult, this->_tree->calc(variables));

    this->Differentiate("x");
    EXPECT_EQ(expectedDerivative, this->_tree->fullParenthesisExpression());

    this->Simplify();
    EXPECT_EQ(expectedSimplifiedDerivative, this->_tree->fullParenthesisExpression());
    EXPECT_PRED2(DOUBLE_EQ, expectedDerivativeResult, this->_tree->calc(variables));

}

TEST_F(ExpressionTreeFixture, MixExression) {

    std::map<std::string, double> variables = { { "x", 1.2 }, { "y", 2.32 } };
    std::string expectedExpression = "((log2<y>+exp<((2*x)+y)>)+(x*0))";
    std::string expectedSimplifiedExpression("(log2<y>+exp<((2*x)+y)>)");
    double expectedResult = 113.382;
    std::string expectedDerivative = "(((1/(y*ln<2>))*0)+(((2*x)+y)*(((0*x)+(2*1))+0)))";
    std::string expectedSimplifiedDerivative = "(((2*x)+y)*2)";
    double expectedDerivativeResult = 9.44;

    this->CreateTree("log2(y)+exp((2*x)+y)+x*0");
    EXPECT_EQ(expectedExpression, this->_tree->fullParenthesisExpression());

    this->Simplify();
    EXPECT_EQ(expectedSimplifiedExpression, this->_tree->fullParenthesisExpression());
    EXPECT_PRED2(DOUBLE_EQ, expectedResult, this->_tree->calc(variables));

    this->Differentiate("x");
    EXPECT_EQ(expectedDerivative, this->_tree->fullParenthesisExpression());

    this->Simplify();
    EXPECT_EQ(expectedSimplifiedDerivative, this->_tree->fullParenthesisExpression());
    EXPECT_PRED2(DOUBLE_EQ, expectedDerivativeResult, this->_tree->calc(variables));

}