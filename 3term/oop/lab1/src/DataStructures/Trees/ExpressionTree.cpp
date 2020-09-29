#include "DataStructures/Trees/ExpressionTree.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <utility>
#include <sstream>

///* ------------------------------------------ class Node ------------------------------------------ *///
// constructor / destructor
ng::ExpressionTree::Node::Node(const std::string& value, Node* left, Node* right)
    : _type(ng::ExpressionTree::_getNodeType(value)), _value(value), _left(left), _right(right) {}

ng::ExpressionTree::Node::Node(std::string value, short type, Node* left, Node* right)
    : _type(type), _value(std::move(value)), _left(left), _right(right) {}

ng::ExpressionTree::Node::Node(const Node& other) {

    this->_type = other._type;
    this->_value = other._value;

    this->_left = other._left ? new Node(*other._left) : nullptr;
    this->_right = other._right ? new Node(*other._right) : nullptr;

}

ng::ExpressionTree::Node::~Node() {

    delete this->_left;
    delete this->_right;

}

// accessors
const short& ng::ExpressionTree::Node::type() const {

    return this->_type;

}

const std::string& ng::ExpressionTree::Node::value() const {

    return this->_value;

}

const ng::ExpressionTree::Node* ng::ExpressionTree::Node::left() const {

    return this->_left;

}

const ng::ExpressionTree::Node* ng::ExpressionTree::Node::right() const {

    return this->_right;

}

// operators
bool ng::ExpressionTree::Node::operator==(const Node& other) const {

    bool result;

    result = this->_type == other._type && this->_value == other._value;

    if (result) result = (this->_left && other._left) ? *this->_left == *other._left : !this->_left && !other._left;
    if (result) result = (this->_right && other._right) ? *this->_right == *other._right : !this->_right && !other._right;

    return result;

}

///* ------------------------------------------ class ng::ExpressionTree ------------------------------------------ *///
// constructor / destructor
ng::ExpressionTree::ExpressionTree() : _root(nullptr), _index(0), _error(false), _result(nullptr) {}

ng::ExpressionTree::ExpressionTree(std::string expression)
    : _root(nullptr), _index(0), _error(false), _expression(std::move(expression)), _result(nullptr) {

    this->_build();

}

ng::ExpressionTree::ExpressionTree(Node* node) : _root(node), _index(0), _error(false), _result(nullptr) {

    this->_initVariables(this->_root);

}

ng::ExpressionTree::ExpressionTree(const ng::ExpressionTree& other)
    : _root(new Node(*other._root)), _index(0), _error(false), _result(nullptr) {}

ng::ExpressionTree::~ExpressionTree() {

    delete this->_root;
    delete this->_result;

}

// accessors
bool ng::ExpressionTree::empty() const { return this->_root == nullptr; }

// public methods
void ng::ExpressionTree::clear() { this->_clear(); }

std::string ng::ExpressionTree::error() const {

    if (this->_error) return this->_errorMessage;
    return "no error";

}

void ng::ExpressionTree::input() {

    this->_clear();
    std::cin >> this->_expression;
    this->_build();

}

void ng::ExpressionTree::build(const std::string& expression) {

    this->_clear();
    this->_expression = expression;
    this->_build();

}

double ng::ExpressionTree::result() {

    return (!this->_result) ? *(this->_result = new double(this->calc())) : *this->_result;

}

double ng::ExpressionTree::calc() {

    if (!this->_variables.empty())
        std::cout << "enter value of variables" << std::endl;

    for (auto& p : this->_variables) {

        std::cout << p.first << ":";
        std::cin >> p.second._value;

    }

    delete this->_result;
    return *(this->_result = new double(this->_calc(this->_root)));

}

double ng::ExpressionTree::calc(const std::map<std::string, double>& variables) {

    for (auto& p : variables)
        this->_variables[p.first]._value = p.second;

    delete this->_result;
    return *(this->_result = new double(this->_calc(this->_root)));

}

void ng::ExpressionTree::bypass() const {

    if (!this->_error)
        this->_bypass(this->_root);
    else
        std::cout << this->_errorMessage;

    std::cout << std::endl;

}

void ng::ExpressionTree::simplify() {

    if (!this->_error)
        this->_simplify(this->_root);
    else
        std::cout << this->_errorMessage << std::endl;

}

ng::ExpressionTree* ng::ExpressionTree::ndifferentiate() const {

    std::string variable;

    return (!this->_selectVariable(variable)) ? nullptr : new ng::ExpressionTree(this->_derivative(this->_root, variable));

}

