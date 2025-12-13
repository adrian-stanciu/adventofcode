#include <bits/stdc++.h>

using namespace std;

int main()
{
    vector<string> grid;
    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    vector<pair<int, int>> points;
    for (auto r = 0; r < ssize(grid); ++r)
        for (auto c = 0; c < ssize(grid[0]); ++c)
            if (grid[r][c] == '#')
                points.emplace_back(r, c);

    vector<int> empty_rows_prefix_sum;
    empty_rows_prefix_sum.push_back(0);
    for (const auto& row : grid) {
        empty_rows_prefix_sum.push_back(empty_rows_prefix_sum.back());
        if (count(begin(row), end(row), '.') == ssize(row))
            ++empty_rows_prefix_sum.back();
    }

    auto count_empty_rows = [&](auto src_r, auto dst_r) {
        return empty_rows_prefix_sum[max(src_r, dst_r) + 1] -
            empty_rows_prefix_sum[min(src_r, dst_r) + 1];
    };

    vector<int> empty_cols_prefix_sum;
    empty_cols_prefix_sum.push_back(0);
    for (auto c = 0; c < ssize(grid[0]); ++c) {
        empty_cols_prefix_sum.push_back(empty_cols_prefix_sum.back());
        auto empty = true;
        for (const auto& row : grid)
            if (row[c] != '.') {
                empty = false;
                break;
            }
        if (empty)
            ++empty_cols_prefix_sum.back();
    }

    auto count_empty_cols = [&](auto src_c, auto dst_c) {
        return empty_cols_prefix_sum[max(src_c, dst_c) + 1] -
            empty_cols_prefix_sum[min(src_c, dst_c) + 1];
    };

    auto sp_sum = [&](long factor) {
        --factor;

        auto sum = 0l;

        for (const auto& [src_r, src_c] : points)
            for (const auto& [dst_r, dst_c] : points)
                sum += abs(dst_r - src_r) + abs(dst_c - src_c) +
                    factor * count_empty_rows(src_r, dst_r) +
                    factor * count_empty_cols(src_c, dst_c);

        return sum / 2;
    };

    cout << sp_sum(2) << '\n';
    cout << sp_sum(1'000'000) << '\n';

    return 0;
}

