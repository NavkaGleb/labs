#include "DataStructures/Trees/ExpressionTree.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <utility>
#include <sstream>

namespace ng {
        
    ///* ------------------------------------------ class Node ------------------------------------------ *///
    // constructor / destructor
    ExpressionTree::Node::Node(const std::string& value, Node* left, Node* right)
        : _type(ExpressionTree::_getNodeType(value)), _value(value), _left(left), _right(right) {}
    
    ExpressionTree::Node::Node(std::string value, short type, Node* left, Node* right)
        : _type(type), _value(std::move(value)), _left(left), _right(right) {}
    
    ExpressionTree::Node::Node(const Node& other) {
    
        this->_type = other._type;
        this->_value = other._value;
    
        this->_left = other._left ? new Node(*other._left) : nullptr;
        this->_right = other._right ? new Node(*other._right) : nullptr;
    
    }
    
    ExpressionTree::Node::~Node() {
    
        delete this->_left;
        delete this->_right;
    
    }
    
    // accessors
    const short& ExpressionTree::Node::type() const { return this->_type; }
    
    const std::string& ExpressionTree::Node::value() const { return this->_value; }
    
    const ExpressionTree::Node* ExpressionTree::Node::left() const { return this->_left; }
    
    const ExpressionTree::Node* ExpressionTree::Node::right() const { return this->_right; }
    
    // operators
    bool ExpressionTree::Node::operator==(const Node& other) const {
    
        bool result;
    
        result = this->_type == other._type && this->_value == other._value;
    
        if (result) result = (this->_left && other._left) ? *this->_left == *other._left : !this->_left && !other._left;
        if (result) result = (this->_right && other._right) ? *this->_right == *other._right : !this->_right && !other._right;
    
        return result;
    
    }

    std::ostream& operator<<(std::ostream& stream, const ExpressionTree::Node& node) {

        stream << node.value();

        if (node.left() || node.right()) {

            stream << " (l: ";

            if (node.left()) {

                stream << node.left()->value();
                stream << ", r: ";

            } else {

                stream << "null, r: ";

            }

            if (node.right()) {

                stream << node.right()->value();
                stream << ")";

            } else {

                stream << "null)";

            }

        }

        return stream;

    }
    
    ///* ------------------------------------------ class ExpressionTree ------------------------------------------ *///
    // constructor / destructor
    ExpressionTree::ExpressionTree() : _root(nullptr) {}
    
    ExpressionTree::ExpressionTree(std::string expression) : _root(nullptr), _expression(std::move(expression)) {

        this->_build();
    
    }
    
    ExpressionTree::ExpressionTree(Node* node) : _root(node) {

        this->_initVariables(this->_root);
        this->_expression = this->fullParenthesisExpression();

    }
    
    ExpressionTree::ExpressionTree(const ExpressionTree& other) : _root(new Node(*other._root)) {}
    
    ExpressionTree::~ExpressionTree() { delete this->_root; }
    
    // accessors
    bool ExpressionTree::empty() const { return this->_root == nullptr; }

    const std::string& ExpressionTree::expression() const { return this->_expression; }

    std::map<std::string, double> ExpressionTree::variables() const {

        std::map<std::string, double> result;

        for (const auto& [key, value] : this->_variables) {
            std::cout << key << " -> " << value.value << " " << value.count << std::endl;

            if (value.count != 0)
                result[key];

        }

        return result;

    }

    std::string ExpressionTree::fullParenthesisExpression() const {

        std::stringstream ss;

        this->_fullParenthesisExpression(this->_root, ss);

        return ss.str();

    }

    // modifiers
    void ExpressionTree::expression(const std::string& expression) {

        this->_expression = expression;
        this->_build();

    }

    void ExpressionTree::expression(std::string&& expression) {

        this->_expression = expression;
        this->_build();

    }

    // public methods
    void ExpressionTree::clear() { this->_clear(); }

    void ExpressionTree::simplify() { this->_simplify(this->_root); }
    
    double ExpressionTree::calc(const std::map<std::string, double>& variables) {

        if (this->_variables.empty() && !variables.empty()) {

            std::cerr << "no variable in expression!" << std::endl;
            return 0.0;

        }

        for (const auto& [key, value] : variables) {

            if (this->_variables.count(key) == 1 && this->_variables[key].count != 0)
                this->_variables[key].value = value;
            else if (this->_variables.count(key) == 1 && this->_variables[key].count == 0)
                this->_variables.erase(key);

        }

        return this->_calc(this->_root);
    
    }

    
    ExpressionTree* ExpressionTree::ndifferentiate(const std::string& variable) const {

        if (this->_variables.count(variable) == 0) {

            std::cerr << "no such variable" << std::endl;
            return nullptr;

        }

        return new ExpressionTree(this->_derivative(this->_root, variable));
    
    }
    
