#include <bits/stdc++.h>

using namespace std;

auto score1(char c)
{
    if (c == ')')
        return 3;
    if (c == ']')
        return 57;
    if (c == '}')
        return 1197;
    if (c == '>')
        return 25137;
    return 0;
}

auto score1(const string& s)
{
    stack<char> st;

    for (auto c : s)
        if (c == '(' || c == '[' || c == '{' || c == '<') {
            st.push(c);
        } else {
            if (st.empty())
                return score1(c);

            auto last = st.top();
            st.pop();

            if (c == ')' && last != '(')
                return score1(c);
            if (c == ']' && last != '[')
                return score1(c);
            if (c == '}' && last != '{')
                return score1(c);
            if (c == '>' && last != '<')
                return score1(c);
        }

    return 0;
}

auto solve1(const vector<string>& v)
{
    return accumulate(begin(v), end(v), 0l, [](auto acc, const auto& s) {
        return acc + score1(s);
    });
}

auto score2(char c)
{
    if (c == '(')
        return 1;
    if (c == '[')
        return 2;
    if (c == '{')
        return 3;
    if (c == '<')
        return 4;
    return 0;
}

auto score2(const string& s)
{
    stack<char> st;

    for (auto c : s) {
        if (c == '(' || c == '[' || c == '{' || c == '<') {
            st.push(c);
        } else {
            if (st.empty())
                return 0l;

            auto last = st.top();
            st.pop();

            if (c == ')' && last != '(')
                return 0l;
            if (c == ']' && last != '[')
                return 0l;
            if (c == '}' && last != '{')
                return 0l;
            if (c == '>' && last != '<')
                return 0l;
        }
    }

    auto score = 0l;

    while (!st.empty()) {
        auto last = st.top();
        st.pop();

        score *= 5;
        score += score2(last);
    }

    return score;
}

auto solve2(const vector<string>& v)
{
    vector<long> scores;

    for (const auto& s : v) {
        auto score = score2(s);
        if (score != 0)
            scores.push_back(score);
    }

    sort(begin(scores), end(scores));

    return scores[scores.size() / 2];
}

int main()
{
    vector<string> v;

    string s;
    while (getline(cin, s))
        v.push_back(move(s));

    cout << solve1(v) << '\n';
    cout << solve2(v) << '\n';

    return 0;
}

