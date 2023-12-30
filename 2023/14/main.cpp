#include <bits/stdc++.h>

using namespace std;

void tilt_north(vector<string>& grid)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    for (auto c = 0; c < m; ++c)
        for (auto empty_pos = 0, r = 0; r < n; ++r)
            if (grid[r][c] == '#') {
                empty_pos = r + 1;
            } else if (grid[r][c] == 'O') {
                grid[r][c] = '.';
                grid[empty_pos][c] = 'O';
                ++empty_pos;
            }
}

auto rotate_right(vector<string>& grid)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    vector<string> rotated_grid(m, string(n, ' '));

    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            rotated_grid[c][m - r - 1] = grid[r][c];

    grid.swap(rotated_grid);
}

auto score(const vector<string>& grid)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    auto sum = 0;

    for (auto c = 0; c < m; ++c)
        for (auto r = 0; r < n; ++r)
            if (grid[r][c] == 'O')
                sum += n - r;

    return sum;
}

auto solve1(vector<string> grid)
{
    tilt_north(grid);
    return score(grid);
}

auto solve2(vector<string>& grid)
{
    static constexpr auto Iters{static_cast<int>(1e9)};

    map<vector<string>, int> grid2iter;

    for (auto i = 1; i <= Iters; ++i) {
        for (auto j = 0; j < 4; ++j) {
            tilt_north(grid);
            rotate_right(grid);
        }

        auto [it, inserted] = grid2iter.emplace(grid, i);
        if (inserted)
            continue;

        auto sol_iter = it->second + (Iters - it->second) % (i - it->second);

        for (const auto& [grid, iter] : grid2iter)
            if (iter == sol_iter)
                return score(grid);

        assert(false);
    }

    return score(grid);
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve1(grid) << '\n';
    cout << solve2(grid) << '\n';

    return 0;
}

