#include <bits/stdc++.h>

#include "parser.hpp"

using namespace std;

optional<int> bfs(const vector<vector<bool>>& grid, int sz)
{
    static constexpr array<pair<int, int>, 4> Neigh{{
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    }};

    queue<tuple<int, int, int>> q;
    vector<vector<bool>> vis(sz, vector<bool>(sz));

    if (!grid[0][0]) {
        q.emplace(0, 0, 0);
        vis[0][0] = true;
    }

    while (!q.empty()) {
        auto [r, c, d] = q.front();
        q.pop();

        if (r == sz - 1 && c == sz - 1)
            return d;

        for (const auto& [dr, dc] : Neigh) {
            auto nr = r + dr;
            if (nr < 0 || nr == sz)
                continue;
            auto nc = c + dc;
            if (nc < 0 || nc == sz)
                continue;

            if (grid[nr][nc])
                continue;
            if (vis[nr][nc])
                continue;

            q.emplace(nr, nc, d + 1);
            vis[nr][nc] = true;
        }
    }

    return nullopt;
}

auto solve(const vector<pair<int, int>>& v, int grid_sz, size_t cnt)
{
    using Span = span<const pair<int, int>>;

    auto build_grid = [&](auto span) {
        vector<vector<bool>> grid(grid_sz, vector<bool>(grid_sz));
        for (const auto& [r, c] : span)
            grid[r][c] = true;
        return grid;
    };

    if (auto sol1 = bfs(build_grid(Span{begin(v), cnt}), grid_sz); sol1)
        cout << *sol1 << '\n';

    optional<size_t> sol2;

    size_t l = 0;
    size_t r = size(v) - 1;
    while (l <= r) {
        auto m = midpoint(l, r);
        if (bfs(build_grid(Span{begin(v), m + 1}), grid_sz)) {
            l = m + 1;
        } else {
            sol2 = m;
            r = m - 1;
        }
    }

    if (sol2)
        cout << v[*sol2].second << ',' << v[*sol2].first << '\n';
}

int main()
{
    vector<pair<int, int>> v;

    string s;
    while (getline(cin, s)) {
        auto parts = split(s, ',');
        v.emplace_back(str2num(parts[1]), str2num(parts[0]));
    }

    solve(v, 71, 1024);

    return 0;
}

