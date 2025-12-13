#include <bits/stdc++.h>

using namespace std;

auto solve(const vector<string>& grid, int n, int m, int r, int c)
{
    static constexpr array<pair<int, int>, 4> Neigh{{
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    }};

    queue<pair<int, int>> q;
    q.emplace(r, c);

    vector<vector<bool>> in_q(n, vector<bool>(m));
    in_q[r][c] = true;

    vector<vector<long>> path_cnt(n, vector<long>(m));
    path_cnt[r][c] = 1l;

    auto reachable_dst_cnt = 0l;
    auto dst_path_cnt = 0l;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (grid[r][c] == '9') {
            ++reachable_dst_cnt;
            dst_path_cnt += path_cnt[r][c];
            continue;
        }

        for (const auto& [dr, dc] : Neigh) {
            auto nr = r + dr;
            if (nr < 0 || nr == n)
                continue;

            auto nc = c + dc;
            if (nc < 0 || nc == m)
                continue;

            if (grid[nr][nc] - grid[r][c] != 1)
                continue;

            path_cnt[nr][nc] += path_cnt[r][c];

            if (!in_q[nr][nc]) {
                q.emplace(nr, nc);
                in_q[nr][nc] = true;
            }
        }
    }

    return make_pair(reachable_dst_cnt, dst_path_cnt);
}

auto solve(const vector<string>& grid, int n, int m)
{
    auto sum1 = 0l;
    auto sum2 = 0l;

    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] == '0') {
                auto [cnt1, cnt2] = solve(grid, n, m, r, c);
                sum1 += cnt1;
                sum2 += cnt2;
            }

    return make_pair(sum1, sum2);
}

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s))
        grid.push_back(move(s));

    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    auto [sum1, sum2] = solve(grid, n, m);

    cout << sum1 << '\n';
    cout << sum2 << '\n';

    return 0;
}

