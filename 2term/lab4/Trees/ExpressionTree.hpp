#pragma once

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>

#include "Tree.hpp"

namespace ng {

	class ExpressionTree : public Tree {
	public:
		// inner class
		class Node {
			// friends
			friend ExpressionTree;

		private:
			// variables
			short _type;
			std::string _value;
			Node* _left;
			Node* _right;

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

		};

	private:
		// enums
		enum NODE_TYPE { UNKNOWN = -1, OPERATOR, FUNCTION, CONSTANT, VARIABLE };

		// inner class
		class Variable {
		public:
			// variables
			double _value;
			int _count;

			// constructor / destructor;
			Variable();
			virtual ~Variable() = default;

		};

		// variables
		Node* _root;                                                                        // root of the tree
		std::string _type;                                                                  // type of the tree
		std::string _expression;                                                            // math expression in normal form
		std::stack<Node*> _operators;                                                       // stack of operators for _postfixExpression
		std::vector<Node*> _postfixExpression;                                              // postfix form of _expression
		std::map<std::string, Variable> _variables;                                         // store the variables from _expression
		int _index;                                                                         // current index in _postfixExpression
		bool _error;                                                                        // tracks the error in _expression
		std::string _errorMessage;                                                          // kind of error
		double* _result;                                                                    // result of expression

		// private methods
		static bool _isOperator(const char& symbol);                                        // check if symbol is operator
		static bool _isOperator(const char& symbol, const char& prevSymbol);                // check if symbol is operator
		static bool _isOperator(const std::string& value);                                  // check if string is operator
		static bool _isUnaryOperator(const std::string& value);                             // check if first element is operator
		static bool _isDigit(const char& symbol);                                           // check if symbol is digit
		static bool _isConstant(const std::string& operand);                                // check if string is constant
		static bool _isMathConstant(const std::string& operand);                            // check if string is math constant
		static bool _isVariable(const std::string& operand);                                // check if string is variable
		static bool _isFunction(const std::string& value);                                  // check if string is basic function
		static short _getNodeType(const std::string& value);                                // return type of node (NODE_TYPE)
		static double _mathConstants(const std::string& constant);                          // return math constant

		static int _priority(const char& symbol);                                           // check priority of operators
		[[nodiscard]] bool _morePriority(const char& symbol) const;                         // check priority between to operators

		void _clearOperators(const char& symbol);                                           // clear stack of operators
		void _clearOperand(std::string& operand);                                           // clear current operand

		void _initVariables(Node* node);                                                    // count variables in expression tree

		void _build(Node* parent);                                                          // build the tree
		void _build();                                                                      // build the postfix expression

		bool _exist(Node* node, const std::string& value) const;                            // check if value exist in sub tree

		void _clear();                                                                      // clear all private fields
		double _calc(Node* node) const;                                                     // calculate the result
		void _bypass(Node* node) const;                                                     // get full parentheses expression

		void _updateVariable(const std::string& key);                                       // update count of variable
		void _updateVariables(Node* node);                                                  // update variables in subtree
		static void _updateNode(Node*& node, const std::string& value, const short& type);  // update node values
		static void _updateNode(Node*& node);                                               // update node values
		void _simplify(Node*& node);                                                        // simplify tree

		bool _selectVariable(std::string& variable) const;                                  // select on of variables to differentiate
		Node* _derivative(Node* node, const std::string& variable) const;                   // get derivative of current node

		void _print(Node* node, const int& level, const char* caption) const;               // tab print

	public:
		// constructor / destructor
		ExpressionTree();
		explicit ExpressionTree(std::string expression);
		explicit ExpressionTree(Node* node);
		ExpressionTree(const ExpressionTree& other);
		~ExpressionTree() override;

		// accessors
		[[nodiscard]] const std::string& type() const override;                             // get type of the tree
		[[nodiscard]] bool empty() const override;                                          // return tree is empty

		// public methods
		void clear();                                                                       // clear all fields
		[[nodiscard]] std::string error() const;                                            // get error

		void input();                                                                       // enter the expression and build the tree
		void build(const std::string& expression);                                          // set the expression and build the tree
		double result();                                                                    // return the result (based on prev values)
		double calc();                                                                      // return the result (based on new values)
		double calc(const std::map<std::string, double>& variables);                        // return the result (based on new values)
		void bypass() const;                                                                // get full parentheses expression

		void simplify();                                                                    // simplify tree

		[[nodiscard]] ExpressionTree* ndifferentiate() const;                               // differentiate _expression
		[[nodiscard]] ExpressionTree* ndifferentiate(const std::string& variable) const;    // differentiate by 'variable'
		ExpressionTree& differentiate();                                                    // differentiate current _expression
		ExpressionTree& differentiate(const std::string& variable);                         // differentiate current _expression

		void print() const;                                                                 // print the tree
		void vprint() const;                                                                // variable print

		// operators
		bool operator==(const ExpressionTree& other) const;

	};

	// operators
	std::ostream& operator<<(std::ostream& ostream, const ng::ExpressionTree::Node& node);

} // namespace ng