ng::ExpressionTree* ng::ExpressionTree::ndifferentiate(const std::string& variable) const {

    return (this->_variables.count(variable) == 0) ? nullptr : new ng::ExpressionTree(this->_derivative(this->_root, variable));

}

ng::ExpressionTree& ng::ExpressionTree::differentiate() {

    std::string variable;

    if (this->_selectVariable(variable)) {

        Node* newRoot = this->_derivative(this->_root, variable);
        delete this->_root;
        this->_root = newRoot;
        this->_initVariables(this->_root);

    }

    return *this;

}

ng::ExpressionTree& ng::ExpressionTree::differentiate(const std::string& variable) {

    if (this->_variables.count(variable) != 0) {

        Node* newRoot = this->_derivative(this->_root, variable);
        delete this->_root;
        this->_root = newRoot;
        this->_initVariables(this->_root);

    }

    return *this;

}

void ng::ExpressionTree::print() const {

    if (!this->_error)
        this->_print(this->_root, 1, "root");
    else
        std::cout << this->_errorMessage << std::endl;

}

void ng::ExpressionTree::vprint() const {

    for (const auto& p : this->_variables)
        std::cout << p.first << " ";
    std::cout << std::endl;

}

// private methods
bool ng::ExpressionTree::_isOperator(char symbol) {

    return symbol == '-' || symbol == '+' || symbol == '*' || symbol == '/' || symbol == '^';

}

bool ng::ExpressionTree::_isOperator(char symbol, char prevSymbol) {

    return ng::ExpressionTree::_isOperator(symbol) && prevSymbol != '(';

}

bool ng::ExpressionTree::_isOperator(const std::string& value) {

    return value.length() == 1 && ng::ExpressionTree::_isOperator(value.front());

}

bool ng::ExpressionTree::_isUnaryOperator(const std::string& value) {

    return value.length() >= 1 && value.front() == '-';

}

bool ng::ExpressionTree::_isDigit(const char& symbol) { return '0' <= symbol && symbol <= '9'; }

