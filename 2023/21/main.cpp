#include <bits/stdc++.h>

using namespace std;

auto solve1(const vector<string>& grid, int iters)
{
    static constexpr array<pair<int, int>, 4> neigh{{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    }};

    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    queue<pair<int, int>> q;
    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] == 'S') {
                q.emplace(r, c);
                break;
            }

    while (iters--) {
        queue<pair<int, int>> nq;
        vector<vector<bool>> visited(n, vector<bool>(m, false));

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (auto [dr, dc] : neigh) {
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

                nq.emplace(nr, nc);
                visited[nr][nc] = true;
            }
        }

        if (iters == 0)
            return make_pair(ssize(nq), visited);

        q.swap(nq);
    }

    assert(false);
}

auto count(const vector<vector<bool>>& visited, int from_r, int from_c, int side_len)
{
    auto cnt = 0;

    auto to_r = from_r + side_len;
    auto to_c = from_c + side_len;
    for (auto r = from_r; r < to_r; ++r)
        for (auto c = from_c; c < to_c; ++c)
            if (visited[r][c])
                ++cnt;

    return cnt;
}

auto solve2(const vector<string>& grid, int iters)
{
    auto n = ssize(grid);

    // assumptions based on input
    assert(n == ssize(grid[0]));
    assert(grid[n / 2][n / 2] == 'S');
    assert((iters - n / 2) % n == 0);
    assert(((iters - n / 2) / n) % 2 == 0);
    for (auto r = 0; r < n; ++r)
        assert(grid[r][n / 2] != '#');
    for (auto c = 0; c < n; ++c)
        assert(grid[n / 2][c] != '#');

    auto ngrid{grid};
    for (auto& row : ngrid)
        for (auto i = 0; i < 4; ++i)
            row.append(begin(row), begin(row) + n);
    for (auto i = 0; i < 4; ++i)
        for (auto r = 0; r < n; ++r)
            ngrid.push_back(ngrid[r]);

    auto sz = ssize(ngrid);

    for (auto r = 0; r < sz; ++r)
        for (auto c = 0; c < sz; ++c)
            if (ngrid[r][c] == 'S')
                ngrid[r][c] = '.';
    ngrid[sz / 2][sz / 2] = 'S';

    auto [_, visited] = solve1(ngrid, sz / 2);

    // under the previous asserts, a 5x5 matrix of original grids, after 5 * n / 2
    // iterations, will look like (where all things will be inside the rhomb):
    // ----------------
    // |  |od|/\|od|  |
    // |  | /|ev|\ |  |
    // ----------------
    // |od|/ |od| \|od|
    // | /|ev|  |ev|\ |
    // ----------------
    // |/e|od|ev|od|e\|
    // |\v|  |  |  |v/|
    // ----------------
    // | \|ev|od|ev|/ |
    // |od|\ |  | /|od|
    // ----------------
    // |  | \|ev|/ |  |
    // |  |od|\/|od|  |
    // ----------------

    // count for complete even grids
    auto ev_cnt = count(visited, 2 * n, 2 * n, n);
    // count for complete odd grids
    auto od_cnt = count(visited, n, 2 * n, n);

    // count for even grids without their inner rhomb
    auto ev_ext_cnt = 4 * ev_cnt -
        (count(visited, n, n, n) + count(visited, n, 3 * n, n) +
        count(visited, 3 * n, n, n) + count(visited, 3 * n, 3 * n, n));
    // count for odd grids without their inner rhomb
    auto od_ext_cnt = count(visited, 0, n, n) + count(visited, 0, 3 * n, n) +
        count(visited, 4 * n, n, n) + count(visited, 4 * n, 3 * n, n);

    // number of grids reached by going north
    auto m = 1l * (iters - n / 2) / n;

    // extrapolate from 5 to 2 * m + 1

    // number of complete even grids
    auto num_ev = 2 * (m * (m + 1) / 2) + m + 1 - 4 * m;
    // number of complete odd grids
    auto num_od = 2 * ((m + 1) * (m + 2) / 2 - 1) + m - 4 * m;

    return num_ev * ev_cnt + num_od * od_cnt + // complete grids
        4 * m * ev_cnt - (m + 1) * ev_ext_cnt + // incomplete even grids
        m * od_ext_cnt; // incomplete odd grids
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve1(grid, 64).first << '\n';
    cout << solve2(grid, 26501365) << '\n';

    return 0;
}

