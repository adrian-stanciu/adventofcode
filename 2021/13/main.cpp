#include <bits/stdc++.h>

using namespace std;

enum class FoldType {
    Up,
    Left,
};

struct Fold {
    FoldType type;
    int at;

    Fold(FoldType type, int at) : type{type}, at{at} {}
};

auto fold_up(const vector<vector<char>>& grid, int at)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    assert(at > 0 && at < n - 1);

    auto new_n = max(at, n - at - 1);

    vector<vector<char>> new_grid(new_n, vector<char>(m, ' '));

    for (auto i = at - 1, k = new_n - 1; i >= 0; --i, --k)
        for (auto j = 0; j < m; ++j)
            if (grid[i][j] == '#')
                new_grid[k][j] = '#';

    for (auto i = at + 1, k = new_n - 1; i < n; ++i, --k)
        for (auto j = 0; j < m; ++j)
            if (grid[i][j] == '#')
                new_grid[k][j] = '#';

    return new_grid;
}

auto transpose(const vector<vector<char>>& grid)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    vector<vector<char>> t_grid(m, vector<char>(n));

    for (auto y = 0; y < m; ++y)
        for (auto x = 0; x < n; ++x)
            t_grid[y][x] = grid[x][y];

    return t_grid;
}

auto fold_left(const vector<vector<char>>& grid, int at)
{
    return transpose(fold_up(transpose(grid), at));
}

auto fold(const vector<vector<char>>& grid, const Fold& fold)
{
    if (fold.type == FoldType::Up)
        return fold_up(grid, fold.at);
    else
        return fold_left(grid, fold.at);
}

auto count_points(const vector<vector<char>>& grid)
{
    auto cnt = 0l;

    for (const auto& row : grid)
        cnt += count(begin(row), end(row), '#');

    return cnt;
}

auto solve(const vector<pair<int, int>>& points, const vector<Fold>& folds)
{
    auto max_x = 0;
    auto max_y = 0;

    for (const auto& [x, y] : points) {
        max_x = max(max_x, x);
        max_y = max(max_y, y);
    }

    vector<vector<char>> grid(max_y + 1, vector<char>(max_x + 1, ' '));
    for (const auto& [x, y] : points)
        grid[y][x] = '#';

    cout << count_points(fold(grid, folds[0])) << '\n';

    for (const auto& f : folds)
        grid = fold(grid, f);

    for (const auto& row : grid) {
        copy(begin(row), end(row), ostream_iterator<char>(cout, ""));
        cout << '\n';
    }
}

int main()
{
    auto to_num = [](const string& s) {
        return strtol(s.data(), nullptr, 10);
    };

    vector<pair<int, int>> points;

    string line;
    while (getline(cin, line)) {
        if (line.empty())
            break;

        auto sep_pos = line.find(',');

        auto x = line.substr(0, sep_pos);
        auto y = line.substr(sep_pos + 1);

        points.emplace_back(to_num(x), to_num(y));
    }

    static const regex r{R"(fold along ([xy])=([0-9]+))"};

    vector<Fold> folds;

    while (getline(cin, line)) {
        smatch matched;
        regex_match(line, matched, r);

        auto type = matched[1].str() == "y" ? FoldType::Up : FoldType::Left;
        auto at = to_num(matched[2].str());

        folds.emplace_back(type, at);
    }

    solve(points, folds);

    return 0;
}

