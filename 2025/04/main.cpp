#include <bits/stdc++.h>

using namespace std;

auto count_nei(const vector<string>& v, int i, int j)
{
    static constexpr array<pair<int, int>, 8> nei{{
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1},
    }};

    auto cnt = 0;

    for (auto [di, dj] : nei) {
        auto ni = i + di;
        if (ni < 0 || ni == ssize(v))
            continue;

        auto nj = j + dj;
        if (nj < 0 || nj == ssize(v[ni]))
            continue;

        if (v[ni][nj] == '@')
            ++cnt;
    }

    return cnt;
}

auto find_accessible(const vector<string>& v)
{
    vector<pair<int, int>> accessible;

    for (auto i = 0; i < ssize(v); ++i)
        for (auto j = 0; j < ssize(v[i]); ++j)
            if (v[i][j] == '@' && count_nei(v, i, j) < 4)
                accessible.emplace_back(i, j);

    return accessible;
}

auto solve1(const vector<string>& v)
{
    return find_accessible(v).size();
}

auto solve2(vector<string>& v)
{
    auto cnt = 0;

    while (true) {
        auto accessible = find_accessible(v);
        if (accessible.empty())
            break;

        cnt += ssize(accessible);

        for (const auto& [i, j] : accessible)
            v[i][j] = '.';
    }

    return cnt;
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

