#include <bits/stdc++.h>

using namespace std;

struct State {
    int r;
    int c;
    int dr;
    int dc;

    State(int r, int c, int dr, int dc) : r{r}, c{c}, dr{dr}, dc{dc} {}
};

struct Visited {
    [[nodiscard]] auto& get(int dr, int dc)
    {
        if (dr == 0) {
            if (dc == -1)
                return visited[0];
            else
                return visited[1];
        } else {
            if (dr == -1)
                return visited[2];
            else
                return visited[3];
        }
    }

    [[nodiscard]] operator bool() const
    {
        return any_of(begin(visited), end(visited), [](auto elem) { return elem; });
    }

private:
    array<bool, 4> visited{};
};

auto solve1(const vector<string>& grid, int r, int c, int dr, int dc)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    queue<State> q;
    q.emplace(r, c, dr, dc);

    vector<vector<Visited>> visited(n, vector<Visited>(m));
    visited[r][c].get(dr, dc) = true;

    auto visit = [&](auto r, auto c, auto dr, auto dc) {
        if (r < 0 || r == n)
            return;
        if (c < 0 || c == m)
            return;
        if (exchange(visited[r][c].get(dr, dc), true))
            return;
        q.emplace(r, c, dr, dc);
    };

    while (!q.empty()) {
        auto [r, c, dr, dc] = q.front();
        q.pop();

        if (grid[r][c] == '.' ||
            (grid[r][c] == '|' && dc == 0) ||
            (grid[r][c] == '-' && dr == 0)) {
            visit(r + dr, c + dc, dr, dc);
        } else if (grid[r][c] == '/') {
            dr *= -1;
            dc *= -1;
            swap(dr, dc);
            visit(r + dr, c + dc, dr, dc);
        } else if (grid[r][c] == '\\') {
            swap(dr, dc);
            visit(r + dr, c + dc, dr, dc);
        } else if (grid[r][c] == '|') {
            for (auto ndr : {-1, 1})
                visit(r + ndr, c, ndr, 0);
        } else if (grid[r][c] == '-') {
            for (auto ndc : {-1, 1})
                visit(r, c + ndc, 0, ndc);
        }
    }

    return accumulate(begin(visited), end(visited), 0, [](auto acc, const auto& row) {
        return acc + count(begin(row), end(row), true);
    });
}

auto solve2(const vector<string>& grid)
{
    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    auto max_visited = 0;

    for (auto r = 0; r < n; ++r) {
        max_visited = max(max_visited, solve1(grid, r, 0, 0, 1));
        max_visited = max(max_visited, solve1(grid, r, m - 1, 0, -1));
    }
    for (auto c = 0; c < m; ++c) {
        max_visited = max(max_visited, solve1(grid, 0, c, 1, 0));
        max_visited = max(max_visited, solve1(grid, n - 1, c, -1, 0));
    }

    return max_visited;
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve1(grid, 0, 0, 0, 1) << '\n';
    cout << solve2(grid) << '\n';

    return 0;
}

