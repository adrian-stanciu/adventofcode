#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto max_num_with_d_digits(const string& s, int d)
{
    vector<int> indexes(d);
    iota(begin(indexes), end(indexes), s.size() - d);

    for (auto i = 0; i < d; ++i)
        for (auto j = indexes[i] - 1; j > (i == 0 ? -1 : indexes[i - 1]); --j)
            if (s[j] >= s[indexes[i]])
                indexes[i] = j;

    string res;
    res.reserve(d);
    for (auto i : indexes)
        res.push_back(s[i]);

    return str2num(res);
}

auto solve(const vector<string>& v, int d)
{
    auto sum = 0l;

    for (const auto& s : v)
        sum += max_num_with_d_digits(s, d);

    return sum;
}

int main()
{
    vector<string> v;

    string s;
    while (getline(cin, s))
        v.push_back(move(s));

    cout << solve(v, 2) << '\n';
    cout << solve(v, 12) << '\n';

    return 0;
}

