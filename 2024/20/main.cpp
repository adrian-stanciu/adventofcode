#include <bits/stdc++.h>

using namespace std;

static constexpr auto Inf{numeric_limits<int>::max()};

auto comp_dist(const vector<string>& grid, int n, int m)
{
    static constexpr array<pair<int, int>, 4> Neigh{{
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    }};

    auto find = [&](auto ch) {
        for (auto r = 0; r < n; ++r)
            for (auto c = 0; c < m; ++c)
                if (grid[r][c] == ch)
                    return make_pair(r, c);
        assert(false);
    };

    auto [sr, sc] = find('S');
    auto [er, ec] = find('E');

    vector<vector<int>> dist(n, vector<int>(m, Inf));

    auto r = er;
    auto c = ec;
    dist[r][c] = 0;

    while (r != sr || c != sc)
        for (auto [dr, dc] : Neigh) {
            auto nr = r + dr;
            auto nc = c + dc;

            if (grid[nr][nc] == '#')
                continue;

            if (dist[nr][nc] == Inf) {
                dist[nr][nc] = dist[r][c] + 1;
                r = nr;
                c = nc;
                break;
            }
        }

    return dist;
}

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s))
        grid.push_back(move(s));

    int n = ssize(grid);
    int m = ssize(grid[0]);

    auto dist = comp_dist(grid, n, m);

    auto count_from = [&](auto r, auto c, auto max_jump, auto min_gain) {
        auto cnt = 0;

        auto min_r = max(r - max_jump, 0);
        auto max_r = min(r + max_jump, n - 1);
        auto min_c = max(c - max_jump, 0);
        auto max_c = min(c + max_jump, m - 1);

        for (auto nr = min_r; nr <= max_r; ++nr)
            for (auto nc = min_c; nc <= max_c; ++nc)
                if (dist[nr][nc] < dist[r][c])
                    if (auto d = abs(nr - r) + abs(nc - c); d <= max_jump)
                        cnt += (dist[r][c] - dist[nr][nc]) - d >= min_gain;

        return cnt;
    };

    auto count = [&](auto max_jump, auto min_gain) {
        auto cnt = 0;

        for (auto r = 0; r < n; ++r)
            for (auto c = 0; c < m; ++c)
                if (dist[r][c] != Inf)
                    cnt += count_from(r, c, max_jump, min_gain);

        return cnt;
    };

    cout << count(2, 100) << '\n';
    cout << count(20, 100) << '\n';

    return 0;
}