    ExpressionTree& ExpressionTree::differentiate(const std::string& variable) {

        if (this->_variables.count(variable) == 0) {

            std::cerr << "no such variable" << std::endl;

        } else {
    
            Node* newRoot = this->_derivative(this->_root, variable);
            delete this->_root;
            this->_root = newRoot;
            this->_variables.clear();
            this->_initVariables(this->_root);
    
        }
    
        return *this;
    
    }
    
    void ExpressionTree::vprint() const {
    
        for (const auto& p : this->_variables)
            std::cout << p.first << " ";
        std::cout << std::endl;
    
    }

    // operators
    std::istream& operator>>(std::istream& stream, ExpressionTree& tree) {

        tree.clear();
        stream >> tree._expression;

        tree._build();

        return stream;

    }

    std::ostream& operator<<(std::ostream& stream, const ExpressionTree& tree) {

        tree._print(stream, tree._root, 1, "root");
        return stream;

    }
    
    // private methods
    bool ExpressionTree::_isOperator(char symbol) {
    
        return symbol == '-' || symbol == '+' || symbol == '*' || symbol == '/' || symbol == '^';
    
    }
    
    bool ExpressionTree::_isOperator(char symbol, char prevSymbol) {
    
        return ExpressionTree::_isOperator(symbol) && prevSymbol != '(';
    
    }
    
    bool ExpressionTree::_isOperator(const std::string& value) {
    
        return value.length() == 1 && ExpressionTree::_isOperator(value.front());
    
    }
    
    bool ExpressionTree::_isUnaryOperator(const std::string& value) {
    
        return value.length() >= 1 && value.front() == '-';
    
    }
    
    bool ExpressionTree::_isDigit(const char& symbol) { return '0' <= symbol && symbol <= '9'; }
    
    bool ExpressionTree::_isConstant(const std::string& operand) {
    
        int point = 0;
    
        for (int i = 0; i < operand.length(); i++) {
    
            if (operand[i] == '-') {
    
                if (i == 0)
                    continue;
                else
                    return false;
    
            }
    
            if (operand[i] == '.') {
    
                if (++point > 1)
                    return false;
    
                continue;
    
            }
    
            if (!ExpressionTree::_isDigit(operand[i]))
                return false;
    
        }
    
        return true;
    
    }
    
    bool ExpressionTree::_isMathConstant(const std::string& operand) {
    
        return operand.find("PI") != std::string::npos ||
               (operand.length() == 1 && operand.front() == 'e') ||
               (operand.length() == 2 && operand.front() == '-' && operand.back() == 'e');
    
    }
    
    bool ExpressionTree::_isVariable(const std::string& operand) {
    
        for (int i = 0; i < operand.length(); i++) {
    
            if (i == 0 && operand[i] == '-')
                continue;
            if (('a' <= operand[i] && operand[i] <= 'z') || ('A' <= operand[i] && operand[i] <= 'Z'))
                continue;
            if (ExpressionTree::_isDigit(operand[i]) && i != (operand[0] == '-'))
                continue;
            return false;
    
        }
    
        return true;
    
    }
    
    bool ExpressionTree::_isFunction(const std::string& operand) {
    
        static std::string functions[] = {
            "sin", "cos", "tan",
            "asin", "acos", "atan",
            "ln", "log2", "log10",
            "sqrt", "exp"
        };
    
        for (const auto& function : functions)
            if (operand.find(function) != std::string::npos)
                return true;
    
        return false;
    
    }
    
    int ExpressionTree::_priority(char symbol) {
    
        if (symbol == '^')
            return 3;
    
        if (symbol == '*' || symbol == '/')
            return 2;
    
        if (symbol == '+' || symbol == '-')
            return 1;
    
        return 0;
    
    }
    
    bool ExpressionTree::_morePriority(char symbol) const {
    
        if (this->_operators.empty())
            return true;
    
        return ExpressionTree::_priority(symbol) > ExpressionTree::_priority(this->_operators.top()->_value[0]);
    
    }
    
