#include <bits/stdc++.h>

using namespace std;

using Grid = vector<vector<int>>;

const array<pair<int, int>, 4> neigh{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

auto find_low_points(const Grid& grid)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    auto is_low_point = [&](auto x, auto y) {
        for (auto [dx, dy] : neigh) {
            auto nx = x + dx;
            if (nx < 0 || nx == n)
                continue;

            auto ny = y + dy;
            if (ny < 0 || ny == m)
                continue;

            if (grid[x][y] >= grid[nx][ny])
                return false;
        }

        return true;
    };

    vector<pair<int, int>> low_points;

    for (auto x = 0; x < n; ++x)
        for (auto y = 0; y < m; ++y)
            if (is_low_point(x, y))
                low_points.emplace_back(x, y);

    return low_points;
}

auto solve1(const Grid& grid, const vector<pair<int, int>>& low_points)
{
    auto sol = 0;
    for (auto [x, y] : low_points)
        sol += grid[x][y] + 1;
    return sol;
}

auto find_size(Grid grid, int x, int y)
{
    auto n = static_cast<int>(grid.size());
    auto m = static_cast<int>(grid[0].size());

    auto sz = 1;

    queue<pair<int, int>> q;
    q.emplace(x, y);

    grid[x][y] = 9;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (auto [dx, dy] : neigh) {
            auto nx = x + dx;
            if (nx < 0 || nx == n)
                continue;

            auto ny = y + dy;
            if (ny < 0 || ny == m)
                continue;

            if (grid[nx][ny] == 9)
                continue;

            ++sz;
            q.emplace(nx, ny);
            grid[nx][ny] = 9;
        }
    }

    return sz;
}

auto solve2(const Grid& grid, const vector<pair<int, int>>& low_points)
{
    vector<int> sizes;
    sizes.reserve(low_points.size());

    for (auto [x, y] : low_points)
        sizes.push_back(find_size(grid, x, y));

    sort(rbegin(sizes), rend(sizes));

    return accumulate(begin(sizes), begin(sizes) + 3, 1l,
        [](auto acc, auto val) { return acc * val; });
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

    auto low_points = find_low_points(grid);

    cout << solve1(grid, low_points) << '\n';
    cout << solve2(grid, low_points) << '\n';

    return 0;
}

