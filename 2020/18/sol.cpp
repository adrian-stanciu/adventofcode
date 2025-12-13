#include <bits/stdc++.h>

using namespace std;

auto postfix(const string& e, const function<bool(char, char)>& gte_precedence)
{
    string res;

    stack<char> s;

    for (auto c : e) {
        if (c == '(') {
            s.push(c);
        } else if (c == ')') {
            while (s.top() != '(') {
                res.push_back(s.top());
                s.pop();
            }
            s.pop(); // pop '('
        } else if (c == '+' || c == '*') {
            while (!s.empty() && s.top() != '(' && gte_precedence(s.top(), c)) {
                res.push_back(s.top());
                s.pop();
            }
            s.push(c);
        } else if (c >= '0' && c <= '9') {
            res.push_back(c);
        }
    }

    while (!s.empty()) {
        res.push_back(s.top());
        s.pop();
    }

    return res;
}

auto eval(const string& e)
{
    stack<long> s;

    for (auto c : e)
        if (c == '+' || c == '*') {
            auto x = s.top();
            s.pop();
            auto y = s.top();
            s.pop();
            s.push(c == '+' ? x + y : x * y);
        } else {
            s.push(c - '0');
        }

    return s.top();
}

auto solve(const vector<string>& v)
{
    auto s = 0l;
    for (const auto& e : v)
        s += eval(e);
    return s;
}

int main()
{
    vector<string> v1;
    vector<string> v2;

    string line;
    while (getline(cin, line)) {
        v1.push_back(postfix(line, [] (auto, auto) {
            return true;
        }));
        v2.push_back(postfix(line, [] (auto op1, auto op2) {
            return op1 == '+' || op2 == '*';
        }));
    }

    cout << solve(v1) << '\n';
    cout << solve(v2) << '\n';

    return 0;
}