    short ExpressionTree::_getNodeType(const std::string& value) {
    
        if (ExpressionTree::_isOperator(value))
            return NodeType::Operator;
    
        if (ExpressionTree::_isFunction(value))
            return NodeType::Func;
    
        if (ExpressionTree::_isConstant(value))
            return NodeType::Const;
    
        if (ExpressionTree::_isVariable(value))
            return NodeType::Var;
    
        return NodeType::Unknown;
    
    }
    
    double ExpressionTree::_mathConstants(const std::string& constant) {

        static std::map<std::string, double> mathConstants {
            { "PI", 3.14159265358979323846 },
            { "e", 2.71828182845904523536 }
        };
    
        return mathConstants[constant];
    
    }
    
    void ExpressionTree::_clearOperators(char symbol) {
    
        /* clear stack of operators */
    
        while (!this->_operators.empty()) {
    
            if (this->_operators.top()->_value == "(") {
    
                if (symbol == ')') {
    
                    delete this->_operators.top();
                    this->_operators.pop();
    
                    if (!this->_operators.empty() && this->_isFunction(this->_operators.top()->_value)) {
    
                        this->_postfixExpression.emplace_back(this->_operators.top());
                        this->_operators.pop();
    
                    }
    
                }
    
                return;
    
            }
    
            if (this->_morePriority(symbol))
                return;
    
            this->_postfixExpression.emplace_back(this->_operators.top());
            this->_operators.pop();
    
        }
    
    }
    
    void ExpressionTree::_clearOperand(std::string& operand) {
    
        if (!operand.empty()) {
    
            if (ExpressionTree::_isFunction(operand)) {
    
                if (ExpressionTree::_isUnaryOperator(operand)) {
    
                    this->_clearOperators('*');
                    this->_operators.push(new Node("*"));
                    this->_postfixExpression.emplace_back(new Node("-1"));
                    operand.erase(operand.begin());
    
                }
    
                this->_operators.push(new Node(operand, NodeType::Func));
    
            } else if (ExpressionTree::_isMathConstant(operand)) {
    
                operand = std::to_string((operand.front() == '-' ? -1.0 : 1.0) * ExpressionTree::_mathConstants(operand));
                this->_postfixExpression.emplace_back(new Node(operand));
    
            } else {
    
                if (ExpressionTree::_isVariable(operand)) {
    
                    if (this->_isUnaryOperator(operand)) {
    
                        this->_clearOperators('*');
                        this->_operators.push(new Node("*"));
                        this->_postfixExpression.emplace_back(new Node("-1"));
                        operand.erase(operand.begin());
    
                        if (operand.empty()) return;
    
                    }
    
                    ++this->_variables[operand].count;
    
                }
    
                this->_postfixExpression.emplace_back(new Node(operand));
    
            }
    
            operand.clear();
    
        }
    
    }
    
    void ExpressionTree::_initVariables(Node* node) {

        if (!node)
            return;
    
        this->_initVariables(node->_left);
    
        if (node->_type == NodeType::Var)
            ++this->_variables[node->_value].count;
    
        this->_initVariables(node->_right);
    
    }
    
    void ExpressionTree::_build(Node* parent, bool init) {

        static int index;

        if (init)
            index = this->_postfixExpression.size() - 1;

        if (!this->_root) {
    
            this->_root = this->_postfixExpression[index--];
            this->_build(this->_root);
    
        }
    
        if (!parent)
            return;
    
        if (parent->_type == NodeType::Operator) {
    
            Node* right = index >= 0 ? this->_postfixExpression[index--] : nullptr;
    
            if (right && (right->_type == NodeType::Operator || right->_type == NodeType::Func))
                this->_build(right);

            Node* left = index >= 0 ? this->_postfixExpression[index--] : nullptr;
    
            if (left && (left->_type == NodeType::Operator || left->_type == NodeType::Func))
                this->_build(left);
    
            parent->_left = left;
            parent->_right = right;
    
        } else if (parent->_type == NodeType::Func) {
    
            Node* left = index >= 0 ? this->_postfixExpression[index--] : nullptr;
    
            if (left && (left->_type == NodeType::Operator || left->_type == NodeType::Func))
                this->_build(left);
    
            parent->_left = left;
            parent->_right = nullptr;
    
        }
    
    }
    
