#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

bool is_valid(const string& str) {

    if (str.empty())
        return true;

    unordered_map<char, char> p;

    p[')'] = '(';
    p[']'] = '[';
    p['}'] = '{';

    stack<char> st;

    for (auto e : str) {

        // search by key
        auto couple = p.find(e);

        if (couple != p.end()) {

            char top = st.empty() ? ' ' : st.top();

            if (top == couple->second)
                st.pop();
            else
                return false;

        } else {

            st.push(e);

        }

    }

    return st.empty();

}

int main() {

    string str;
    cin >> str;

    cout << "is valid = " << is_valid(str) << endl;


    return 0;

}
