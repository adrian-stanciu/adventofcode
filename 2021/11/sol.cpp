#include <bits/stdc++.h>

using namespace std;

using Grid = vector<vector<int>>;

const array<pair<int, int>, 8> neigh{{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1},
}};

auto count_flashes(Grid& grid, int n, int m)
{
    queue<pair<int, int>> to_flash;

    for (auto x = 0; x < n; ++x)
        for (auto y = 0; y < m; ++y)
            if (++grid[x][y] == 10)
                to_flash.emplace(x, y);

    while (!to_flash.empty()) {
        auto [x, y] = to_flash.front();
        to_flash.pop();

        for (auto [dx, dy] : neigh) {
            auto nx = x + dx;
            if (nx < 0 || nx == n)
                continue;

            auto ny = y + dy;
            if (ny < 0 || ny == m)
                continue;

            if (grid[nx][ny] == 10)
                continue;

            if (++grid[nx][ny] == 10)
                to_flash.emplace(nx, ny);
        }
    }

    auto cnt = 0;

    for (auto& row : grid)
        for (auto& val : row)
            if (val == 10) {
                ++cnt;
                val = 0;
            }

    return cnt;
}

auto solve1(Grid grid, int num_iters)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    auto sol = 0;

    while (num_iters--)
        sol += count_flashes(grid, n, m);

    return sol;
}

auto solve2(Grid grid)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    auto sz = n * m;

    auto prev_cnt = 0;
    for (auto t = 1;; ++t) {
        auto cnt = count_flashes(grid, n, m);

        if (prev_cnt + sz == cnt)
            return t;

        prev_cnt = cnt;
    }
}

int main()
{
    Grid grid;

    string s;
    while (getline(cin, s)) {
        vector<int> row;

        for (auto c : s)
            row.push_back(c - '0');

        grid.push_back(move(row));
    }

    cout << solve1(grid, 100) << '\n';
    cout << solve2(grid) << '\n';

    return 0;
}

