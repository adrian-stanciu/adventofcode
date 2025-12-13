#include <bits/stdc++.h>

using namespace std;

using VisitedElem = array<bool, 4>;
using VisitedRow = vector<array<bool, 4>>;
using Visited = vector<VisitedRow>;

auto is_visited(const VisitedElem& elem)
{
    return find(begin(elem), end(elem), true) != end(elem);
}

auto solve1(const vector<string>& grid, int n, int m, int r, int c, Visited& vis)
{
    auto dr = -1;
    auto dc = 0;

    auto dir = [&]() {
        if (dr)
            return dr < 0 ? 0 : 1;
        else
            return dc < 0 ? 2 : 3;
    };

    for (auto& row : vis)
        for (auto& elem : row)
            elem = {};

    vis[r][c][dir()] = true;

    while (true) {
        auto nr = r + dr;
        if (nr < 0 || nr == n)
            break;

        auto nc = c + dc;
        if (nc < 0 || nc == m)
            break;

        if (grid[nr][nc] == '#') {
            if (dr) {
                dc = -dr;
                dr = 0;
            } else {
                dr = dc;
                dc = 0;
            }

            if (exchange(vis[r][c][dir()], true))
                return -1;
        } else {
            r = nr;
            c = nc;

            if (exchange(vis[r][c][dir()], true))
                return -1;
        }
    }

    return accumulate(begin(vis), end(vis), 0, [](auto acc, const auto& row) {
        return acc + count_if(begin(row), end(row), is_visited);
    });
}

auto solve2(const vector<vector<int>> r_blocks, const vector<vector<int>> c_blocks,
    int r, int c, Visited& vis)
{
    auto dr = -1;
    auto dc = 0;

    auto dir = [&]() {
        if (dr)
            return dr < 0 ? 0 : 1;
        else
            return dc < 0 ? 2 : 3;
    };

    for (auto& row : vis)
        for (auto& elem : row)
            elem = {};

    vis[r][c][dir()] = true;

    auto find_block_pos = [&]() -> optional<pair<int, int>> {
        const auto& v = dr ? c_blocks[c] : r_blocks[r];

        auto it = lower_bound(begin(v), end(v), dr ? r : c);

        if (dr < 0 || dc < 0) {
            if (it == begin(v))
                return nullopt;
            --it;
            return make_pair(dr ? *it : r, dr ? c : *it);
        } else {
            if (it == end(v))
                return nullopt;
            return make_pair(dr ? *it : r, dr ? c : *it);
        }
    };

    while (true) {
        auto block_pos = find_block_pos();
        if (!block_pos)
            return false;

        r = block_pos->first - dr;
        c = block_pos->second - dc;

        if (dr) {
            dc = -dr;
            dr = 0;
        } else {
            dr = dc;
            dc = 0;
        }

        if (exchange(vis[r][c][dir()], true))
            return true;
    }
}

auto solve2(const vector<string>& grid, int n, int m, int r, int c, Visited& vis1)
{
    vector<vector<int>> r_blocks(n);
    vector<vector<int>> c_blocks(m);

    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            if (grid[i][j] == '#') {
                r_blocks[i].push_back(j);
                c_blocks[j].push_back(i);
            }

    Visited vis2(n, VisitedRow(m));

    auto cnt = 0;

    for (auto i = 0; i < n; ++i)
        for (auto j = 0; j < m; ++j)
            if (is_visited(vis1[i][j]) && (i != r || j != c)) {
                r_blocks[i].insert(lower_bound(begin(r_blocks[i]), end(r_blocks[i]), j), j);
                c_blocks[j].insert(lower_bound(begin(c_blocks[j]), end(c_blocks[j]), i), i);

                if (solve2(r_blocks, c_blocks, r, c, vis2))
                    ++cnt;

                r_blocks[i].erase(find(begin(r_blocks[i]), end(r_blocks[i]), j));
                c_blocks[j].erase(find(begin(c_blocks[j]), end(c_blocks[j]), i));
            }

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

    auto [r, c] = [&]() {
        for (auto r = 0; r < n; ++r)
            for (auto c = 0; c < m; ++c)
                if (grid[r][c] == '^')
                    return make_pair(r, c);
        assert(false);
    }();

    Visited vis1(n, VisitedRow(m));

    cout << solve1(grid, n, m, r, c, vis1) << '\n';
    cout << solve2(grid, n, m, r, c, vis1) << '\n';

    return 0;
}

