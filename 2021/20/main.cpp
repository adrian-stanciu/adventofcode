#include <bits/stdc++.h>

using namespace std;

using Grid = vector<string>;

auto position(const Grid& grid, int r, int c)
{
    auto n = 0;

    for (auto i = r - 1; i <= r + 1; ++i)
        for (auto j = c - 1; j <= c + 1; ++j) {
            n *= 2;
            n += grid[i][j] == '#';
        }

    return n;
}

auto border(const Grid& grid, char border_elem)
{
    Grid new_grid;

    const string border_row(grid.front().size() + 4, border_elem);

    new_grid.push_back(border_row);
    new_grid.push_back(border_row);

    for (const auto& row : grid) {
        string new_row;

        new_row.push_back(border_elem);
        new_row.push_back(border_elem);

        new_row.append(row);

        new_row.push_back(border_elem);
        new_row.push_back(border_elem);

        new_grid.push_back(move(new_row));
    }

    new_grid.push_back(border_row);
    new_grid.push_back(border_row);

    return new_grid;
}

auto solve(const string& mapping, Grid grid, int num_iters)
{
    auto border_elem = '.';
    auto fill_elem = mapping.front();

    for (auto i = 0; i < num_iters; ++i) {
        grid = border(grid, border_elem);

        auto n = static_cast<int>(grid.size());
        auto m = static_cast<int>(grid.front().size());

        auto new_grid{grid};
        for (auto& row : new_grid)
            for (auto& elem : row)
                elem = fill_elem;

        for (auto r = 1; r < n - 1; ++r)
            for (auto c = 1; c < m - 1; ++c)
                new_grid[r][c] = mapping[position(grid, r, c)];

        grid.swap(new_grid);

        border_elem = fill_elem;
        fill_elem = border_elem == '#' ? mapping.back() : mapping.front();
    }

    auto cnt = 0l;

    for (const auto& row : grid)
        cnt += count(begin(row), end(row), '#');

    return cnt;
}

int main()
{
    string mapping;
    cin >> mapping;

    Grid grid;

    string row;
    while (getline(cin, row)) {
        if (row.empty())
            continue;

        grid.push_back(move(row));
    }

    cout << solve(mapping, grid, 2) << '\n';
    cout << solve(mapping, grid, 50) << '\n';

    return 0;
}

