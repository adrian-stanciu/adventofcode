#include <bits/stdc++.h>

using namespace std;

static constexpr array<pair<int, int>, 4> neigh{{
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
}};

auto are_connected(const vector<string>& grid, int r, int c, int nr, int nc)
{
    switch (grid[r][c]) {
    case '|':
        switch (grid[nr][nc]) {
        case '|':
            return abs(nr - r) == 1 && nc == c;
        case '-':
            return false;
        case 'L':
            return nr == r + 1;
        case 'J':
            return nr == r + 1;
        case '7':
            return nr == r - 1;
        case 'F':
            return nr == r - 1;
        }
        break;
    case '-':
        switch (grid[nr][nc]) {
        case '|':
            return false;
        case '-':
            return nr == r && abs(nc - c) == 1;
        case 'L':
            return nc == c - 1;
        case 'J':
            return nc == c + 1;
        case '7':
            return nc == c + 1;
        case 'F':
            return nc == c - 1;
        }
        break;
    case 'L':
        switch (grid[nr][nc]) {
        case '|':
            return nr == r - 1;
        case '-':
            return nc == c + 1;
        case 'L':
            return false;
        case 'J':
            return nc == c + 1;
        case '7':
            return nr == r - 1 || nc == c + 1;
        case 'F':
            return nr == r - 1;
        }
        break;
    case 'J':
        switch (grid[nr][nc]) {
        case '|':
            return nr == r - 1;
        case '-':
            return nc == c - 1;
        case 'L':
            return nc == c - 1;
        case 'J':
            return false;
        case '7':
            return nr == r - 1;
        case 'F':
            return nr == r - 1 || nc == c - 1;
        }
        break;
    case '7':
        switch (grid[nr][nc]) {
        case '|':
            return nr == r + 1;
        case '-':
            return nc == c - 1;
        case 'L':
            return nr == r + 1 || nc == c - 1;
        case 'J':
            return nr == r + 1;
        case '7':
            return false;
        case 'F':
            return nc == c - 1;
        }
        break;
    case 'F':
        switch (grid[nr][nc]) {
        case '|':
            return nr == r + 1;
        case '-':
            return nc == c + 1;
        case 'L':
            return nr == r + 1;
        case 'J':
            return nr == r + 1 || nc == c + 1;
        case '7':
            return nc == c + 1;
        case 'F':
            return false;
        }
        break;
    }

    return false;
}

auto get_cycle(const vector<string>& grid, int start_r, int start_c)
{
    set<pair<int, int>> cycle;

    auto parent_r = -1;
    auto parent_c = -1;
    auto r = start_r;
    auto c = start_c;

    while (true) {
        bool found = false;

        for (auto [dr, dc] : neigh) {
            auto nr = r + dr;
            if (nr < 0 || nr == ssize(grid))
                continue;
            auto nc = c + dc;
            if (nc < 0 || nc == ssize(grid[0]))
                continue;

            if (nr == parent_r && nc == parent_c)
                continue;

            if (!are_connected(grid, r, c, nr, nc))
                continue;

            cycle.emplace(nr, nc);

            if (nr == start_r && nc == start_c)
                return cycle;

            parent_r = r;
            parent_c = c;
            r = nr;
            c = nc;

            found = true;
        }

        if (!found) {
            cycle.clear();
            return cycle;
        }
    }
}

auto solve1(vector<string>& grid)
{
    auto [start_r, start_c] = [&]() {
        for (auto r = 0; r < ssize(grid); ++r)
            for (auto c = 0; c < ssize(grid[0]); ++c)
                if (grid[r][c] == 'S')
                    return make_pair(r, c);
        assert(false);
    }();

    set<pair<int, int>> max_cycle;
    char max_cycle_pipe = '?';

    for (auto pipe : {'|', '-', 'L', 'J', '7', 'F'}) {
        grid[start_r][start_c] = pipe;

        auto cycle = get_cycle(grid, start_r, start_c);
        if (ssize(cycle) > ssize(max_cycle)) {
            max_cycle = move(cycle);
            max_cycle_pipe = pipe;
        }
    }

    assert(max_cycle_pipe != '?');
    grid[start_r][start_c] = max_cycle_pipe;

    for (auto r = 0; r < ssize(grid); ++r)
        for (auto c = 0; c < ssize(grid[0]); ++c)
            if (!max_cycle.contains(make_pair(r, c)))
                grid[r][c] = '.';

    return ssize(max_cycle) / 2;
}

auto solve2(vector<string>& grid)
{
    vector<string> extended_grid;
    extended_grid.reserve(ssize(grid));

    for (auto r = 0; r < ssize(grid); ++r) {
        string row;
        row.reserve(2 * ssize(grid[0]) - 1);

        for (auto c = 0;; ++c) {
            row.push_back(grid[r][c]);

            if (c == ssize(grid[0]) - 1)
                break;

            if (are_connected(grid, r, c, r, c + 1))
                row.push_back('#');
            else
                row.push_back('o');
        }

        extended_grid.push_back(move(row));
    }

    grid.swap(extended_grid);

    extended_grid.clear();
    extended_grid.reserve(2 * ssize(grid) - 1);

    for (auto r = 0;; ++r) {
        extended_grid.push_back(grid[r]);

        if (r == ssize(grid) - 1)
            break;

        string row;
        row.reserve(ssize(grid[0]));

        for (auto c = 0; c < ssize(grid[0]); ++c)
            if (are_connected(grid, r, c, r + 1, c))
                row.push_back('#');
            else
                row.push_back('o');

        extended_grid.push_back(move(row));
    }

    grid.swap(extended_grid);

    auto fill = [&](auto start_r, auto start_c) {
        queue<pair<int, int>> q;

        if (grid[start_r][start_c] == '.' || grid[start_r][start_c] == 'o')
            q.emplace(start_r, start_c);

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (auto [dr, dc] : neigh) {
                auto nr = r + dr;
                if (nr < 0 || nr == ssize(grid))
                    continue;
                auto nc = c + dc;
                if (nc < 0 || nc == ssize(grid[0]))
                    continue;

                if (grid[nr][nc] != '.' && grid[nr][nc] != 'o')
                    continue;

                grid[nr][nc] = 'x';

                q.emplace(nr, nc);
            }
        }
    };

    for (auto r = 0; r < ssize(grid); ++r) {
        fill(r, 0);
        fill(r, ssize(grid[0]) - 1);
    }
    for (auto c = 0; c < ssize(grid[0]); ++c) {
        fill(0, c);
        fill(ssize(grid) - 1, c);
    }

    return accumulate(begin(grid), end(grid), 0, [](auto acc, const auto& row) {
        return acc + count(begin(row), end(row), '.');
    });
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve1(grid) << '\n';
    cout << solve2(grid) << '\n';

    return 0;
}

