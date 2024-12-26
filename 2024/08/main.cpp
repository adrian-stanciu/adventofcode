#include <bits/stdc++.h>

using namespace std;

auto count(const vector<vector<bool>>& has)
{
    auto cnt = 0l;
    for (const auto& row : has)
        cnt += count(begin(row), end(row), true);
    return cnt;
}

auto solve1(const unordered_map<char, vector<pair<int, int>>>& obj, int n, int m)
{
    vector<vector<bool>> has(n, vector<bool>(m));

    for (const auto& [_, v] : obj) {
        auto sz = ssize(v);
        for (auto i = 0; i < sz; ++i)
            for (auto j = i + 1; j < sz; ++j) {
                auto [ri, ci] = v[i];
                auto [rj, cj] = v[j];

                auto dr = rj - ri;
                auto dc = cj - ci;

                auto nri = ri - dr;
                auto nci = ci - dc;
                if ((nri >= 0 && nri < n) && (nci >= 0 && nci < m))
                    has[nri][nci] = true;

                auto nrj = rj + dr;
                auto ncj = cj + dc;
                if ((nrj >= 0 && nrj < n) && (ncj >= 0 && ncj < m))
                    has[nrj][ncj] = true;
            }
    }

    return count(has);
}

auto solve2(const unordered_map<char, vector<pair<int, int>>>& obj, int n, int m)
{
    vector<vector<bool>> has(n, vector<bool>(m));

    for (const auto& [_, v] : obj) {
        auto sz = ssize(v);
        for (auto i = 0; i < sz; ++i)
            for (auto j = i + 1; j < sz; ++j) {
                auto [ri, ci] = v[i];
                auto [rj, cj] = v[j];

                auto dr = rj - ri;
                auto dc = cj - ci;

                while ((ri >= 0 && ri < n) && (ci >= 0 && ci < m)) {
                    has[ri][ci] = true;
                    ri -= dr;
                    ci -= dc;
                }

                while ((rj >= 0 && rj < n) && (cj >= 0 && cj < m)) {
                    has[rj][cj] = true;
                    rj += dr;
                    cj += dc;
                }
            }
    }

    return count(has);
}

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s))
        grid.push_back(move(s));

    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    unordered_map<char, vector<pair<int, int>>> obj;

    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] != '.')
                obj[grid[r][c]].emplace_back(r, c);

    cout << solve1(obj, n, m) << '\n';
    cout << solve2(obj, n, m) << '\n';

    return 0;
}

