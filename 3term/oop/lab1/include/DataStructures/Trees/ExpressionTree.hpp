#pragma once

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>

namespace ng {

    class ExpressionTree {
    public:
        // inner class
        class Node {
        public:
            // constructor / destructor
            explicit Node(const std::string& value, Node* left = nullptr, Node* right = nullptr);
            Node(std::string value, short type, Node* left = nullptr, Node* right = nullptr);
            Node(const Node& other);
            virtual ~Node();

            // accessors
            [[nodiscard]] const short& type() const;
            [[nodiscard]] const std::string& value() const;
            [[nodiscard]] const Node* left() const;
            [[nodiscard]] const Node* right() const;

            // operators
            bool operator==(const Node& other) const;
            friend std::ostream& operator<<(std::ostream& stream, const Node& node);

        private:
            // variables
            short _type;
            std::string _value;
            Node* _left;
            Node* _right;

            // friends
            friend ExpressionTree;

        };

        // constructor / destructor
        ExpressionTree();
        explicit ExpressionTree(std::string expression);
        ExpressionTree(Node* node);
        ExpressionTree(const ExpressionTree& other);
        ~ExpressionTree();

        // accessors
        [[nodiscard]] bool empty() const;
        [[nodiscard]] const std::string& expression() const;
        [[nodiscard]] std::map<std::string, double> variables() const;
        [[nodiscard]] std::string fullParenthesisExpression() const;

        // modifiers
        void expression(const std::string& expression);
        void expression(std::string&& expression);

        // public methods
        void clear();
        void simplify();
        double calc(const std::map<std::string, double>& variables = {});

        ExpressionTree* ndifferentiate(const std::string& variable = "") const;
        ExpressionTree& differentiate(const std::string& variable = "");

        void vprint() const;

        // operators
        friend std::istream& operator>>(std::istream& stream, ExpressionTree& tree);
        friend std::ostream& operator<<(std::ostream& stream, const ExpressionTree& tree);

    private:
        // enums
        enum NodeType { Unknown = -1, Operator, Func, Const, Var };

        // inner structs
        struct Variable {
            // variables
            double value;
            int count;

            // constructor
            Variable() : value(0.0), count(0) {}

        };

        // variables
        Node* _root;
        std::string _expression;
        std::stack<Node*> _operators;
        std::vector<Node*> _postfixExpression;
        std::map<std::string, Variable> _variables;

        // private methods
        static bool _isOperator(char symbol);
        static bool _isOperator(char symbol, char prevSymbol);
        static bool _isOperator(const std::string& value);
        static bool _isUnaryOperator(const std::string& value);
        static bool _isDigit(const char& symbol);
        static bool _isConstant(const std::string& operand);
        static bool _isMathConstant(const std::string& operand);
        static bool _isVariable(const std::string& operand);
        static bool _isFunction(const std::string& value);
        static short _getNodeType(const std::string& value);
        static double _mathConstants(const std::string& constant);

        static int _priority(char symbol);
        bool _morePriority(char symbol) const;

        void _clearOperators(char symbol);
        void _clearOperand(std::string& operand);

        void _initVariables(Node* node);

        void _build(Node* parent, bool init = false);
        void _build();

        bool _exist(Node* node, const std::string& value) const;

        void _clear();
        double _calc(Node* node) const;
        void _fullParenthesisExpression(Node* node, std::ostream& stream = std::cout) const;

        void _updateVariable(const std::string& key);
        void _updateVariables(Node* node);
        void _updateNode(Node*& node, const std::string& value, const short& type) const;
        void _updateNode(Node*& node) const;
        void _simplify(Node*& node);

        Node* _derivative(Node* node, const std::string& variable) const;

        void _print(std::ostream& stream, Node* node, const int& level, const char* caption, bool nextLine = false) const;

    };

} // namespace ng