#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>

using namespace std;

vector<string> polishNotation;
string sc;
int brackets(0);
bool error = false;
int correct(0);
bool zero = false;

string removeSpaces(string&);
void createPolishNotation(string&);
void parsePolishNotation();

int main() {

    string inputString;
    cout << "please, enter correct expression:\n";
    getline(cin, inputString);
    inputString = removeSpaces(inputString);

    createPolishNotation(inputString);
    if (error || brackets != 0 || correct != 1)
        cout << "\ninvalid input. please try again and don't break my program)))\n";
    else {
        cout << "\nreverse polish notation:\n";
        for (const auto & i : polishNotation)
            cout << i << " ";
        cout << '\n';
        parsePolishNotation();
    }

    return 0;

}

string removeSpaces(string& str) {

    int i = 0;
    string newStr;

    while (str[i]) {
        if (str[i] != ' ')
            newStr += str[i];
        i++;
    }

    return newStr;

}

void clearStack(stack<string>& signStack) {

    while (!signStack.empty()) {

        if (signStack.top() == "(") {
            signStack.pop();

            if (!signStack.empty() && (signStack.top() == "sin" || signStack.top() == "cos"))
                polishNotation.emplace_back(signStack.top()), signStack.pop();

            return;
        }

        polishNotation.emplace_back(signStack.top());
        signStack.pop();
        correct--;

    }

}

void clearLastElem(stack<string>& signStack, const string& elem) {

    if (signStack.top() != "(" && signStack.top() != "sin" && signStack.top() != "cos")
        polishNotation.emplace_back(signStack.top()), signStack.pop(), correct--;

    signStack.push(elem);

}

void clearString(string& str) {

    correct++;
    polishNotation.emplace_back(str);
    str = "";
    
}

void createPolishNotation(string& inputString) {

    const string numberString = "0123456789";
    const string signString = "+-/*%^)";
    const string trigonometricFunctions = "sin cos";

    // sin(1 - (-2)^2)
    // sin(cos(2 + 3) + sin(2 ^ 3))
    // 1 - (2 + 5) * 4
    // (sin(23)-sin(48))*(cos(-2+2)*sin(-2)+023)

    stack<string> signStack;
    string currentNumber;
    bool number = false;
    bool sign = false;
    char elem;
    string strElem;

    for (unsigned int i = 0; i < inputString.length(); i++) {

        elem = inputString[i];
        strElem = string(1, elem);

        if (numberString.find(elem) != string::npos) {

			currentNumber += elem;
			number = true;
			sign = false;

		} else if (trigonometricFunctions.find(elem) != string::npos) {

            for (unsigned int j = i; j < i + 3; j++)
                sc += inputString[j];

            if (trigonometricFunctions.find(sc) == string::npos) 
                error = true;
            
            i += 3, brackets++;
            signStack.push(sc);
            sc = "";
            signStack.push("(");

        } else if (elem == '(') {

			signStack.push(strElem);
			brackets++;
			sign = true;

		} else if (signString.find(elem) != string::npos) {

            if (sign && inputString[i - 1] == '(' && elem != '-') 
                error = true;
            
            if (!currentNumber.empty())
                clearString(currentNumber);

            sign = true;

            if ((elem == '-' && i == 0) || (elem == '-' && inputString[i - 1] == '(' && inputString[i + 1] == '('))
                polishNotation.emplace_back("-1"), signStack.push("*"), correct++;
            else if ((elem == '-' && inputString[i + 1] == '(')) {
                if (signStack.empty())
                    signStack.push("+"), signStack.push("*");
                else
                    clearLastElem(signStack, "+"), signStack.push("*");
                polishNotation.emplace_back("-1");
                correct++;
            }
            else if (elem == '-' && ((!signStack.empty() && signStack.top() == "(" && !number) || (signStack.empty() && !number)))
                currentNumber += elem;
            else if (elem == ')')
                brackets--, clearStack(signStack);
            else if (signStack.empty())
                signStack.push(strElem);
            else if ((!signStack.empty() && (elem == '*' || elem == '/' || elem == '%') && (signStack.top() == "+" || signStack.top() == "-")))
                signStack.push(strElem);
            else if (!signStack.empty() && elem == '^' && signStack.top() != "^" && signStack.top() != "sin" && signStack.top() != "cos")
                signStack.push(strElem);
            else
                clearLastElem(signStack, strElem);

            number = false;

        }

    }

    if (!currentNumber.empty()) {
        polishNotation.push_back(currentNumber), correct++;
    }
    clearStack(signStack);

}

void numberCounting(stack<double> &st, const string& sign, double num) {
    double curNum = st.top();
    st.pop();

    if (sign == "+")
        curNum += num;
    else if (sign == "-")
        curNum -= num;
    else if (sign == "*")
        curNum *= num;
    else if (sign == "/")
        curNum /= num;
    else if (sign == "^")
        curNum = pow(curNum, num);
    else if (sign == "%") {
        int integerNum = (int)num;
        int integerCurNum = (int)curNum;

        if (integerNum == 0) {
            zero = true;
            return;
        }

        integerCurNum %= integerNum;
        curNum = integerCurNum;
    }


    st.push(curNum);

}

void trigonometricFunction(stack<double>& st, const string& function) {
    double curNum = st.top();
    st.pop();

    if (function == "sin")
        curNum = sin(curNum);
    else
        curNum = cos(curNum);

    st.push(curNum);
}

void parsePolishNotation() {

    const string signString = "+-/*%^";
    stack<double> expressionValue;
    double currentNumber;

    for (const auto & elem : polishNotation) {

        if (elem == "sin" || elem == "cos")
            trigonometricFunction(expressionValue, elem);
        else if (signString.find(elem) == string::npos)
            expressionValue.push(stof(elem));
        else {

            currentNumber = expressionValue.top();
            expressionValue.pop();
            numberCounting(expressionValue, elem, currentNumber);

        }

    }

    if (zero)
        cout << "\nthis program can't solve expression like this. sorry(\n";
    else
        cout << "\nanswer = " << expressionValue.top() << '\n';

}
