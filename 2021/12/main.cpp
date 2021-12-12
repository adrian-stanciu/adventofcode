#include <bits/stdc++.h>

using namespace std;

auto is_small(const string& s)
{
    return s[0] >= 'a' && s[0] <= 'z';
}

void dfs1(const vector<vector<int>>& g, const vector<bool>& small, int src, int dst,
    int u, vector<bool>& visited, int& cnt)
{
    if (u == dst) {
        ++cnt;
        return;
    }

    for (auto v : g[u]) {
        if (small[v] && visited[v])
            continue;

        visited[v] = true;
        dfs1(g, small, src, dst, v, visited, cnt);
        visited[v] = false;
    }
}

auto solve1(const vector<vector<int>>& g, const vector<bool>& small, int src, int dst)
{
    vector<bool> visited(g.size(), false);
    visited[src] = true;

    auto cnt = 0;
    dfs1(g, small, src, dst, src, visited, cnt);
    return cnt;
}

void dfs2(const vector<vector<int>>& g, const vector<bool>& small, int src, int dst,
    int u, vector<bool>& visited, bool pass_twice, int& cnt)
{
    if (u == dst) {
        ++cnt;
        return;
    }

    for (auto v : g[u]) {
        if (small[v] && visited[v]) {
            if (pass_twice && v != src)
                dfs2(g, small, src, dst, v, visited, false, cnt);
        } else {
            visited[v] = true;
            dfs2(g, small, src, dst, v, visited, pass_twice, cnt);
            visited[v] = false;
        }
    }
}

auto solve2(const vector<vector<int>>& g, const vector<bool>& small, int src, int dst)
{
    vector<bool> visited(g.size(), false);
    visited[src] = true;

    auto cnt = 0;
    dfs2(g, small, src, dst, src, visited, true, cnt);
    return cnt;
}

void solve(const vector<pair<string, string>>& edges)
{
    unordered_map<string, int> name2id;
    auto n = 0;

    auto save_name = [&](const auto& name) {
        if (name2id.find(name) == end(name2id))
            name2id[name] = n++;
    };

    for (const auto& [u_name, v_name] : edges) {
        save_name(u_name);
        save_name(v_name);
    }

    vector<vector<int>> g(n);
    vector<bool> small(n, false);

    for (const auto& [u_name, v_name] : edges) {
        auto u_id = name2id[u_name];
        auto v_id = name2id[v_name];

        g[u_id].push_back(v_id);
        g[v_id].push_back(u_id);

        small[u_id] = is_small(u_name);
        small[v_id] = is_small(v_name);
    }

    auto src = name2id["start"];
    auto dst = name2id["end"];

    cout << solve1(g, small, src, dst) << '\n';
    cout << solve2(g, small, src, dst) << '\n';
}

int main()
{
    vector<pair<string, string>> edges;

    string line;
    while (getline(cin, line)) {
        auto sep_pos = line.find('-');

        auto u_name = line.substr(0, sep_pos);
        auto v_name = line.substr(sep_pos + 1);

        edges.emplace_back(move(u_name), move(v_name));
    }

    solve(edges);

    return 0;
}

