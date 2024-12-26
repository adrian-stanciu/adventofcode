#include <bits/stdc++.h>

using namespace std;

auto count_fwd(const string& s)
{
    static constexpr auto Value{"XMAS"sv};

    auto cnt = 0;

    size_t i = 0;
    while (true) {
        i = s.find(Value, i);
        if (i == s.npos)
            break;

        ++cnt;
        i += Value.size();
    }

    return cnt;
}

auto count(string& s)
{
    auto cnt = 0;

    cnt += count_fwd(s);
    reverse(begin(s), end(s));
    cnt += count_fwd(s);
    reverse(begin(s), end(s));

    return cnt;
}

auto count_on_rows(vector<string>& grid)
{
    auto cnt = 0;

    for (auto& s : grid)
        cnt += count(s);

    return cnt;
}

auto count_on_diags(const vector<string>& grid, int n, int m)
{
    auto cnt = 0;

    for (auto r = 0; r < n; ++r) {
        string s;
        for (auto i = r, j = 0; i < n && j < m; ++i, ++j)
            s.push_back(grid[i][j]);
        cnt += count(s);
    }

    for (auto c = 1; c < m; ++c) {
        string s;
        for (auto i = 0, j = c; i < n && j < m; ++i, ++j)
            s.push_back(grid[i][j]);
        cnt += count(s);
    }

    return cnt;
}

auto transpose(const vector<string>& grid, int n, int m)
{
    vector<string> t_grid(m, string(n, '.'));

    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            t_grid[j][i] = grid[i][j];

    return t_grid;
}

auto rotate(const vector<string>& grid, int n, int m)
{
    vector<string> r_grid(m, string(n, '.'));

    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            r_grid[j][m - i - 1] = grid[i][j];

    return r_grid;
}

auto solve1(vector<string>& grid, int n, int m)
{
    auto cnt = 0;

    cnt += count_on_rows(grid);
    auto t_grid = transpose(grid, n, m);
    cnt += count_on_rows(t_grid);

    cnt += count_on_diags(grid, n, m);
    auto r_grid = rotate(grid, n, m);
    cnt += count_on_diags(r_grid, n, m);

    return cnt;
}

auto solve2(const vector<string>& grid, int n, int m)
{
    auto is_valid = [](auto ch1, auto ch2) {
        if (ch1 > ch2)
            swap(ch1, ch2);
        return ch1 == 'M' && ch2 == 'S';
    };

    auto cnt = 0;

    for (auto i = 1; i < n - 1; ++i)
        for (auto j = 1; j < m - 1; ++j)
            if (grid[i][j] == 'A' &&
                is_valid(grid[i - 1][j - 1], grid[i + 1][j + 1]) &&
                is_valid(grid[i - 1][j + 1], grid[i + 1][j - 1]))
                ++cnt;

    return cnt;
}

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s))
        grid.push_back(move(s));

    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    cout << solve1(grid, n, m) << '\n';
    cout << solve2(grid, n, m) << '\n';

    return 0;
}

