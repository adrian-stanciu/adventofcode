#include <bits/stdc++.h>

using namespace std;

struct State {
    int d;
    int r;
    int c;
    int dr;
    int dc;
    int avail_steps;

    State(int d, int r, int c, int dr, int dc, int avail_steps)
    : d{d}, r{r}, c{c}, dr{dr}, dc{dc}, avail_steps{avail_steps}
    {}

    auto operator<=>(const State&) const = default;
};

struct Dist {
    Dist(auto sz)
    {
        for (auto& v : distances)
            v.resize(sz, numeric_limits<int>::max());
    }

    [[nodiscard]] auto& get(int dr, int dc, auto avail_steps)
    {
        if (dr == 0) {
            if (dc == -1)
                return distances[0][avail_steps];
            else
                return distances[1][avail_steps];
        } else {
            if (dr == -1)
                return distances[2][avail_steps];
            else
                return distances[3][avail_steps];
        }
    }

private:
    array<vector<int>, 4> distances;
};

auto solve(const vector<string>& grid, int dr, int dc, int min_steps, int max_steps)
{
    constexpr array<pair<int, int>, 4> neigh{{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    }};

    const auto n = ssize(grid);
    const auto m = ssize(grid[0]);

    auto d = 0;
    for (auto i = 1; i <= min_steps; ++i)
        d += grid[i * dr][i * dc] - '0';

    std::set<State> states;
    states.emplace(d, min_steps * dr, min_steps * dc, dr, dc, max_steps - min_steps);

    vector<vector<Dist>> dists(n, vector<Dist>(m, max_steps));
    dists[min_steps * dr][min_steps * dc].get(dr, dc, max_steps - min_steps) = d;

    auto visit = [&](auto new_d, auto r, auto c, auto dr, auto dc,
        auto avail_steps, auto todo_steps) {
        avail_steps -= todo_steps;
        if (avail_steps < 0)
            return;

        while (todo_steps--) {
            r += dr;
            if (r < 0 || r == n)
                return;

            c += dc;
            if (c < 0 || c == m)
                return;

            new_d += grid[r][c] - '0';
        }

        auto &old_d = dists[r][c].get(dr, dc, avail_steps);

        if (old_d > new_d) {
            State s{old_d, r, c, dr, dc, avail_steps};

            auto it = states.find(s);
            if (it != end(states))
                states.erase(it);

            s.d = new_d;
            states.insert(move(s));

            old_d = new_d;
        }
    };

    while (!states.empty()) {
        auto [d, r, c, dr, dc, avail_steps] = *states.begin();
        states.erase(states.begin());

        if (r == n - 1 && c == m - 1)
            return d;

        for (auto [ndr, ndc] : neigh)
            if (ndr == dr && ndc == dc)
                visit(d, r, c, dr, dc, avail_steps, 1);
            else if (ndr != -dr || ndc != -dc)
                visit(d, r, c, ndr, ndc, max_steps, min_steps);
    }

    assert(false);
}

auto solve(const vector<string>& grid, int min_steps, int max_steps)
{
    return min(solve(grid, 0, 1, min_steps, max_steps),
        solve(grid, 1, 0, min_steps, max_steps));
}

int main()
{
    vector<string> grid;

    string row;
    while (getline(cin, row))
        grid.push_back(move(row));

    cout << solve(grid, 1, 3) << '\n';
    cout << solve(grid, 4, 10) << '\n';

    return 0;
}