    void ExpressionTree::_build() {

        std::string operand;
        char prevSymbol = '(';
        char symbol;
        int parentheses = 0;
    
        for (int i = 0; i < this->_expression.length(); i++) {
    
            if (i != 0) prevSymbol = this->_expression[i - 1];
            symbol = this->_expression[i];
    
            if (symbol == '(') {
    
                parentheses++;
                this->_clearOperand(operand);
                this->_operators.push(new Node(std::string(1, symbol)));
    
            } else if (symbol == ')') {
    
                parentheses--;
                this->_clearOperand(operand);
                this->_clearOperators(symbol);
    
            } else if (this->_isOperator(symbol, prevSymbol)) {
    
                this->_clearOperand(operand);
                this->_clearOperators(symbol);
                this->_operators.push(new Node(std::string(1, symbol)));
    
            } else {
    
                operand.push_back(symbol);
    
            }
    
        }
    
        this->_clearOperand(operand);
        this->_clearOperators(symbol);
    
        if (parentheses != 0) {

            if (parentheses >= 0)
                throw std::invalid_argument("to much open parentheses");

            throw std::invalid_argument("to much close parentheses");
    
        }

        this->_build(this->_root, true);
    
    }
    
    bool ExpressionTree::_exist(Node* node, const std::string& value) const {

        if (!node)
            return false;
        if (node->_value == value)
            return true;
        if (this->_exist(node->_left, value))
            return true;
        return this->_exist(node->_right, value);
    
    }
    
    void ExpressionTree::_clear() {
    
        this->_expression.clear();
        this->_postfixExpression.clear();
        this->_variables.clear();
    
        delete this->_root;
        this->_root = nullptr;
    
    }
    
    double ExpressionTree::_calc(Node* node) const {
    
        if (!node->_left && !node->_right)
            return (node->_type == NodeType::Var) ? this->_variables.at(node->_value).value : std::stod(node->_value);
    
        double result = 0.f;
    
        if (node->_type == NodeType::Operator) {
    
            if (node->_value == "+")
                result = this->_calc(node->_left) + this->_calc(node->_right);
            if (node->_value == "-")
                result = this->_calc(node->_left) - this->_calc(node->_right);
            if (node->_value == "*")
                result = this->_calc(node->_left) * this->_calc(node->_right);
            if (node->_value == "/")
                result = this->_calc(node->_left) / this->_calc(node->_right);
            if (node->_value == "^")
                result = std::pow(this->_calc(node->_left), this->_calc(node->_right));
    
        } else if (node->_type == NodeType::Func) {
    
            if (node->_value == "sin")
                result = std::sin(this->_calc(node->_left));
            if (node->_value == "cos")
                result = std::cos(this->_calc(node->_left));
            if (node->_value == "asin")
                result = std::asin(this->_calc(node->_left));
            if (node->_value == "acos")
                result = std::acos(this->_calc(node->_left));
            if (node->_value == "atan")
                result = std::atan(this->_calc(node->_left));
            if (node->_value == "ln")
                result = std::log(this->_calc(node->_left));
            if (node->_value == "log2")
                result = std::log2(this->_calc(node->_left));
            if (node->_value == "log10")
                result = std::log10(this->_calc(node->_left));
            if (node->_value == "sqrt")
                result = std::sqrt(this->_calc(node->_left));
            if (node->_value == "exp")
                result = std::exp(this->_calc(node->_left));
    
        }
    
        return result;
    
    }
    
    void ExpressionTree::_fullParenthesisExpression(Node* node, std::ostream& stream) const {
    
        if (!node)
            return;
    
        if (node->_type == NodeType::Operator)
            stream << "(";
    
        else if (node->_type == NodeType::Func)
            stream << node->_value << "<";
    
        this->_fullParenthesisExpression(node->_left, stream);
    
        if (node->_type != NodeType::Func)
            stream << node->_value;
    
        if (node->_type == NodeType::Func)
            stream << ">";
    
        this->_fullParenthesisExpression(node->_right, stream);
    
        if (node->_type == NodeType::Operator)
            stream << ")";
    
    }
    
    void ExpressionTree::_updateVariable(const std::string& key) {

        if (--this->_variables[key].count == 0)
            this->_variables.erase(key);
    
    }
    
    void ExpressionTree::_updateVariables(Node* node) {

        if (!node)
            return;
    
        this->_updateVariables(node->_left);
    
        if (node->_type == NodeType::Var)
            this->_updateVariable(node->_value);
    
        this->_updateVariables(node->_right);
    
    }
    
