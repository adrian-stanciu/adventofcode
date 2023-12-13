#include <bits/stdc++.h>

using namespace std;

auto find_reflection_row(const vector<string>& grid, int diffs_needed)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    for (auto r = 1; r < n; ++r) {
        auto diffs = 0;
        for (auto c = 0; c < m; ++c)
            for (auto ru = r - 1, rd = r; ru >= 0 && rd < n; --ru, ++rd)
                if (grid[ru][c] != grid[rd][c])
                    ++diffs;
        if (diffs == diffs_needed)
            return r;
    }

    return -1;
}

auto find_reflection_col(const vector<string>& grid, int diffs_needed)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    for (auto c = 1; c < m; ++c) {
        auto diffs = 0;
        for (auto r = 0; r < n; ++r)
            for (auto cl = c - 1, cr = c; cl >= 0 && cr < m; --cl, ++cr)
                if (grid[r][cl] != grid[r][cr])
                    ++diffs;
        if (diffs == diffs_needed)
            return c;
    }

    return -1;
}

void solve_grid(const vector<string>& grid, int diffs_needed, int& row_sum, int& col_sum)
{
    auto r = find_reflection_row(grid, diffs_needed);
    if (r != -1)
        row_sum += r;

    auto c = find_reflection_col(grid, diffs_needed);
    if (c != -1)
        col_sum += c;

    assert(r != -1 || c != -1);
    assert(r == -1 || c == -1);
}

int main()
{
    auto row_sum1 = 0;
    auto col_sum1 = 0;

    auto row_sum2 = 0;
    auto col_sum2 = 0;

    auto solve = [&](const auto& grid) {
        solve_grid(grid, 0, row_sum1, col_sum1);
        solve_grid(grid, 1, row_sum2, col_sum2);
    };

    vector<string> grid;

    string row;
    while (getline(cin, row))
        if (row.empty()) {
            solve(grid);
            grid.clear();
        } else {
            grid.push_back(move(row));
        }

    if (!grid.empty())
        solve(grid);

    auto score = [](auto row_sum, auto col_sum) {
        return 100 * row_sum + col_sum;
    };

    cout << score(row_sum1, col_sum1) << '\n';
    cout << score(row_sum2, col_sum2) << '\n';

    return 0;
}

