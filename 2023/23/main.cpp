#include <bits/stdc++.h>

using namespace std;

static constexpr array<tuple<int, int, char>, 4> neigh{{
    {-1, 0, '^'}, {1, 0, 'v'}, {0, -1, '<'}, {0, 1, '>'}
}};

void dfs(int n, int m, const vector<string>& grid, vector<vector<bool>>& visited,
    int r, int c, int dst_r, int dst_c, int cur_d, int& max_d)
{
    if (r == dst_r && c == dst_c) {
        max_d = max(max_d, cur_d);
        return;
    }

    visited[r][c] = true;

    for (auto [dr, dc, dir] : neigh) {
        if (grid[r][c] != '.' && grid[r][c] != dir)
            continue;

        auto nr = r + dr;
        if (nr < 0 || nr == n)
            continue;
        auto nc = c + dc;
        if (nc < 0 || nc == m)
            continue;
        if (visited[nr][nc])
            continue;
        if (grid[nr][nc] == '#')
            continue;

        dfs(n, m, grid, visited, nr, nc, dst_r, dst_c, cur_d + 1, max_d);
    }

    visited[r][c] = false;
}

auto solve1(const vector<string>& grid, int src_c, int dst_c)
{
    auto n = ssize(grid);
    auto m = ssize(grid[0]);
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    auto max_d = 0;
    dfs(n, m, grid, visited, 0, src_c, n - 1, dst_c, 0, max_d);
    return max_d;
}

void dfs(int n, int m, const vector<string>& grid, vector<vector<bool>>& visited,
    int r, int c, const vector<vector<int>>& grid2graph, int src, int cur_d,
    vector<vector<pair<int, int>>>& g)
{
    if (auto dst = grid2graph[r][c]; dst != -1 && dst != src) {
        g[src].emplace_back(dst, cur_d);
        g[dst].emplace_back(src, cur_d);

        src = dst;
        cur_d = 0;
    }

    if (visited[r][c])
        return;

    visited[r][c] = true;

    for (auto [dr, dc, _] : neigh) {
        auto nr = r + dr;
        if (nr < 0 || nr == n)
            continue;
        auto nc = c + dc;
        if (nc < 0 || nc == m)
            continue;
        if (grid[nr][nc] == '#')
            continue;

        dfs(n, m, grid, visited, nr, nc, grid2graph, src, cur_d + 1, g);
    }
}

auto build_graph(const vector<string>& grid, int src_c, int dst_c)
{
    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    auto num_nodes = 0;
    vector<vector<int>> grid2graph(n, vector<int>(m, -1));
    grid2graph[0][src_c] = num_nodes++;
    grid2graph[n - 1][dst_c] = num_nodes++;

    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] == '.') {
                auto num_neigh = 0;
                for (auto [dr, dc, _] : neigh) {
                    auto nr = r + dr;
                    if (nr < 0 || nr == n)
                        continue;
                    auto nc = c + dc;
                    if (nc < 0 || nc == m)
                        continue;
                    if (grid[nr][nc] == '.')
                        ++num_neigh;
                }
                if (num_neigh > 2)
                    grid2graph[r][c] = num_nodes++;
            }

    vector<vector<pair<int, int>>> g(num_nodes);
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    dfs(n, m, grid, visited, 0, src_c, grid2graph, grid2graph[0][src_c], 0, g);

    return make_tuple(num_nodes, g, grid2graph[n - 1][dst_c]);
}

void dfs(int n, const vector<vector<pair<int, int>>>& g, vector<bool>& visited,
    int u, int dst, int cur_d, int& max_d)
{
    if (u == dst) {
        max_d = max(max_d, cur_d);
        return;
    }

    visited[u] = true;

    for (const auto& [v, d] : g[u])
        if (!visited[v])
            dfs(n, g, visited, v, dst, cur_d + d, max_d);

    visited[u] = false;
}

auto solve2(const vector<string>& grid, int src_c, int dst_c)
{
    auto [n, g, dst] = build_graph(grid, src_c, dst_c);
    vector<bool> visited(n, false);
    auto max_d = 0;
    dfs(n, g, visited, 0, dst, 0, max_d);
    return max_d;
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    auto src_c = grid.front().find_first_of('.');
    auto dst_c = grid.back().find_first_of('.');

    cout << solve1(grid, src_c, dst_c) << '\n';

    for (auto& row : grid)
        for (auto& e : row)
            if (e != '#')
                e = '.';

    cout << solve2(grid, src_c, dst_c) << '\n';

    return 0;
}