    void ExpressionTree::_updateNode(Node*& node, const std::string& value, const short& type) const {
    
        if (node->_type == NodeType::Operator) {
    
            delete node->_left;
            delete node->_right;
            node->_left = nullptr;
            node->_right = nullptr;
    
        } else if (node->_type == NodeType::Func) {
    
            delete node->_left;
            node->_left = nullptr;
    
        }
    
        node->_value = value;
        node->_type = type;
    
    }
    
    void ExpressionTree::_updateNode(Node*& node) const {
    
        Node* newNode;
    
        if (node->_left->_type == NodeType::Const) {
    
            newNode = new Node(*node->_right);
            node->_right = nullptr;
    
        } else {
    
            newNode = new Node(*node->_left);
            node->_left = nullptr;
    
        }
    
        delete node;
        node = newNode;
    
    }
    
    void ExpressionTree::_simplify(Node*& node) {
    
        if (!node || (node->_type != NodeType::Operator && node->_type != NodeType::Func))
            return;
    
        this->_simplify(node->_left);
        this->_simplify(node->_right);
    
        if (node->_type == NodeType::Operator) {
    
            Node* left = node->_left;
            Node* right = node->_right;
    
            if (left->_type == NodeType::Const && right->_type == NodeType::Const) {
    
                /* all children are CONSTANT */
    
                // removing trailing zeros
                std::string value = std::to_string(this->_calc(node));
                value.erase(value.find_last_not_of('0') + 1, std::string::npos);
                value.erase(value.find_last_not_of('.') + 1, std::string::npos);
    
                ExpressionTree::_updateNode(node, value, NodeType::Const);
    
            } else if (left->_type == NodeType::Const || right->_type == NodeType::Const) {
    
                if (left->_type == NodeType::Const && std::stod(left->_value) == 0.0) {
    
                    /* left child is CONSTANT and equals 0.0 */
    
                    if (node->_value == "+" || node->_value == "-") {
    
                        ExpressionTree::_updateNode(node);
    
                    } else if (node->_value == "*" || node->_value == "/" || node->_value == "^") {
    
                        this->_updateVariables(right);
                        ExpressionTree::_updateNode(node, "0", NodeType::Const);
    
                    }
    
                } else if (right->_type == NodeType::Const && std::stod(right->_value) == 0.0) {
    
                    /* right child is CONSTANT and equals 0.0 */
    
                    if (node->_value == "+" || node->_value == "-") {
    
                        ExpressionTree::_updateNode(node);
    
                    } else if (node->_value == "*") {
    
                        this->_updateVariables(left);
                        ExpressionTree::_updateNode(node, "0", NodeType::Const);
    
                    } else if (node->_value == "^") {
    
                        this->_updateVariables(left);
                        ExpressionTree::_updateNode(node, "1", NodeType::Const);
    
                    }
    
                } else if (left->_type == NodeType::Const && std::stod(left->_value) == 1.0) {
    
                    /* left child is CONSTANT and equals 1.0 */
    
                    if (node->_value == "*") {
    
                        ExpressionTree::_updateNode(node);
    
                    } else if (node->_value == "^") {
    
                        this->_updateVariables(right);
                        ExpressionTree::_updateNode(node, "1", NodeType::Const);
    
                    }
    
                } else if (right->_type == NodeType::Const && std::stod(right->_value) == 1.0) {
    
                    /* right child is CONSTANT and equals 1.0 */
    
                    if (node->_value == "*" || node->_value == "/" || node->_value == "^") {
    
                        ExpressionTree::_updateNode(node);
    
                    }
    
                }
    
            }
    
        } else if (node->_type == NodeType::Func && node->_left->_type == NodeType::Const) {
    
            // removing trailing zeros
            std::string value = std::to_string(this->_calc(node));
            value.erase(value.find_last_not_of('0') + 1, std::string::npos);
            value.erase(value.find_last_not_of('.') + 1, std::string::npos);
    
            ExpressionTree::_updateNode(node, value, NodeType::Const);
    
        }
    
    }
    
