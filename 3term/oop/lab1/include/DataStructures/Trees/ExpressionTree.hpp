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
        explicit ExpressionTree(Node* node);
        ExpressionTree(const ExpressionTree& other);
        ~ExpressionTree();

        // accessors
        [[nodiscard]] bool empty() const;

        // public methods
        void clear();
        [[nodiscard]] std::string error() const;

        void build(const std::string& expression);
        double result();
        double calc();
        double calc(const std::map<std::string, double>& variables);
        void bypass() const;

        void simplify();

        [[nodiscard]] ExpressionTree* ndifferentiate() const;                               // differentiate _expression
        [[nodiscard]] ExpressionTree* ndifferentiate(const std::string& variable) const;    // differentiate by 'variable'
        ExpressionTree& differentiate();                                                    // differentiate current _expression
        ExpressionTree& differentiate(const std::string& variable);                         // differentiate current _expression

        void print() const;
        void vprint() const;

        void input();

        // operators
        bool operator==(const ng::ExpressionTree& tree) const;
        friend std::istream& operator>>(std::istream& stream, ExpressionTree& tree);

    private:
        // enums
        enum NodeType { Unknown = -1, Operator, Func, Const, Var };

        // inner structs
        struct Variable {
            // variables
            double _value;
            int _count;

            // constructor
            Variable() : _value(0.0), _count(0) {}

        };

        // variables
        Node* _root;
        std::string _expression;
        std::stack<Node*> _operators;
        std::vector<Node*> _postfixExpression;
        std::map<std::string, Variable> _variables;
        bool _error;
        int _index;
        std::string _errorMessage;
        double* _result;

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
        [[nodiscard]] bool _morePriority(char symbol) const;

        void _clearOperators(char symbol);
        void _clearOperand(std::string& operand);

        void _initVariables(Node* node);

        void _build(Node* parent);
        void _build();

        bool _exist(Node* node, const std::string& value) const;

        void _clear();
        double _calc(Node* node) const;
        void _bypass(Node* node) const;

        void _updateVariable(const std::string& key);
        void _updateVariables(Node* node);
        static void _updateNode(Node*& node, const std::string& value, const short& type);
        static void _updateNode(Node*& node);
        void _simplify(Node*& node);

        bool _selectVariable(std::string& variable) const;
        Node* _derivative(Node* node, const std::string& variable) const;

        void _print(Node* node, const int& level, const char* caption) const;

        // operators
//        friend std::ostream& operator<<(std::ostream& stream, const ng::ExpressionTree::Node& node);

    };

    std::ostream& operator<<(std::ostream& ostream, const ng::ExpressionTree::Node& node);

} // namespace ng