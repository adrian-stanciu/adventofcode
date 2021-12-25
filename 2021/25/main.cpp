#include <bits/stdc++.h>

using namespace std;

auto solve(vector<string> grid)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    auto do_moves = [&](auto val, auto dr, auto dc, auto& changed) {
        auto new_grid{grid};

        for (auto i = 0; i < n; ++i)
            for (auto j = 0; j < m; ++j)
                if (grid[i][j] == val) {
                    auto r = (i + dr) % n;
                    auto c = (j + dc) % m;

                    if (grid[r][c] == '.') {
                        swap(new_grid[i][j], new_grid[r][c]);
                        changed = true;
                    }
                }

        grid.swap(new_grid);
    };

    auto cnt = 0;

    while (true) {
        ++cnt;

        auto changed = false;

        do_moves('>', 0, 1, changed);
        do_moves('v', 1, 0, changed);

        if (!changed)
            break;
    }

    return cnt;
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve(grid) << '\n';

    return 0;
}