    ExpressionTree::Node* ExpressionTree::_derivative(Node* node, const std::string& variable) const {
    
        if (node->_type == NodeType::Var && node->_value == variable) {

            return new Node("1");
    
        } else if (node->_type == NodeType::Const || node->_type == NodeType::Var) {
    
            return new Node("0");
    
        } else if (node->_type == NodeType::Operator) {
    
            if (node->_value == "+") {
    
                Node* left = this->_derivative(node->_left, variable);
                Node* right = this->_derivative(node->_right, variable);
                return new Node("+", left, right);
    
            } else if (node->_value == "-") {
    
                Node* left = this->_derivative(node->_left, variable);
                Node* right = this->_derivative(node->_right, variable);
                return new Node("-", left, right);
    
            } else if (node->_value == "*") {
    
                Node* left = new Node("*", this->_derivative(node->_left, variable), new Node(*node->_right));
                Node* right = new Node("*", new Node(*node->_left), this->_derivative(node->_right, variable));
                return new Node("+", left, right);
    
            } else if (node->_value == "/") {
    
                Node* left = new Node("-");
                left->_left = new Node("*", this->_derivative(node->_left, variable), new Node(*node->_right));
                left->_right = new Node("*", new Node(*node->_left), this->_derivative(node->_right, variable));
                Node* right = new Node("^", new Node(*node->_right), new Node("2"));
                return new Node("/", left, right);
    
            } else if (node->_value == "^" && this->_exist(node->_left, variable) && this->_exist(node->_right, variable)) {
    
                Node* left = new Node("exp", new Node("*", new Node("ln", new Node(*node->_left)), new Node(*node->_right)));
                Node* right = this->_derivative(new Node("*", new Node("ln", new Node(*node->_left)), new Node(*node->_right)), variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "^" && this->_exist(node->_left, variable)) {
    
                Node* left = new Node("*");
                left->_left = new Node(*node->_right);
                left->_right = new Node("^", new Node(*node->_left), new Node("-", new Node(*node->_right), new Node("1")));
                Node* right = this->_derivative(node->_left, variable);
    
                return new Node("*", left, right);
    
            } else if (node->_value == "^" && this->_exist(node->_right, variable)) {
    
                Node* left = new Node("*", new Node(*node), new Node("ln", new Node(*node->_left)));
                Node* right = this->_derivative(node->_right, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "^") {
    
                return new Node("0");
    
            }
    
        } else if (node->_type == NodeType::Func) {
    
            if (node->_value == "sin") {
    
                Node* left = new Node("cos", new Node(*node->_left));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "cos") {

                Node* left = new Node("*", new Node("-1"), new Node("sin", new Node(*node->_left)));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "tan") {
    
                Node* left = new Node("/", new Node("1"), new Node("^", new Node("cos", new Node(*node->_left)), new Node("2")));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "asin") {
    
                Node* left = new Node("/");
                left->_left = new Node("1");
                left->_right = new Node("sqrt", new Node("-", new Node("1"), new Node("^", new Node(*node->_left), new Node("2"))));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "acos") {
    
                Node* left = new Node("/");
                left->_left = new Node("1");
                left->_right = new Node("sqrt", new Node("-", new Node("1"), new Node("^", new Node(*node->_left), new Node("2"))));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", new Node("*", new Node("-1"), left), right);
    
            } else if (node->_value == "atan") {
    
                Node* left = new Node("/");
                left->_left = new Node("1");
                left->_right = new Node("+", new Node("1"), new Node("^", new Node(*node->_left), new Node("2")));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "ln") {
    
                Node* left = new Node("/", new Node("1"), new Node(*node->_left));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "log2") {
    
                Node* left = new Node("/", new Node("1"), new Node("*", new Node(*node->_left), new Node("ln", new Node("2"))));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "log10") {
    
                Node* left = new Node("/", new Node("1"), new Node("*", new Node(*node->_left), new Node("ln", new Node("10"))));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "sqrt") {
    
                Node* left = new Node("*", new Node("0.5"), new Node("^", new Node(*node->_left), new Node("-0.5")));
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);
    
            } else if (node->_value == "exp") {

                Node* left = new Node(*node->_left);
                Node* right = this->_derivative(node->_left, variable);
                return new Node("*", left, right);

            }
    
        }
    
        return nullptr;
    
    }
    
    void ExpressionTree::_print(std::ostream& stream, Node* node, const int& level,
        const char* caption, bool nextLine) const {
    
        if (!node) {

            stream << caption << ": null" << std::endl;
            return;
    
        }

        stream << caption << ": " << *node;
    
        if (node->_left || node->_right) {

            stream << " (" << std::endl;
    
            for (int i = 0; i < level; i++) stream << "| ";
            this->_print(stream, node->_left, level + 1, "left", true);
    
            for (int i = 0; i < level; i++) stream << "| ";
            this->_print(stream, node->_right, level + 1, "right", true);
    
            for (int i = 0; i < level - 1; i++) stream << "| ";
            stream << ")";
    
        }

        if (nextLine)
            stream << std::endl;
    
    }
    
}