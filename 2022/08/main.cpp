#include <bits/stdc++.h>

using namespace std;

template <typename T>
auto rotate_right(const vector<vector<T>>& mat)
{
    auto n = ssize(mat);
    auto m = ssize(mat[0]);

    vector<vector<T>> rot_mat(m, vector<T>(n));

    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            rot_mat[j][n - i - 1] = mat[i][j];

    return rot_mat;
}

auto solve1(vector<vector<int>>& grid)
{
    auto num_rows = ssize(grid);
    auto num_cols = ssize(grid[0]);

    vector<vector<bool>> seen(num_rows, vector<bool>(num_cols, false));

    auto mark_seen_to_right = [&]() {
        for (auto i = 0; i < num_rows; ++i) {
            auto max_h = -1;
            for (auto j = 0; j < num_cols; ++j)
                if (grid[i][j] > max_h) {
                    seen[i][j] = true;
                    max_h = grid[i][j];
                }
        }
    };

    for (auto i = 0; i < 4; ++i) {
        mark_seen_to_right();
        grid = rotate_right(grid);
        seen = rotate_right(seen);
    }

    return accumulate(begin(seen), end(seen), 0, [](auto acc, const auto& row) {
        return acc + count(begin(row), end(row), true);
    });
}

auto solve2(vector<vector<int>>& grid)
{
    auto num_rows = ssize(grid);
    auto num_cols = ssize(grid[0]);

    vector<vector<long>> prod(num_rows, vector<long>(num_cols, 1l));

    auto mul_seen_to_right = [&]() {
        for (auto i = 0; i < num_rows; ++i) {
            stack<int> idx_of_next_gte_val_to_right;
            for (auto j = num_cols - 1; j >= 0; --j) {
                while (!idx_of_next_gte_val_to_right.empty() &&
                    grid[i][j] > grid[i][idx_of_next_gte_val_to_right.top()])
                    idx_of_next_gte_val_to_right.pop();

                if (idx_of_next_gte_val_to_right.empty())
                    prod[i][j] *= num_cols - 1 - j;
                else
                    prod[i][j] *= idx_of_next_gte_val_to_right.top() - j;

                idx_of_next_gte_val_to_right.push(j);
            }
        }
    };

    for (auto i = 0; i < 4; ++i) {
        mul_seen_to_right();
        grid = rotate_right(grid);
        prod = rotate_right(prod);
    }

    auto max_score = 0l;
    for (const auto& row : prod)
        max_score = max(max_score, *max_element(begin(row), end(row)));
    return max_score;
}

int main()
{
    vector<vector<int>> grid;

    string line;
    while (getline(cin, line)) {
        vector<int> row;
        row.reserve(line.size());
        for (auto c : line)
            row.push_back(c - '0');
        grid.push_back(move(row));
    }

    cout << solve1(grid) << '\n';
    cout << solve2(grid) << '\n';

    return 0;
}

