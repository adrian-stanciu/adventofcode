#include <bits/stdc++.h>

using namespace std;

auto solve(const vector<string>& grid, int n, int m, int r, int c,
    vector<vector<bool>>& vis)
{
    auto count_vertices = [&](auto r, auto c) {
        static constexpr array<pair<int, int>, 4> Neigh{{
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        }};

        auto elem = [&](auto r, auto c) {
            return (r < 0 || r == n || c < 0 || c == m) ? '\0' : grid[r][c];
        };

        auto cnt = 0;

        // outer vertices
        for (const auto& [dr, dc] : Neigh)
            if (elem(r + dr, c) != grid[r][c] && elem(r, c + dc) != grid[r][c])
                ++cnt;

        // inner vertices
        for (const auto& [dr, dc] : Neigh)
            if (elem(r + dr, c) == grid[r][c] && elem(r, c + dc) == grid[r][c] &&
                elem(r + dr, c + dc) != grid[r][c])
                ++cnt;

        return cnt;
    };

    static constexpr array<pair<int, int>, 4> Neigh{{
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    }};

    auto a = 0l;
    auto p = 0l;
    auto num_vertices = 0l; // = number of edges

    auto ch = grid[r][c];

    queue<pair<int, int>> q;
    q.emplace(r, c);

    vis[r][c] = true;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        ++a;

        num_vertices += count_vertices(r, c);

        for (const auto& [dr, dc] : Neigh) {
            auto nr = r + dr;
            auto nc = c + dc;

            if (nr < 0 || nr == n || nc < 0 || nc == m || grid[nr][nc] != ch) {
                ++p;
                continue;
            }

            if (!vis[nr][nc]) {
                q.emplace(nr, nc);

                vis[nr][nc] = true;
            }
        }
    }

    return make_tuple(a, p, num_vertices);
}

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s))
        grid.push_back(move(s));

    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    auto sol1 = 0l;
    auto sol2 = 0l;

    vector<vector<bool>> vis(n, vector<bool>(m));

    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (!vis[r][c]) {
                auto [a, p, num_edges] = solve(grid, n, m, r, c, vis);

                sol1 += 1l * a * p;
                sol2 += 1l * a * num_edges;
            }

    cout << sol1 << '\n';
    cout << sol2 << '\n';

    return 0;
}

