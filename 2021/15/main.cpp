#include <bits/stdc++.h>

using namespace std;

using Row = vector<int>;
using Grid = vector<Row>;

auto solve(const Grid& grid)
{
    static const array<pair<int, int>, 4> neigh{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    auto n = static_cast<int>(grid.size());

    using trio = tuple<int, int, int>;
    auto cmp = [](const auto& lhs, const auto& rhs) { return lhs > rhs; };
    priority_queue<trio, vector<trio>, decltype(cmp)> cost2pos{cmp};
    cost2pos.emplace(0, 0, 0);

    Grid pos2cost(n, Row(n, numeric_limits<int>::max()));
    pos2cost[0][0] = 0;

    while (!cost2pos.empty()) {
        auto [cost, y, x] = cost2pos.top();
        cost2pos.pop();

        if (y == n - 1 && x == n - 1)
            return cost;

        for (auto [dy, dx] : neigh) {
            auto ny = y + dy;
            if (ny < 0 || ny == n)
                continue;

            auto nx = x + dx;
            if (nx < 0 || nx == n)
                continue;

            auto new_cost = cost + grid[ny][nx];

            if (new_cost < pos2cost[ny][nx]) {
                cost2pos.emplace(new_cost, ny, nx);
                pos2cost[ny][nx] = new_cost;
            }
        }
    }

    return -1;
}

auto scale(const Grid& grid, int scale_factor)
{
    auto val = [](auto x) {
        if (x > 9)
            return x % 10 + 1;
        else
            return x;
    };

    auto n = static_cast<int>(grid.size());
    auto new_n = scale_factor * n;
    Grid new_grid(new_n, Row(new_n));

    for (auto y = 0; y < n; ++y)
        for (auto x = 0; x < n; ++x)
            for (auto k = 0; k < scale_factor; ++k)
                new_grid[y][k * n + x] = val(grid[y][x] + k);

    for (auto k = 1; k < scale_factor; ++k)
        for (auto y = 0; y < n; ++y)
            for (auto x = 0; x < new_n; ++x)
                new_grid[k * n + y][x] = val(new_grid[y][x] + k);

    return new_grid;
}

int main()
{
    Grid grid;

    string s;
    while (getline(cin, s)) {
        Row row;

        for (auto c : s)
            row.push_back(c - '0');

        grid.push_back(move(row));
    }

    cout << solve(grid) << '\n';
    cout << solve(scale(grid, 5)) << '\n';

    return 0;
}

