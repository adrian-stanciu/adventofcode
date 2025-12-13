#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

auto solve(const vector<pair<char, int>>& v)
{
    auto cnt1 = 0;
    auto cnt2 = 0;

    for (auto pos = 50; auto [dir, len] : v) {
        auto prev_pos = pos;

        cnt2 += len / 100;
        len %= 100;

        if (dir == 'L')
            len *= -1;

        pos += len;

        if (prev_pos != 0 && (pos <= 0 || pos >= 100))
            ++cnt2;

        pos += 100;
        pos %= 100;

        if (pos == 0)
            ++cnt1;
    }

    cout << cnt1 << '\n';
    cout << cnt2 << '\n';
}

int main()
{
    vector<pair<char, int>> v;

    string s;
    while (getline(cin, s)) {
        const string_view sv{s};
        v.emplace_back(sv[0], str2num(sv.substr(1)));
    }

    solve(v);

    return 0;
}