bool ng::ExpressionTree::_isConstant(const std::string& operand) {

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

bool ng::ExpressionTree::_isMathConstant(const std::string& operand) {

    return operand.find("PI") != std::string::npos ||
           (operand.length() == 1 && operand.front() == 'e') ||
           (operand.length() == 2 && operand.front() == '-' && operand.back() == 'e');

}

bool ng::ExpressionTree::_isVariable(const std::string& operand) {

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

bool ng::ExpressionTree::_isFunction(const std::string& operand) {

    static std::string functions[] = {
        "sin", "cos", "tan",
        "asin", "acos", "atan",
        "ln", "log2", "log10",
        "sqrt", "abs", "exp"
    };

    for (const auto& function : functions)
        if (operand.find(function) != std::string::npos)
            return true;

    return false;

}

int ng::ExpressionTree::_priority(char symbol) {

    if (symbol == '^')
        return 3;

    if (symbol == '*' || symbol == '/')
        return 2;

    if (symbol == '+' || symbol == '-')
        return 1;

    return 0;

}

bool ng::ExpressionTree::_morePriority(char symbol) const {

    if (this->_operators.empty())
        return true;

    return ExpressionTree::_priority(symbol) > ExpressionTree::_priority(this->_operators.top()->_value[0]);

}

short ng::ExpressionTree::_getNodeType(const std::string& value) {

    if (ng::ExpressionTree::_isOperator(value))
        return NodeType::Operator;

    if (ng::ExpressionTree::_isFunction(value))
        return NodeType::Func;

    if (ng::ExpressionTree::_isConstant(value))
        return NodeType::Const;

    if (ng::ExpressionTree::_isVariable(value))
        return NodeType::Var;

    return NodeType::Unknown;

}

double ng::ExpressionTree::_mathConstants(const std::string& constant) {

    /* return math constant value */

    static std::map<std::string, double> mathConstants {
        { "PI", 3.14159265358979323846 },
        { "e", 2.71828182845904523536 }
    };

    return mathConstants[constant];

}

void ng::ExpressionTree::_clearOperators(char symbol) {

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

void ng::ExpressionTree::_clearOperand(std::string& operand) {

    if (!operand.empty()) {

        if (ng::ExpressionTree::_isFunction(operand)) {

            if (ng::ExpressionTree::_isUnaryOperator(operand)) {

                this->_clearOperators('*');
                this->_operators.push(new Node("*"));
                this->_postfixExpression.emplace_back(new Node("-1"));
                operand.erase(operand.begin());

            }

            this->_operators.push(new Node(operand, NodeType::Func));

        } else if (ng::ExpressionTree::_isMathConstant(operand)) {

            operand = std::to_string((operand.front() == '-' ? -1.0 : 1.0) * ng::ExpressionTree::_mathConstants(operand));
            this->_postfixExpression.emplace_back(new Node(operand));

        } else {

            if (ng::ExpressionTree::_isVariable(operand)) {

                if (this->_isUnaryOperator(operand)) {

                    this->_clearOperators('*');
                    this->_operators.push(new Node("*"));
                    this->_postfixExpression.emplace_back(new Node("-1"));
                    operand.erase(operand.begin());

                    if (operand.empty()) return;

                }

                this->_variables[operand]._count++;

            }

            this->_postfixExpression.emplace_back(new Node(operand));

        }

        operand.clear();

    }

}

void ng::ExpressionTree::_initVariables(Node* node) {

    if (!node)
        return;

    this->_initVariables(node->_left);

    if (node->_type == NodeType::Var)
        this->_variables[node->_value]._count++;

    this->_initVariables(node->_right);

}

void ng::ExpressionTree::_build(Node* parent) {

    if (!this->_root) {

        this->_root = this->_postfixExpression[this->_index--];
        this->_build(this->_root);

    }

    if (!parent)
        return;

    if (parent->_type == NodeType::Operator) {

        Node* right = this->_index >= 0 ? this->_postfixExpression[this->_index--] : nullptr;

        if (right && (right->_type == NodeType::Operator || right->_type == NodeType::Func))
            this->_build(right);


        Node* left = this->_index >= 0 ? this->_postfixExpression[this->_index--] : nullptr;

        if (left && (left->_type == NodeType::Operator || left->_type == NodeType::Func))
            this->_build(left);

        parent->_left = left;
        parent->_right = right;

    } else if (parent->_type == NodeType::Func) {

        Node* left = this->_index >= 0 ? this->_postfixExpression[this->_index--] : nullptr;

        if (left && (left->_type == NodeType::Operator || left->_type == NodeType::Func))
            this->_build(left);

        parent->_left = left;
        parent->_right = nullptr;

    }

}

void ng::ExpressionTree::_build() {

    /* build _postfixNotation base on _expression */

    std::string operand;
    char prevSymbol = '(';
    char symbol;

    int parentheses = 0;
    bool operators = false;

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

        this->_error = true;
        this->_errorMessage = parentheses >= 0 ? "to much open parentheses" : "to much close parentheses";

    }

    this->_index = static_cast<int>(this->_postfixExpression.size()) - 1;
    this->_build(this->_root);

}

bool ng::ExpressionTree::_exist(Node* node, const std::string& value) const {

    /* check if value exist in tree */

    if (!node)
        return false;
    if (node->_value == value)
        return true;
    if (this->_exist(node->_left, value))
        return true;
    return this->_exist(node->_right, value);

}

void ng::ExpressionTree::_clear() {

    /* clear tree fields */

    this->_expression.clear();
    this->_postfixExpression.clear();
    this->_variables.clear();

    delete this->_root;
    this->_root = nullptr;

}

double ng::ExpressionTree::_calc(Node* node) const {

    if (!node->_left && !node->_right)
        return (node->_type == NodeType::Var) ? this->_variables.at(node->_value)._value : std::stod(node->_value);

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
        if (node->_value == "abs")
            result = std::abs(this->_calc(node->_left));
        if (node->_value == "exp")
            result = std::exp(this->_calc(node->_left));

    }

    return result;

}

void ng::ExpressionTree::_bypass(Node* node) const {

    if (!node)
        return;

    if (node->_type == NodeType::Operator)
        std::cout << "(";

    else if (node->_type == NodeType::Func)
        std::cout << node->_value << "<";

    this->_bypass(node->_left);

    if (node->_type != NodeType::Func)
        std::cout << node->_value;

    if (node->_type == NodeType::Func)
        std::cout << ">";

    this->_bypass(node->_right);

    if (node->_type == NodeType::Operator)
        std::cout << ")";

}

void ng::ExpressionTree::_updateVariable(const std::string& key) {

    /* update amount of variable in tree. if no variable in tree delete it from _variables */

    if (--this->_variables[key]._count == 0)
        this->_variables.erase(key);

}

void ng::ExpressionTree::_updateVariables(Node* node) {

    if (!node)
        return;

    this->_updateVariables(node->_left);

    if (node->_type == NodeType::Var)
        this->_updateVariable(node->_value);

    this->_updateVariables(node->_right);

}

void ng::ExpressionTree::_updateNode(Node*& node, const std::string& value, const short& type) {

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

void ng::ExpressionTree::_updateNode(Node*& node) {

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

void ng::ExpressionTree::_simplify(Node*& node) {

    if (this->_error || !node || (node->_type != NodeType::Operator && node->_type != NodeType::Func))
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

            ng::ExpressionTree::_updateNode(node, value, NodeType::Const);

        } else if (left->_type == NodeType::Const || right->_type == NodeType::Const) {

            if (left->_type == NodeType::Const && std::stod(left->_value) == 0.0) {

                /* left child is CONSTANT and equals 0.0 */

                if (node->_value == "+" || node->_value == "-") {

                    ng::ExpressionTree::_updateNode(node);

                } else if (node->_value == "*" || node->_value == "/" || node->_value == "^") {

                    this->_updateVariables(right);
                    ng::ExpressionTree::_updateNode(node, "0", NodeType::Const);

                }

            } else if (right->_type == NodeType::Const && std::stod(right->_value) == 0.0) {

                /* right child is CONSTANT and equals 0.0 */

                if (node->_value == "+" || node->_value == "-") {

                    ng::ExpressionTree::_updateNode(node);

                } else if (node->_value == "*") {

                    this->_updateVariables(left);
                    ng::ExpressionTree::_updateNode(node, "0", NodeType::Const);

                } else if (node->_value == "^") {

                    this->_updateVariables(left);
                    ng::ExpressionTree::_updateNode(node, "1", NodeType::Const);

                }

            } else if (left->_type == NodeType::Const && std::stod(left->_value) == 1.0) {

                /* left child is CONSTANT and equals 1.0 */

                if (node->_value == "*") {

                    ng::ExpressionTree::_updateNode(node);

                } else if (node->_value == "^") {

                    this->_updateVariables(right);
                    ng::ExpressionTree::_updateNode(node, "1", NodeType::Const);

                }

            } else if (right->_type == NodeType::Const && std::stod(right->_value) == 1.0) {

                /* right child is CONSTANT and equals 1.0 */

                if (node->_value == "*" || node->_value == "/" || node->_value == "^") {

                    ng::ExpressionTree::_updateNode(node);

                }

            }

        }

    } else if (node->_type == NodeType::Func && node->_left->_type == NodeType::Const) {

        // removing trailing zeros
        std::string value = std::to_string(this->_calc(node));
        value.erase(value.find_last_not_of('0') + 1, std::string::npos);
        value.erase(value.find_last_not_of('.') + 1, std::string::npos);

        ng::ExpressionTree::_updateNode(node, value, NodeType::Const);

    }

}

bool ng::ExpressionTree::_selectVariable(std::string& variable) const {

    std::cout << "there are all variables in expression: ";
    this->vprint();
    std::cout << "choose a variable by which you will differentiate:";
    std::cin >> variable;

    if (this->_variables.count(variable) == 0) {

        std::cout << "no variable " << variable << std::endl;
        return false;

    }

    return true;

}

ng::ExpressionTree::Node* ng::ExpressionTree::_derivative(Node* node, const std::string& variable) const {

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

        }

    }

    return nullptr;

}

