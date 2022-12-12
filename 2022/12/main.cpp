#include <bits/stdc++.h>

using namespace std;

const auto Inf = numeric_limits<int>::max();

auto bfs(const vector<string>& g, int n, int m, const pair<int, int>& s, const pair<int, int>& e)
{
    const array<pair<int, int>, 4> neigh{{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    }};

    queue<pair<int, int>> q;
    q.push(s);

    vector<vector<int>> dist(n, vector<int>(m, -1));
    dist[s.first][s.second] = 0;

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

            if (nr == e.first && nc == e.second)
                return dist[r][c] + 1;

            q.push(make_pair(nr, nc));
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
    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            if (g[i][j] == 'S') {
                s = make_pair(i, j);
                g[i][j] = 'a';
            } else if (g[i][j] == 'E') {
                e = make_pair(i, j);
                g[i][j] = 'z';
            }
    cout << bfs(g, n, m, s, e) << '\n';

    auto min_d = Inf;
    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            if (g[i][j] == 'a')
                min_d = min(min_d, bfs(g, n, m, make_pair(i, j), e));
    cout << min_d << '\n';
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

