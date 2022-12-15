#include <bits/stdc++.h>

using namespace std;

const auto Inf = numeric_limits<int>::max();

auto bfs(const vector<string>& g, int n, int m,
    const vector<pair<int, int>>& src, const pair<int, int>& dst)
{
    const array<pair<int, int>, 4> neigh{{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    }};

    queue<pair<int, int>> q;
    vector<vector<int>> dist(n, vector<int>(m, -1));
    for (const auto& [r, c] : src) {
        q.emplace(r, c);
        dist[r][c] = 0;
    }

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (const auto& [dr, dc] : neigh) {
            auto nr = r + dr;
            if (nr < 0 || nr == n)
                continue;
            auto nc = c + dc;
            if (nc < 0 || nc == m)
                continue;

            if (dist[nr][nc] != -1)
                continue;

            if (g[nr][nc] - 'a' > g[r][c] - 'a' + 1)
                continue;

            if (make_pair(nr, nc) == dst)
                return dist[r][c] + 1;

            q.emplace(nr, nc);
            dist[nr][nc] = dist[r][c] + 1;
        }
    }

    return Inf;
}

void solve(vector<string>& g)
{
    auto n = ssize(g);
    auto m = ssize(g[0]);

    pair<int, int> s;
    pair<int, int> e;
    vector<pair<int, int>> a_vec;
    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            if (g[i][j] == 'S') {
                s = {i, j};
                g[i][j] = 'a';
                a_vec.emplace_back(i, j);
            } else if (g[i][j] == 'E') {
                e = {i, j};
                g[i][j] = 'z';
            } else if (g[i][j] == 'a') {
                a_vec.emplace_back(i, j);
            }

    cout << bfs(g, n, m, {s}, e) << '\n';
    cout << bfs(g, n, m, a_vec, e) << '\n';
}

int main()
{
    vector<string> g;

    string line;
    while (getline(cin, line))
        g.push_back(move(line));

    solve(g);

    return 0;
}