void ng::ExpressionTree::_print(Node* node, const int& level, const char* caption) const {

    if (!node) {

        std::cout << caption << ": null" << std::endl;
        return;

    }

    std::cout << caption << ": " << *node;

    if (node->_left || node->_right) {

        std::cout << " (" << std::endl;

        for (int i = 0; i < level; i++) std::cout << "| ";
        this->_print(node->_left, level + 1, "left");

        for (int i = 0; i < level; i++) std::cout << "| ";
        this->_print(node->_right, level + 1, "right");

        for (int i = 0; i < level - 1; i++) std::cout << "| ";
        std::cout << ")";

    }

    std::cout << std::endl;

}

// operators
bool ng::ExpressionTree::operator==(const ng::ExpressionTree& other) const {

    return *this->_root == *other._root;

}

std::ostream& ng::operator<<(std::ostream& ostream, const ng::ExpressionTree::Node& node) {

    ostream << node.value();

    if (node.left() || node.right()) {

        ostream << " (l: ";

        if (node.left()) {

            ostream << node.left()->value();
            ostream << ", r: ";

        } else {

            ostream << "null, r: ";

        }

        if (node.right()) {

            ostream << node.right()->value();
            ostream << ")";

        } else {

            ostream << "null)";

        }

    }

    return ostream;

}