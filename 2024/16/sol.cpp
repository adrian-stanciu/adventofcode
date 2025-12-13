#include <bits/stdc++.h>

using namespace std;

struct State {
    int r;
    int c;
    int dr;
    int dc;

    auto operator==(const State& s) const
    {
        return r == s.r && c == s.c && dr == s.dr && dc == s.dc;
    }
};

struct StateHasher {
    auto operator()(const State& s) const
    {
        // based on boost's hash_combine()
        size_t h = 0;
        h ^= s.r + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= s.c + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= s.dr + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= s.dc + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

struct StateData {
    int d;
    vector<State> parents;
};

using DataMap = unordered_map<State, StateData, StateHasher>;

auto dijkstra(const vector<string>& grid, int sr, int sc, int er, int ec, DataMap& d_map)
{
    auto state_cmp = [&](const auto& lhs, const auto& rhs) {
        auto d_lhs = d_map[lhs].d;
        auto d_rhs = d_map[rhs].d;

        if (d_lhs == d_rhs)
            return tie(lhs.r, lhs.c, lhs.dr, lhs.dc) < tie(rhs.r, rhs.c, rhs.dr, rhs.dc);
        else
            return d_lhs < d_rhs;
    };
    set<State, decltype(state_cmp)> states{state_cmp};

    const State s{sr, sc, 0, 1};
    d_map[s] = StateData{0, {}};
    states.emplace(s);

    while (!states.empty()) {
        auto s = *begin(states);
        states.erase(begin(states));
        auto d = d_map[s].d;

        if (s.r == er && s.c == ec)
            return d;

        vector<pair<State, int>> children;
        if (auto nr = s.r + s.dr, nc = s.c + s.dc; grid[s.r + s.dr][s.c + s.dc] != '#')
            children.emplace_back(State{nr, nc, s.dr, s.dc}, d + 1);
        children.emplace_back(State{s.r, s.c, s.dc, s.dr}, d + 1000);
        children.emplace_back(State{s.r, s.c, -s.dc, -s.dr}, d + 1000);

        for (const auto& [ns, nd] : children)
            if (auto it = d_map.find(ns); it == end(d_map)) {
                d_map[ns] = StateData{nd, {s}};
                states.emplace(ns);
            } else if (it->second.d > nd) {
                if (auto it = states.find(ns); it != end(states))
                    states.erase(it);
                it->second.d = nd;
                it->second.parents.clear();
                it->second.parents.push_back(s);
                states.emplace(ns);
            } else if (it->second.d == nd) {
                it->second.parents.push_back(s);
            }
    }

    assert(false);
}

auto bfs(int n, int m, int er, int ec, const DataMap& d_map, int min_d)
{
    static constexpr array<pair<int, int>, 4> Neigh{{
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    }};

    queue<State> q;
    unordered_set<State, StateHasher> uniq;

    for (const auto& [dr, dc] : Neigh) {
        const State s{er, ec, dr, dc};
        if (auto it = d_map.find(s); it != end(d_map) && it->second.d == min_d) {
            q.push(s);
            uniq.emplace(s);
        }
    }

    vector<vector<bool>> vis(n, vector<bool>(m));

    while (!q.empty()) {
        auto s = q.front();
        q.pop();

        vis[s.r][s.c] = true;

        for (const auto& p : d_map.at(s).parents)
            if (!uniq.contains(p)) {
                q.push(p);
                uniq.emplace(p);
            }
    }

    return accumulate(begin(vis), end(vis), 0, [](auto acc, const auto& row) {
        return acc + count(begin(row), end(row), true);
    });
}

auto solve(const vector<string>& grid)
{
    auto n = ssize(grid);
    auto m = ssize(grid[0]);

    auto sr = 0;
    auto sc = 0;
    auto er = 0;
    auto ec = 0;
    for (auto r = 0; r < n; ++r)
        for (auto c = 0; c < m; ++c)
            if (grid[r][c] == 'S') {
                sr = r;
                sc = c;
            } else if (grid[r][c] == 'E') {
                er = r;
                ec = c;
            }

    DataMap d_map;
    auto min_d = dijkstra(grid, sr, sc, er, ec, d_map);
    auto num_uniq_tiles_on_any_best_path = bfs(n, m, er, ec, d_map, min_d);

    cout << min_d << '\n';
    cout << num_uniq_tiles_on_any_best_path << '\n';
}

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s))
        grid.push_back(move(s));

    solve(grid);

    return 0;
}

